#include "qcefviewtest.h"

#include <QDir>
#include <QCoreApplication>
#include <QHBoxLayout>

QCefViewTest::QCefViewTest(QWidget* parent)
  : QMainWindow(parent)
{
  ui.setupUi(this);
  QHBoxLayout* layout = new QHBoxLayout();
  layout->setContentsMargins(2, 2, 2, 2);
  layout->setSpacing(3);

  connect(ui.btn_changeColor, &QPushButton::clicked, this, &QCefViewTest::onBtnChangeColorClicked);
  layout->addWidget(ui.nativeContainer);

  QDir dir = QCoreApplication::applicationDirPath();

#if defined(OS_MACOS)
  QString uri = QString("file://") + QDir::toNativeSeparators(dir.filePath("../Resources/QCefViewTestPage.html"));
#else
  QString uri = QString("file://") + QDir::toNativeSeparators(dir.filePath("QCefViewTestPage.html"));
#endif

  // cefview = new CustomCefView(uri, this);
  cefview = new CustomCefView("http://www.w3.org", this);
  ui.cefContainer->layout()->addWidget(cefview);
  layout->addWidget(ui.cefContainer);

  centralWidget()->setLayout(layout);
}

QCefViewTest::~QCefViewTest() {}

void
QCefViewTest::onBtnChangeColorClicked()
{
  cefview->changeColor();
}
