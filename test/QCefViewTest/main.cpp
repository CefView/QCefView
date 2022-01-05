#include "qcefviewtest.h"
#include <QtWidgets/QApplication>

#include <QCefContext.h>

int
main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  QCefSetting settings;
  settings.setBridgeObjectName("CallBridge");

  QCefContext cefContext(&a, settings, argc, argv);

  QCefViewTest w;
  w.show();

  int rc = a.exec();

  return rc;
}
