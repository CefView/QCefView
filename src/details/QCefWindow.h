#pragma once
#pragma region qt_headers
#include <QExposeEvent>
#include <QHideEvent>
#include <QResizeEvent>
#include <QShowEvent>
#include <QWidget>
#include <QWindow>
#pragma endregion

class QCefWindow : public QWindow
{
public:
  using QWindow::QWindow;

  QCefWindow();

  ~QCefWindow();

  QWidget* attachCefWindow(QWindow* win, QWidget* parent);

  void detachCefWindow();

  void applyMask(const QRegion& region);

  QWindow* cefWindow();

protected:
  void syncCefWindowPosOnResize();
  void syncCefWindowPosOnExpose();

protected:
  void exposeEvent(QExposeEvent* e) override;
  void resizeEvent(QResizeEvent* e) override;
  void focusInEvent(QFocusEvent* e) override;
  void focusOutEvent(QFocusEvent* e) override;
  QObject* focusObject() const override;

private:
  QWidget* cefWidget_ = nullptr;
  QWindow* cefWindow_ = nullptr;
};
