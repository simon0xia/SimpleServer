#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include "precompile.h"

class ClientSocket : public QTcpSocket
{
	Q_OBJECT

public:
	ClientSocket(QObject *parent = 0);
	~ClientSocket();

	bool setSocketDescriptor(qintptr socketDescriptor);

protected:
	void timerEvent(QTimerEvent *event);

private slots:
	void receiveMessage();
	void ClientClose();
	void Socketerror(QAbstractSocket::SocketError);

	void ProcessMessage(MsgID msgID, QDataStream &oStream);

	void DisplayStatus(int level, QString msg);

	void msg_CL_Login(QDataStream &oStream);
	void msg_CL_User(QDataStream &oStream);
	void msg_CL_Password(QDataStream &oStream);

private:
	int socketID;
	int lifeTimer;
	bool bLogin;
	QTime elapsedLife;
	QSqlDatabase db;

	//配置项，不可修改。
	int maxWaitTime, waitInterval;
};

#endif // CLIENTSOCKET_H
