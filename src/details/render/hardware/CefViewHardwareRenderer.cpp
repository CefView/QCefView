#include "CefViewHardwareRenderer.h"

CefViewHardwareRenderer::CefViewHardwareRenderer() {}

CefViewHardwareRenderer::~CefViewHardwareRenderer() {}


void
CefViewHardwareRenderer::setBackgroundColor(const CefColor& color)
{
}

void
CefViewHardwareRenderer::updatePopupVisibility(bool visible)
{
}

void
CefViewHardwareRenderer::updatePopupRect(const CefRect& rect)
{
}

void
CefViewHardwareRenderer::updateFrame(const CefRenderHandler::PaintElementType& type,
                                     const CefRenderHandler::RectList& dirtyRects,
                                     const void* buffer,
                                     const CefSize& size)
{
  throw std::logic_error("The method or operation is not implemented.");
}

void
CefViewHardwareRenderer::render(void* painter, int width, int height)
{
}

void
CefViewHardwareRenderer::updateTexture(const CefRenderHandler::PaintElementType& type,
                                       const CefRenderHandler::RectList& dirtyRects,
                                       const void* handle,
                                       int format)
{
  throw std::logic_error("The method or operation is not implemented.");
}
