#include <QCefView.h>

#pragma region qt_headers
#include <QPainter>
#include <QPoint>
#include <QResizeEvent>
#include <QVBoxLayout>
#include <QtDebug>
#pragma endregion qt_headers

#include <QCefContext.h>

#include "details/QCefEventPrivate.h"
#include "details/QCefViewPrivate.h"

QCefView::QCefView(const QString url, const QCefSetting* setting, QWidget* parent /*= 0*/)
  : QWidget(parent)
  , d_ptr(new QCefViewPrivate(this, url, setting))
{
  setAttribute(Qt::WA_PaintOnScreen);
  // setAttribute(Qt::WA_OpaquePaintEvent);
  setAttribute(Qt::WA_StaticContents);

  // BUG: when size goes into zero, CEF OSR will crash
  setMinimumSize(1, 1);
  setMouseTracking(true);
  setFocusPolicy(Qt::WheelFocus);
}

QCefView::QCefView(QWidget* parent /*= 0*/)
  : QCefView("about:blank", nullptr, parent)
{}

QCefView::~QCefView()
{
  qDebug() << this << "is being destructed";
}

int
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

  return d->triggerEvent(event.eventName(), event.d_func()->args, CefViewBrowserClient::MAIN_FRAME);
}

bool
QCefView::triggerEvent(const QCefEvent& event, int64_t frameId)
{
  Q_D(QCefView);

  return d->triggerEvent(event.eventName(), event.d_func()->args, frameId);
}

bool
QCefView::broadcastEvent(const QCefEvent& event)
{
  Q_D(QCefView);

  return d->triggerEvent(event.eventName(), event.d_func()->args, CefViewBrowserClient::ALL_FRAMES);
}

bool
QCefView::responseQCefQuery(const QCefQuery& query)
{
  Q_D(QCefView);

  return d->responseQCefQuery(query);
}

bool
QCefView::executeJavascript(int64_t frameId, const QString& code, const QString& url)
{
  Q_D(QCefView);

  return d->executeJavascript(frameId, code, url);
}

bool
QCefView::executeJavascriptWithResult(int64_t frameId, const QString& code, const QString& url, int64_t context)
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

bool
QCefView::onBeforPopup(int64_t frameId,
                       const QString& targetUrl,
                       const QString& targetFrameName,
                       QCefView::WindowOpenDisposition targetDisposition,
                       QCefSetting& settings,
                       bool& DisableJavascriptAccess)
{
  // return false to allow the popup browser
  return false;
}

void
QCefView::onPopupCreated(QWindow* wnd)
{}

QVariant
QCefView::inputMethodQuery(Qt::InputMethodQuery query) const
{
  Q_D(const QCefView);
  switch (query) {
    case Qt::ImCursorRectangle:
      return QVariant(d->qImeCursorRect_);
    case Qt::ImAnchorRectangle:
      break;
    case Qt::ImFont:
      break;
    case Qt::ImCursorPosition:
      break;
    case Qt::ImSurroundingText:
      break;
    case Qt::ImCurrentSelection:
      break;
    case Qt::ImMaximumTextLength:
      break;
    case Qt::ImAnchorPosition:
      break;
    default:
      break;
  }

  return QWidget::inputMethodQuery(query);
}

void
QCefView::showEvent(QShowEvent* event)
{
  Q_D(QCefView);
  d->onVisibilityChanged(true);
}

void
QCefView::hideEvent(QHideEvent* event)
{
  Q_D(QCefView);
  d->onVisibilityChanged(false);
}

void
QCefView::focusInEvent(QFocusEvent* event)
{
  Q_D(QCefView);
  d->onFocusChanged(true);
}

void
QCefView::focusOutEvent(QFocusEvent* event)
{
  Q_D(QCefView);
  d->onFocusChanged(false);
}

void
QCefView::resizeEvent(QResizeEvent* event)
{
  Q_D(QCefView);
  d->onSizeChanged(event->size(), event->oldSize());
}

void
QCefView::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);

  Q_D(QCefView);
  painter.drawPixmap(event->rect(), d->qCefViewFrame_, event->rect());

  if (d->showPopup_) {
    painter.drawPixmap(d->qPopupRect_, d->qCefPopupFrame_);
  }
}
