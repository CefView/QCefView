#include "CefViewRendererFactory.h"

#include "hardware/CefViewHardwareRenderer.h"
#include "software/CefViewSoftwareRenderer.h"

std::shared_ptr<ICefViewRenderer>
CefViewRendererFactory::createRenderer(bool hardware)
{
  if (hardware) {
    return std::make_shared<CefViewHardwareRenderer>();
  } else {
    return std::make_shared<CefViewSoftwareRenderer>();
  }
}
