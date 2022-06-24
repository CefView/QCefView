#include "CefViewWidget.h"

#include <QColor>
#include <QRandomGenerator>
#include <QResizeEvent>
#include <QWindow>

#if defined(Q_OS_WINDOWS)
#include <windows.h>
#elif defined(Q_OS_MACOS)
#else
#endif

CefViewWidget::CefViewWidget(const QString url, const QCefSetting* setting, QWidget* parent /* = 0*/)
  : QCefView(url, setting, parent)
{}

CefViewWidget::~CefViewWidget() {}

void
CefViewWidget::onBrowserWindowCreated(QWindow* win)
{
  cefWindow = win;

#if defined(Q_OS_WINDOWS)
  // initialize the region with rounded-corners for CEF window
  ::SetWindowRgn(
    (HWND)(cefWindow->winId()), ::CreateRoundRectRgn(0, 0, size().width(), size().height(), 100, 100), TRUE);
#elif defined(Q_OS_MACOS)

#else

#endif
}

void
CefViewWidget::resizeEvent(QResizeEvent* event)
{
  QCefView::resizeEvent(event);

#if defined(Q_OS_WINDOWS)
  // update the region with rounded-corners for CEF window
  if (cefWindow) {
    ::SetWindowRgn((HWND)(cefWindow->winId()),
                   ::CreateRoundRectRgn(0, 0, event->size().width(), event->size().height(), 100, 100),
                   TRUE);
  }
#elif defined(Q_OS_MACOS)

#else

#endif
}
