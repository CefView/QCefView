#include "QCefViewPrivate.h"

#pragma region std_headers
#include <stdexcept>
#pragma endregion std_headers

#pragma region cef_headers
#include <include/cef_app.h>
#include <include/cef_browser.h>
#include <include/cef_frame.h>
#include <include/cef_parser.h>
#pragma endregion cef_headers

#include <CefViewCoreProtocol.h>

#include "CCefManager.h"

QList<QCefViewPrivate::FolderMapping> QCefViewPrivate::folderMappingList_;

QList<QCefViewPrivate::ArchiveMapping> QCefViewPrivate::archiveMappingList_;

QCefViewPrivate::QCefViewPrivate(const QString& url, QCefView* view, QCefWindow* win)
  : q_ptr(view)
  , pQCefViewHandler_(nullptr)
  , pCefHandlerDelegate_(nullptr)
{
  // Set window info
  CefWindowInfo window_info;

#if defined(OS_WINDOWS)
  window_info.SetAsChild((CefWindowHandle)(win->winId()), RECT{ 0, 0, 0, 0 });
#elif defined(OS_MACOS)
  window_info.SetAsChild((CefWindowHandle)(win->winId()), 0, 0, 0, 0);
#elif defined(OS_LINUX)
  window_info.SetAsChild((CefWindowHandle)(win->winId()), CefRect{ 0, 0, 0, 0 });
#endif

  CefBrowserSettings browserSettings;
  // disable all plugins
  browserSettings.plugins = STATE_DISABLED;

  // create the delegate
  auto pCefDelegate = std::make_shared<CCefHandlerDelegate>(view, win);

  // create the browser
  auto pQCefViewHandler = new CefViewBrowserHandler(pCefDelegate);

  // add archive mapping
  for (auto archiveMapping : archiveMappingList_) {
    pQCefViewHandler->AddArchiveResourceProvider(
      archiveMapping.path.toStdString(), archiveMapping.url.toStdString(), archiveMapping.psw.toStdString());
  }

  // add local folder mapping
  for (auto folderMapping : folderMappingList_) {
    pQCefViewHandler->AddLocalDirectoryResourceProvider(
      folderMapping.path.toStdString(), folderMapping.url.toStdString(), folderMapping.priority);
  }

  // create the main browser window.
  if (!CefBrowserHost::CreateBrowser(window_info,       // window info
                                     pQCefViewHandler,  // handler
                                     url.toStdString(), // url
                                     browserSettings,   // settings
                                     nullptr,
                                     CefRequestContext::GetGlobalContext())) {
    throw std::exception();
  }

  // hold the browser handler
  CCefManager::getInstance()->registerBrowserHandler(pQCefViewHandler);

  // update members
  pCefHandlerDelegate_ = pCefDelegate;
  pQCefViewHandler_ = pQCefViewHandler;
}

QCefViewPrivate::~QCefViewPrivate()
{
  if (pQCefViewHandler_) {
    pQCefViewHandler_->CloseAllBrowsers(true);
    CCefManager::getInstance()->removeBrowserHandler(pQCefViewHandler_);
    pQCefViewHandler_ = nullptr;
  }

  if (pCefHandlerDelegate_)
    pCefHandlerDelegate_.reset();
}

void
QCefViewPrivate::closeAllBrowsers()
{
  if (pQCefViewHandler_) {
    pQCefViewHandler_->CloseAllBrowsers(true);
  }
}

void
QCefViewPrivate::addLocalFolderResource(const QString& path, const QString& url)
{
  if (pQCefViewHandler_) {
    pQCefViewHandler_->AddLocalDirectoryResourceProvider(path.toStdString(), url.toStdString());
  }
}

void
QCefViewPrivate::addArchiveResource(const QString& path, const QString& url, const QString& password)
{
  if (pQCefViewHandler_) {
    pQCefViewHandler_->AddArchiveResourceProvider(path.toStdString(), url.toStdString(), password.toStdString());
  }
}

