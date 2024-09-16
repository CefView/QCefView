#include "CefViewSoftwareRenderer.h"

#include <QMutexLocker>
#include <QPainter>

CefViewSoftwareRenderer::CefViewSoftwareRenderer() {}

CefViewSoftwareRenderer::~CefViewSoftwareRenderer() {}

void
CefViewSoftwareRenderer::setBackgroundColor(const CefColor& color)
{
  backgroundColor_ = color;
}

void
CefViewSoftwareRenderer::updatePopupVisibility(bool visible)
{
  showPopup_ = visible;

  // clear the previous popup frame data if exist
  qCefPopupFrame_ = QImage();
  popupRect_ = CefRect();
}

void
CefViewSoftwareRenderer::updatePopupRect(const CefRect& rect)
{
  popupRect_ = rect;
}

void
CefViewSoftwareRenderer::updateFrame(const CefRenderHandler::PaintElementType& type,
                                     const CefRenderHandler::RectList& dirtyRects,
                                     const void* buffer,
                                     const CefSize& size)
{
  // create image wrapper
  QImage frame =
    QImage(static_cast<const uchar*>(buffer), size.width, size.height, QImage::Format_ARGB32_Premultiplied);

  if (PET_VIEW == type) {
    if (qCefViewFrame_.size() != frame.size() || CefColorGetA(backgroundColor_) == 0) {
      // update full image
      QMutexLocker lock(&(qViewPaintLock_));
      qCefViewFrame_ = frame.copy();
    } else {
      QMutexLocker lock(&(qViewPaintLock_));
      // update only dirty regions
      QPainter painter(&qCefViewFrame_);
      for (auto& rc : dirtyRects) {
        QRect rect(rc.x, rc.y, rc.width, rc.height);
        painter.drawImage(rect, frame, rect);
      }
    }
  } else if (PET_POPUP == type) {
    if (qCefPopupFrame_.size() == frame.size()) {
      QMutexLocker lock(&(qPopupPaintLock_));
      // update region
      QPainter painter(&qCefPopupFrame_);
      for (auto& rc : dirtyRects) {
        QRect rect(rc.x, rc.y, rc.width, rc.height);
        painter.drawImage(rect, frame, rect);
      }
    } else {
      QMutexLocker lock(&(qPopupPaintLock_));
      qCefPopupFrame_ = frame.copy();
    }
  } else {
  }
}

void
CefViewSoftwareRenderer::render(void* painter, int width, int height)
{
  if (!painter) {
    return;
  }

  // perform the painting
  QPainter* p = reinterpret_cast<QPainter*>(painter);
  {
    // paint cef view
    QMutexLocker lock(&(qViewPaintLock_));
    p->drawImage(QRect{ 0, 0, width, height }, qCefViewFrame_);
  }

  {
    // paint cef popup
    QMutexLocker lock(&(qPopupPaintLock_));
    if (showPopup_) {
      p->drawImage(QRect{ popupRect_.x, popupRect_.y, popupRect_.width, popupRect_.height }, qCefPopupFrame_);
    }
  }
}

void
CefViewSoftwareRenderer::updateTexture(const CefRenderHandler::PaintElementType& type,
                                       const CefRenderHandler::RectList& dirtyRects,
                                       const void* handle,
                                       int format)
{
  throw std::logic_error("The method or operation is not implemented.");
}
