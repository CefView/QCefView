#include "../../details/QCefContextPrivate.h"

#include <QDir>

#include <CefViewCoreProtocol.h>

#include "../../details/QCefConfigPrivate.h"

bool
QCefContextPrivate::initializeCef(const QCefConfig* config)
{
  // Build CefSettings
  CefSettings cef_settings;
  QCefConfigPrivate::CopyToCefSettings(config, &cef_settings);

  if (CefString(&cef_settings.browser_subprocess_path).empty()) {
    QString strExePath = QDir(QCoreApplication::applicationDirPath()).filePath(kCefViewRenderProcessName);
    CefString(&cef_settings.browser_subprocess_path) = QDir::toNativeSeparators(strExePath).toStdString();
  }

#if CEF_VERSION_MAJOR >= 125 && CEF_VERSION_MAJOR <= 127
  //  https://github.com/chromiumembedded/cef/issues/3685
  cef_settings.chrome_runtime = true;
#endif

  // fixed values
  cef_settings.pack_loading_disabled = false;

#if defined(CEF_USE_QT_EVENT_LOOP)
  cef_settings.multi_threaded_message_loop = false;
  cef_settings.external_message_pump = true;
#else
  cef_settings.multi_threaded_message_loop = true;
  cef_settings.external_message_pump = false;
#endif

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
}
