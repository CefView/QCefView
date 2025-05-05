#ifndef OPENGLRENDERBACKEND_H
#define OPENGLRENDERBACKEND_H

#pragma once
#include "details/render/ICefViewRenderer.h"

class OpenGLRenderBackend : public ICefViewRenderer
{
public:
  OpenGLRenderBackend();

  ~OpenGLRenderBackend();

  bool isHardware() override { return true; }

  bool initialize(void* wid, int width, int height, float scale, const CefColor& background) override;

  void uninitialize() override;

  void resize(int width, int height, float scale) override;

  void updatePopupVisibility(bool visible) override;

  void updatePopupRect(const CefRect& rect) override;

  void updateFrameData(const CefRenderHandler::PaintElementType& type,
                       const CefRenderHandler::RectList& dirtyRects,
                       const FrameDataType& dataType,
                       const FrameData& data) override;

  void render(void* painter) override;
};
#endif
