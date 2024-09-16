#ifndef CEFVIEWSOFTWARERENDERER_H
#define CEFVIEWSOFTWARERENDERER_H

#pragma once

#include <QImage>
#include <QMutex>

#include "../ICefViewRenderer.h"

class CefViewSoftwareRenderer : public ICefViewRenderer
{
private:
  CefColor backgroundColor_;

  bool showPopup_ = false;
  CefRect popupRect_;

  QMutex qViewPaintLock_;
  QImage qCefViewFrame_;
  QMutex qPopupPaintLock_;
  QImage qCefPopupFrame_;

public:
  CefViewSoftwareRenderer();
  ~CefViewSoftwareRenderer();

  bool isHardware() override { return false; }

  void setBackgroundColor(const CefColor& color) override;

  void updatePopupVisibility(bool visible) override;

  void updatePopupRect(const CefRect& rect) override;

  void updateFrame(const CefRenderHandler::PaintElementType& type,
                   const CefRenderHandler::RectList& dirtyRects,
                   const void* buffer,
                   const CefSize& size) override;

  void render(void* painter, int width, int height) override;

  void updateTexture(const CefRenderHandler::PaintElementType& type,
                     const CefRenderHandler::RectList& dirtyRects,
                     const void* handle,
                     int format) override;
};

#endif
