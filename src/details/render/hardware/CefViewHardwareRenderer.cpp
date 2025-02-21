#include "CefViewHardwareRenderer.h"

#if defined(OS_WINDOWS)
#include "win/details/render/hardware/DX11RenderBackend.h"
#elif defined(OS_MACOS)
#elif defined(OS_LINUX)
#else
#endif

CefViewHardwareRenderer::CefViewHardwareRenderer()
#if defined(OS_WINDOWS)
  : pBackend_(std::make_unique<DX11RenderBackend>())
#elif defined(OS_MACOS)
  : pBackend_(nullptr)
#elif defined(OS_LINUX)
  : pBackend_(nullptr)
#else
#endif
{
}

CefViewHardwareRenderer::~CefViewHardwareRenderer()
{
  pBackend_.reset();
}

void
CefViewHardwareRenderer::setBackgroundColor(const CefColor& color)
{
  pBackend_->setBackgroundColor(color);
}

void
CefViewHardwareRenderer::updatePopupVisibility(bool visible)
{
  pBackend_->updatePopupVisibility(visible);
}

void
CefViewHardwareRenderer::updatePopupRect(const CefRect& rect)
{
  pBackend_->updatePopupRect(rect);
}

void
CefViewHardwareRenderer::updateFrame(const CefRenderHandler::PaintElementType& type,
                                     const CefRenderHandler::RectList& dirtyRects,
                                     const void* buffer,
                                     const CefSize& size)
{
  pBackend_->updateFrame(type, dirtyRects, buffer, size);
}

void
CefViewHardwareRenderer::updateTexture(const CefRenderHandler::PaintElementType& type,
                                       const CefRenderHandler::RectList& dirtyRects,
                                       const void* handle,
                                       int format)
{
  throw std::logic_error("The method or operation is not implemented.");
}

void
CefViewHardwareRenderer::render(void* painter, int width, int height)
{
  pBackend_->render(nullptr, width, height);
}
