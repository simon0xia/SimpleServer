#include "clientthread.h"
#include "clientsocket.h"

ClientThread::ClientThread(QString whost, qint32 wport, QString wuser, QString wpwd, QTextBrowser *browser, QObject *parent)
	: QThread(parent), host(whost), port(wport), user(wuser), pwd(wpwd), disp(browser)
{

}

ClientThread::~ClientThread()
{

}

void ClientThread::run()
{
	disp->append(QStringLiteral("尝试连接远程服务器..."));

	ClientSocket *tcpClient = new ClientSocket(disp, this);
	tcpClient->connectToHost(host, port);
	if (tcpClient->waitForConnected(1000))
	{
		//disp->append(QStringLiteral("连接服务器成功"));
	}
	else
	{
		//disp->append(QStringLiteral("连接失败。错误原因为：%1").arg(tcpClient->errorString()));
		tcpClient->deleteLater();
	}

	exec();
}