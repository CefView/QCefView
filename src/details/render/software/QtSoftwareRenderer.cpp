#include "QtSoftwareRenderer.h"

#include <QMutexLocker>
#include <QPainter>

QtSoftwareRenderer::QtSoftwareRenderer() {}

QtSoftwareRenderer::~QtSoftwareRenderer() {}

bool
QtSoftwareRenderer::initialize(void* wid, int width, int height, float scale, const CefColor& background)
{
  m_width = width;
  m_height = height;
  m_scale = scale;
  m_backgroundColor = background;
  return true;
}

void
QtSoftwareRenderer::uninitialize()
{
}

void
QtSoftwareRenderer::resize(int width, int height, float scale)
{
  QMutexLocker lock(&(m_qViewPaintLock));
  m_width = width;
  m_height = height;
}

void
QtSoftwareRenderer::updatePopupVisibility(bool visible)
{
  m_showPopup = visible;

  // clear the previous popup frame data if exist
  m_qCefPopupFrame = QImage();
  m_popupRect = CefRect();
}

void
QtSoftwareRenderer::updatePopupRect(const CefRect& rect)
{
  m_popupRect = rect;
}

void
QtSoftwareRenderer::updateFrameData(const CefRenderHandler::PaintElementType& type,
                                    const CefRenderHandler::RectList& dirtyRects,
                                    const FrameDataType& dataType,
                                    const FrameData& data)
{
  // we only process Image data
  if (dataType != FrameDataType::CpuImage) {
    return;
  }

  // create image wrapper
  QImage frame = QImage(static_cast<const uchar*>(data.image.buffer),
                        data.image.width,
                        data.image.height,
                        QImage::Format_ARGB32_Premultiplied);

  if (PET_VIEW == type) {
    if (m_qCefViewFrame.size() != frame.size() || CefColorGetA(m_backgroundColor) == 0) {
      // update full image
      QMutexLocker lock(&(m_qViewPaintLock));
      m_qCefViewFrame = frame.copy();
    } else {
      QMutexLocker lock(&(m_qViewPaintLock));
      // update only dirty regions
      QPainter painter(&m_qCefViewFrame);
      for (auto& rc : dirtyRects) {
        QRect rect(rc.x, rc.y, rc.width, rc.height);
        painter.drawImage(rect, frame, rect);
      }
    }
  } else if (PET_POPUP == type) {
    if (m_qCefPopupFrame.size() == frame.size()) {
      QMutexLocker lock(&(m_qPopupPaintLock));
      // update region
      QPainter painter(&m_qCefPopupFrame);
      for (auto& rc : dirtyRects) {
        QRect rect(rc.x, rc.y, rc.width, rc.height);
        painter.drawImage(rect, frame, rect);
      }
    } else {
      QMutexLocker lock(&(m_qPopupPaintLock));
      m_qCefPopupFrame = frame.copy();
    }
  } else {
  }
}

void
QtSoftwareRenderer::render(void* painter)
{
  if (!painter) {
    return;
  }

  // perform the painting
  QPainter* p = reinterpret_cast<QPainter*>(painter);
  {
    // paint cef view
    QMutexLocker lock(&(m_qViewPaintLock));
    p->drawImage(QRect{ 0, 0, m_width, m_height }, m_qCefViewFrame);
  }

  {
    // paint cef popup
    QMutexLocker lock(&(m_qPopupPaintLock));
    if (m_showPopup) {
      p->drawImage(QRect{ m_popupRect.x, m_popupRect.y, m_popupRect.width, m_popupRect.height }, m_qCefPopupFrame);
    }
  }
}
