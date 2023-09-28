#ifndef QCEFVIEWTEST_H
#define QCEFVIEWTEST_H

#include <QCloseEvent>
#include <QMainWindow>

#include "ui_MainWindow.h"

#include "CefViewWidget.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();

protected:
  void createLeftCefView();
  void createRightCefView();
  void setupWindow();

  // QCefView slots
protected slots:

  void onInvokeMethod(int browserId, int64_t frameId, const QString& method, const QVariantList& arguments);

  void onQCefQueryRequest(int browserId, int64_t frameId, const QCefQuery& query);

  void onJavascriptResult(int browserId, int64_t frameId, const QString& context, const QVariant& result);

  void onLoadingStateChanged(int browserId, bool isLoading, bool canGoBack, bool canGoForward);

  void onLoadStart(int browserId, qint64 frameId, bool isMainFrame, int transitionType);

  void onLoadEnd(int browserId, qint64 frameId, bool isMainFrame, int httpStatusCode);

  void onLoadError(int browserId,
                   qint64 frameId,
                   bool isMainFrame,
                   int errorCode,
                   const QString& errorMsg,
                   const QString& failedUrl);

  // ui slots
protected slots:
  void onBtnShowDevToolsClicked();

  void onBtnReloadRightViewClicked();

  void onBtnRecreateRightViewClicked();

  void onBtnChangeColorClicked();

  void onBtnSetFocusClicked();

  void onBtnCallJSCodeClicked();

  void onBtnNewBrowserClicked();

private:
  Ui::MainWindow m_ui;

  CefViewWidget* m_pLeftCefViewWidget = nullptr;
  CefViewWidget* m_pRightCefViewWidget = nullptr;
};

#endif // QCEFVIEWTEST_H
