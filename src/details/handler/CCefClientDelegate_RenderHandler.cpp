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
  rect.Set(0, 0, 1, 1);
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  auto size = QSize(1, 1);

  runInMainThreadAndWait([&]() {
    if (pCefViewPrivate->q_ptr) {
      size = pCefViewPrivate->q_ptr->size();
    }
  });

  rect.Set(0, 0, size.width() ? size.width() : 1, size.height() ? size.height() : 1);
}

bool
CCefClientDelegate::getScreenPoint(CefRefPtr<CefBrowser>& browser, int viewX, int viewY, int& screenX, int& screenY)
{
  AcquireAndValidateCefViewPrivateWithReturn(pCefViewPrivate, false);

  auto ptScreen = QPoint(0, 0);

  runInMainThreadAndWait([&]() {
    if (pCefViewPrivate->q_ptr) {
      ptScreen = pCefViewPrivate->q_ptr->mapToGlobal(QPoint(viewX, viewY));
    }
  });

  screenX = ptScreen.x();
  screenY = ptScreen.y();
  return true;
}

bool
CCefClientDelegate::getScreenInfo(CefRefPtr<CefBrowser>& browser, CefScreenInfo& screen_info)
{
  AcquireAndValidateCefViewPrivateWithReturn(pCefViewPrivate, false);

  auto size = QSize(0, 0);
  auto scale = 0.0f;
  runInMainThreadAndWait([&]() {
    if (pCefViewPrivate->q_ptr) {
      size = pCefViewPrivate->q_ptr->size();
      scale = pCefViewPrivate->scaleFactor();
    }
  });

  screen_info.rect = { 0, 0, size.width(), size.height() };
  screen_info.available_rect = { 0, 0, size.width(), size.height() };
  screen_info.device_scale_factor = scale;
  return true;
}

void
CCefClientDelegate::onPopupShow(CefRefPtr<CefBrowser>& browser, bool show)
{
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  pCefViewPrivate->osr.pRenderer_->updatePopupVisibility(show);
}

void
CCefClientDelegate::onPopupSize(CefRefPtr<CefBrowser>& browser, const CefRect& rect)
{
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  pCefViewPrivate->osr.pRenderer_->updatePopupRect(rect);
}

void
CCefClientDelegate::onPaint(CefRefPtr<CefBrowser>& browser,
                            CefRenderHandler::PaintElementType type,
                            const CefRenderHandler::RectList& dirtyRects,
                            const void* buffer,
                            int width,
                            int height)
{
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  // update CEF image frame
  ICefViewRenderer::FrameData data;
  data.pixelInfo.buffer = buffer;
  data.pixelInfo.width = width;
  data.pixelInfo.height = height;
  ICefViewRenderer::FrameDataType dataType = ICefViewRenderer::FrameDataType::CpuPixel;
  pCefViewPrivate->osr.pRenderer_->updateFrameData(type,       //
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
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  // update CEF image texture2d
  ICefViewRenderer::FrameData data;
  data.textureInfo.handle = shared_handle;
  ICefViewRenderer::FrameDataType dataType = ICefViewRenderer::FrameDataType::GpuTexture;
  pCefViewPrivate->osr.pRenderer_->updateFrameData(type,       //
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
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  // update CEF image texture2d
  ICefViewRenderer::FrameData data;
#if defined(OS_WINDOWS)
  data.textureInfo.handle = info.shared_texture_handle;
#elif defined(OS_MACOS)
  data.textureInfo.handle = info.shared_texture_io_surface;
#elif defined(OS_LINUX)
  if (info.plane_count) {
    data.textureInfo.handle = reinterpret_cast<void*>(info.planes[0].fd);
    data.textureInfo.size = info.planes[0].size;
  } else {
    data.textureInfo.handle = nullptr;
  }
#else
#error "Unsupported platform"
#endif
  data.textureInfo.format = info.format;
  ICefViewRenderer::FrameDataType dataType = ICefViewRenderer::FrameDataType::GpuTexture;
  pCefViewPrivate->osr.pRenderer_->updateFrameData(type,       //
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
  AcquireAndValidateCefViewPrivateWithReturn(pCefViewPrivate, false);

  return pCefViewPrivate->shouldAllowDrag(dragData, allowedOps, x, y);
}

void
CCefClientDelegate::updateDragCursor(CefRefPtr<CefBrowser>& browser, CefRenderHandler::DragOperation operation)
{
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  qDebug() << "CCefClientDelegate::updateDragCursor:" << operation;

  return pCefViewPrivate->updateDragOperation(operation);
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
  AcquireAndValidateCefViewPrivate(pCefViewPrivate);

  // qDebug() << "OnImeCompositionRangeChanged:" << selected_range.from << " - " << selected_range.to
  //         << ", RC:" << character_bounds.size();

  if (!character_bounds.empty()) {
    auto r = character_bounds[character_bounds.size() - 1];
    QRect rc(r.x, r.y, r.width, r.height);
    QMetaObject::invokeMethod(pCefViewPrivate.data(),      //
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
