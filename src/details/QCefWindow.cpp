#include "QCefWindow.h"

#pragma region stdc_headers
#include <math.h>
#pragma region stdc_headers

#pragma region qt_headers
#include <QCoreApplication>
#include <QResizeEvent>
#include <QPaintDevice>
#include <QPainter>
#include <QDebug>
#pragma endregion qt_headers

QCefWindow::QCefWindow(QWindow* parent, QCefView* hostView /*= 0*/)
  : QWindow(parent)
  , browserWindow_(0)
{
  setFlags(Qt::FramelessWindowHint);
}

QCefWindow::~QCefWindow()
{
  if (browserWindow_) {
    browserWindow_ = 0;
  }
}

void
QCefWindow::setBrowserWindowId(CefWindowHandle win)
{
  if (win) {
    // this method may be called from threads other than Qt main thread
    // in order to make sure the QWindow is created in Qt main thread
    // we need to run the following logic by posting it to Qt main thread
    QMetaObject::invokeMethod(
      this,
      [=]() {
        this->browserWindow_ = QWindow::fromWinId((WId)win);
        this->browserWindow_->setParent(this);
        syncBrowserWindow();
      },
      Qt::QueuedConnection);
  }
}

void
QCefWindow::syncBrowserWindow()
{
  if (browserWindow_) {
    double w = width() * devicePixelRatio();
    double h = height() * devicePixelRatio();
    browserWindow_->setGeometry(0, 0, ceil(w), ceil(h));
  }
  return;
}

void
QCefWindow::exposeEvent(QExposeEvent* e)
{
  syncBrowserWindow();
  QWindow::exposeEvent(e);
}

void
QCefWindow::resizeEvent(QResizeEvent* e)
{
  syncBrowserWindow();
  QWindow::resizeEvent(e);
}
