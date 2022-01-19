#if defined(OS_WINDOWS)
#include <windows.h>
#else
#endif

#include <QColor>
#include <QRandomGenerator>

#include "CefViewWidget.h"

CefViewWidget::CefViewWidget(const QString url, const QCefSetting* setting, QWidget* parent /* = 0*/)
  : QCefView(url, setting, parent)
{}

CefViewWidget::~CefViewWidget() {}

void
CefViewWidget::changeColor()
{
  QColor color(QRandomGenerator::global()->generate());

  QCefEvent event("colorChange");
  event.setStringProperty("color", color.name());
  broadcastEvent(event);
}
