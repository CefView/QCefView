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
#include "QCefSettingPrivate.h"
#include "ValueConvertor.h"

QCefViewPrivate::QCefViewPrivate(QCefView* view, const QString& url, const QCefSetting* setting)
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
QCefViewPrivate::createBrowser(QCefView* view, const QString url, const QCefSetting* setting)
{
  // create a temporary windows as the parent of the CEF browser window to be created
  QWindow fakeParent;

  // Set window info
  CefWindowInfo window_info;
  window_info.SetAsChild((CefWindowHandle)(fakeParent.winId()), { 0, 0, 0, 0 });

  // create the browser settings
  CefBrowserSettings browserSettings;
  if (setting) {
    QCefSettingPrivate::CopyToCefBrowserSettings(setting, &browserSettings);
  }

  // create browser object
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

  // create QWidget from cef browser widow, this will re-parent the CEF browser window
  QWidget* browserWidget = QWidget::createWindowContainer(browserWindow, view, Qt::FramelessWindowHint);
  if (!browserWidget) {
    Q_ASSERT_X(browserWidget, "QCefViewPrivate::createBrowser", "Failed to cretae QWidget from cef browser window");
    pCefBrowser->GetHost()->CloseBrowser(true);
    return;
  }

  qBrowserWindow_ = browserWindow;
  qBrowserWidget_ = browserWidget;
  pCefBrowser_ = pCefBrowser;

  qBrowserWindow_->installEventFilter(this);
  view->window()->installEventFilter(this);

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

void
QCefViewPrivate::setCefWindowFocus(bool focus)
{
  if (pCefBrowser_) {
    CefRefPtr<CefBrowserHost> host = pCefBrowser_->GetHost();
    if (host) {
      host->SetFocus(focus);
    }
  }
}

void
QCefViewPrivate::focusChanged(QWidget* old, QWidget* now)
{
  Q_Q(QCefView);

  // qDebug() << q << q->window()->isActiveWindow() << ":focus changed from:" << old << " -> " << now;
  // qDebug() << q->windowHandle() << "focusWindow:" << QGuiApplication::focusWindow();

  if (!now || now->window() != q->window())
    return;

  if (now == q) {
    // QCefView got focus, need to move the focus to the CEF browser window
    // This only works when changing focus by TAB key
    if (old && old->window() == q->window())
      setCefWindowFocus(true);
  } else {
    // Because setCefWindowFocus will not release CEF browser window focus,
    // here we need to activate the new focused widget forcefully.
    // This code should be executed only when click any focus except
    // the QCefView while QCefView is holding the focus
    if (!old && !QGuiApplication::focusWindow())
      now->activateWindow();
  }
}

bool
QCefViewPrivate::eventFilter(QObject* watched, QEvent* event)
{
  Q_Q(QCefView);

  // filter event to the level window
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

  // filter event to the browser window
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
                              int64_t frameId /*= CefViewBrowserHandler::MAIN_FRAME*/)
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
QCefViewPrivate::executeJavascript(int64_t frameId, const QString& code, const QString& url)
{
  if (code.isEmpty())
    return false;

  if (pCefBrowser_) {
    CefRefPtr<CefFrame> frame = pCefBrowser_->GetFrame(frameId);
    if (frame) {
      CefString c;
      c.FromString(code.toStdString());

      CefString u;
      if (url.isEmpty()) {
        u = frame->GetURL();
      } else {
        u.FromString(url.toStdString());
      }

      frame->ExecuteJavaScript(c, u, 0);

      return true;
    }
  }

  return false;
}

bool
QCefViewPrivate::executeJavascriptWithResult(int64_t frameId, const QString& code, const QString& url, int64_t context)
{
  if (code.isEmpty())
    return false;

  if (pContext_ && pContext_->pClient_) {
    auto frame = frameId == 0 ? pCefBrowser_->GetMainFrame() : pCefBrowser_->GetFrame(frameId);
    if (!frame)
      return false;

    CefString c;
    c.FromString(code.toStdString());

    CefString u;
    if (url.isEmpty()) {
      u = frame->GetURL();
    } else {
      u.FromString(url.toStdString());
    }

    return pContext_->pClient_->AsyncExecuteJSCode(pCefBrowser_, frame, c, u, context);
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
QCefViewPrivate::sendEventNotifyMessage(int64_t frameId, const QString& name, const QVariantList& args)
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
    ValueConvertor::QVariantToCefValue(cVal.get(), &qV);
    arguments->SetValue(idx++, cVal);
  }

  return pContext_->pClient_->TriggerEvent(pCefBrowser_, frameId, msg);
}

void
QCefViewPrivate::onCefWindowLostTabFocus(bool next)
{
  // The focus was returned from CEF window, QCefView needs to handle
  // this event and give the focus to the correct next or previous widget
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
QCefViewPrivate::onCefWindowGotFocus()
{
  // CEF browser window got focus
  Q_Q(QCefView);

  // if the QCefView has no focus, we move focus on it
  if (!q->hasFocus()) {
    qDebug() << "CEF window got focus, move focus on QCefView";
    q->setFocus();
  }
}

bool
QCefViewPrivate::setPreference(const QString& name, const QVariant& value, const QString& error)
{
  if (pCefBrowser_) {
    CefRefPtr<CefBrowserHost> host = pCefBrowser_->GetHost();
    if (host) {
      CefString n;
      n.FromString(name.toStdString());

      auto v = CefValue::Create();
      ValueConvertor::QVariantToCefValue(v.get(), &value);

      CefString e;
      auto r = host->GetRequestContext()->SetPreference(n, v, e);
      error.fromStdString(e.ToString());
      return r;
    }
  }

  return false;
}
