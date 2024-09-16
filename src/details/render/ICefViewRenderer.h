#ifndef ICEFVIEWRENDERER_H
#define ICEFVIEWRENDERER_H

#pragma once

#include <include/cef_app.h>

using CefColor = cef_color_t;

class ICefViewRenderer
{
public:
  virtual ~ICefViewRenderer() {};

  virtual bool isHardware() = 0;

  virtual void setBackgroundColor(const CefColor& color) = 0;

  virtual void updatePopupVisibility(bool visible) = 0;

  virtual void updatePopupRect(const CefRect& rect) = 0;

  virtual void updateFrame(const CefRenderHandler::PaintElementType& type,
                           const CefRenderHandler::RectList& dirtyRects,
                           const void* buffer,
                           const CefSize& size) = 0;

  virtual void updateTexture(const CefRenderHandler::PaintElementType& type,
                             const CefRenderHandler::RectList& dirtyRects,
                             const void* handle,
                             int format) = 0;

  virtual void render(void* painter, int width, int height) = 0;
};

#endif
