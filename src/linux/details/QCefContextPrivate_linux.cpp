#include "../../details/QCefContextPrivate.h"

#include "../../details/QCefConfigPrivate.h"

bool
QCefContextPrivate::initializeCef(const QCefConfig* config)
{
  // Build CefSettings
  CefSettings cef_settings;
  QCefConfigPrivate::CopyToCefSettings(config, &cef_settings);

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
  auto bridgeObjectName = config ? config->bridgeObjectName().toStdString() : std::string();
  auto app = new CefViewBrowserApp(bridgeObjectName, appDelegate);
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
