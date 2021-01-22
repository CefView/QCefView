#include "qcefviewtest.h"
#include <QtWidgets/QApplication>

int
main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  QCefViewTest w;
  w.show();
  return a.exec();
}
