#include "QCefWindow.h"

#include <QtDebug>

QCefWindow::QCefWindow()
  : QWindow()
{
  setFlag(Qt::FramelessWindowHint);
}

QCefWindow::~QCefWindow()
{
  qDebug() << this << "is being destructed";
}

void
QCefWindow::attachCefWindow(QWindow* win)
{
  detachCefWindow();
  cefWindow_ = win;
  cefWindow_->setParent(this);
}

void
QCefWindow::detachCefWindow()
{
  if (cefWindow_) {
    cefWindow_->hide();
    cefWindow_->setParent(nullptr);
    cefWindow_ = nullptr;
  }
}

QWindow*
QCefWindow::cefWindow()
{
  return cefWindow_;
}

void
QCefWindow::exposeEvent(QExposeEvent* e)
{
  //
  QWindow::exposeEvent(e);
}

void
QCefWindow::resizeEvent(QResizeEvent* e)
{
  //
  if (cefWindow_) {
    cefWindow_->resize(e->size());
  }
  QWindow::resizeEvent(e);
}

void
QCefWindow::showEvent(QShowEvent* e)
{
  //
  QWindow::showEvent(e);
}

void
QCefWindow::hideEvent(QHideEvent* e)
{
  //
  QWindow::hideEvent(e);
}
