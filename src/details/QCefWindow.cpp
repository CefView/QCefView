#include "QCefWindow.h"

#if defined(Q_OS_WINDOWS)
#include <windows.h>
#endif

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

  cefWidget_ = QWidget::createWindowContainer(widgetSourceWindow, //
                                              parent,             //
                                              Qt::WindowTransparentForInput | Qt::WindowDoesNotAcceptFocus);
  return cefWidget_;
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
    cefWidget_ = nullptr;
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
QCefWindow::syncCefWindowPos()
{
#if defined(Q_OS_WINDOWS)
  if (cefWidget_ && cefWindow_ && cefWindow_->winId()) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    qreal scaleFactor = cefWidget_->devicePixelRatioF();
#else
    qreal scaleFactor = cefWidget_->devicePixelRatio();
#endif
    auto w = width() * scaleFactor;
    auto h = height() * scaleFactor;
    ::SetWindowPos((HWND)(cefWindow_->winId()), NULL, 0, 0, w, h, SWP_NOZORDER | SWP_NOSENDCHANGING | SWP_DEFERERASE);
  }
#endif
}

void
QCefWindow::exposeEvent(QExposeEvent* e)
{
#if defined(Q_OS_WINDOWS)
  syncCefWindowPos();
#endif
}

void
QCefWindow::resizeEvent(QResizeEvent* e)
{
#if defined(Q_OS_WINDOWS)
  syncCefWindowPos();
#elif defined(Q_OS_LINUX)
  if (cefWindow_) {
    cefWindow_->resize(e->size());
  }
#else
  // do nothing
#endif
  QWindow::resizeEvent(e);
}
