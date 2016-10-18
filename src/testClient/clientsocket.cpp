#include "clientsocket.h"
#include "MessageDefine.h"
#include "def_types.h"

#include <QTimer>

ClientSocket::ClientSocket(QTextBrowser *browser, QObject *parent)
	: QTcpSocket(parent), disp(browser)
{
	connect(this, SIGNAL(connected()), this, SLOT(sendMsg_Login()));
	connect(this, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
	connect(this, SIGNAL(disconnected()), this, SLOT(ClientClose()));
	connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
		this, SLOT(Socketerror(QAbstractSocket::SocketError)));

	preSendTime = QTime::currentTime();
	startTimer(HeartTime);

	disp->append("start");

	sendMsg_Login();
}

ClientSocket::~ClientSocket()
{
	disp->append("over");
}

void ClientSocket::timerEvent(QTimerEvent *event)
{
	QTime cur = QTime::currentTime();
	if (preSendTime.secsTo(cur) > 1)
	{
		QByteArray iData;
		QDataStream iStream(&iData, QIODevice::WriteOnly);
		iStream.setByteOrder(QDataStream::BigEndian);
		iStream << (MsgID)CL_Heart;

		sendPacketToServer(iData);
	}
}

void ClientSocket::ClientClose()
{
	disp->append("Client disconnected.");
	deleteLater();
}

void ClientSocket::Socketerror(QAbstractSocket::SocketError socketError)
{
	disp->append(tr("An error occurred. Code:%1").arg(socketError));
	ClientClose();
}

void ClientSocket::receiveMessage()
{
	QByteArray recvBa = this->readAll();
	int nRead = recvBa.size();
	if (nRead == 0)
	{
		disp->append("read 0 bytes, no data was currently available for reading, or that an error occurred.");
	}
	else
	{
		//IncReceivedBytes(nRead);
		
		disp->append(QStringLiteral("R: %1").arg(QString(recvBa.left(20))));
	}
}

qint64 ClientSocket::sendPacketToServer(const QByteArray &packet)
{
	qint64 sendBytes = this->write(packet);

	if (sendBytes  = packet.size())
	{
		preSendTime = QTime::currentTime();
	}
	disp->append(QStringLiteral("W: %1").arg(packet.data()));
	return sendBytes;
}

void ClientSocket::sendMsg_Login()
{
	QString user = "snail_test1";
	QString pwd = "abcd1234#aa";

	QByteArray iData;
	QDataStream iStream(&iData, QIODevice::WriteOnly);
	iStream << (MsgID)CL_Login;
	iStream << user << pwd;

	sendPacketToServer(iData);
}