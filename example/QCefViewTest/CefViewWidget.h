#ifndef CUSTOMCEFVIEW_H
#define CUSTOMCEFVIEW_H

#include <QScreen>

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

protected slots:
  void onScreenChanged(QScreen* screen);

private:
  virtual void onBrowserWindowCreated(QWindow* win);

  virtual void resizeEvent(QResizeEvent* event);

private:
  QWindow* m_cefWindow = nullptr;

  int m_cornerRadius = 50;
};

#endif // CUSTOMCEFVIEW_H
