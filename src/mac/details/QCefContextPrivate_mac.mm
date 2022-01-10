#include "../../details/QCefContextPrivate.h"

#import <Cocoa/Cocoa.h>
#import <objc/runtime.h>

#pragma region cef_headers
#include <include/cef_application_mac.h>
#include <include/cef_sandbox_mac.h>
#include <include/wrapper/cef_library_loader.h>
#pragma endregion cef_headers

#include "../../details/QCefConfigPrivate.h"

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

bool g_handling_send_event = false;

@interface NSApplication (QCefApp) <CefAppProtocol>
- (void)_swizzled_sendEvent:(NSEvent *)event;
- (void)_swizzled_terminate:(id)sender;
@end

@implementation NSApplication (QCefApp)
// wraps sendEvent and terminate
+ (void)load {
    Method original = class_getInstanceMethod(self, @selector(sendEvent));
    Method swizzled =
        class_getInstanceMethod(self, @selector(_swizzled_sendEvent));
    Method originalTerm = class_getInstanceMethod(self,
                                                  @selector(terminate:));
    Method swizzledTerm =
        class_getInstanceMethod(self, @selector(_swizzled_terminate:));

    method_exchangeImplementations(original, swizzled);
    method_exchangeImplementations(originalTerm, swizzledTerm);
}

- (BOOL)isHandlingSendEvent {
    return g_handling_send_event;
}

- (void)setHandlingSendEvent:(BOOL)handlingSendEvent {
    g_handling_send_event = handlingSendEvent;
}

- (void)_swizzled_sendEvent:(NSEvent *)event {
    CefScopedSendingEvent sendingEventScoper;

    [self _swizzled_sendEvent:event];
}

- (void)_swizzled_terminate:(id)sender {
    [self _swizzled_terminate:sender];
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

bool
QCefContextPrivate::initializeCef(const QCefConfigPrivate* config)
{
  // load the cef library
  if (!loadCefLibrary()) {
    return false;
  }

  // Build CefSettings
  CefSettings cef_settings;
  CefString(&cef_settings.framework_dir_path) = cefFrameworkPath();
  CefString(&cef_settings.browser_subprocess_path) = cefSubprocessPath();

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
  auto app = new CefViewBrowserApp(config->bridgeObjectName_, pAppDelegate_);
  if (!CefInitialize(main_args, cef_settings, app, nullptr)) {
    assert(0);
    return false;
  }

  app_ = app;

  return true;
}

void
QCefContextPrivate::uninitializeCef()
{
  if (!app_)
    return;

  // Destroy the application
  app_ = nullptr;

  // shutdown the cef
  CefShutdown();

  freeCefLibrary();
}
