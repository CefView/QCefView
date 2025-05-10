#ifndef QTSOFTWARERENDERER_H
#define QTSOFTWARERENDERER_H

#pragma once

#include <QBackingStore>
#include <QImage>

#include "details/render/ICefViewRenderer.h"

class QtSoftwareRenderer : public ICefViewRenderer
{
  Q_OBJECT

private:
  QScopedPointer<QBackingStore> m_backingStore;

  // popup status
  bool m_showPopup = false;
  CefRect m_popupRect;

  // images for internal processing
  QImage m_cefViewImage;
  QImage m_cefPopupImage;

public:
  QtSoftwareRenderer();

  ~QtSoftwareRenderer();

  bool isHardware() override { return false; }

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
