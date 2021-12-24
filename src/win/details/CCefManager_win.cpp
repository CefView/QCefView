#include "../../details/CCefManager.h"

#include "../../details/CCefSetting.h"

bool
CCefManager::initializeCef(int argc, const char* argv[])
{
  std::lock_guard<std::mutex> lock(init_locker_);

  // This is not the first time initialization
  if (is_initialized_)
    return true;

  // Enable High-DPI support on Windows 7 or newer.
  CefEnableHighDPISupport();

  // This is the first time initialization
  CCefSetting::initializeInstance();

  // Build CefSettings
  CefSettings cef_settings;
  CefString(&cef_settings.browser_subprocess_path) = CCefSetting::browser_sub_process_path;
  CefString(&cef_settings.resources_dir_path) = CCefSetting::resource_directory_path;
  CefString(&cef_settings.locales_dir_path) = CCefSetting::locales_directory_path;
  CefString(&cef_settings.user_agent) = CCefSetting::user_agent;
  CefString(&cef_settings.cache_path) = CCefSetting::cache_path;
  CefString(&cef_settings.user_data_path) = CCefSetting::user_data_path;
  CefString(&cef_settings.locale) = CCefSetting::locale;
  CefString(&cef_settings.accept_language_list) = CCefSetting::accept_language_list;

  cef_settings.persist_session_cookies = CCefSetting::persist_session_cookies;
  cef_settings.persist_user_preferences = CCefSetting::persist_user_preferences;
  cef_settings.remote_debugging_port = CCefSetting::remote_debugging_port;
  cef_settings.background_color = CCefSetting::background_color;
  cef_settings.no_sandbox = true;
  cef_settings.pack_loading_disabled = false;
  cef_settings.multi_threaded_message_loop = true;

#ifndef NDEBUG
  cef_settings.log_severity = LOGSEVERITY_DEFAULT;
  cef_settings.remote_debugging_port = CCefSetting::remote_debugging_port;
#else
  cef_settings.log_severity = LOGSEVERITY_DISABLE;
#endif

  // Initialize CEF.
  CefMainArgs main_args(::GetModuleHandle(nullptr));
  auto app = new CefViewBrowserApp(CCefSetting::bridge_object_name);
  void* sandboxInfo = nullptr;
  if (!CefInitialize(main_args, cef_settings, app_, sandboxInfo)) {
    assert(0);
    return false;
  }

  app_ = app;
  is_initialized_ = true;

  return true;
}

void
CCefManager::uninitializeCef()
{
  if (!isInitialized())
    return;

  // Destroy the application
  app_ = nullptr;

  // shutdown the cef
  CefShutdown();
}

bool
CCefManager::isInitialized()
{
  std::lock_guard<std::mutex> lock(init_locker_);
  return is_initialized_;
}

bool
CCefManager::addCookie(const std::string& name,
                       const std::string& value,
                       const std::string& domain,
                       const std::string& url)
{
  CefCookie cookie;
  CefString(&cookie.name).FromString(name);
  CefString(&cookie.value).FromString(value);
  CefString(&cookie.domain).FromString(domain);
  return CefCookieManager::GetGlobalManager(nullptr)->SetCookie(CefString(url), cookie, nullptr);
}

void
CCefManager::registerBrowserHandler(CefRefPtr<CefViewBrowserHandler> handler)
{
  std::lock_guard<std::mutex> lock(handler_set_locker_);
  handler_set_.insert(handler);
}

void
CCefManager::removeBrowserHandler(CefRefPtr<CefViewBrowserHandler> handler)
{
  std::lock_guard<std::mutex> lock(handler_set_locker_);
  if (handler_set_.empty())
    return;

  handler_set_.erase(handler);
  if (handler_set_.empty() && is_exiting_) {
  }
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
