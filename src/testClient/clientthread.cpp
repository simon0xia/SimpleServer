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
	disp->append(QStringLiteral("��������Զ�̷�����..."));

	ClientSocket *tcpClient = new ClientSocket(disp, this);
	tcpClient->connectToHost(host, port);
	if (tcpClient->waitForConnected(1000))
	{
		//disp->append(QStringLiteral("���ӷ������ɹ�"));
	}
	else
	{
		//disp->append(QStringLiteral("����ʧ�ܡ�����ԭ��Ϊ��%1").arg(tcpClient->errorString()));
		tcpClient->deleteLater();
	}

	exec();
}