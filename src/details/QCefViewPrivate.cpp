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

#if defined(OS_LINUX)
Display*
X11GetDisplay(QWidget* widget)
{
  Q_ASSERT_X(widget, "X11GetDisplay", "Invalid parameter widget");
  if (!widget) {
    qWarning("Invalid parameter widget");
    return nullptr;
  }

  auto platformInterface = QApplication::platformNativeInterface();
  Q_ASSERT_X(platformInterface, "X11GetDisplay", "Failed to get platform native interface");
  if (!platformInterface) {
    qWarning("Failed to get platform native interface");
    return nullptr;
  }

  auto screen = widget->window()->windowHandle()->screen();
  Q_ASSERT_X(screen, "X11GetDisplay", "Failed to get screen");
  if (!screen) {
    qWarning("Failed to get screen");
    return nullptr;
  }

  return (Display*)platformInterface->nativeResourceForScreen("display", screen);
}
#endif

QSet<QCefViewPrivate*> QCefViewPrivate::sLiveInstances;

void
QCefViewPrivate::destroyAllInstance()
{
  auto s = sLiveInstances;
  for (auto& i : s) {
    i->destroyCefBrowser();
  }
}

QCefViewPrivate::QCefViewPrivate(QCefView* view, const QString& url, const QCefSetting* setting)
  : q_ptr(view)
  , pContextPrivate_(QCefContext::instance()->d_func())
  , pClient_(nullptr)
  , pClientDelegate_(nullptr)
  , pCefBrowser_(nullptr)
  , qBrowserWindow_(nullptr)
  , qBrowserWidget_(nullptr)
{
  sLiveInstances.insert(this);

  createCefBrowser(view, url, setting);

  connect(qApp, &QApplication::focusChanged, this, &QCefViewPrivate::applicationFocusChanged);
}

QCefViewPrivate::~QCefViewPrivate()
{
  destroyCefBrowser();
  sLiveInstances.remove(this);
}

void
QCefViewPrivate::createCefBrowser(QCefView* view, const QString url, const QCefSetting* setting)
{
  // create browser client handler delegate
  auto pClientDelegate = std::make_shared<CCefClientDelegate>(this);

  // create browser client handler
  auto pClient = new CefViewBrowserClient(pContextPrivate_->getCefApp(), pClientDelegate);

  for (auto& folderMapping : pContextPrivate_->folderResourceMappingList()) {
    pClient->AddLocalDirectoryResourceProvider(
      folderMapping.path.toStdString(), folderMapping.url.toStdString(), folderMapping.priority);
  }

  for (auto& archiveMapping : pContextPrivate_->archiveResourceMappingList()) {
    pClient->AddArchiveResourceProvider(archiveMapping.path.toStdString(),
                                        archiveMapping.url.toStdString(),
                                        archiveMapping.password.toStdString(),
                                        archiveMapping.priority);
  }

  // Set window info
  CefWindowInfo window_info;
  window_info.SetAsChild((CefWindowHandle)view->winId(), { 0, 0, view->maximumWidth(), view->maximumHeight() });

  // create the browser settings
  CefBrowserSettings browserSettings;
  if (setting) {
    QCefSettingPrivate::CopyToCefBrowserSettings(setting, &browserSettings);
  }

  // create browser object
  bool success = CefBrowserHost::CreateBrowser(window_info,       // window info
                                               pClient,           // handler
                                               url.toStdString(), // url
                                               browserSettings,   // settings
                                               nullptr,
                                               CefRequestContext::GetGlobalContext());
  Q_ASSERT_X(success, "QCefViewPrivate::createBrowser", "Failed to create cef browser");
  if (!success) {
    qWarning("Failed to create cef browser");
    return;
  }

  view->window()->installEventFilter(this);
  view->installEventFilter(this);

  pClient_ = pClient;
  pClientDelegate_ = pClientDelegate;
  return;
}

