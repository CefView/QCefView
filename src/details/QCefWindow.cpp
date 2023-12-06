#include "QCefWindow.h"

#include <QDebug>

QCefWindow::QCefWindow()
  : QWindow()
{
}

QCefWindow::~QCefWindow()
{
  qDebug() << this << "is being destructed";
}

QWidget*
QCefWindow::attachCefWindow(QWindow* win, QWidget* parent)
{
  // keep cef window
  cefWindow_ = win;

#if defined(Q_OS_MACOS)
  // for macOS we just create the widget with CEF window,
  // and current QCefWindow is useless
  QWindow* widgetSourceWindow = cefWindow_;
#else
  // for Windows & Linux we create the widget with
  // current QCefWindow
  win->setParent(this);
  QWindow* widgetSourceWindow = this;
#endif

  return QWidget::createWindowContainer(widgetSourceWindow, //
                                        parent,             //
                                        Qt::WindowTransparentForInput | Qt::WindowDoesNotAcceptFocus);
}

void
QCefWindow::detachCefWindow()
{
  if (cefWindow_) {
#if defined(Q_OS_MACOS)

#else
    cefWindow_->hide();
#if defined(Q_OS_LINUX)
    cefWindow_->setParent(nullptr);
#endif
#endif
    cefWindow_ = nullptr;
  }
}

void
QCefWindow::applyMask(const QRegion& region)
{
#if defined(Q_OS_MACOS)
  if (cefWindow_) {
    cefWindow_->setMask(region);
  }
#else
  this->setMask(region);
#endif
}

QWindow*
QCefWindow::cefWindow()
{
  return cefWindow_;
}

void
QCefWindow::resizeEvent(QResizeEvent* e)
{
#if defined(Q_OS_MACOS)
#else
  if (cefWindow_) {
    cefWindow_->resize(e->size());
  }
#endif
  QWindow::resizeEvent(e);
}
