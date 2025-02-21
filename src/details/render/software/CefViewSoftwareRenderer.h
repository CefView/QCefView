#ifndef CEFVIEWSOFTWARERENDERER_H
#define CEFVIEWSOFTWARERENDERER_H

#pragma once

#include <QImage>
#include <QMutex>

#include "../ICefViewRenderer.h"

class CefViewSoftwareRenderer : public ICefViewRenderer
{
private:
  bool showPopup_ = false;
  CefRect popupRect_;
  CefColor backgroundColor_ = 0;

  QMutex qViewPaintLock_;
  QImage qCefViewFrame_;
  QMutex qPopupPaintLock_;
  QImage qCefPopupFrame_;

public:
  CefViewSoftwareRenderer();
  ~CefViewSoftwareRenderer();

  bool isHardware() override { return false; }

  bool initialize(void* wid) override;

  void uninitialize() override;

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
