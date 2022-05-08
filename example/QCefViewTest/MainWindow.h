#ifndef QCEFVIEWTEST_H
#define QCEFVIEWTEST_H

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
  void createCefView();

  // QCefView slots
protected slots:
  void onDraggableRegionChanged(const QRegion& draggableRegion, const QRegion& nonDraggableRegion);

  void onInvokeMethod(int browserId, int64_t frameId, const QString& method, const QVariantList& arguments);

  void onQCefQueryRequest(int browserId, int64_t frameId, const QCefQuery& query);

  void onJavascriptResult(int browserId, int64_t frameId, int64_t context, const QVariant& result);

  // ui slots
protected slots:
  void onBtnRecreateClicked();

  void onBtnChangeColorClicked();

  void onBtnCallJSCodeClicked();

  void onBtnNewBrowserClicked();

protected:
  bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result);

private:
  Ui::MainWindow ui;

  CefViewWidget* cefViewWidget = nullptr;
  QRegion draggableRegion_;
  QRegion nonDraggableRegion_;
};

#endif // QCEFVIEWTEST_H
