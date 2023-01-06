#include "MainWindow.h"

#include <QCoreApplication>
#include <QDebug>
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

#ifdef Q_OS_MACOS
  this->ui.nativeContainer->setContentsMargins(0, 28, 0, 0);
#endif

  setupWindow();

  // setWindowFlags(Qt::FramelessWindowHint);
  // setAttribute(Qt::WA_TranslucentBackground);

  connect(ui.btn_reCreate, &QPushButton::clicked, this, &MainWindow::onBtnRecreateClicked);
  connect(ui.btn_changeColor, &QPushButton::clicked, this, &MainWindow::onBtnChangeColorClicked);
  connect(ui.btn_setFocus, &QPushButton::clicked, this, &MainWindow::onBtnSetFocusClicked);
  connect(ui.btn_callJSCode, &QPushButton::clicked, this, &MainWindow::onBtnCallJSCodeClicked);
  connect(ui.btn_newBrowser, &QPushButton::clicked, this, &MainWindow::onBtnNewBrowserClicked);
  connect(ui.btn_quitApp, &QPushButton::clicked, qApp, &QCoreApplication::quit);

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

  rightCefViewWidget = new CefViewWidget("https://www.testufo.com", nullptr, this);
  ui.rightCefViewContainer->layout()->addWidget(rightCefViewWidget);
}

MainWindow::~MainWindow() {}

void
MainWindow::createCefView()
{
  ///*
  // build settings for per QCefView
  QCefSetting setting;

#if CEF_VERSION_MAJOR < 100
  setting.setPlugins(false);
#endif

  setting.setWindowlessFrameRate(60);
  setting.setBackgroundColor(QColor::fromRgba(qRgba(255, 255, 220, 255)));
  // setting.setBackgroundColor(Qt::blue);

  // create the QCefView widget and add it to the layout container
  leftCefViewWidget = new CefViewWidget(INDEX_URL, &setting);

  // this site is for test web events
  // leftCefViewContainer = new CefViewWidget("http://xcal1.vodafone.co.uk/", &setting, this);

  //
  // leftCefViewContainer = new CefViewWidget("https://mdn.dev/", &setting, this);

  // this site is for test OSR performance
  // rightCefViewWidget = new CefViewWidget("https://www.testufo.com", &setting, this);

  // this site is test for input devices
  // leftCefViewContainer = new CefViewWidget("https://devicetests.com", &setting);

  ui.leftCefViewContainer->layout()->addWidget(leftCefViewWidget);

  // allow show context menu for both OSR and NCW mode
  leftCefViewWidget->setContextMenuPolicy(Qt::DefaultContextMenu);

  // all the following values will disable the context menu for both NCW and OSR mode
  // cefViewWidget->setContextMenuPolicy(Qt::NoContextMenu);
  // cefViewWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
  // cefViewWidget->setContextMenuPolicy(Qt::CustomContextMenu);
  // cefViewWidget->setContextMenuPolicy(Qt::PreventContextMenu);

  // connect the invokeMethod to the slot
  connect(leftCefViewWidget, &QCefView::invokeMethod, this, &MainWindow::onInvokeMethod);

  // connect the cefQueryRequest to the slot
  connect(leftCefViewWidget, &QCefView::cefQueryRequest, this, &MainWindow::onQCefQueryRequest);

  connect(leftCefViewWidget, &QCefView::draggableRegionChanged, this, &MainWindow::onDraggableRegionChanged);

  connect(leftCefViewWidget, &QCefView::reportJavascriptResult, this, &MainWindow::onJavascriptResult);

  connect(leftCefViewWidget, &QCefView::loadStart, this, &MainWindow::onLoadStart);
  connect(leftCefViewWidget, &QCefView::loadEnd, this, &MainWindow::onLoadEnd);
  connect(leftCefViewWidget, &QCefView::loadError, this, &MainWindow::onLoadError);
  connect(leftCefViewWidget, &QCefView::newDownloadItem, this, &MainWindow::onNewDownloadItem);
  connect(leftCefViewWidget, &QCefView::updateDownloadItem, this, &MainWindow::onUpdateDownloadItem);
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
  leftCefViewWidget->responseQCefQuery(query);
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
MainWindow::onNewDownloadItem(QCefDownloadItemPointer item)
{
  qDebug() << "onNewDownloadItem:"
           << "  id: " << item->id() << "\n"
           << "  name: " << item->suggestedFileName() << "\n"
           << "  path: " << item->fullPath() << "\n"
           << "  percent: " << item->percentComplete() << "%, " << item->totalBytes() << "/" << item->receivedBytes()
           << "\n"
           << "  canceled: " << item->isCanceled() << "\n"
           << "  complete: " << item->isComplete();

  item->start("", true);
}

void
MainWindow::onUpdateDownloadItem(QCefDownloadItemPointer item)
{
  qDebug() << "onNewDownloadItem:"
           << "  id: " << item->id() << "\n"
           << "  name: " << item->suggestedFileName() << "\n"
           << "  path: " << item->fullPath() << "\n"
           << "  percent: " << item->percentComplete() << "%, " << item->totalBytes() << "/" << item->receivedBytes()
           << "\n"
           << "  canceled: " << item->isCanceled() << "\n"
           << "  complete: " << item->isComplete();
}

void
MainWindow::onBtnRecreateClicked()
{
  if (leftCefViewWidget) {
    leftCefViewWidget->deleteLater();
    leftCefViewWidget = nullptr;
  }

  createCefView();
}

void
MainWindow::onBtnChangeColorClicked()
{
  if (leftCefViewWidget) {
    // create a random color
    QColor color(QRandomGenerator::global()->generate());

    // create the cef event and set the arguments
    QCefEvent event("colorChange");
    event.arguments().append(QVariant::fromValue(color.name(QColor::HexArgb)));

    // broadcast the event to all frames in all browsers created by this QCefView widget
    leftCefViewWidget->broadcastEvent(event);
  }
}

void
MainWindow::onBtnCallJSCodeClicked()
{
  int64_t context = 1000;
  QString code = "alert('hello QCefView'); return {k1: 'str', k2: true, k3: 100};";
  leftCefViewWidget->executeJavascriptWithResult(QCefView::MainFrameID, code, "", context);
}

void
MainWindow::onBtnSetFocusClicked()
{
  if (leftCefViewWidget) {
    leftCefViewWidget->setFocus();
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

#ifndef Q_OS_MACOS
void
MainWindow::setupWindow()
{
}
#endif
