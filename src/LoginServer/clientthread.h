#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include "precompile.h"
#include "clientsocket.h"

class ClientThread : public QThread
{
	Q_OBJECT

public:
	ClientThread(int socketDescriptor, QObject *parent);
	~ClientThread();

	void run() Q_DECL_OVERRIDE;

signals:
	void error(QTcpSocket::SocketError socketError);

private:
	int socketDescriptor;
	ClientSocket *tcpSocket;
};

#endif // CLIENTTHREAD_H
