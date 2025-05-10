#ifndef OPENGLRENDERBACKEND_H
#define OPENGLRENDERBACKEND_H

#pragma once

#include "details/render/ICefViewRenderer.h"

class OpenGLRenderBackend : public ICefViewRenderer
{
  Q_OBJECT

public:
  OpenGLRenderBackend();

  ~OpenGLRenderBackend();

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
