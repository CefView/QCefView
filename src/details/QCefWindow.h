#pragma once
#pragma region qt_headers
#include <QExposeEvent>
#include <QHideEvent>
#include <QResizeEvent>
#include <QShowEvent>
#include <QWindow>
#pragma endregion qt_headers

class QCefWindow : public QWindow
{
public:
  using QWindow::QWindow;

  QCefWindow();

  ~QCefWindow();

  void attachCefWindow(QWindow* win);

  void detachCefWindow();

  QWindow* cefWindow();

protected:
  virtual void resizeEvent(QResizeEvent* e) override;

private:
  QWindow* cefWindow_ = nullptr;
};
