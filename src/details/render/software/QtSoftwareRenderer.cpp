#include "QtSoftwareRenderer.h"

#include <QDebug>
#include <QMutexLocker>
#include <QPainter>
#include <QWindow>

QtSoftwareRenderer::QtSoftwareRenderer() {}

QtSoftwareRenderer::~QtSoftwareRenderer() {}

bool
QtSoftwareRenderer::initialize(QWidget* widget, int width, int height, float scale, const QColor& clear)
{
  if (!widget) {
    return false;
  }
  m_widget = widget;

  return true;
}

void
QtSoftwareRenderer::uninitialize()
{
  m_widget.clear();
}

void
QtSoftwareRenderer::resize(int width, int height, float scale)
{
}

void
QtSoftwareRenderer::render()
{
  auto size = widgetSize();
  if (size.isEmpty()) {
    return;
  }

  auto clear = widgetBackground();
  auto scale = widgetScale();
  {
    QPainter painter(m_widget);
    painter.fillRect(QRect(QPoint(), size), QColor(clear));

    // paint cef view
    {
      QMutexLocker locker(&m_cefViewLock);
      painter.drawImage(
        QRect{
          0,                                                //
          0,                                                //
          static_cast<int>(m_cefViewImage.width() / scale), //
          static_cast<int>(m_cefViewImage.height() / scale) //
        },
        m_cefViewImage);
    }

    if (!m_showPopup) {
      return;
    }

    // paint cef popup
    {
      QMutexLocker locker(&m_cefPopupLock);
      painter.drawImage(
        QRect{
          m_popupRect.x,                                     //
          m_popupRect.y,                                     //
          static_cast<int>(m_cefPopupImage.width() / scale), //
          static_cast<int>(m_cefPopupImage.height() / scale) //
        },
        m_cefPopupImage);
    }
  }

  return;
}

void
QtSoftwareRenderer::updatePopupVisibility(bool visible)
{
  m_showPopup = visible;

  if (!visible) {
    m_popupRect = CefRect();
    m_cefPopupImage = QImage();
  }
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
  QImage frame = QImage(static_cast<const uchar*>(data.image.buffer), //
                        data.image.width,                             //
                        data.image.height,                            //
                        QImage::Format_ARGB32);

  // ref target image
  QImage targetImage;
  if (PET_VIEW == type) {
    targetImage = m_cefViewImage;
  } else if (PET_POPUP == type) {
    targetImage = m_cefPopupImage;
  } else {
    return;
  }

  if (targetImage.size() != frame.size()           //
      || (                                         //
           dirtyRects.size() >= 1 &&               //
           dirtyRects[0].x == 0 &&                 //
           dirtyRects[0].y == 0 &&                 //
           dirtyRects[0].width == frame.width() && //
           dirtyRects[0].height == frame.height()  //
           )                                       //
  ) {
    // update full image
    targetImage = frame.copy();
  } else {
    //  update only dirty regions
    QPainter painter(&targetImage);
    for (auto& rc : dirtyRects) {
      QRect rect(rc.x, rc.y, rc.width, rc.height);
      painter.drawImage(rect, frame, rect);
    }
  }

  // ref target image
  if (PET_VIEW == type) {
    QMutexLocker locker(&m_cefViewLock);
    m_cefViewImage = targetImage;
  } else if (PET_POPUP == type) {
    QMutexLocker locker(&m_cefPopupLock);
    m_cefPopupImage = targetImage;
  } else {
    return;
  }

  // trigger render
  auto widget = m_widget;
  QMetaObject::invokeMethod(m_widget, [widget]() {
    // request update
    if (widget)
      widget->update();
  });
}
