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
  // assign parent so it will be destroyed by the parent
  this->setParent(cefWindow_);
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
QCefWindow::syncCefWindowPosOnExpose()
{
#if defined(Q_OS_WINDOWS)
  if (cefWidget_ && cefWindow_ && cefWindow_->winId()) {
    // #if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    //     qreal widgetScaleFactor = cefWidget_->devicePixelRatioF();
    // #else
    //     qreal widgetScaleFactor = cefWidget_->devicePixelRatio();
    // #endif
    //     int widgetWidth = cefWidget_->width();
    //     int widgetHeigth = cefWidget_->height();
    //     qDebug() << "------------- container widget:"
    //              << "(" << widgetWidth << " x " << widgetHeigth << ") @ " << widgetScaleFactor;
    //     qreal width = widgetWidth * widgetScaleFactor;
    //     qreal height = widgetHeigth * widgetScaleFactor;
    //     qDebug() << "------------- width:" << widgetWidth << " x " << widgetScaleFactor << " = " << width;
    //     qDebug() << "------------- height:" << widgetHeigth << " x " << widgetScaleFactor << " = " << height;

    qreal windowScaleFactor = this->devicePixelRatio();
    int windowWidth = this->width();
    int windowHeight = this->height();
    qDebug() << "------------- container window:"
             << "(" << windowWidth << " x " << windowHeight << ") @ " << windowScaleFactor;
    qreal width = windowWidth * windowScaleFactor;
    qreal height = windowHeight * windowScaleFactor;
    qDebug() << "------------- width:" << windowWidth << " x " << windowScaleFactor << " = " << width;
    qDebug() << "------------- height:" << windowHeight << " x " << windowScaleFactor << " = " << height;

    ::SetWindowPos((HWND)(cefWindow_->winId()),
                   NULL,
                   0,
                   0,
                   width,
                   height,
                   SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSENDCHANGING | SWP_DEFERERASE);
  }
#endif
}

void
QCefWindow::syncCefWindowPosOnResize()
{
#if defined(Q_OS_WINDOWS)
  if (cefWidget_ && cefWindow_ && cefWindow_->winId()) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    qreal widgetScaleFactor = cefWidget_->devicePixelRatioF();
#else
    qreal widgetScaleFactor = cefWidget_->devicePixelRatio();
#endif
    int widgetWidth = cefWidget_->width();
    int widgetHeigth = cefWidget_->height();
    qDebug() << "------------- container widget:"
             << "(" << widgetWidth << " x " << widgetHeigth << ") @ " << widgetScaleFactor;
    qreal width = widgetWidth * widgetScaleFactor;
    qreal height = widgetHeigth * widgetScaleFactor;
    qDebug() << "------------- width:" << widgetWidth << " x " << widgetScaleFactor << " = " << width;
    qDebug() << "------------- height:" << widgetHeigth << " x " << widgetScaleFactor << " = " << height;

    // qreal windowScaleFactor = this->devicePixelRatio();
    // int windowWidth = this->width();
    // int windowHeight = this->height();
    // qDebug() << "------------- container window:"
    //          << "(" << windowWidth << " x " << windowHeight << ") @ " << windowScaleFactor;
    // width = windowWidth * windowScaleFactor;
    // height = windowHeight * windowScaleFactor;
    // qDebug() << "------------- width:" << windowWidth << " x " << windowScaleFactor << " = " << width;
    // qDebug() << "------------- height:" << windowHeight << " x " << windowScaleFactor << " = " << height;

    ::SetWindowPos((HWND)(cefWindow_->winId()),
                   NULL,
                   0,
                   0,
                   width,
                   height,
                   SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSENDCHANGING | SWP_DEFERERASE);
  }
#endif
}

void
QCefWindow::exposeEvent(QExposeEvent* e)
{
#if defined(Q_OS_WINDOWS)
  qDebug() << "------------- QCefWindow::exposeEvent -------------";
  syncCefWindowPosOnExpose();
#endif
  QWindow::exposeEvent(e);
}

void
QCefWindow::resizeEvent(QResizeEvent* e)
{
#if defined(Q_OS_WINDOWS)
  qDebug() << "------------- QCefWindow::resizeEvent -------------";
  syncCefWindowPosOnResize();
#elif defined(Q_OS_LINUX)
  if (cefWindow_) {
    cefWindow_->resize(e->size());
  }
#else
  // do nothing
#endif
  QWindow::resizeEvent(e);
}
