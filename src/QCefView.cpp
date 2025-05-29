#include <QCefView.h>

#pragma region qt_headers
#include <QPainter>
#include <QPoint>
#include <QResizeEvent>
#include <QVBoxLayout>
#include <QtDebug>
#pragma endregion

#include <QCefContext.h>

#include "details/QCefEventPrivate.h"
#include "details/QCefViewPrivate.h"
#include "details/utils/CommonUtils.h"

#if CEF_VERSION_MAJOR < 122
const QCefFrameId QCefView::MainFrameID = 0;
const QCefFrameId QCefView::AllFrameID = -1;
#else
const QCefFrameId QCefView::MainFrameID = "0";
const QCefFrameId QCefView::AllFrameID = "-1";
#endif

QCefView::QCefView(const QString& url,
                   const QCefSetting* setting,
                   QWidget* parent /*= 0*/,
                   Qt::WindowFlags f /*= Qt::WindowFlags()*/)
  : QWidget(parent, f)
  , d_ptr(new QCefViewPrivate(QCefContext::instance()->d_func(), this))
{
  if (d_ptr->isOSRModeEnabled_) {
    setAttribute(Qt::WA_NativeWindow);
    setAttribute(Qt::WA_DontCreateNativeAncestors);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_PaintOnScreen);
  }

  // create browser
  d_ptr->createCefBrowser(url, setting ? setting->d_func() : nullptr);

  // track mouse
  setMouseTracking(true);
  // set focus policy
  setFocusPolicy(Qt::WheelFocus);
}

QCefView::QCefView(QWidget* parent /*= 0*/, Qt::WindowFlags f /*= Qt::WindowFlags()*/)
  : QCefView("about:blank", nullptr, parent, f)
{
}

QCefView::~QCefView()
{
  qDebug() << this << "is being destructed";

  if (d_ptr) {
    // destroy under layer cef browser
    d_ptr->destroyCefBrowser();
    d_ptr.reset();
  }
}

void
QCefView::addLocalFolderResource(const QString& path, const QString& url, int priority /*= 0*/)
{
  Q_D(QCefView);

  d->addLocalFolderResource(path, url, priority);
}

void
QCefView::addArchiveResource(const QString& path,
                             const QString& url,
                             const QString& password /*= ""*/,
                             int priority /*= 0*/)
{
  Q_D(QCefView);

  d->addArchiveResource(path, url, password, priority);
}

QCefBrowserId
QCefView::browserId()
{
  Q_D(QCefView);

  return d->browserId();
}

void
QCefView::navigateToString(const QString& content)
{
  Q_D(QCefView);

  d->navigateToString(content);
}

void
QCefView::navigateToUrl(const QString& url)
{
  Q_D(QCefView);

  d->navigateToUrl(url);
}

bool
QCefView::browserCanGoBack()
{
  Q_D(QCefView);

  return d->browserCanGoBack();
}

bool
QCefView::browserCanGoForward()
{
  Q_D(QCefView);

  return d->browserCanGoForward();
}

void
QCefView::browserGoBack()
{
  Q_D(QCefView);

  d->browserGoBack();
}

void
QCefView::browserGoForward()
{
  Q_D(QCefView);

  d->browserGoForward();
}

bool
QCefView::browserIsLoading()
{
  Q_D(QCefView);

  return d->browserIsLoading();
}

void
QCefView::browserReload()
{
  Q_D(QCefView);

  d->browserReload();
}

void
QCefView::browserStopLoad()
{
  Q_D(QCefView);

  d->browserStopLoad();
}

bool
QCefView::triggerEvent(const QCefEvent& event)
{
  Q_D(QCefView);

  return d->triggerEvent(event.eventName(), event.d_func()->args, QCefView::MainFrameID);
}

bool
QCefView::triggerEvent(const QCefEvent& event, const QCefFrameId& frameId)
{
  Q_D(QCefView);

  return d->triggerEvent(event.eventName(), event.d_func()->args, frameId);
}

bool
QCefView::broadcastEvent(const QCefEvent& event)
{
  Q_D(QCefView);

  return d->triggerEvent(event.eventName(), event.d_func()->args, QCefView::AllFrameID);
}

bool
QCefView::responseQCefQuery(const QCefQuery& query)
{
  Q_D(QCefView);

  return d->responseQCefQuery(query);
}

bool
QCefView::executeJavascript(const QCefFrameId& frameId, const QString& code, const QString& url)
{
  Q_D(QCefView);

  return d->executeJavascript(frameId, code, url);
}

bool
QCefView::executeJavascriptWithResult(const QCefFrameId& frameId,
                                      const QString& code,
                                      const QString& url,
                                      const QString& context)
{
  Q_D(QCefView);

  return d->executeJavascriptWithResult(frameId, code, url, context);
}

bool
QCefView::setPreference(const QString& name, const QVariant& value, const QString& error)
{
  Q_D(QCefView);

  return d->setPreference(name, value, error);
}

void
QCefView::setOSRFrameRate(int fps)
{
  Q_D(QCefView);

  return d->setOSRFrameRate(fps);
}

bool
QCefView::hasDevTools()
{
  Q_D(QCefView);

  return d->hasDevTools();
}

void
QCefView::showDevTools()
{
  Q_D(QCefView);

  d->showDevTools();
}

void
QCefView::closeDevTools()
{
  Q_D(QCefView);

  d->closeDevTools();
}

