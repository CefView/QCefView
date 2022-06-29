#include "MainWindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonValue>
#include <QMessageBox>
#include <QRandomGenerator>

#include <QCefContext.h>

#define URL_ROOT "http://QCefViewDoc"
#define INDEX_URL URL_ROOT "/index.html"
#define TUTORIAL_URL URL_ROOT "/tutorial.html"

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent /*, Qt::FramelessWindowHint*/)
{
  ui.setupUi(this);

  connect(ui.btn_reCreate, &QPushButton::clicked, this, &MainWindow::onBtnRecreateClicked);
  connect(ui.btn_changeColor, &QPushButton::clicked, this, &MainWindow::onBtnChangeColorClicked);
  connect(ui.btn_setFocus, &QPushButton::clicked, this, &MainWindow::onBtnSetFocusClicked);
  connect(ui.btn_callJSCode, &QPushButton::clicked, this, &MainWindow::onBtnCallJSCodeClicked);
  connect(ui.btn_newBrowser, &QPushButton::clicked, this, &MainWindow::onBtnNewBrowserClicked);
  connect(ui.btn_quitApp, &QPushButton::clicked, qApp, &QCoreApplication::quit);

  QHBoxLayout* layout = new QHBoxLayout();
  layout->setContentsMargins(2, 2, 2, 2);
  layout->setSpacing(3);
  layout->addWidget(ui.nativeContainer);
  layout->addWidget(ui.cefContainer);
  centralWidget()->setLayout(layout);

  // build the path to the web resource
  QDir dir = QCoreApplication::applicationDirPath();
#if defined(Q_OS_MACOS)
  QString webResourceDir = /*QString("file://") +*/ QDir::toNativeSeparators(dir.filePath("../Resources/webres"));
#else
  QString webResourceDir = /*QString("file://") +*/ QDir::toNativeSeparators(dir.filePath("webres"));
#endif

  // add a local folder to URL map (global)
  QCefContext::instance()->addLocalFolderResource(webResourceDir, URL_ROOT);

  createCefView();
}

MainWindow::~MainWindow() {}

void
MainWindow::createCefView()
{
  ///*
  // build settings for per QCefView
  QCefSetting setting;
  setting.setPlugins(false);
  setting.setWindowlessFrameRate(60);
  setting.setBackgroundColor(QColor::fromRgba(qRgba(255, 255, 220, 255)));
  // setting.setBackgroundColor(Qt::blue);

  // create the QCefView widget and add it to the layout container
  // cefViewWidget = new CefViewWidget(INDEX_URL, &setting);

  // this site is for test web events
  cefViewWidget = new CefViewWidget("http://output.jsbin.com/rinece", &setting, this);

  //
  // cefViewWidget = new CefViewWidget("https://mdn.dev/", &setting, this);

  // this site is for test OSR performance
  // cefViewWidget = new CefViewWidget("https://www.testufo.com", &setting, this);

  // this site is test for input devices
  // cefViewWidget = new CefViewWidget("https://devicetests.com", &setting);

  ui.cefContainer->layout()->addWidget(cefViewWidget);

  // cefViewWidget->setCefContextMenuPolicy(Qt::CefDisableAllContextMenu);

  // connect the invokeMethod to the slot
  connect(cefViewWidget, &QCefView::invokeMethod, this, &MainWindow::onInvokeMethod);

  // connect the cefQueryRequest to the slot
  connect(cefViewWidget, &QCefView::cefQueryRequest, this, &MainWindow::onQCefQueryRequest);

  connect(cefViewWidget, &QCefView::draggableRegionChanged, this, &MainWindow::onDraggableRegionChanged);

  connect(cefViewWidget, &QCefView::reportJavascriptResult, this, &MainWindow::onJavascriptResult);

  connect(cefViewWidget, &QCefView::loadStart, this, &MainWindow::onLoadStart);
  connect(cefViewWidget, &QCefView::loadEnd, this, &MainWindow::onLoadEnd);
  connect(cefViewWidget, &QCefView::loadError, this, &MainWindow::onLoadError);
  //*/
}

void
MainWindow::onDraggableRegionChanged(const QRegion& draggableRegion, const QRegion& nonDraggableRegion)
{
  draggableRegion_ = draggableRegion;
  nonDraggableRegion_ = nonDraggableRegion;
}

