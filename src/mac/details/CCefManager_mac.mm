#include "../../details/CCefManager.h"

#import <Cocoa/Cocoa.h>

#pragma region cef_headers
#include <include/cef_sandbox_mac.h>
#include <include/wrapper/cef_library_loader.h>
#pragma endregion cef_headers

#include "../../details/CCefSetting.h"

#define CEF_BINARY_NAME "Chromium Embedded Framework"
#define CEF_FRAMEWORK_NAME "Chromium Embedded Framework.framework"
#define HELPER_BUNDLE_NAME "CefViewWing.app"
#define HELPER_BINARY_NAME "CefViewWing"

@interface PathFactory : NSObject
+ (NSString*) AppMainBundlePath;
+ (NSString*) CefFrameworkPath;
+ (NSString*) CefSubprocessPath;
@end

@implementation PathFactory
+ (NSString*) AppMainBundlePath {
  return [[NSBundle mainBundle] bundlePath];
}

+ (NSString*) CefFrameworkPath {
  NSString* path = [[NSBundle bundleForClass:[PathFactory class]] resourcePath];
  path = [path stringByAppendingPathComponent:@CEF_FRAMEWORK_NAME];
  return path;
}

+ (NSString*) CefSubprocessPath {
  NSString* path = [[NSBundle bundleForClass:[PathFactory class]] resourcePath];
  path = [path stringByAppendingPathComponent:@HELPER_BUNDLE_NAME];
  path = [path stringByAppendingPathComponent:@"Contents"];
  path = [path stringByAppendingPathComponent:@"MacOS"];
  path = [path stringByAppendingPathComponent:@HELPER_BINARY_NAME];
  return path;
}
@end

const char*
appMainBundlePath()
{
  static std::string path;
  if (!path.empty())
    return path.c_str();

  @autoreleasepool {
    path = [PathFactory AppMainBundlePath].UTF8String;
  }
  return path.c_str();
}

const char*
cefFrameworkPath()
{
  static std::string path;
  if (!path.empty())
    return path.c_str();

  @autoreleasepool {
    path = [PathFactory CefFrameworkPath].UTF8String;
  }
  return path.c_str();
}

const char*
cefSubprocessPath()
{
  static std::string path;
  if (!path.empty())
    return path.c_str();

  @autoreleasepool {
    path = [PathFactory CefSubprocessPath].UTF8String;
  }
  return path.c_str();
}

const char*
cefLibraryPath()
{
  static std::string path;
  if (!path.empty())
    return path.c_str();

  path = cefFrameworkPath();
  path += "/";
  path += CEF_BINARY_NAME;
  return path.c_str();
}

bool
loadCefLibrary()
{
  return (1 == cef_load_library(cefLibraryPath()));
}

void
freeCefLibrary()
{
  cef_unload_library();
}

void
runMessageLoop()
{
  CefRunMessageLoop();
}

void
exitMessageLoop()
{
  CefQuitMessageLoop();
}

bool
CCefManager::initializeCef(int argc, char* argv[], const QCefSetting& settings)
{
  // load the cef library
  if (!loadCefLibrary()) {
    return false;
  }

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
  cef_settings.no_sandbox = true;
  cef_settings.pack_loading_disabled = false;
  cef_settings.multi_threaded_message_loop = false;

  // Initialize CEF.
  CefMainArgs main_args(argc, argv);
  auto app = new CefViewBrowserApp(settings.d->bridgeObjectName_.ToString());
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

  freeCefLibrary();
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
