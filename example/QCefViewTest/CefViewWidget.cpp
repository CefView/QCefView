#include "CefViewWidget.h"

#include <QColor>
#include <QRandomGenerator>

CefViewWidget::CefViewWidget(const QString url, const QCefSetting* setting, QWidget* parent /* = 0*/)
  : QCefView(url, setting, parent)
{}

CefViewWidget::~CefViewWidget() {}
