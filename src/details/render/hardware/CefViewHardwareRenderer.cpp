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
#error "Unsupported platform"
#endif
{
}

CefViewHardwareRenderer::~CefViewHardwareRenderer()
{
  pBackend_.reset();
}

bool
CefViewHardwareRenderer::initialize(void* wid, int width, int height, float scale, const CefColor& background)
{
  return pBackend_->initialize(wid, width, height, scale, background);
}

void
CefViewHardwareRenderer::uninitialize()
{
  pBackend_->uninitialize();
}

void
CefViewHardwareRenderer::resize(int width, int height, float scale)
{
  pBackend_->resize(width, height, scale);
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
CefViewHardwareRenderer::updateFrameData(const CefRenderHandler::PaintElementType& type,
                                         const CefRenderHandler::RectList& dirtyRects,
                                         const FrameDataType& dataType,
                                         const FrameData& data)
{
  pBackend_->updateFrameData(type, dirtyRects, dataType, data);
}

void
CefViewHardwareRenderer::render(void* painter)
{
  pBackend_->render(nullptr);
}
