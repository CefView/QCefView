#ifndef CEFVIEWHARDWARERENDERER_H
#define CEFVIEWHARDWARERENDERER_H

#pragma once

#include <memory>

#include "../ICefViewRenderer.h"

class CefViewHardwareRenderer : public ICefViewRenderer
{
private:
  std::unique_ptr<ICefViewRenderer> pBackend_;

public:
  CefViewHardwareRenderer();
  ~CefViewHardwareRenderer();

  bool isHardware() override { return false; }

  void setBackgroundColor(const CefColor& color) override;

  void updatePopupVisibility(bool visible) override;

  void updatePopupRect(const CefRect& rect) override;

  void updateFrame(const CefRenderHandler::PaintElementType& type,
                   const CefRenderHandler::RectList& dirtyRects,
                   const void* buffer,
                   const CefSize& size) override;

  void updateTexture(const CefRenderHandler::PaintElementType& type,
                     const CefRenderHandler::RectList& dirtyRects,
                     const void* handle,
                     int format) override;

  void render(void* painter, int width, int height) override;
};

#endif
