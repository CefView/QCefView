#ifndef QCEFVIEWTEST_H
#define QCEFVIEWTEST_H

#include <QtWidgets/QMainWindow>
#include "ui_qcefviewtest.h"
#include "customcefview.h"

class QCefViewTest : public QMainWindow
{
	Q_OBJECT

public:
	QCefViewTest(QWidget *parent = 0);
	~QCefViewTest();

protected slots:
	void onBtnChangeColorClicked();
private:
	Ui::QCefViewTestClass ui;
	CustomCefView* cefview;
};

#endif // QCEFVIEWTEST_H
