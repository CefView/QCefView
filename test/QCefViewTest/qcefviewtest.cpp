#include "qcefviewtest.h"

#include <QCoreApplication>
#include <QDir>
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

  QCefSetting setting;

  // cefview = new CustomCefView(uri, &setting, this);
  cefView = new CustomCefView("https://www.w3schools.com/", &setting, this);
  ui.cefContainer->layout()->addWidget(cefView);
  layout->addWidget(ui.cefContainer);

  centralWidget()->setLayout(layout);
}

QCefViewTest::~QCefViewTest() {}

void
QCefViewTest::onBtnChangeColorClicked()
{
  static bool closed = false;
  if (!closed) {
    auto b = cefView->close();
    closed = true;
  } else {
    cefView->show();
    closed = false;
  }
  // delete cefView;
  // cefview->changeColor();
}
