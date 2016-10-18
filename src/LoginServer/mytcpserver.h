#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include "precompile.h"

class myTcpServer : public QTcpServer
{
	Q_OBJECT

public:
	myTcpServer(QObject *parent = 0);
	~myTcpServer();

protected:
	void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;

private:
	
};

#endif // MYTCPSERVER_H
