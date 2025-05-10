#ifndef METALRENDERBACKEND_H
#define METALRENDERBACKEND_H

#pragma once

#include <memory>

#include "details/render/ICefViewRenderer.h"

class MetalRenderBackend : public ICefViewRenderer
{
  Q_OBJECT

private:
  class Implementation;
  std::unique_ptr<Implementation> m_pImpl;

public:
  MetalRenderBackend();

  ~MetalRenderBackend();

  bool isHardware() override { return true; }

  bool initialize(QWidget* widget, int width, int height, float scale, const QColor& clear) override;

  void uninitialize() override;

  void resize(int width, int height, float scale) override;

  void updatePopupVisibility(bool visible) override;

  void updatePopupRect(const CefRect& rect) override;

  void updateFrameData(const CefRenderHandler::PaintElementType& type,
                       const CefRenderHandler::RectList& dirtyRects,
                       const FrameDataType& dataType,
                       const FrameData& data) override;

  void render() override;
};

#endif
