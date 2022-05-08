#include "CefViewWidget.h"

#if defined(Q_OS_WINDOWS)
#include <windows.h>
#endif

#if defined(Q_OS_MACOS)
#endif

#if defined(Q_OS_LINUX)
#endif

#include <QColor>
#include <QRandomGenerator>

CefViewWidget::CefViewWidget(const QString url, const QCefSetting* setting, QWidget* parent /* = 0*/)
  : QCefView(url, setting, parent)
{}

CefViewWidget::~CefViewWidget() {}

bool
CefViewWidget::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
#if defined(Q_OS_WINDOWS)
  MSG* msg = (MSG*)message;
  qDebug("--------------- MSG: hwnd=%p, message=0x%08x", msg->hwnd, msg->message);
#endif
  return QCefView::nativeEvent(eventType, message, result);
}
