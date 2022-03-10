#include "../../details/QCefContextPrivate.h"

#include "../../details/QCefConfigPrivate.h"

bool
QCefContextPrivate::initializeCef(const QCefConfig* config)
{
  // Build CefSettings
  CefSettings cef_settings;
  if (config)
    QCefConfigPrivate::CopyToCefSettings(config, &cef_settings);

  // fixed values
  cef_settings.pack_loading_disabled = false;
  cef_settings.multi_threaded_message_loop = false;
  cef_settings.external_message_pump = true;

#if !defined(CEF_USE_SANDBOX)
  cef_settings.no_sandbox = true;
#endif

  // Initialize CEF.
  auto cmdArgs = QCefConfigPrivate::GetCommandLineArgs(config);
  auto appDelegate = std::make_shared<CCefAppDelegate>(this, cmdArgs);
  auto app = new CefViewBrowserApp(config->bridgeObjectName_, appDelegate);
  CefMainArgs main_args(config->argc, config->argv);
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
