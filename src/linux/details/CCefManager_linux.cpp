#include "../../details/CCefManager.h"

#include "../../details/QCefSettingPrivate.h"

bool
CCefManager::initializeCef(const QCefSettingPrivate* settings)
{
  // Build CefSettings
  CefSettings cef_settings;
  if (!settings->browserSubProcessPath_.empty())
    CefString(&cef_settings.browser_subprocess_path) = settings->browserSubProcessPath_;
  if (!settings->resourceDirectoryPath_.empty())
    CefString(&cef_settings.resources_dir_path) = settings->resourceDirectoryPath_;
  if (!settings->localesDirectoryPath_.empty())
    CefString(&cef_settings.locales_dir_path) = settings->localesDirectoryPath_;
  if (!settings->userAgent_.empty())
    CefString(&cef_settings.user_agent) = settings->userAgent_;
  if (!settings->cachePath_.empty())
    CefString(&cef_settings.cache_path) = settings->cachePath_;
  if (!settings->userDataPath_.empty())
    CefString(&cef_settings.user_data_path) = settings->userDataPath_;
  if (!settings->locale_.empty())
    CefString(&cef_settings.locale) = settings->locale_;
  if (!settings->acceptLanguageList_.empty())
    CefString(&cef_settings.accept_language_list) = settings->acceptLanguageList_;

  cef_settings.persist_session_cookies = settings->persistSessionCookies_;
  cef_settings.persist_user_preferences = settings->persistUserPreferences_;
  cef_settings.background_color = settings->backgroundColor_;

#ifndef NDEBUG
  cef_settings.log_severity = LOGSEVERITY_DEFAULT;
  cef_settings.remote_debugging_port = settings->remoteDebuggingport_;
#else
  cef_settings.log_severity = LOGSEVERITY_DISABLE;
#endif

  // fixed values
  cef_settings.pack_loading_disabled = false;
  cef_settings.multi_threaded_message_loop = false;
  cef_settings.external_message_pump = true;

#if !defined(CEF_USE_SANDBOX)
  cef_settings.no_sandbox = true;
#endif

  // Initialize CEF.
  CefMainArgs main_args(settings->argc, settings->argv);
  auto app = new CefViewBrowserApp(settings->bridgeObjectName_, shared_from_this());
  if (!CefInitialize(main_args, cef_settings, app, nullptr)) {
    assert(0);
    return false;
  }

  app_ = app;

  return true;
}

void
CCefManager::uninitializeCef()
{
  if (!app_)
    return;

  // Destroy the application
  app_ = nullptr;

  // shutdown the cef
  CefShutdown();
}

void
CCefManager::removeBrowserHandler(CefRefPtr<CefViewBrowserHandler> handler)
{
  std::lock_guard<std::mutex> lock(handler_set_locker_);
  if (handler_set_.empty())
    return;

  handler->CloseAllBrowsers(true);

  handler_set_.erase(handler);
}

void
CCefManager::closeAllBrowserHandler()
{
  std::lock_guard<std::mutex> lock(handler_set_locker_);
  if (handler_set_.empty()) {
    return;
  }

  for (auto handler : handler_set_) {
    handler->CloseAllBrowsers(true);
  }
}