void
MainWindow::onInvokeMethod(int browserId, int64_t frameId, const QString& method, const QVariantList& arguments)
{
  // extract the arguments and dispatch the invocation to corresponding handler
  if (0 == method.compare("TestMethod")) {
    QString title("QCef InvokeMethod Notify");
    QString text = QString("================== Current Thread: QT_UI ==================\r\n"
                           "Frame: %1\r\n"
                           "Method: %2\r\n"
                           "Arguments:\r\n")
                     .arg(frameId)
                     .arg(method);

    for (int i = 0; i < arguments.size(); i++) {
      auto jv = QJsonValue::fromVariant(arguments[i]);

      // clang-format off
          text.append(
              QString("%1 Type:%2, Value:%3\r\n")
              .arg(i).arg(arguments[i].typeName()).arg(arguments[i].toString())
          );
      // clang-format on
    }

    auto jsonValue = QJsonDocument::fromVariant(arguments);
    auto jsonString = QString(jsonValue.toJson());
    text.append(QString("\r\nArguments List in JSON format:\r\n%1").arg(jsonString));

    QMessageBox::information(this->window(), title, text);
  } else {
  }
}

void
MainWindow::onQCefQueryRequest(int browserId, int64_t frameId, const QCefQuery& query)
{
  QString title("QCef Query Request");
  QString text = QString("Current Thread: QT_UI\r\n"
                         "Query: %1")
                   .arg(query.request());

  QMessageBox::information(this->window(), title, text);

  QString response = query.request().toUpper();
  query.setResponseResult(true, response);
  cefViewWidget->responseQCefQuery(query);
}

void
MainWindow::onJavascriptResult(int browserId, int64_t frameId, int64_t context, const QVariant& result)
{
  auto jsonValue = QJsonDocument::fromVariant(result);
  auto jsonString = QString(jsonValue.toJson());

  QString title("Javascript result notification");
  QString text = QString("Context id: %1\r\nResult in JSON format:\r\n%2").arg(context).arg(jsonString);

  QMessageBox::information(this->window(), title, text);
}

void
MainWindow::onLoadingStateChanged(int browserId, bool isLoading, bool canGoBack, bool canGoForward)
{
  qDebug() << "onLoadingStateChanged, browserId:" << browserId << ", isLoading:" << isLoading
           << ", canGoBack:" << canGoBack << ", canGoForward:" << canGoForward;
}

void
MainWindow::onLoadStart(int browserId, qint64 frameId, bool isMainFrame, int transition_type)
{
  qDebug() << "onLoadStart, browserId:" << browserId << ", frameId:" << frameId << ", isMainFrame:" << isMainFrame
           << ", transitionType:" << transition_type;
}

void
MainWindow::onLoadEnd(int browserId, qint64 frameId, bool isMainFrame, int httpStatusCode)
{
  qDebug() << "onLoadEnd, browserId:" << browserId << ", frameId:" << frameId << ", isMainFrame:" << isMainFrame
           << ", httpStatusCode:" << httpStatusCode;
}

void
MainWindow::onLoadError(int browserId,
                        qint64 frameId,
                        bool isMainFrame,
                        int errorCode,
                        const QString& errorMsg,
                        const QString& failedUrl)
{
  qDebug() << "onLoadError, browserId:" << browserId << ", frameId:" << frameId << ", isMainFrame:" << isMainFrame
           << ", errorCode:" << errorCode;
}

void
MainWindow::onBtnRecreateClicked()
{
  if (cefViewWidget) {
    cefViewWidget->deleteLater();
    cefViewWidget = nullptr;
  }

  createCefView();
}

void
MainWindow::onBtnChangeColorClicked()
{
  if (cefViewWidget) {
    // create a random color
    QColor color(QRandomGenerator::global()->generate());

    // create the cef event and set the arguments
    QCefEvent event("colorChange");
    event.arguments().append(QVariant::fromValue(color.name(QColor::HexArgb)));

    // broadcast the event to all frames in all browsers created by this QCefView widget
    cefViewWidget->broadcastEvent(event);
  }
}

void
MainWindow::onBtnCallJSCodeClicked()
{
  int64_t context = 1000;
  QString code = "alert('hello QCefView'); return {k1: 'str', k2: true, k3: 100};";
  cefViewWidget->executeJavascriptWithResult(QCefView::MainFrameID, code, "", context);
}

void
MainWindow::onBtnSetFocusClicked()
{
  if (cefViewWidget) {
    cefViewWidget->setFocus();
  }
}

void
MainWindow::onBtnNewBrowserClicked()
{
  QMainWindow* w = new QMainWindow(nullptr);
  w->setAttribute(Qt::WA_DeleteOnClose);

  QCefSetting settings;
  QCefView* view = new QCefView("https://cefview.github.io/QCefView/", &settings, w);

  w->setCentralWidget(view);
  w->resize(1024, 768);
  w->show();
}
