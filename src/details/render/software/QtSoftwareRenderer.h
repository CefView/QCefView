#ifndef QTSOFTWARERENDERER_H
#define QTSOFTWARERENDERER_H

#pragma once

#include <QImage>
#include <QMutex>

#include "../ICefViewRenderer.h"

class QtSoftwareRenderer : public ICefViewRenderer
{
private:
  bool m_showPopup = false;
  CefRect m_popupRect;
  CefColor m_backgroundColor = 0;

  float m_scale = 1.0f;
  int m_width = 1;
  int m_height = 1;

  QMutex m_qViewPaintLock;
  QImage m_qCefViewFrame;
  QMutex m_qPopupPaintLock;
  QImage m_qCefPopupFrame;

public:
  QtSoftwareRenderer();
  ~QtSoftwareRenderer();

  bool isHardware() override { return false; }

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
