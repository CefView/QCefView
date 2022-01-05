#include "qcefviewtest.h"
#include <QtWidgets/QApplication>

#include <QCefContext.h>

int
main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  QCefConfig config(argc, argv);
  config.setBridgeObjectName("CallBridge");
  QCefContext cefContext(&a, &config);

  QCefViewTest w;
  w.show();

  int rc = a.exec();

  return rc;
}
