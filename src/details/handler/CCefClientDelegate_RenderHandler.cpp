#include "details/CCefClientDelegate.h"

#include <QApplication>
#include <QDebug>
#include <QImage>
#include <QScreen>

#include "details/QCefViewPrivate.h"

bool
CCefClientDelegate::getRootScreenRect(CefRefPtr<CefBrowser>& browser, CefRect& rect)
{
  return false;
}

void
CCefClientDelegate::getViewRect(CefRefPtr<CefBrowser>& browser, CefRect& rect)
{
  if (!IsValidBrowser(browser)) {
    rect.Set(0, 0, 1, 1);
    return;
  }

  auto size = pCefViewPrivate_->q_ptr->size();
  rect.Set(0, 0, size.width() ? size.width() : 1, size.height() ? size.height() : 1);
}

bool
CCefClientDelegate::getScreenPoint(CefRefPtr<CefBrowser>& browser, int viewX, int viewY, int& screenX, int& screenY)
{
  if (!IsValidBrowser(browser))
    return false;

  auto ptScreen = pCefViewPrivate_->q_ptr->mapToGlobal(QPoint(viewX, viewY));
  screenX = ptScreen.x();
  screenY = ptScreen.y();
  return true;
}

bool
CCefClientDelegate::getScreenInfo(CefRefPtr<CefBrowser>& browser, CefScreenInfo& screen_info)
{
  if (!IsValidBrowser(browser))
    return false;

  auto size = pCefViewPrivate_->q_ptr->size();
  screen_info.rect = { 0, 0, size.width(), size.height() };
  screen_info.available_rect = { 0, 0, size.width(), size.height() };
  screen_info.device_scale_factor = pCefViewPrivate_->scaleFactor();
  return true;
}

void
CCefClientDelegate::onPopupShow(CefRefPtr<CefBrowser>& browser, bool show)
{
  if (!IsValidBrowser(browser))
    return;

  pCefViewPrivate_->osr.pRenderer_->updatePopupVisibility(show);
}

void
CCefClientDelegate::onPopupSize(CefRefPtr<CefBrowser>& browser, const CefRect& rect)
{
  if (!IsValidBrowser(browser))
    return;

  pCefViewPrivate_->osr.pRenderer_->updatePopupRect(rect);
}

void
CCefClientDelegate::onPaint(CefRefPtr<CefBrowser>& browser,
                            CefRenderHandler::PaintElementType type,
                            const CefRenderHandler::RectList& dirtyRects,
                            const void* buffer,
                            int width,
                            int height)
{
  if (!IsValidBrowser(browser))
    return;

  // update CEF image frame
  ICefViewRenderer::FrameData data;
  data.image.buffer = buffer;
  data.image.width = width;
  data.image.height = height;
  ICefViewRenderer::FrameDataType dataType = ICefViewRenderer::FrameDataType::CpuImage;
  pCefViewPrivate_->osr.pRenderer_->updateFrameData(type,       //
                                                    dirtyRects, //
                                                    dataType,   //
                                                    data        //
  );
}

#if CEF_VERSION_MAJOR < 125
//
// For CEF version below 125, hardware acceleration is not supported officially.
// You need to compile your own CEF with a patch applied. Please refer to:
// https://www.magpcss.org/ceforum/viewtopic.php?f=6&t=17551
// You need to build CEF/Chromium locally and apply this PR:
// https://bitbucket.org/chromiumembedded/cef/pull-requests/285
//
void
CCefClientDelegate::onAcceleratedPaint(CefRefPtr<CefBrowser>& browser,
                                       CefRenderHandler::PaintElementType type,
                                       const CefRenderHandler::RectList& dirtyRects,
                                       void* shared_handle)
{
  if (!IsValidBrowser(browser))
    return;

  // update CEF image texture2d
  ICefViewRenderer::FrameData data;
  data.texture.handle = shared_handle;
  ICefViewRenderer::FrameDataType dataType = ICefViewRenderer::FrameDataType::GpuTexture;
  pCefViewPrivate_->osr.pRenderer_->updateFrameData(type,       //
                                                    dirtyRects, //
                                                    dataType,   //
                                                    data        //
  );
}
#else
void
CCefClientDelegate::onAcceleratedPaint(CefRefPtr<CefBrowser>& browser,
                                       CefRenderHandler::PaintElementType type,
                                       const CefRenderHandler::RectList& dirtyRects,
                                       const CefAcceleratedPaintInfo& info)
{
  if (!IsValidBrowser(browser))
    return;

  // update CEF image texture2d
  ICefViewRenderer::FrameData data;
#if defined(OS_WINDOWS)
  data.texture.handle = info.shared_texture_handle;
#elif defined(OS_MACOS)
  data.texture.handle = info.shared_texture_io_surface;
#elif defined(OS_LINUX)
  if (info.plane_count) {
    data.texture.handle = reinterpret_cast<void*>(info.planes[0].fd);
    data.texture.size = info.planes[0].size;
  } else {
    data.texture.handle = nullptr;
  }
#else
#error "Unsupported platform"
#endif
  data.texture.format = info.format;
  ICefViewRenderer::FrameDataType dataType = ICefViewRenderer::FrameDataType::GpuTexture;
  pCefViewPrivate_->osr.pRenderer_->updateFrameData(type,       //
                                                    dirtyRects, //
                                                    dataType,   //
                                                    data        //
  );
}
#endif

bool
CCefClientDelegate::startDragging(CefRefPtr<CefBrowser>& browser,
                                  CefRefPtr<CefDragData>& dragData,
                                  CefRenderHandler::DragOperationsMask allowedOps,
                                  int x,
                                  int y)
{
  if (!IsValidBrowser(browser))
    return false;

  return pCefViewPrivate_->shouldAllowDrag(dragData, allowedOps, x, y);
}

void
CCefClientDelegate::updateDragCursor(CefRefPtr<CefBrowser>& browser, CefRenderHandler::DragOperation operation)
{
  if (!IsValidBrowser(browser))
    return;

  return pCefViewPrivate_->updateDragOperation(operation);
}

void
CCefClientDelegate::onScrollOffsetChanged(CefRefPtr<CefBrowser>& browser, double x, double y)
{
}

void
CCefClientDelegate::onImeCompositionRangeChanged(CefRefPtr<CefBrowser>& browser,
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
CCefClientDelegate::onTextSelectionChanged(CefRefPtr<CefBrowser>& browser,
                                           const CefString& selected_text,
                                           const CefRange& selected_range)
{
}

void
CCefClientDelegate::onVirtualKeyboardRequested(CefRefPtr<CefBrowser>& browser,
                                               CefRenderHandler::TextInputMode input_mode)
{
}
