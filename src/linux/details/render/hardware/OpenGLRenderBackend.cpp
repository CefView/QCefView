#include "OpenGLRenderBackend.h"

OpenGLRenderBackend::OpenGLRenderBackend() {}

OpenGLRenderBackend::~OpenGLRenderBackend() {}

bool
OpenGLRenderBackend::initialize(QWidget* widget, int width, int height, float scale, const QColor& clear)
{
  // TODO: it seems hardware acceleration is not supported by CEF currently.
  return false;
}

void
OpenGLRenderBackend::uninitialize()
{
}

void
OpenGLRenderBackend::resize(int width, int height, float scale)
{
}

void
OpenGLRenderBackend::updatePopupVisibility(bool visible)
{
}

void
OpenGLRenderBackend::updatePopupRect(const CefRect& rect)
{
}

void
OpenGLRenderBackend::updateFrameData(const CefRenderHandler::PaintElementType& type,
                                     const CefRenderHandler::RectList& dirtyRects,
                                     const FrameDataType& dataType,
                                     const FrameData& data)
{
}

void
OpenGLRenderBackend::render()
{
}
