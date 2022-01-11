#include "../../details/QCefContextPrivate.h"

#include "../../details/QCefConfigPrivate.h"

bool
QCefContextPrivate::initializeCef(const QCefConfigPrivate* config)
{
  // Build CefSettings
  CefSettings cef_settings;
  if (!config->browserSubProcessPath_.empty())
    CefString(&cef_settings.browser_subprocess_path) = config->browserSubProcessPath_;
  if (!config->resourceDirectoryPath_.empty())
    CefString(&cef_settings.resources_dir_path) = config->resourceDirectoryPath_;
  if (!config->localesDirectoryPath_.empty())
    CefString(&cef_settings.locales_dir_path) = config->localesDirectoryPath_;
  if (!config->userAgent_.empty())
    CefString(&cef_settings.user_agent) = config->userAgent_;
  if (!config->cachePath_.empty())
    CefString(&cef_settings.cache_path) = config->cachePath_;
  if (!config->userDataPath_.empty())
    CefString(&cef_settings.user_data_path) = config->userDataPath_;
  if (!config->locale_.empty())
    CefString(&cef_settings.locale) = config->locale_;
  if (!config->acceptLanguageList_.empty())
    CefString(&cef_settings.accept_language_list) = config->acceptLanguageList_;

  cef_settings.persist_session_cookies = config->persistSessionCookies_;
  cef_settings.persist_user_preferences = config->persistUserPreferences_;
  cef_settings.background_color = config->backgroundColor_;

#ifndef NDEBUG
  cef_settings.log_severity = LOGSEVERITY_DEFAULT;
  cef_settings.remote_debugging_port = config->remoteDebuggingport_;
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
  CefMainArgs main_args(config->argc, config->argv);
  auto appDelegate = std::make_shared<CCefAppDelegate>(this);
  auto app = new CefViewBrowserApp(config->bridgeObjectName_, appDelegate);
  if (!CefInitialize(main_args, cef_settings, app, nullptr)) {
    assert(0);
    return false;
  }

  pApp_ = app;
  pAppDelegate_ = appDelegate;

  return true;
}

void
QCefContextPrivate::uninitializeCef()
{
  if (!pApp_)
    return;

  pAppDelegate_ = nullptr;
  pApp_ = nullptr;

  // shutdown the cef
  CefShutdown();
}
