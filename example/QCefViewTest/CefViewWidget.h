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

  virtual void onBrowserWindowCreated(QWindow* win);

  virtual void resizeEvent(QResizeEvent* event);

private:
  QWindow* cefWindow = nullptr;
};

#endif // CUSTOMCEFVIEW_H
