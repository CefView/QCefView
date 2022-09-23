#include "CefViewWidget.h"

#include <QColor>
#include <QPainterPath>
#include <QRandomGenerator>
#include <QResizeEvent>
#include <QWindow>

CefViewWidget::CefViewWidget(const QString url, const QCefSetting* setting, QWidget* parent /* = 0*/)
  : QCefView(url, setting, parent)
{}

CefViewWidget::~CefViewWidget() {}

void
CefViewWidget::onScreenChanged(QScreen* screen)
{
  if (!m_cefWindow)
    return;

  updateMask();
}

void
CefViewWidget::onBrowserWindowCreated(QWindow* win)
{
  m_cefWindow = win;
  if (!m_cefWindow)
    return;

  connect(window()->windowHandle(), SIGNAL(screenChanged(QScreen*)), this, SLOT(onScreenChanged(QScreen*)));

  updateMask();
}

void
CefViewWidget::resizeEvent(QResizeEvent* event)
{
  QCefView::resizeEvent(event);
  updateMask();
}

void
CefViewWidget::updateMask()
{
  QPainterPath path;
  path.addRoundedRect(rect(), 50, 50);
  QRegion mask = QRegion(path.toFillPolygon().toPolygon());
  setMask(mask);
}
