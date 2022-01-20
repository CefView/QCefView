#include <QCoreApplication>
#include <QDir>
#include <QHBoxLayout>
#include <QMessageBox>

#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{
  ui.setupUi(this);
  QHBoxLayout* layout = new QHBoxLayout();
  layout->setContentsMargins(2, 2, 2, 2);
  layout->setSpacing(3);

  connect(ui.btn_changeColor, &QPushButton::clicked, this, &MainWindow::onBtnChangeColorClicked);
  layout->addWidget(ui.nativeContainer);

  QDir dir = QCoreApplication::applicationDirPath();

#if defined(OS_MACOS)
  QString uri = QString("file://") + QDir::toNativeSeparators(dir.filePath("../Resources/QCefViewTestPage.html"));
#else
  QString uri = QString("file://") + QDir::toNativeSeparators(dir.filePath("QCefViewTestPage.html"));
#endif

  QCefSetting setting;
  setting.setBackgroundColor(QColor::fromRgb(0, 0, 255));

  cefViewWidget = new CefViewWidget(uri, &setting, this);
  // cefView = new CustomCefView("https://www.w3schools.com/", &setting, this);
  ui.cefContainer->layout()->addWidget(cefViewWidget);
  layout->addWidget(ui.cefContainer);

  connect(cefViewWidget,
          SIGNAL(draggableRegionChanged(const QRegion&, const QRegion&)),
          this,
          SLOT(onDraggableRegionChanged(const QRegion&, const QRegion&)));

  connect(cefViewWidget,
          SIGNAL(invokeMethod(int, int, const QString&, const QVariantList&)),
          this,
          SLOT(onInvokeMethod(int, int, const QString&, const QVariantList&)));

  connect(cefViewWidget,
          SIGNAL(cefQueryRequest(int, int, const QCefQuery&)),
          this,
          SLOT(onQCefQueryRequest(int, int, const QCefQuery&)));

  centralWidget()->setLayout(layout);
}

MainWindow::~MainWindow() {}

void
MainWindow::onBtnChangeColorClicked()
{
  if (cefViewWidget)
    cefViewWidget->changeColor();
}

void
MainWindow::onDraggableRegionChanged(const QRegion& draggableRegion, const QRegion& nonDraggableRegion)
{
  draggableRegion_ = draggableRegion;
  nonDraggableRegion_ = nonDraggableRegion;
}

void
MainWindow::onInvokeMethod(int browserId, int frameId, const QString& method, const QVariantList& arguments)
{
  if (0 == method.compare("TestMethod")) {
    QMetaObject::invokeMethod(
      this,
      [=]() {
        QString title("QCef InvokeMethod Notify");
        QString text = QString("Current Thread: QT_UI\r\n"
                               "Method: %1\r\n"
                               "Arguments:\r\n")
                         .arg(method);

        for (int i = 0; i < arguments.size(); i++) {
          // clang-format off
          text.append(QString("%1 Type:%2, Value:%3\r\n")
              .arg(i)
              .arg(arguments[i].typeName())
              .arg(arguments[i].toString()));
          // clang-format on
        }

        QMessageBox::information(this->window(), title, text);
      },
      Qt::QueuedConnection);
  } else {
  }
}

void
MainWindow::onQCefQueryRequest(int browserId, int frameId, const QCefQuery& query)
{
  QMetaObject::invokeMethod(
    this,
    [=]() {
      QString title("QCef Query Request");
      QString text = QString("Current Thread: QT_UI\r\n"
                             "Query: %1")
                       .arg(query.request());

      QMessageBox::information(this->window(), title, text);

      QString response = query.request().toUpper();
      query.setResponseResult(true, response);
      cefViewWidget->responseQCefQuery(query);
    },
    Qt::QueuedConnection);
}

#if defined(OS_WINDOWS)
#include <windows.h>
#include <windowsx.h>
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
bool
MainWindow::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
#else
bool
MainWindow::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
#endif
  // MSG* msg = (MSG*)message;

  // qDebug() << "MSG.message:0x" << Qt::hex << msg->message;

  // switch (msg->message) {
  //  case WM_NCHITTEST:
  //    int xPos = GET_X_LPARAM(msg->lParam) - this->frameGeometry().x();
  //    int yPos = GET_Y_LPARAM(msg->lParam) - this->frameGeometry().y();

  //    qDebug() << "HitTest Point:(" << xPos << ", " << yPos << ")";

  //    if (this->childAt(xPos, yPos) == 0) {
  //      *result = HTCAPTION;

  //      if (xPos > 0 && xPos < 8)
  //        *result = HTLEFT;
  //      if (xPos > (this->width() - 8) && xPos < (this->width() - 0))
  //        *result = HTRIGHT;
  //      if (yPos > 0 && yPos < 8)
  //        *result = HTTOP;
  //      if (yPos > (this->height() - 8) && yPos < (this->height() - 0))
  //        *result = HTBOTTOM;
  //      if (xPos > 18 && xPos < 22 && yPos > 18 && yPos < 22)
  //        *result = HTTOPLEFT;
  //      if (xPos > (this->width() - 22) && xPos < (this->width() - 18) && yPos > 18 && yPos < 22)
  //        *result = HTTOPRIGHT;
  //      if (xPos > 18 && xPos < 22 && yPos > (this->height() - 22) && yPos < (this->height() - 18))
  //        *result = HTBOTTOMLEFT;
  //      if (xPos > (this->width() - 22) && xPos < (this->width() - 18) && yPos > (this->height() - 22) &&
  //          yPos < (this->height() - 18))
  //        *result = HTBOTTOMRIGHT;

  //      return true;
  //    }
  //}

  return QMainWindow::nativeEvent(eventType, message, result);
}
