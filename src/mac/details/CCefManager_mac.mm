#include "CCefManager.h"

#include "CCefSetting.h"

CCefManager::CCefManager()
  : bridgeObjectName()
  , debugPort(0)
  , backgroundColor(-1)
  , is_initialized_(false)
  , is_exiting_(false)
{}

CCefManager&
CCefManager::getInstance()
{
  static CCefManager s_instance;
  return s_instance;
}

bool
CCefManager::initializeCef(int argc, const char* argv[])
{
  std::lock_guard<std::mutex> lock(init_locker_);

  // This is not the first time initialization
  if (is_initialized_)
    return true;

#if defined(OS_MACOS)
  // load the cef library
  if (!loadCefLibrary()) {
    return false;
  }
#endif

#if defined(OS_WINDOWS)
  // Enable High-DPI support on Windows 7 or newer.
  CefEnableHighDPISupport();
#endif

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

  auto app = new CefViewBrowserApp(CCefSetting::bridge_object_name);

#if defined(OS_WINDOWS)
  HINSTANCE hInstance = ::GetModuleHandle(nullptr);
  CefMainArgs main_args(hInstance);
#else
  CefMainArgs main_args(argc, (char**)argv);
#endif

  // Initialize CEF.
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

#if defined(OS_MACOS)
  freeCefLibrary();
#endif
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
  if (handler_set_.empty() && is_exiting_)
    CefQuitMessageLoop();
}

void
CCefManager::closeAllBrowserHandler()
{
  is_exiting_ = true;
  std::lock_guard<std::mutex> lock(handler_set_locker_);
  if (handler_set_.empty()) {
    CefQuitMessageLoop();
    return;
  }

  for (auto handler : handler_set_) {
    handler->CloseAllBrowsers(true);
    NSView* view = (__bridge NSView*)(handler->GetBrowser()->GetHost()->GetWindowHandle());
    [view removeFromSuperview];
  }
}

void
CCefManager::runMessageLoop()
{
  CefRunMessageLoop();
}

void
CCefManager::exitMessageLoop()
{
  CefQuitMessageLoop();
}

const char*
CCefManager::cefSubprocessPath()
{
  static std::string path;
  if (!path.empty())
    return path.c_str();

  @autoreleasepool {
    NSString* fxPath = [[NSBundle bundleForClass:CocoaCefSetting.class] resourcePath];
    fxPath = [fxPath stringByAppendingPathComponent:@HELPER_BUNDLE_NAME];
    fxPath = [fxPath stringByAppendingPathComponent:@"Contents"];
    fxPath = [fxPath stringByAppendingPathComponent:@"MacOS"];
    fxPath = [fxPath stringByAppendingPathComponent:@HELPER_BINARY_NAME];
    path = fxPath.UTF8String;
  }
  return path.c_str();
}

const char*
CCefManager::cefFrameworkPath()
{
  static std::string path;
  if (!path.empty())
    return path.c_str();

  @autoreleasepool {
    NSString* fxPath = [[NSBundle bundleForClass:CocoaCefSetting.class] resourcePath];
    fxPath = [fxPath stringByAppendingPathComponent:@CEF_FRAMEWORK_NAME];
    path = fxPath.UTF8String;
  }
  return path.c_str();
}

const char*
CCefManager::cefLibraryPath()
{
  static std::string path;
  if (!path.empty())
    return path.c_str();

  path = cefFrameworkPath();
  path += "/";
  path += CEF_BINARY_NAME;
  return path.c_str();
}

const char*
CCefManager::appMainBundlePath()
{
  static std::string path;
  if (!path.empty())
    return path.c_str();

  @autoreleasepool {
    path = [[[NSBundle mainBundle] bundlePath] UTF8String];
  }
  return path.c_str();
}

bool
CCefManager::loadCefLibrary()
{
  return (1 == cef_load_library(cefLibraryPath()));
}

void
CCefManager::freeCefLibrary()
{
  cef_unload_library();
}
