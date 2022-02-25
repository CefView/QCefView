#include "QCefViewPrivate.h"

#pragma region std_headers
#include <stdexcept>
#pragma endregion std_headers

#pragma region qt_headers
#include <QApplication>
#include <QDebug>
#include <QPlatformSurfaceEvent>
#include <QVBoxLayout>
#include <QWindow>
#if defined(OS_LINUX)
#include <X11/Xlib.h>
#include <qpa/qplatformnativeinterface.h>
#endif
#pragma endregion qt_headers

#pragma region cef_headers
#include <include/cef_app.h>
#include <include/cef_browser.h>
#include <include/cef_frame.h>
#include <include/cef_parser.h>
#pragma endregion cef_headers

#include <CefViewCoreProtocol.h>

#include "CCefClientDelegate.h"
#include "QCefContext.h"
#include "ValueConvertor.h"

QCefViewPrivate::QCefViewPrivate(QCefView* view, const QString& url, const QCefSettingPrivate* setting)
  : q_ptr(view)
  , pContext_(QCefContext::instance()->d_func())
  , pCefBrowser_(nullptr)
  , qBrowserWindow_(nullptr)
  , qBrowserWidget_(nullptr)
{
  connect(qApp, &QApplication::focusChanged, this, &QCefViewPrivate::focusChanged);

  createBrowser(view, url, setting);
}

QCefViewPrivate::~QCefViewPrivate()
{
  destroyBrowser();
}

void
QCefViewPrivate::createBrowser(QCefView* view, const QString url, const QCefSettingPrivate* setting)
{
  // Set window info
  CefWindowInfo window_info;
#if defined(OS_MACOS)
  CefWindowHandle p = (CefWindowHandle)(view->winId());
  window_info.SetAsChild(p, 0, 0, 0, 0);
#elif defined(OS_WINDOWS)
  CefWindowHandle p = (CefWindowHandle)(view->winId());
  window_info.SetAsChild(p, RECT{ 0, 0, 0, 0 });
#elif defined(OS_LINUX)
  // Don't know why, on Linux platform if we use QCefView's winId() as
  // the parent, it will complain about `BadWindow`,
  // and the browser window will not be created, this never happens
  // on Windows and macOS, so we create a temporal QWindow as the
  // parent to create CEF browser window.
  QWindow w;
  CefWindowHandle p = (CefWindowHandle)(w.winId());
  window_info.SetAsChild(p, CefRect{ 0, 0, 0, 0 });
#endif

  // create the browser object
  CefBrowserSettings browserSettings;
  if (setting)
    setting->CopyToCefBrowserSettings(browserSettings);

  browserSettings.plugins = STATE_DISABLED;
  auto pCefBrowser = CefBrowserHost::CreateBrowserSync(window_info,         // window info
                                                       pContext_->pClient_, // handler
                                                       url.toStdString(),   // url
                                                       browserSettings,     // settings
                                                       nullptr,
                                                       CefRequestContext::GetGlobalContext());
  if (!pCefBrowser) {
    Q_ASSERT_X(pCefBrowser, "QCefViewPrivate::createBrowser", "Failed to create cer browser");
    return;
  }

  // register view to client delegate
  pContext_->pClientDelegate_->insertBrowserViewMapping(pCefBrowser, this);

  // create QWindow from native browser window handle
  QWindow* browserWindow = QWindow::fromWinId((WId)(pCefBrowser->GetHost()->GetWindowHandle()));
  if (!browserWindow) {
    Q_ASSERT_X(browserWindow, "QCefViewPrivate::createBrowser", "Failed to query QWindow from cef browser window");
    pCefBrowser->GetHost()->CloseBrowser(true);
    return;
  }

  // create QWidget from cef browser widow
  QWidget* browserWidget = QWidget::createWindowContainer(browserWindow, view, Qt::X11BypassWindowManagerHint);
  if (!browserWidget) {
    Q_ASSERT_X(browserWidget, "QCefViewPrivate::createBrowser", "Failed to cretae QWidget from cef browser window");
    pCefBrowser->GetHost()->CloseBrowser(true);
    return;
  }

  qBrowserWindow_ = browserWindow;
  qBrowserWidget_ = browserWidget;
  pCefBrowser_ = pCefBrowser;

  view->window()->installEventFilter(this);
  qBrowserWindow_->installEventFilter(this);

  return;
}

