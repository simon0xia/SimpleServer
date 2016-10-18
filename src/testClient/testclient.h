#ifndef TESTCLIENT_H
#define TESTCLIENT_H

#include <QtWidgets/QMainWindow>
#include "ui_testclient.h"

class testClient : public QMainWindow
{
	Q_OBJECT

public:
	testClient(QWidget *parent = 0);
	~testClient();

private slots:
	void startTest(QString, qint32, QString, QString);

private:
	Ui::testClientClass ui;
};

#endif // TESTCLIENT_H
