#include "mytcpserver.h"
#include "clientthread.h"
#include "clientmanager.h"

myTcpServer::myTcpServer(QObject *parent)
	: QTcpServer(parent)
{

}

myTcpServer::~myTcpServer()
{
	ClientManagerIns.DeleteAll();
}

void myTcpServer::incomingConnection(qintptr socketDescriptor)
{
	ClientThread *thread = new ClientThread(socketDescriptor, this);
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
	thread->start();

	ClientManagerIns.addClient(socketDescriptor, thread);
}