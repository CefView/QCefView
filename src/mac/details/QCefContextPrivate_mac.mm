#include "../../details/QCefContextPrivate.h"

#import <Cocoa/Cocoa.h>
#import <objc/runtime.h>

#pragma region cef_headers
#include <include/cef_application_mac.h>
#include <include/cef_sandbox_mac.h>
#include <include/wrapper/cef_library_loader.h>
#pragma endregion 

#include <CefViewCoreProtocol.h>

#include "../../details/QCefConfigPrivate.h"

#define CEF_BINARY_NAME "Chromium Embedded Framework"
#define CEF_FRAMEWORK_NAME "Chromium Embedded Framework.framework"
#define HELPER_BINARY_NAME kCefViewRenderProcessName
#define HELPER_BUNDLE_NAME HELPER_BINARY_NAME ".app"
#define CEFVIEW_FOLDER_NAME "CefView"

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
  NSString* path =  [[NSBundle bundleForClass:[PathFactory class]] resourcePath];
  path = [path stringByAppendingPathComponent:@CEFVIEW_FOLDER_NAME];
  path = [path stringByAppendingPathComponent:@CEF_FRAMEWORK_NAME];
  return path;
}

+ (NSString*) CefSubprocessPath {
  NSString* path =  [[NSBundle bundleForClass:[PathFactory class]] resourcePath];
  path = [path stringByAppendingPathComponent:@CEFVIEW_FOLDER_NAME];
  path = [path stringByAppendingPathComponent:@HELPER_BUNDLE_NAME];
  path = [path stringByAppendingPathComponent:@"Contents"];
  path = [path stringByAppendingPathComponent:@"MacOS"];
  path = [path stringByAppendingPathComponent:@HELPER_BINARY_NAME];
  return path;
}
@end

bool g_handling_send_event = false;

@interface NSApplication (CocoaCefApp) <CefAppProtocol>
- (void)_swizzled_sendEvent:(NSEvent *)event;
- (void)_swizzled_terminate:(id)sender;
- (void)_swizzled_run;
@end

@implementation NSApplication (CocoaCefApp)
// wraps sendEvent, terminate and run
+ (void)load {
  // swizzle the sendEvent method
  Method original_sendEvent = class_getInstanceMethod(self, @selector(sendEvent:));
  Method swizzled_sendEvent = class_getInstanceMethod(self, @selector(_swizzled_sendEvent:));
  method_exchangeImplementations(original_sendEvent, swizzled_sendEvent);

  // swizzle the terminate method
  Method original_terminate = class_getInstanceMethod(self, @selector(terminate:));
  Method swizzled_terminate = class_getInstanceMethod(self, @selector(_swizzled_terminate:));
  method_exchangeImplementations(original_terminate, swizzled_terminate);

  // swizzle the run method
  // Method original_run = class_getInstanceMethod(self, @selector(run));
  // Method swizzled_run = class_getInstanceMethod(self, @selector(_swizzled_run));
  // method_exchangeImplementations(original_run, swizzled_run);
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

- (void)_swizzled_run {
  Method original_run = class_getInstanceMethod(self.class, @selector(run));
  Method swizzled_run = class_getInstanceMethod(self.class, @selector(_swizzled_run));
  method_exchangeImplementations(original_run, swizzled_run);
  CefRunMessageLoop();
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
QCefContextPrivate::initializeCef(const QCefConfig* config)
{
  // load the cef library
  if (!loadCefLibrary()) {
    return false;
  }

  // Build CefSettings
  CefSettings cef_settings;
  QCefConfigPrivate::CopyToCefSettings(config, &cef_settings);

#if CEF_VERSION_MAJOR >= 125 && CEF_VERSION_MAJOR <= 127
  //  https://github.com/chromiumembedded/cef/issues/3685
  cef_settings.chrome_runtime = true;
#endif

  // fixed values
  CefString(&cef_settings.framework_dir_path) = cefFrameworkPath();
  CefString(&cef_settings.browser_subprocess_path) = cefSubprocessPath();
  cef_settings.pack_loading_disabled = false;
  cef_settings.external_message_pump = true;
  cef_settings.multi_threaded_message_loop = false;

#if !defined(CEF_USE_SANDBOX)
  cef_settings.no_sandbox = true;
#endif

  // Initialize CEF.
  auto cmdArgs = QCefConfigPrivate::GetCommandLineArgs(config);
  auto appDelegate = std::make_shared<CCefAppDelegate>(this, cmdArgs);
  auto builtinSchemeName = config ? config->builtinSchemeName().toStdString() : std::string();
  auto bridgeObjectName = config ? config->bridgeObjectName().toStdString() : std::string();
  auto app = new CefViewBrowserApp(builtinSchemeName, bridgeObjectName, appDelegate);

  CefMainArgs main_args(argc_, argv_);
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

  freeCefLibrary();
}

