#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <qthread.h>
#include <QTextBrowser>
#include "clientsocket.h"

class ClientThread : public QThread
{
	Q_OBJECT

public:
	ClientThread(QString host, qint32 port, QString user, QString pwd, QTextBrowser *browser, QObject *parent);
	~ClientThread();

	void run() Q_DECL_OVERRIDE;

signals:
	void error(QTcpSocket::SocketError socketError);

private:
	QString host, user, pwd;
	qint32 port;
	QTextBrowser *disp;
};

#endif // CLIENTTHREAD_H
