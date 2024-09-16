#include <QApplication>
#include <QDir>
#include <QStandardPaths>

#include <QCefContext.h>

#include "MainWindow.h"

int
main(int argc, char* argv[])
{
#if (QT_VERSION <= QT_VERSION_CHECK(6, 0, 0))
  // For off-screen rendering, Qt::AA_EnableHighDpiScaling must be enabled. If not,
  // then all devicePixelRatio methods will always return 1.0,
  // so CEF will not scale the web content
  // NOET: There is bugs in Qt 6.2.4, the HighDpi doesn't work
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
  QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
#endif

  // create QApplication instance
  QApplication a(argc, argv);

  // build QCefConfig
  QCefConfig config;
  // set user agent
  config.setUserAgent("QCefViewTest");
  // set log level
  config.setLogLevel(QCefConfig::LOGSEVERITY_DEFAULT);
  // set JSBridge object name (default value is CefViewClient)
  config.setBridgeObjectName("CallBridge");
  // set Built-in scheme name (default value is CefView)
  config.setBuiltinSchemeName("CefView");
  // port for remote debugging (default is 0 and means to disable remote debugging)
  config.setRemoteDebuggingPort(9000);
  // set background color for all browsers
  // (QCefSetting.setBackgroundColor will overwrite this value for specified browser instance)
  // config.setBackgroundColor(Qt::lightGray);

  // WindowlessRenderingEnabled is set to true by default,
  // set to false to disable the OSR mode
  config.setWindowlessRenderingEnabled(true);

  // add command line args, you can any cef supported switches or parameters
  config.addCommandLineSwitch("use-mock-keychain");
  // config.addCommandLineSwitch("disable-gpu");
  // config.addCommandLineSwitch("enable-media-stream");
  // config.addCommandLineSwitch("allow-file-access-from-files");
  // config.addCommandLineSwitch("disable-spell-checking");
  // config.addCommandLineSwitch("disable-site-isolation-trials");
  // config.addCommandLineSwitch("enable-aggressive-domstorage-flushing");
  config.addCommandLineSwitchWithValue("renderer-process-limit", "1");
  // allow remote debugging
  config.addCommandLineSwitchWithValue("remote-allow-origins", "*");
  // config.addCommandLineSwitchWithValue("disable-features", "BlinkGenPropertyTrees,TranslateUI,site-per-process");

  // set cache folder
  config.setCachePath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));

  // create QCefContext instance with config,
  // the lifecycle of cefContext must be the same as QApplication instance
  QCefContext cefContext(&a, argc, argv, &config);

  // application window
  MainWindow w;
  w.show();

  // flying
  return a.exec();
}
