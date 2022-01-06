#include "qcefviewtest.h"
#include <QtWidgets/QApplication>

#include <QCefContext.h>

int
main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  // build QCefConfig
  QCefConfig config(argc, argv);
  config.setBridgeObjectName("CallBridge");
  config.setRemoteDebuggingPort(9000);
  config.setUserAgent("QCefViewTest");

  // initialize QCefContext instance with config
  QCefContext cefContext(&a, &config);

  QCefViewTest w;
  w.show();

  int rc = a.exec();

  return rc;
}