void
QCefViewPrivate::addCookie(const QString& name, const QString& value, const QString& domain, const QString& url)
{
  CCefManager::getInstance()->addCookie(
    name.toStdString(), value.toStdString(), domain.toStdString(), url.toStdString());
}

void
QCefViewPrivate::navigateToString(const QString& content)
{
  if (pQCefViewHandler_) {
    std::string data = content.toStdString();
    data = CefURIEncode(CefBase64Encode(data.c_str(), data.size()), false).ToString();
    data = "data:text/html;base64," + data;
    pQCefViewHandler_->GetBrowser()->GetMainFrame()->LoadURL(data);
  }
}

void
QCefViewPrivate::navigateToUrl(const QString& url)
{
  if (pQCefViewHandler_) {
    CefString strUrl;
    strUrl.FromString(url.toStdString());
    pQCefViewHandler_->GetBrowser()->GetMainFrame()->LoadURL(strUrl);
  }
}

bool
QCefViewPrivate::browserCanGoBack()
{
  if (pQCefViewHandler_)
    return pQCefViewHandler_->GetBrowser()->CanGoBack();

  return false;
}

bool
QCefViewPrivate::browserCanGoForward()
{
  if (pQCefViewHandler_)
    return pQCefViewHandler_->GetBrowser()->CanGoForward();

  return false;
}

void
QCefViewPrivate::browserGoBack()
{
  if (pQCefViewHandler_)
    pQCefViewHandler_->GetBrowser()->GoBack();
}

void
QCefViewPrivate::browserGoForward()
{
  if (pQCefViewHandler_)
    pQCefViewHandler_->GetBrowser()->GoForward();
}

bool
QCefViewPrivate::browserIsLoading()
{
  if (pQCefViewHandler_)
    return pQCefViewHandler_->GetBrowser()->IsLoading();

  return false;
}

void
QCefViewPrivate::browserReload()
{
  if (pQCefViewHandler_)
    pQCefViewHandler_->GetBrowser()->Reload();
}

void
QCefViewPrivate::browserStopLoad()
{
  if (pQCefViewHandler_)
    pQCefViewHandler_->GetBrowser()->StopLoad();
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
  if (pQCefViewHandler_) {
    CefString res;
    res.FromString(query.response().toStdString());
    return pQCefViewHandler_->ResponseQuery(query.id(), query.result(), res, query.error());
  }
  return false;
}

void
QCefViewPrivate::notifyMoveOrResizeStarted()
{
  if (pQCefViewHandler_) {
    CefRefPtr<CefBrowser> browser = pQCefViewHandler_->GetBrowser();
    if (browser) {
      CefRefPtr<CefBrowserHost> host = browser->GetHost();
      if (host)
        host->NotifyMoveOrResizeStarted();
    }
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

  return pQCefViewHandler_->TriggerEvent(frameId, msg);
}

void
QCefViewPrivate::onToplevelWidgetMoveOrResize()
{
  notifyMoveOrResizeStarted();
}

void
QCefViewPrivate::setContextMenuHandler(CefContextMenuHandler* handler)
{
  if (pQCefViewHandler_)
    pQCefViewHandler_->SetContextMenuHandler(handler);
}

void
QCefViewPrivate::setDialogHandler(CefDialogHandler* handler)
{
  if (pQCefViewHandler_)
    pQCefViewHandler_->SetDialogHandler(handler);
}

void
QCefViewPrivate::setDisplayHandler(CefDisplayHandler* handler)
{
  if (pQCefViewHandler_)
    pQCefViewHandler_->SetDisplayHandler(handler);
}

void
QCefViewPrivate::setDownloadHandler(CefDownloadHandler* handler)
{
  if (pQCefViewHandler_)
    pQCefViewHandler_->SetDownloadHandler(handler);
}

void
QCefViewPrivate::setJSDialogHandler(CefJSDialogHandler* handler)
{
  if (pQCefViewHandler_)
    pQCefViewHandler_->SetJSDialogHandler(handler);
}

void
QCefViewPrivate::setKeyboardHandler(CefKeyboardHandler* handler)
{
  if (pQCefViewHandler_)
    pQCefViewHandler_->SetKeyboardHandler(handler);
}
