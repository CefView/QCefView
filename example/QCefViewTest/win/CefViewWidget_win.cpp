#include "CefViewWidget.h"

#include <QColor>
#include <QRandomGenerator>
#include <QResizeEvent>
#include <QWindow>

#include <windows.h>

void
CefViewWidget::onBrowserWindowCreated(QWindow* win)
{
  m_cefWindow = win;
  if (!m_cefWindow)
    return;

  // initialize the region with rounded-corners for CEF window
  ::SetWindowRgn((HWND)(m_cefWindow->winId()),
                 ::CreateRoundRectRgn(0, 0, size().width(), size().height(), m_cornerRadius, m_cornerRadius),
                 TRUE);
}

void
CefViewWidget::resizeEvent(QResizeEvent* event)
{
  QCefView::resizeEvent(event);

  // update the region with rounded-corners for CEF window
  if (m_cefWindow) {
    ::SetWindowRgn(
      (HWND)(m_cefWindow->winId()),
      ::CreateRoundRectRgn(0, 0, event->size().width(), event->size().height(), m_cornerRadius, m_cornerRadius),
      TRUE);
  }
}