void
QCefViewPrivate::closeBrowser()
{
  if (!pCefBrowser_)
    return;

  // remove the browser from parent tree, or CEF will send close
  // event to the top level window, this will cause the application
  // to exit the event loop, this is not what we expected to happen
  qBrowserWindow_->setParent(nullptr);

  // clean resource
  pCefBrowser_->StopLoad();
  pCefBrowser_->GetHost()->CloseBrowser(true);

  // remove from delegate mapping
  pContext_->pClientDelegate_->removeBrowserViewMapping(pCefBrowser_);

  pCefBrowser_ = nullptr;
  qBrowserWidget_ = nullptr;
  qBrowserWindow_ = nullptr;
}

void
QCefViewPrivate::destroyBrowser()
{
  // close again
  closeBrowser();
}

int
QCefViewPrivate::browserId()
{
  if (pCefBrowser_)
    return pCefBrowser_->GetIdentifier();

  return -1;
}

void
QCefViewPrivate::navigateToString(const QString& content)
{
  if (pCefBrowser_) {
    std::string data = content.toStdString();
    data = CefURIEncode(CefBase64Encode(data.c_str(), data.size()), false).ToString();
    data = "data:text/html;base64," + data;
    pCefBrowser_->GetMainFrame()->LoadURL(data);
  }
}

void
QCefViewPrivate::navigateToUrl(const QString& url)
{
  if (pCefBrowser_) {
    CefString strUrl;
    strUrl.FromString(url.toStdString());
    pCefBrowser_->GetMainFrame()->LoadURL(strUrl);
  }
}

bool
QCefViewPrivate::browserCanGoBack()
{
  if (pCefBrowser_)
    return pCefBrowser_->CanGoBack();

  return false;
}

bool
QCefViewPrivate::browserCanGoForward()
{
  if (pCefBrowser_)
    return pCefBrowser_->CanGoForward();

  return false;
}

void
QCefViewPrivate::browserGoBack()
{
  if (pCefBrowser_)
    pCefBrowser_->GoBack();
}

void
QCefViewPrivate::browserGoForward()
{
  if (pCefBrowser_)
    pCefBrowser_->GoForward();
}

bool
QCefViewPrivate::browserIsLoading()
{
  if (pCefBrowser_)
    return pCefBrowser_->IsLoading();

  return false;
}

void
QCefViewPrivate::browserReload()
{
  if (pCefBrowser_)
    pCefBrowser_->Reload();
}

void
QCefViewPrivate::browserStopLoad()
{
  if (pCefBrowser_)
    pCefBrowser_->StopLoad();
}

bool
QCefViewPrivate::triggerEvent(const QString& name,
                              const QVariantList& args,
                              int frameId /*= CefViewBrowserHandler::MAIN_FRAME*/)
{
  if (!name.isEmpty()) {
    return sendEventNotifyMessage(frameId, name, args);
  }

  return false;
}

bool
QCefViewPrivate::responseQCefQuery(const QCefQuery& query)
{
  if (pContext_ && pContext_->pClient_) {
    CefString res;
    res.FromString(query.response().toStdString());
    return pContext_->pClient_->ResponseQuery(query.id(), query.result(), res, query.error());
  }
  return false;
}

bool
QCefViewPrivate::executeJavascript(int frameId, const QString& code, const QString& url, int startLine /*= 0*/)
{
  if (pCefBrowser_) {
    CefRefPtr<CefFrame> frame = pCefBrowser_->GetFrame(frameId);
    if (frame) {
      CefString c;
      c.FromString(code.toStdString());

      CefString u;
      u.FromString(url.toStdString());

      frame->ExecuteJavaScript(c, u, startLine);

      return true;
    }
  }

  return false;
}

void
QCefViewPrivate::notifyMoveOrResizeStarted()
{
  if (pCefBrowser_) {
    CefRefPtr<CefBrowserHost> host = pCefBrowser_->GetHost();
    if (host)
      host->NotifyMoveOrResizeStarted();
  }
}

