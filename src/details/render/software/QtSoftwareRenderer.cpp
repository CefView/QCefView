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
  m_backingStore.reset(new QBackingStore(widget->windowHandle()));

  return true;
}

void
QtSoftwareRenderer::uninitialize()
{
  m_backingStore.reset();
}

void
QtSoftwareRenderer::resize(int width, int height, float scale)
{
  if (CefCurrentlyOn(TID_UI)) {
    // resize backingstore
    m_backingStore->resize(QSize(width, height));
    // render immediately
    render();
  } else {
    // perform the resize in render thread
    QPointer<QtSoftwareRenderer> self = this;
    CefPostTask(TID_UI, new RenderTask([=]() {
                  if (self) {
                    // resize backingstore
                    m_backingStore->resize(QSize(width, height));
                    // render immediately
                    self->render();
                  }
                }));
  }
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

  if (PET_VIEW == type) {
    if (m_cefViewImage.size() != frame.size()        //
        || (                                         //
             dirtyRects.size() >= 1 &&               //
             dirtyRects[0].x == 0 &&                 //
             dirtyRects[0].y == 0 &&                 //
             dirtyRects[0].width == frame.width() && //
             dirtyRects[0].height == frame.height()  //
             )                                       //
    ) {
      // update full image
      m_cefViewImage = frame.copy();
    } else {
      //  update only dirty regions
      QPainter painter(&m_cefViewImage);
      for (auto& rc : dirtyRects) {
        QRect rect(rc.x, rc.y, rc.width, rc.height);
        painter.drawImage(rect, frame, rect);
      }
    }
  } else if (PET_POPUP == type) {
    if (m_cefPopupImage.size() != frame.size()       //
        || (                                         //
             dirtyRects.size() >= 1 &&               //
             dirtyRects[0].x == 0 &&                 //
             dirtyRects[0].y == 0 &&                 //
             dirtyRects[0].width == frame.width() && //
             dirtyRects[0].height == frame.height()  //
             )                                       //
    ) {
      // update full image
      m_cefPopupImage = frame.copy();
    } else {
      //  update only dirty regions
      QPainter painter(&m_cefPopupImage);
      for (auto& rc : dirtyRects) {
        QRect rect(rc.x, rc.y, rc.width, rc.height);
        painter.drawImage(rect, frame, rect);
      }
    }
  } else {
    return;
  }

  render();
}

void
QtSoftwareRenderer::render()
{
  auto scale = widgetScale();
  auto size = widgetSize();
  auto clear = widgetBackground();

  if (!m_backingStore || size.isEmpty()) {
    return;
  }

  // resize if not match
  if (m_backingStore->size() != size)
    m_backingStore->resize(size);

  // begin paint
  QRect paintRegion(QPoint(), size);
  m_backingStore->beginPaint(paintRegion);

  // acquire paint device
  if (QPaintDevice* device = m_backingStore->paintDevice()) {
    QPainter painter(device);
    // fill background
    painter.fillRect(paintRegion, QColor(clear));

    // paint cef view
    {
      painter.drawImage(
        QRect{
          0,                                                //
          0,                                                //
          static_cast<int>(m_cefViewImage.width() / scale), //
          static_cast<int>(m_cefViewImage.height() / scale) //
        },
        m_cefViewImage);
    }

    // paint cef popup
    if (m_showPopup) {
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

  m_backingStore->endPaint();
  m_backingStore->flush(paintRegion);
  return;
}