void
QCefViewPrivate::cefBrowserCreated(CefRefPtr<CefBrowser>& browser)
{
  Q_Q(QCefView);

  // create QWindow from native browser window handle
  QWindow* browserWindow = QWindow::fromWinId((WId)(browser->GetHost()->GetWindowHandle()));

  // create QWidget from cef browser widow, this will re-parent the CEF browser window
  QWidget* browserWidget = QWidget::createWindowContainer(
    browserWindow,
    q,
    Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowTransparentForInput | Qt::WindowDoesNotAcceptFocus);
  Q_ASSERT_X(browserWidget, "QCefViewPrivate::createBrowser", "Failed to create QWidget from cef browser window");
  if (!browserWidget) {
    qWarning("Failed to create QWidget from cef browser window");
    browser->GetHost()->CloseBrowser(true);
    return;
  }

  qBrowserWindow_ = browserWindow;
  qBrowserWidget_ = browserWidget;
  pCefBrowser_ = browser;

  qBrowserWidget_->installEventFilter(this);
  qBrowserWindow_->installEventFilter(this);

  // hide the browser widget before re-parenting to reduce flicker
  browserWidget->hide();
  q->layout()->addWidget(qBrowserWidget_);
  browserWidget->show();
}

void
QCefViewPrivate::destroyCefBrowser()
{
  if (!pClient_)
    return;

  // remove the browser from parent tree, or CEF will send close
  // event to the top level window, this will cause the application
  // to exit the event loop, this is not what we expected to happen
  qBrowserWindow_->setParent(nullptr);

  // clean all browsers
  pClient_->CloseAllBrowsers();

  pClient_ = nullptr;
  qBrowserWidget_ = nullptr;
  qBrowserWindow_ = nullptr;
  pCefBrowser_ = nullptr;
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
QCefViewPrivate::applicationFocusChanged(QWidget* old, QWidget* now)
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

  auto et = event->type();

  // filter event to QCefView
  if (watched == q) {
    switch (et) {
      case QEvent::ParentAboutToChange: {
        q->window()->removeEventFilter(this);
      } break;
      case QEvent::ParentChange: {
        q->window()->installEventFilter(this);
      } break;
      default:
        break;
    }
  }

  // filter event to the top level window
  if (watched == q->window()) {
    switch (et) {
      case QEvent::Move: {
        notifyMoveOrResizeStarted();
      } break;
      default:
        break;
    }
  }

  // filter event to the browser widget
  if (watched == qBrowserWidget_) {
    switch (et) {
      case QEvent::Resize: {
        notifyMoveOrResizeStarted();
      } break;
      case QEvent::Show: {
#if defined(OS_LINUX)
        if (::XMapWindow(X11GetDisplay(qBrowserWidget_), qBrowserWindow_->winId()) <= 0)
          qWarning() << "Failed to move input focus";
          // BUG-TO-BE-FIXED after remap, the browser window will not resize automatically
          // with the QCefView widget
#endif
#if defined(OS_WINDOWS)
        // force to re-layout
        q->layout()->invalidate();
#endif
      } break;
      case QEvent::Hide: {
#if defined(OS_WINDOWS)
        // resize the browser window to 0 so that CEF will notify the
        // "visibilitychanged" event in Javascript
        qBrowserWindow_->resize(0, 0);
#endif
      } break;
      default:
        break;
    }
  }

  // filter event to the browser window
  if (watched == qBrowserWindow_) {
    if (QEvent::PlatformSurface != et)
      return QObject::eventFilter(watched, event);

    auto t = ((QPlatformSurfaceEvent*)event)->surfaceEventType();
    if (QPlatformSurfaceEvent::SurfaceAboutToBeDestroyed == t) {
      // browser window is being destroyed, need to close the browser window in advance
      destroyCefBrowser();
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
  if (pClient_) {
    CefString res;
    res.FromString(query.response().toStdString());
    return pClient_->ResponseQuery(query.id(), query.result(), res, query.error());
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

  if (pClient_) {
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

    return pClient_->AsyncExecuteJSCode(pCefBrowser_, frame, c, u, context);
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

  return pClient_->TriggerEvent(pCefBrowser_, frameId, msg);
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
    widget->setFocus(reason);

#if defined(OS_LINUX)
    if (::XSetInputFocus(X11GetDisplay(widget), widget->window()->winId(), RevertToNone, CurrentTime) <= 0)
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
