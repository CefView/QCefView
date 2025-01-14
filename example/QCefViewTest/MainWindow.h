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

  void onInvokeMethod(const QCefBrowserId& browserId,
                      const QCefFrameId& frameId,
                      const QString& method,
                      const QVariantList& arguments);

  void onQCefUrlRequest(const QCefBrowserId& browserId, const QCefFrameId& frameId, const QString& url);

  void onQCefQueryRequest(const QCefBrowserId& browserId, const QCefFrameId& frameId, const QCefQuery& query);

  void onJavascriptResult(const QCefBrowserId& browserId,
                          const QCefFrameId& frameId,
                          const QString& context,
                          const QVariant& result);

  void onLoadingStateChanged(const QCefBrowserId& browserId, bool isLoading, bool canGoBack, bool canGoForward);

  void onLoadStart(const QCefBrowserId& browserId, const QCefFrameId& frameId, bool isMainFrame, int transitionType);

  void onLoadEnd(const QCefBrowserId& browserId, const QCefFrameId& frameId, bool isMainFrame, int httpStatusCode);

  void onLoadError(const QCefBrowserId& browserId,
                   const QCefFrameId& frameId,
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

  QCefView* m_pLeftCefViewWidget = nullptr;
  QCefView* m_pRightCefViewWidget = nullptr;
};

#endif // QCEFVIEWTEST_H
