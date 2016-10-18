#include "wtest.h"
#include "clientthread.h"

wTest::wTest(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.lbl_time->setText("0:0:0");
	startTimer(1000);
	time_Start.start();
}

wTest::~wTest()
{

}

void wTest::timerEvent(QTimerEvent *)
{
	int n = time_Start.elapsed() / 1000;
	int h = n / 3600;
	int m = (n % 3600) / 60;
	int s = n % 60;

	ui.lbl_time->setText(QStringLiteral("%1:%2:%3").arg(h).arg(m).arg(s));
}

void wTest::startTest(QString host, qint32 port, QString user, QString pwd)
{
	this->host = host;
	this->port = port;
	this->user = user;
	this->pwd = pwd;

	ui.lbl_host->setText(QStringLiteral("%1: %2").arg(host).arg(port));

	startClientThread();
}

void wTest::startClientThread()
{
	for (int i = 0; i < 1; i++)
	{
		QTextBrowser *disp[MaxClient] = { ui.browser1, ui.browser2, ui.browser3, ui.browser4, ui.browser5, ui.browser6 };
		ClientThread *thread = new ClientThread(host, port, user, pwd, disp[i], this);
		connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
		thread->start();
	}	
}