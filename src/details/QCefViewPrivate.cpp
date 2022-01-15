#include "QCefViewPrivate.h"

#pragma region std_headers
#include <stdexcept>
#pragma endregion std_headers

#pragma region qt_headers
#include <QPlatformSurfaceEvent>
#include <QVBoxLayout>
#include <QWindow>
#include <QtDebug>
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

QCefViewPrivate::QCefViewPrivate(QCefView* view, const QString& url)
  : q_ptr(view)
  , pContext_(QCefContext::instance()->d_func())
  , pCefBrowser_(nullptr)
  , qBrowserWidget_(nullptr)
  , qBrowserWindow_(nullptr)
{
  createBrowser(url, nullptr);
}

QCefViewPrivate::~QCefViewPrivate()
{
  destroyBrowser();
}

void
QCefViewPrivate::createBrowser(const QString url, const QCefSetting* setting)
{
  Q_Q(QCefView);

  // Don't know why, on Linux platform if we use QCefView's winId() as
  // the parent, it will complain about `BadWindow`,
  // and the browser window will not be created, this never happen
  // on Windows and macOS, so we create a temporal QWindow as the
  // parent to create CEF browser window.
  QWindow w;
  CefWindowHandle p = (CefWindowHandle)(w.winId());

  // Set window info
  CefWindowInfo window_info;
#if defined(OS_MACOS)
  window_info.SetAsChild(p, 0, 0, 0, 0);
#elif defined(OS_WINDOWS)
  window_info.SetAsChild(p, RECT{ 0, 0, 0, 0 });
#elif defined(OS_LINUX)
  window_info.SetAsChild(p, CefRect{ 0, 0, 0, 0 });
#endif

  // create the browser object
  CefBrowserSettings browserSettings;
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
  pContext_->pClientDelegate_->insertBrowserViewMapping(pCefBrowser, q);

  // create QWindow from native browser window handle
  QWindow* browserWindow = QWindow::fromWinId((WId)(pCefBrowser->GetHost()->GetWindowHandle()));
  if (!browserWindow) {
    Q_ASSERT_X(browserWindow, "QCefViewPrivate::createBrowser", "Failed to query QWindow from cef browser window");
    pCefBrowser->GetHost()->CloseBrowser(true);
    return;
  }

  // create QWidget from cef browser widow
  QWidget* browserWidget = QWidget::createWindowContainer(browserWindow, q);
  if (!browserWidget) {
    Q_ASSERT_X(browserWidget, "QCefViewPrivate::createBrowser", "Failed to cretae QWidget from cef browser window");
    pCefBrowser->GetHost()->CloseBrowser(true);
    return;
  }

  pCefBrowser_ = pCefBrowser;
  qBrowserWindow_ = browserWindow;
  qBrowserWidget_ = browserWidget;

  qBrowserWindow_->installEventFilter(this);
  return;
}

void
QCefViewPrivate::destroyBrowser()
{
  if (!pCefBrowser_)
    return;

  // remove from parent tree
  qBrowserWindow_->setParent(nullptr);

  // remove from delegate mapping
  pContext_->pClientDelegate_->removeBrowserViewMapping(pCefBrowser_);

  // clean resource
  pCefBrowser_->StopLoad();
  pCefBrowser_->GetHost()->CloseBrowser(true);
  pCefBrowser_ = nullptr;
  qBrowserWidget_ = nullptr;
  qBrowserWindow_ = nullptr;
}

int
QCefViewPrivate::browserId()
{
  if (pCefBrowser_)
    return pCefBrowser_->GetIdentifier();

  return -1;
}

void
QCefViewPrivate::closeAllBrowsers()
{}

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
                              const QVariantMap& args,
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
QCefViewPrivate::sendEventNotifyMessage(int frameId, const QString& name, const QVariantMap& args)
{
  CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create(TRIGGEREVENT_NOTIFY_MESSAGE);
  CefRefPtr<CefListValue> arguments = msg->GetArgumentList();

  int idx = 0;
  CefString eventName = name.toStdString();
  arguments->SetString(idx++, eventName);

  CefRefPtr<CefDictionaryValue> dict = CefDictionaryValue::Create();

  CefString cefStr;
  cefStr.FromString(name.toUtf8().constData());
  dict->SetString("name", cefStr);

  QList<QString> keys = args.keys();
  for (const QString& key : keys) {
    auto value = args[key];
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    auto valueType = value.type();
#define TypeClass QVariant
#define StringType String
#else
    auto valueType = value.typeId();
#define TypeClass QMetaType
#define StringType QString
#endif
    if (valueType == TypeClass::Bool)
      dict->SetBool(key.toUtf8().constData(), value.toBool());
    else if (valueType == TypeClass::Int || valueType == TypeClass::UInt)
      dict->SetInt(key.toUtf8().constData(), value.toInt());
    else if (valueType == TypeClass::Double)
      dict->SetDouble(key.toUtf8().constData(), value.toDouble());
    else if (valueType == TypeClass::StringType) {
      cefStr.FromString(value.toString().toUtf8().constData());
      dict->SetString(key.toUtf8().constData(), cefStr);
    } else {
      Q_ASSERT_X(false, "QCefViewPrivate::sendEventNotifyMessage", "Unsupport event argument type");
    }
  }

  arguments->SetDictionary(idx++, dict);

  return pContext_->pClient_->TriggerEvent(pCefBrowser_, frameId, msg);
}

void
QCefViewPrivate::onToplevelWidgetMoveOrResize()
{
  notifyMoveOrResizeStarted();
}

bool
QCefViewPrivate::eventFilter(QObject* watched, QEvent* event)
{
  if (watched == qBrowserWindow_) {
    if (QEvent::PlatformSurface == event->type()) {
      auto e = (QPlatformSurfaceEvent*)event;
      auto sufaceType = e->surfaceEventType();
      if (e->surfaceEventType() == QPlatformSurfaceEvent::SurfaceAboutToBeDestroyed) {
        destroyBrowser();
      }
    }
  }
  return QObject::eventFilter(watched, event);
}
