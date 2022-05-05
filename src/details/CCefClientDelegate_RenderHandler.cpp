#include "CCefClientDelegate.h"

#include <QDebug>
#include <QImage>
#include <QScreen>

#include "QCefViewPrivate.h"

#if defined(CEF_USE_OSR)

bool
CCefClientDelegate::GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
  if (!IsValidBrowser(browser)) {
    return false;
  }

  QRect rcScreen = pCefViewPrivate_->q_ptr->windowHandle()->screen()->geometry();

  qDebug() << "GetRootScreenRect:" << rcScreen;

  rect.Set(rcScreen.x(), rcScreen.y(), rcScreen.width(), rcScreen.height());
  return true;
}

void
CCefClientDelegate::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
  if (!IsValidBrowser(browser)) {
    rect.Set(0, 0, 1, 1);
    return;
  }

  // Note: the documentation of the CefRenderHandler::GetViewRect
  // says `the rect is relative to the screen coordinates` but actually
  // it is relative to the top level window. In the source code of of the cefclient example
  // it just returns the rect retrieved from GetClinetRect which is relative to the window
  QSize rcSize = pCefViewPrivate_->q_ptr->size();
  QPoint ptWindow = pCefViewPrivate_->q_ptr->mapTo(pCefViewPrivate_->q_ptr->window(), QPoint(0, 0));

  qDebug() << "GetViewRect:" << QRect(ptWindow, rcSize);

  rect.Set(ptWindow.x(), ptWindow.y(), rcSize.width() ? rcSize.width() : 1, rcSize.height() ? rcSize.height() : 1);
}

bool
CCefClientDelegate::GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int& screenX, int& screenY)
{
  if (!IsValidBrowser(browser))
    return false;

  QPoint ptScreen = pCefViewPrivate_->q_ptr->mapToGlobal(QPoint(viewX, viewY));
  screenX = ptScreen.x();
  screenY = ptScreen.y();
  return true;
}

bool
CCefClientDelegate::GetScreenInfo(CefRefPtr<CefBrowser> browser, CefScreenInfo& screen_info)
{
  if (!IsValidBrowser(browser))
    return false;

  QScreen* currentScreen = pCefViewPrivate_->q_ptr->windowHandle()->screen();
  QRect rect = currentScreen->geometry();
  QRect availableRect = currentScreen->availableGeometry();
  screen_info.Set(currentScreen->devicePixelRatio(),                                                      //
                  currentScreen->depth(),                                                                 //
                  0,                                                                                      //
                  false,                                                                                  //
                  { rect.x(), rect.y(), rect.width(), rect.height() },                                    //
                  { availableRect.x(), availableRect.y(), availableRect.width(), availableRect.height() } //
  );

  // qDebug() << "GetScreenInfo: pixel ratio:" << screen_info.device_scale_factor << "rect:" << rect
  //          << "availableRect:" << availableRect;

  return true;
}

void
CCefClientDelegate::OnPopupShow(CefRefPtr<CefBrowser> browser, bool show)
{
  if (!IsValidBrowser(browser))
    return;

  pCefViewPrivate_->onOsrShowPopup(show);
}

void
CCefClientDelegate::OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect)
{
  if (!IsValidBrowser(browser))
    return;

  pCefViewPrivate_->onOsrResizePopup(QRect{ rect.x, rect.y, rect.width, rect.height });
}

void
CCefClientDelegate::OnPaint(CefRefPtr<CefBrowser> browser,
                            CefRenderHandler::PaintElementType type,
                            const CefRenderHandler::RectList& dirtyRects,
                            const void* buffer,
                            int width,
                            int height)
{
  if (!IsValidBrowser(browser))
    return;

  QImage frame = QImage(static_cast<const uchar*>(buffer), width, height, QImage::Format_ARGB32_Premultiplied);

  if (PET_VIEW == type) {
    pCefViewPrivate_->onOsrUpdateViewFrame(frame, dirtyRects);
  } else if (PET_POPUP == type) {
    pCefViewPrivate_->onOsrUpdatePopupFrame(frame, dirtyRects);
  } else {
  }
}

void
CCefClientDelegate::OnAcceleratedPaint(CefRefPtr<CefBrowser> browser,
                                       CefRenderHandler::PaintElementType type,
                                       const CefRenderHandler::RectList& dirtyRects,
                                       void* shared_handle)
{}

bool
CCefClientDelegate::StartDragging(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefDragData> drag_data,
                                  CefRenderHandler::DragOperationsMask allowed_ops,
                                  int x,
                                  int y)
{
  return false;
}

void
CCefClientDelegate::UpdateDragCursor(CefRefPtr<CefBrowser> browser, CefRenderHandler::DragOperation operation)
{
  return;
}

void
CCefClientDelegate::OnScrollOffsetChanged(CefRefPtr<CefBrowser> browser, double x, double y)
{}

void
CCefClientDelegate::OnImeCompositionRangeChanged(CefRefPtr<CefBrowser> browser,
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
    QMetaObject::invokeMethod(pCefViewPrivate_,             //
                              "onOsrImeCursorRectChanged",  //
                              Q_ARG(const QRect&, rc));
  }

  return;
}

void
CCefClientDelegate::OnTextSelectionChanged(CefRefPtr<CefBrowser> browser,
                                           const CefString& selected_text,
                                           const CefRange& selected_range)
{}

void
CCefClientDelegate::OnVirtualKeyboardRequested(CefRefPtr<CefBrowser> browser,
                                               CefRenderHandler::TextInputMode input_mode)
{}

#endif
