#include "clientthread.h"


ClientThread::ClientThread(int socketDescriptor, QObject *parent)
	: QThread(parent), socketDescriptor(socketDescriptor)
{

}

ClientThread::~ClientThread()
{
	qDebug() << "Thread was quit";

	tcpSocket->deleteLater();
}

void ClientThread::run()
{
	tcpSocket = new ClientSocket;
	if (!tcpSocket->setSocketDescriptor(socketDescriptor)) {
		emit error(tcpSocket->error());		
	}
	exec();
}