#include "../../details/CCefManager.h"

#include "../../details/CCefSetting.h"

bool
CCefManager::initializeCef(int argc, char* argv[], const QCefSetting& settings)
{
  // Enable High-DPI support on Windows 7 or newer.
  CefEnableHighDPISupport();

  // Build CefSettings
  CefSettings cef_settings;
  if (!settings.d->browserSubProcessPath_.empty())
    CefString(&cef_settings.browser_subprocess_path) = settings.d->browserSubProcessPath_;
  if (!settings.d->resourceDirectoryPath_.empty())
    CefString(&cef_settings.resources_dir_path) = settings.d->resourceDirectoryPath_;
  if (!settings.d->localesDirectoryPath_.empty())
    CefString(&cef_settings.locales_dir_path) = settings.d->localesDirectoryPath_;
  if (!settings.d->userAgent_.empty())
    CefString(&cef_settings.user_agent) = settings.d->userAgent_;
  if (!settings.d->cachePath_.empty())
    CefString(&cef_settings.cache_path) = settings.d->cachePath_;
  if (!settings.d->userDataPath_.empty())
    CefString(&cef_settings.user_data_path) = settings.d->userDataPath_;
  if (!settings.d->locale_.empty())
    CefString(&cef_settings.locale) = settings.d->locale_;
  if (!settings.d->acceptLanguageList_.empty())
    CefString(&cef_settings.accept_language_list) = settings.d->acceptLanguageList_;

  cef_settings.persist_session_cookies = settings.d->persistSessionCookies_;
  cef_settings.persist_user_preferences = settings.d->persistUserPreferences_;
  cef_settings.background_color = settings.d->backgroundColor_;

#ifndef NDEBUG
  cef_settings.log_severity = LOGSEVERITY_DEFAULT;
  cef_settings.remote_debugging_port = settings.d->remoteDebuggingport_;
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
  CefMainArgs main_args(::GetModuleHandle(nullptr));
  auto app = new CefViewBrowserApp(settings.d->bridgeObjectName_);
  void* sandboxInfo = nullptr;
#if defined(CEF_USE_SANDBOX)
  // Manage the life span of the sandbox information object. This is necessary
  // for sandbox support on Windows. See cef_sandbox_win.h for complete details.
  static CefScopedSandboxInfo scoped_sandbox;
  sandbox_info = scoped_sandbox.sandbox_info();
#endif
  if (!CefInitialize(main_args, cef_settings, app, sandboxInfo)) {
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
CCefManager::doCefWork()
{
  CefDoMessageLoopWork();
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
  is_exiting_ = true;
  std::lock_guard<std::mutex> lock(handler_set_locker_);
  if (handler_set_.empty()) {
    return;
  }

  for (auto handler : handler_set_) {
    handler->CloseAllBrowsers(true);
  }
}
