#ifndef WTEST_H
#define WTEST_H

#include <QWidget>
#include <QString>
#include <QTcpSocket>
#include <QTime>
#include "ui_wtest.h"

class wTest : public QWidget
{
	Q_OBJECT
	static const int MaxClient = 6;

public:
	wTest(QWidget *parent = 0);
	~wTest();

public slots:
	void startTest(QString, qint32, QString, QString);

protected:
	void timerEvent(QTimerEvent *);

private:
	void startClientThread();

private:
	Ui::wTest ui;

	QString host, user, pwd;
	qint32 port;

	QTime time_Start;
};

#endif // WTEST_H