void
QCefView::setAllowDrag(bool allow)
{
  Q_D(QCefView);

  d->allowDrag_ = allow;
}

bool
QCefView::allowDrag() const
{
  Q_D(const QCefView);

  return d->allowDrag_;
}

void
QCefView::setFocus(Qt::FocusReason reason)
{
  Q_D(QCefView);

  d->setCefWindowFocus(true);
}

QCefView*
QCefView::onNewBrowser(const QCefFrameId& sourceFrameId,
                       const QString& url,
                       const QString& name,
                       CefWindowOpenDisposition targetDisposition,
                       QRect& rect,
                       QCefSetting& settings)
{
  QCefView* popup = new QCefView(url, &settings, nullptr, Qt::WindowFlags());
  if (!popup) {
    // failed to create QCefView, cancel popup
    return nullptr;
  }

  // config the popup QCefView
  if (!name.isEmpty()) {
    popup->setWindowTitle(name);
  }
  popup->setAttribute(Qt::WA_DeleteOnClose, true);
  popup->resize(rect.size());
  popup->show();

  return popup;
}

bool
QCefView::onNewPopup(const QCefFrameId& frameId,
                     const QString& targetUrl,
                     QString& targetFrameName,
                     QCefView::CefWindowOpenDisposition targetDisposition,
                     QRect& rect,
                     QCefSetting& settings,
                     bool& disableJavascriptAccess)
{
  return false;
}

void
QCefView::onNewDownloadItem(const QSharedPointer<QCefDownloadItem>& item, const QString& suggestedName)
{
}

void
QCefView::onUpdateDownloadItem(const QSharedPointer<QCefDownloadItem>& item)
{
}

bool
QCefView::onRequestCloseFromWeb()
{
  // delete self
  deleteLater();

  return true;
}

QVariant
QCefView::inputMethodQuery(Qt::InputMethodQuery query) const
{
  Q_D(const QCefView);

  if (d->isOSRModeEnabled_) {
    // OSR mode
    auto r = d->onViewInputMethodQuery(query);
    if (r.isValid())
      return r;
  }

  return QWidget::inputMethodQuery(query);
}

QPaintEngine*
QCefView::paintEngine() const
{
  Q_D(const QCefView);

  if (d->isOSRModeEnabled_) {
    return nullptr;
  }

  return QWidget::paintEngine();
}

bool
QCefView::event(QEvent* event)
{
  Q_D(QCefView);

  switch (event->type()) {
    case QEvent::Paint: {
      QPaintEvent* e = static_cast<QPaintEvent*>(event);
      d->onPaintEvent(e);
    } break;
    case QEvent::InputMethod: {
      QInputMethodEvent* e = static_cast<QInputMethodEvent*>(event);
      d->onViewInputMethodEvent(e);
    } break;
    case QEvent::Show: {
      d->onViewVisibilityChanged(true);
    } break;
    case QEvent::Hide: {
      d->onViewVisibilityChanged(false);
    } break;
    case QEvent::FocusIn: {
      d->onViewFocusChanged(true);
    } break;
    case QEvent::FocusOut: {
      d->onViewFocusChanged(false);
    } break;
    case QEvent::Move: {
      d->onViewMoved();
    } break;
    case QEvent::Resize: {
      QResizeEvent* e = static_cast<QResizeEvent*>(event);
      d->onViewSizeChanged(e->size(), e->oldSize());
    } break;
    case QEvent::KeyPress:
    case QEvent::KeyRelease: {
      QKeyEvent* e = static_cast<QKeyEvent*>(event);
      d->onViewKeyEvent(e);
      if (d->isOSRModeEnabled_ && (e->key() == Qt::Key_Tab || e->key() == Qt::Key_Backtab)) {
        return true;
      }
    } break;
    case QEvent::MouseMove:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease: {
      QMouseEvent* e = static_cast<QMouseEvent*>(event);
      d->onViewMouseEvent(e);
    } break;
    case QEvent::Wheel: {
      QWheelEvent* e = static_cast<QWheelEvent*>(event);
      d->onViewWheelEvent(e);
    } break;
    case QEvent::Leave: {
      QPoint mousePos = QCursor::pos();
      QMouseEvent moveEvent(QEvent::MouseMove, mousePos, mousePos, Qt::NoButton, Qt::NoButton, Qt::NoModifier);
      d->onViewMouseEvent(&moveEvent);
    } break;
    case QEvent::ContextMenu: {
      QContextMenuEvent* e = static_cast<QContextMenuEvent*>(event);
      d->onContextMenuEvent(mapToGlobal(e->pos()));
    } break;
    case QEvent::DragEnter: {
      if (d->isOSRModeEnabled_) {
        QDragEnterEvent* e = static_cast<QDragEnterEvent*>(event);
        d->onDragEnter(e);
      }
    } break;
    case QEvent::DragMove: {
      QDragMoveEvent* e = static_cast<QDragMoveEvent*>(event);
      d->onDragMove(e);
    } break;
    case QEvent::DragLeave: {
      QDragLeaveEvent* e = static_cast<QDragLeaveEvent*>(event);
      d->onDragLeave(e);
    } break;
    case QEvent::Drop: {
      QDropEvent* e = static_cast<QDropEvent*>(event);
      d->onDrop(e);
    } break;
    default:
      break;
  }

  return QWidget::event(event);
}
