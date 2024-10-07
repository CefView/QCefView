#include "details/CCefClientDelegate.h"

#include <QApplication>
#include <QDebug>
#include <QImage>
#include <QScreen>

#include "details/QCefViewPrivate.h"

// OSR mode

bool
CCefClientDelegate::getRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
  if (!IsValidBrowser(browser)) {
    return false;
  }

  // get the screen which the view is currently residing in
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
  QScreen* currentScreen = pCefViewPrivate_->q_ptr->screen();
#else
  QWidget* ancestorWidget = pCefViewPrivate_->q_ptr->window();
  QWindow* ancestorWindow = ancestorWidget ? ancestorWidget->windowHandle() : nullptr;
  QScreen* currentScreen = ancestorWindow ? ancestorWindow->screen() : nullptr;
#endif

  if (!currentScreen) {
    // the view is not visible, so we retrieve the main screen info
    currentScreen = QApplication::screens().at(0);
  }

  QRect rcScreen = currentScreen->geometry();

  // qDebug() << "CCefClientDelegate::GetRootScreenRect:" << rcScreen;

  rect.Set(rcScreen.x(), rcScreen.y(), rcScreen.width(), rcScreen.height());
  return true;
}

void
CCefClientDelegate::getViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
  if (!IsValidBrowser(browser)) {
    rect.Set(0, 0, 1, 1);
    return;
  }

  QSize rcSize = pCefViewPrivate_->q_ptr->size();

  // qDebug() << "CCefClientDelegate::GetViewRect:" << QRect(ptWindow, rcSize);

  rect.Set(0, 0, rcSize.width() ? rcSize.width() : 1, rcSize.height() ? rcSize.height() : 1);
}

bool
CCefClientDelegate::getScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int& screenX, int& screenY)
{
  if (!IsValidBrowser(browser))
    return false;

  QPoint ptScreen = pCefViewPrivate_->q_ptr->mapToGlobal(QPoint(viewX, viewY));
  screenX = ptScreen.x();
  screenY = ptScreen.y();
  return true;
}

bool
CCefClientDelegate::getScreenInfo(CefRefPtr<CefBrowser> browser, CefScreenInfo& screen_info)
{
  if (!IsValidBrowser(browser))
    return false;

    // get the screen which the view is currently residing in
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
  QScreen* currentScreen = pCefViewPrivate_->q_ptr->screen();
#else
  QWidget* ancestorWidget = pCefViewPrivate_->q_ptr->window();
  QWindow* ancestorWindow = ancestorWidget ? ancestorWidget->windowHandle() : nullptr;
  QScreen* currentScreen = ancestorWindow ? ancestorWindow->screen() : nullptr;
#endif

  if (!currentScreen) {
    // the view is not visible, so we retrieve the main screen info
    currentScreen = QApplication::screens().at(0);
  }

  QRect rect = currentScreen->geometry();
  QRect availableRect = currentScreen->availableGeometry();
  screen_info.Set(currentScreen->devicePixelRatio(),                                                      //
                  currentScreen->depth(),                                                                 //
                  0,                                                                                      //
                  false,                                                                                  //
                  { rect.x(), rect.y(), rect.width(), rect.height() },                                    //
                  { availableRect.x(), availableRect.y(), availableRect.width(), availableRect.height() } //
  );

  // qDebug() << "CCefClientDelegate::GetScreenInfo: pixel ratio:" << screen_info.device_scale_factor << "rect:" << rect
  //           << "availableRect:" << availableRect;

  return true;
}

void
CCefClientDelegate::onPopupShow(CefRefPtr<CefBrowser> browser, bool show)
{
  if (!IsValidBrowser(browser))
    return;

  pCefViewPrivate_->onOsrShowPopup(show);
}

void
CCefClientDelegate::onPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect)
{
  if (!IsValidBrowser(browser))
    return;

  pCefViewPrivate_->onOsrResizePopup(QRect{ rect.x, rect.y, rect.width, rect.height });
}

void
CCefClientDelegate::onPaint(CefRefPtr<CefBrowser> browser,
                            CefRenderHandler::PaintElementType type,
                            const CefRenderHandler::RectList& dirtyRects,
                            const void* buffer,
                            int width,
                            int height)
{
  if (!IsValidBrowser(browser))
    return;

  QImage frame;
  QRegion region;

  frame = QImage(static_cast<const uchar*>(buffer), width, height, QImage::Format_ARGB32_Premultiplied);
  for (auto& rect : dirtyRects) {
    region += QRect{ rect.x, rect.y, rect.width, rect.height };
  }

  if (PET_VIEW == type) {
    pCefViewPrivate_->onOsrUpdateViewFrame(frame, region);
  } else if (PET_POPUP == type) {
    pCefViewPrivate_->onOsrUpdatePopupFrame(frame, region);
  } else {
  }
}

#if CEF_VERSION_MAJOR < 124
void
CCefClientDelegate::onAcceleratedPaint(CefRefPtr<CefBrowser> browser,
                                       CefRenderHandler::PaintElementType type,
                                       const CefRenderHandler::RectList& dirtyRects,
                                       void* shared_handle)
{
}
#else
void
CCefClientDelegate::onAcceleratedPaint(CefRefPtr<CefBrowser> browser,
                                       CefRenderHandler::PaintElementType type,
                                       const CefRenderHandler::RectList& dirtyRects,
                                       const CefAcceleratedPaintInfo& info)
{
  QRegion region;
  for (auto& rect : dirtyRects) {
    region += QRect{ rect.x, rect.y, rect.width, rect.height };
  }

  if (PET_VIEW == type) {
    pCefViewPrivate_->onOsrUpdateViewTexture(info, region);
  } else if (PET_POPUP == type) {
    pCefViewPrivate_->onOsrUpdatePopupTexture(info, region);
  } else {
  }
}
#endif

bool
CCefClientDelegate::startDragging(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefDragData> drag_data,
                                  CefRenderHandler::DragOperationsMask allowed_ops,
                                  int x,
                                  int y)
{
  return false;
}

void
CCefClientDelegate::updateDragCursor(CefRefPtr<CefBrowser> browser, CefRenderHandler::DragOperation operation)
{
}

void
CCefClientDelegate::onScrollOffsetChanged(CefRefPtr<CefBrowser> browser, double x, double y)
{
}

void
CCefClientDelegate::onImeCompositionRangeChanged(CefRefPtr<CefBrowser> browser,
                                                 const CefRange& selected_range,
                                                 const CefRenderHandler::RectList& character_bounds)
{
  if (!IsValidBrowser(browser))
    return;

  // qDebug() << "OnImeCompositionRangeChanged:" << selected_range.from << " - " << selected_range.to
  //         << ", RC:" << character_bounds.size();

  if (!character_bounds.empty()) {
    auto r = character_bounds[character_bounds.size() - 1];
    QRect rc(r.x, r.y, r.width, r.height);
    QMetaObject::invokeMethod(pCefViewPrivate_,            //
                              "onOsrImeCursorRectChanged", //
                              Q_ARG(const QRect&, rc));
  }
}

void
CCefClientDelegate::onTextSelectionChanged(CefRefPtr<CefBrowser> browser,
                                           const CefString& selected_text,
                                           const CefRange& selected_range)
{
}

void
CCefClientDelegate::onVirtualKeyboardRequested(CefRefPtr<CefBrowser> browser,
                                               CefRenderHandler::TextInputMode input_mode)
{
}
