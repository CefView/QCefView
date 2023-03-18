#include <QApplication>

#include <QCefContext.h>

#include "MainWindow.h"

int
main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  // build QCefConfig
  QCefConfig config;
  config.setUserAgent("QCefViewTest");
  config.setLogLevel(QCefConfig::LOGSEVERITY_DEFAULT);
  config.setBridgeObjectName("CallBridge");
  config.setRemoteDebuggingPort(9000);
  config.setBackgroundColor(Qt::lightGray);

  // WindowlessRenderingEnabled is set to true by default, set to false to disable the OSR mode
  // config.setWindowlessRenderingEnabled(false);

  // add command line args
  config.addCommandLineSwitch("use-mock-keychain");
  // config.addCommandLineSwitch("disable-gpu");
  // config.addCommandLineSwitch("enable-media-stream");
  // config.addCommandLineSwitch("allow-file-access-from-files");
  // config.addCommandLineSwitch("disable-spell-checking");
  // config.addCommandLineSwitch("disable-site-isolation-trials");
  // config.addCommandLineSwitch("enable-aggressive-domstorage-flushing");
  config.addCommandLineSwitchWithValue("renderer-process-limit", "1");
  // config.addCommandLineSwitchWithValue("disable-features", "BlinkGenPropertyTrees,TranslateUI,site-per-process");

  // initialize QCefContext instance with config
  QCefContext cefContext(&a, argc, argv, &config);

  MainWindow w;
  w.show();

  return a.exec();
}
