#include "CefViewWidget.h"

#include <QColor>
#include <QRandomGenerator>
#include <QResizeEvent>
#include <QWindow>

void
CefViewWidget::onBrowserWindowCreated(QWindow* win)
{
  m_cefWindow = win;
}

void
CefViewWidget::resizeEvent(QResizeEvent* event)
{
  QCefView::resizeEvent(event);
}
