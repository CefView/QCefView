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
  // create browser
  d_ptr->createCefBrowser(this, url, setting ? setting->d_func() : nullptr);

  // set window attributes for OSR mode
  if (d_ptr->isOSRModeEnabled_) {
    // OSR mode
    setBackgroundRole(QPalette::Window);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    // import for hardware rendering
    if (d_ptr->osr.pRenderer_ && d_ptr->osr.pRenderer_->isHardware()) {
      setAttribute(Qt::WA_PaintOnScreen);
    }
  }

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
QCefView::setDisablePopupContextMenu(bool disable)
{
  Q_D(QCefView);

  d->disablePopupContextMenu_ = disable;
}

bool
QCefView::isPopupContextMenuDisabled()
{
  Q_D(QCefView);

  return d->disablePopupContextMenu_;
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
QCefView::setEnableDragAndDrop(bool enable)
{
  Q_D(QCefView);

  d->enableDragAndDrop_ = enable;
}

bool
QCefView::isDragAndDropEnabled() const
{
  Q_D(const QCefView);

  return d->enableDragAndDrop_;
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

void
QCefView::leaveEvent(QEvent* event)
{
  Q_D(QCefView);

  QPoint mousePos = QCursor::pos();
  QMouseEvent moveEvent(QEvent::MouseMove, mousePos, mousePos, Qt::NoButton, Qt::NoButton, Qt::NoModifier);
  d->onViewMouseEvent(&moveEvent);

  QWidget::leaveEvent(event);
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

void
QCefView::render(QPainter* painter)
{
  Q_D(QCefView);

  d->render(painter);
}

QPaintEngine*
QCefView::paintEngine() const
{
  Q_D(const QCefView);

  if (d->isOSRModeEnabled_ && d->osr.pRenderer_ && d->osr.pRenderer_->isHardware()) {
    return nullptr;
  }

  return QWidget::paintEngine();
}

void
QCefView::paintEvent(QPaintEvent* event)
{
  Q_D(QCefView);

  d->onPaintEvent(event);
}

void
QCefView::inputMethodEvent(QInputMethodEvent* event)
{
  Q_D(QCefView);
  d->onViewInputMethodEvent(event);
}

void
QCefView::showEvent(QShowEvent* event)
{
  Q_D(QCefView);
  d->onViewVisibilityChanged(true);
  QWidget::showEvent(event);
}

void
QCefView::hideEvent(QHideEvent* event)
{
  Q_D(QCefView);
  d->onViewVisibilityChanged(false);
  QWidget::hideEvent(event);
}

void
QCefView::focusInEvent(QFocusEvent* event)
{
  Q_D(QCefView);
  d->onViewFocusChanged(true);
  QWidget::focusInEvent(event);
}

void
QCefView::focusOutEvent(QFocusEvent* event)
{
  Q_D(QCefView);
  d->onViewFocusChanged(false);
  QWidget::focusOutEvent(event);
}

void
QCefView::resizeEvent(QResizeEvent* event)
{
  Q_D(QCefView);
  d->onViewSizeChanged(event->size(), event->oldSize());
  QWidget::resizeEvent(event);
}

void
QCefView::keyPressEvent(QKeyEvent* event)
{
  Q_D(QCefView);
  d->onViewKeyEvent(event);
  QWidget::keyPressEvent(event);
}

void
QCefView::keyReleaseEvent(QKeyEvent* event)
{
  Q_D(QCefView);
  d->onViewKeyEvent(event);
  QWidget::keyReleaseEvent(event);
}

void
QCefView::mouseMoveEvent(QMouseEvent* event)
{
  Q_D(QCefView);
  d->onViewMouseEvent(event);
  QWidget::mouseMoveEvent(event);
}

void
QCefView::mousePressEvent(QMouseEvent* event)
{
  Q_D(QCefView);
  d->onViewMouseEvent(event);
  QWidget::mousePressEvent(event);
}

void
QCefView::mouseReleaseEvent(QMouseEvent* event)
{
  Q_D(QCefView);
  d->onViewMouseEvent(event);
  QWidget::mouseReleaseEvent(event);
}

void
QCefView::wheelEvent(QWheelEvent* event)
{
  Q_D(QCefView);
  d->onViewWheelEvent(event);
  QWidget::wheelEvent(event);
}

void
QCefView::contextMenuEvent(QContextMenuEvent* event)
{
  Q_D(QCefView);
  d->onContextMenuEvent(mapToGlobal(event->pos()));
  QWidget::contextMenuEvent(event);
}
