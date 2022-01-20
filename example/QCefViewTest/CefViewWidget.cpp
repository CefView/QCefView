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
  QVariantList args;
  args.append(QVariant::fromValue(color.value()));
  event.setArguments(args);
  broadcastEvent(event);
}
