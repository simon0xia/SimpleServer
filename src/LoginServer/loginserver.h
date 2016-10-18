#ifndef LOGINSERVER_H
#define LOGINSERVER_H

#include "precompile.h"
#include "mytcpserver.h"

class LoginServer : public QObject
{
	Q_OBJECT

public:
	LoginServer(QObject *parent = 0);
	~LoginServer();

	bool startServer();

private:
	myTcpServer svr;

	int lsPort;
};

#endif // LOGINSERVER_H
