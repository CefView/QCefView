#include "qcefviewtest.h"
#include <QtWidgets/QApplication>

#include <QCef.h>

int
main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  QCefSetting settings;
  settings.setBridgeObjectName("CallBridge");
  QCef::Initialize(settings, argc, argv);

  QCefViewTest w;
  w.show();

  int rc = a.exec();

  QCef::Uninitialize();
  return rc;
}
