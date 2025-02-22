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
