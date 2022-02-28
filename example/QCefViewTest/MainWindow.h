#ifndef QCEFVIEWTEST_H
#define QCEFVIEWTEST_H

#include <QMainWindow>

#include "ui_MainWindow.h"

#include <QCefView.h>

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();

  // QCefView slots
protected slots:
  void onDraggableRegionChanged(const QRegion& draggableRegion, const QRegion& nonDraggableRegion);

  void onInvokeMethod(int browserId, int64_t frameId, const QString& method, const QVariantList& arguments);

  void onQCefQueryRequest(int browserId, int64_t frameId, const QCefQuery& query);

  void onJavascriptResult(int browserId, int64_t frameId, int64_t context, const QVariant& result);

  // ui slots
protected slots:
  void onBtnChangeColorClicked();

  void onBtnCallJSCodeClicked();

  void onBtnNewBrowserClicked();

private:
  Ui::MainWindow ui;

  QCefView* cefViewWidget;
  QRegion draggableRegion_;
  QRegion nonDraggableRegion_;
};

#endif // QCEFVIEWTEST_H
