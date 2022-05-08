#ifndef CUSTOMCEFVIEW_H
#define CUSTOMCEFVIEW_H

#include <QCefView.h>

/// <summary>
/// Represents the customized QCefView
/// </summary>
class CefViewWidget : public QCefView
{
  Q_OBJECT

public:
  CefViewWidget(const QString url, const QCefSetting* setting, QWidget* parent = 0);

  ~CefViewWidget();
};

#endif // CUSTOMCEFVIEW_H
