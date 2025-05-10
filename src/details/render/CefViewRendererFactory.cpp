#include "CefViewRendererFactory.h"

#include "hardware/CefViewHardwareRenderer.h"
#include "software/QtSoftwareRenderer.h"

namespace CefViewRendererFactory {
CefViewRendererPtr
createRenderer(bool hardware)
{
  if (hardware) {
    return createHardwareRenderer();
  } else {
    return CefViewRendererPtr(new QtSoftwareRenderer);
  }
}
} // namespace CefViewRendererFactory
