#include "CefViewHardwareRenderer.h"

#if defined(OS_WINDOWS)
#include "win/details/render/hardware/DX11RenderBackend.h"
#elif defined(OS_MACOS)
#include "mac/details/render/hardware/MetalRenderBackend.h"
#elif defined(OS_LINUX)
#include "linux/details/render/hardware/OpenGLRenderBackend.h"
#else
#error "Unsupported platform"
#endif

CefViewRendererPtr
createHardwareRenderer()
{
#if defined(OS_WINDOWS)
  return std::make_shared<DX11RenderBackend>();
#elif defined(OS_MACOS)
  return std::make_shared<MetalRenderBackend>();
#elif defined(OS_LINUX)
  return std::make_shared<OpenGLRenderBackend>();
#else
#error "Unsupported platform"
#endif
}