bool
QCefViewPrivate::sendEventNotifyMessage(int frameId, const QString& name, const QVariantList& args)
{
  CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create(TRIGGEREVENT_NOTIFY_MESSAGE);
  CefRefPtr<CefListValue> arguments = msg->GetArgumentList();

  //** arguments(CefValueList)
  //** +------------+
  //** | event name |
  //** | event arg1 |
  //** | event arg2 |
  //** | event arg3 |
  //** | event arg4 |
  //** |    ...     |
  //** |    ...     |
  //** |    ...     |
  //** |    ...     |
  //** +------------+
  int idx = 0;
  CefString eventName = name.toStdString();
  arguments->SetString(idx++, eventName);
  for (auto& qV : args) {
    auto cVal = CefValue::Create();
    ValueConvertor::QVariantToCefValue(cVal, &qV);
    arguments->SetValue(idx++, cVal);
  }

  return pContext_->pClient_->TriggerEvent(pCefBrowser_, frameId, msg);
}

void
QCefViewPrivate::onTakeFocus(bool next)
{
  Q_Q(QCefView);
  auto reason = next ? Qt::TabFocusReason : Qt::BacktabFocusReason;
  auto widget = next ? q->nextInFocusChain() : q->previousInFocusChain();

  // find correct widget
  while (widget && 0 == (widget->focusPolicy() & Qt::TabFocus)) {
    widget = next ? widget->nextInFocusChain() : widget->previousInFocusChain();
  }

  if (widget) {
    // TO-DO bug: this does not work on Linux(X11), need to find a workaround
    widget->window()->raise();
    widget->activateWindow();
    widget->setFocus(reason);

#if defined(OS_LINUX)
    auto window = widget->window()->winId();
    auto screen = widget->window()->windowHandle()->screen();
    auto platformInterface = QApplication::platformNativeInterface();
    auto display = (Display*)platformInterface->nativeResourceForScreen("display", screen);
    auto ret = XSetInputFocus(display, window, RevertToNone, CurrentTime);
    if (ret <= 0)
      qWarning() << "Failed to move input focus";
#endif
  }
}

void
QCefViewPrivate::setFocus(bool focus)
{
  if (pCefBrowser_) {
    CefRefPtr<CefBrowserHost> host = pCefBrowser_->GetHost();
    if (host) {
      host->SetFocus(focus);
      host->SendFocusEvent(focus);
      if (!focus)
        host->SendCaptureLostEvent();
    }
  }
}

void
QCefViewPrivate::onGotFocus()
{
  // CEF browser window got focus
}

void
QCefViewPrivate::focusChanged(QWidget* /*old*/, QWidget* now)
{
  if (!now)
    return;

  Q_Q(QCefView);

  if (now == q) {
    // QCefView got focus, need to move the focus to the browser window
    setFocus(true);
  } else {
    // Bug fix: https://github.com/CefView/QCefView/issues/30
    // When QCefView got focus then click some other widgets(for example a QLineEdit),
    // the QCefView will not release the input focus, so we need to watch the focus change event.

    // release the browser window focus status
    setFocus(false);
  }
}

bool
QCefViewPrivate::eventFilter(QObject* watched, QEvent* event)
{
  Q_Q(QCefView);

  // filter event from top level window
  if (watched == q->window()) {
    switch (event->type()) {
      case QEvent::MouseButtonPress: {
        q->window()->activateWindow();
      } break;
      case QEvent::ParentAboutToChange: {
        q->window()->removeEventFilter(this);
      } break;
      case QEvent::ParentChange: {
        q->window()->installEventFilter(this);
      } break;
      case QEvent::Move:
      case QEvent::Resize: {
        notifyMoveOrResizeStarted();
      } break;
      default:
        break;
    }
  }

  // filter event from the browser window
  if (watched == qBrowserWindow_) {
    if (QEvent::PlatformSurface == event->type()) {
      auto e = (QPlatformSurfaceEvent*)event;
      if (e->surfaceEventType() == QPlatformSurfaceEvent::SurfaceAboutToBeDestroyed) {
        // browser window is being destroyed, need to close the browser window in advance
        closeBrowser();
      }
    }
  }

  return QObject::eventFilter(watched, event);
}
