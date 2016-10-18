#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QTcpSocket>
#include <QTime>
#include <QByteArray>
#include <QDataStream>
#include <QTextBrowser>

class ClientSocket : public QTcpSocket
{
	Q_OBJECT
	static const int HeartTime = 1000;

public:
	ClientSocket(QTextBrowser *browser, QObject *parent);
	~ClientSocket();

private slots:
	void receiveMessage();
	void ClientClose();
	void Socketerror(QAbstractSocket::SocketError);

	void sendMsg_Login();

protected:
	void timerEvent(QTimerEvent *);

private:
	qint64 sendPacketToServer(const QByteArray &packet);

	

private:
	QTime preSendTime;
	QTextBrowser *disp;
	
};

#endif // CLIENTSOCKET_H
