#include "qcefviewtest.h"

#include <QDir>
#include <QCoreApplication>
#include <QHBoxLayout>

QCefViewTest::QCefViewTest(QWidget* parent)
  : QMainWindow(parent)
{
  setWindowFlag(Qt::FramelessWindowHint);
  ui.setupUi(this);
  QHBoxLayout* layout = new QHBoxLayout();
  layout->setContentsMargins(2, 2, 2, 2);
  layout->setSpacing(3);

  connect(ui.btn_changeColor, SIGNAL(clicked()), this, SLOT(onBtnChangeColorClicked()));
  layout->addWidget(ui.nativeContainer);

  QDir dir = QCoreApplication::applicationDirPath();

#if defined(OS_MACOS)
  QString uri = QString("file://") + QDir::toNativeSeparators(dir.filePath("../Resources/QCefViewTestPage.html"));
#else
  QString uri = QString("file://") + QDir::toNativeSeparators(dir.filePath("QCefViewTestPage.html"));
#endif

  cefview = new CustomCefView(uri, this);
  // cefview = new CustomCefView("http://www.w3.org", this);
  ui.cefContainer->layout()->addWidget(cefview);
  layout->addWidget(ui.cefContainer);

  centralWidget()->setLayout(layout);
}

QCefViewTest::~QCefViewTest() {}

#include <windows.h>
#include <windowsx.h>

bool
QCefViewTest::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
  MSG* msg = (MSG*)message;
  switch (msg->message) {
    case WM_NCHITTEST:
      int xPos = GET_X_LPARAM(msg->lParam) - this->frameGeometry().x();
      int yPos = GET_Y_LPARAM(msg->lParam) - this->frameGeometry().y();
      break;
  }
  return QMainWindow::nativeEvent(eventType, message, result);
}
void
QCefViewTest::onBtnChangeColorClicked()
{
  cefview->changeColor();
}
