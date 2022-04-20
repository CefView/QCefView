#include "CCefClientDelegate.h"

#include <QDebug>
#include <QImage>
#include <QScreen>

#include "QCefViewPrivate.h"

bool
CCefClientDelegate::GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
  return false;
  if (!IsValidBrowser(browser))
    return false;

  QRect rcWindow = pCefViewPrivate_->q_ptr->window()->frameGeometry();
  rect.Set(rcWindow.left(), rcWindow.right(), rcWindow.width(), rcWindow.height());
  qDebug() << "======== GetRootScreenRect:" << rcWindow;
  return true;
}

void
CCefClientDelegate::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
  if (!IsValidBrowser(browser)) {
    rect.Set(0, 0, 1, 1);
    return;
  }

  QSize rcSize = pCefViewPrivate_->q_ptr->size();
  QPoint ptScreen = pCefViewPrivate_->q_ptr->mapToGlobal(QPoint(0, 0));
  rect.Set(ptScreen.x(), ptScreen.y(), rcSize.width(), rcSize.height());
  qDebug() << "======== GetViewRect:" << QRect{ ptScreen.x(), ptScreen.y(), rcSize.width(), rcSize.height() };
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

  QScreen* screen = pCefViewPrivate_->q_ptr->screen();
  QRect rect = screen->geometry();
  QRect availableRect = screen->availableGeometry();
  screen_info.Set(screen->devicePixelRatio(),                                                                    //
                  screen->depth(),                                                                               //
                  0,                                                                                             //
                  false,                                                                                         //
                  CefRect{ rect.x(), rect.y(), rect.width(), rect.height() },                                    //
                  CefRect{ availableRect.x(), availableRect.y(), availableRect.width(), availableRect.height() } //
  );

  qDebug() << "======== GetScreenInfo:" << rect << ", availableRect" << availableRect;

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

  qDebug() << "======== OnPopupSize:" << QRect{ rect.x, rect.y, rect.width, rect.height };

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

  QRegion region;
  CefRenderHandler::RectList::const_iterator it = dirtyRects.begin();
  for (; it != dirtyRects.end(); ++it) {
    region += QRegion(it->x, it->y, it->width, it->height);
  }

  QImage frame = QImage(static_cast<const uchar*>(buffer), width, height, QImage::Format_ARGB32_Premultiplied);

  if (PET_VIEW == type) {
    pCefViewPrivate_->onOsrUpdateViewFrame(frame, region);
  } else if (PET_POPUP == type) {
    pCefViewPrivate_->onOsrUpdatePopupFrame(frame, region);
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
                              "onImeCursorRectChanged",     //
                              Qt::BlockingQueuedConnection, //
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
