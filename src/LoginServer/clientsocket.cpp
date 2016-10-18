#include "clientsocket.h"
#include "clientmanager.h"
#include "MessageDefine.h"
#include "ConnectionPool.h"

ClientSocket::ClientSocket(QObject *parent)
	: QTcpSocket(parent)
{
	connect(this, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
	connect(this, SIGNAL(disconnected()), this, SLOT(ClientClose()));
	connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
		this, SLOT(Socketerror(QAbstractSocket::SocketError)));

	bLogin = false;

	//初始化配置项
	QSettings dbSet("Settings.ini");
	maxWaitTime = dbSet.value("Comm/WaitTime", 5000).toInt();
	waitInterval = dbSet.value("Comm/WaitInterval", 1000).toInt();

	//打开数据库连接
	db = ConnectionPool::openConnection();

	//开启心跳计时
	lifeTimer = startTimer(waitInterval);
	elapsedLife.start();
}

ClientSocket::~ClientSocket()
{
	ConnectionPool::closeConnection(db);
}

void ClientSocket::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == lifeTimer)
	{
		if (elapsedLife.elapsed() > maxWaitTime)
		{
			this->disconnectFromHost();
		}
	}
}

bool ClientSocket::setSocketDescriptor(qintptr socketDescriptor)
{
	socketID = socketDescriptor;
	return QAbstractSocket::setSocketDescriptor(socketID);
}

void ClientSocket::ClientClose()
{
	DisplayStatus(lvl_info, "Client disconnected.");

	ClientManagerIns.DeleteClient(socketID);
}

void ClientSocket::Socketerror(QAbstractSocket::SocketError socketError)
{
	DisplayStatus(lvl_error, QStringLiteral("An error occurred. Code:%1\n").arg(socketError));
}

void ClientSocket::DisplayStatus(int level, QString msg)
{
	qDebug() << tr("[%1] %2 %3").arg(socketID).arg(level).arg(msg);
}

void ClientSocket::receiveMessage()
{
	QByteArray recvBa = this->readAll();
	int nRead = recvBa.size();
	if (nRead == 0)
	{
		DisplayStatus(lvl_error, QStringLiteral("read 0 bytes, no data was currently available for reading, or that an error occurred."));
		return;
	}
	
	MsgID msgID;
	QDataStream oStream(recvBa);

	while (!oStream.atEnd())
	{
		oStream >> msgID;
		if (msgID < CL_MsgStart || msgID >= CL_MsgEnd)
		{
			DisplayStatus(lvl_waring, QStringLiteral("Undefined message: %1").arg(msgID));
			return;
		}

		//重置心跳计时
		elapsedLife.restart();

#ifdef _DEBUG
		if (msgID != CL_Heart)
		{
			DisplayStatus(lvl_info, QStringLiteral("Msg: %1").arg(msgID));
		}
#endif

		//未登陆状态，只处理登陆、新建帐号、修改密码三项服务。
		if (bLogin || msgID == CL_Login || msgID == CL_User || msgID == CL_Password)
		{
			ProcessMessage(msgID, oStream);
		}
	}
}
void ClientSocket::ProcessMessage(MsgID msgID, QDataStream &oStream)
{
	switch (msgID)
	{
	case CL_Login:		msg_CL_Login(oStream); 		break;
	case CL_User:		msg_CL_User(oStream);		break;
	case CL_Password:	msg_CL_Password(oStream);	break;
	default:
		DisplayStatus(lvl_error, QStringLiteral("unhandled message: %1").arg(msgID));
		break;
	}
}

void ClientSocket::msg_CL_Login(QDataStream &oStream)
{
	QString user, pwd;
	oStream >> user >> pwd;

	bool sqlExecRes = false;
	int resCode = meSuccess;

	QSqlQuery query(db);
	QString sql = QString("SELECT last_login_time FROM account where `name` = '%1' AND `password` = '%2'")
		.arg(user).arg(pwd);
	query.prepare(sql);

	if (query.exec())
	{
		if (query.size()) {
			DisplayStatus(lvl_info, QStringLiteral("%1 was login").arg(user));
			sqlExecRes = true;
		} else {
			resCode = meUser;
			DisplayStatus(lvl_waring, QStringLiteral("no record. user:%1 pwd:%2").arg(user).arg(pwd));
		}
	} else {
		resCode = meIO;
		DisplayStatus(lvl_waring, query.lastError().text());
	}

	QByteArray iData;
	QDataStream iStream(&iData, QIODevice::WriteOnly);
	iStream << (MsgID)LC_Login << sqlExecRes << resCode;
	this->write(iData);
}

void ClientSocket::msg_CL_User(QDataStream &oStream)
{
	QString user, pwd, qq, mail;
	oStream >> user >> pwd >> qq >> mail;

	QSqlQuery query(db);
	QString sql = QString("INSERT INTO account(name, password, QQ, Email) VALUES('%1', '%2', '%3', '%4')")
		.arg(user).arg(pwd).arg(qq).arg(mail);
	query.prepare(sql);
	bool res = query.exec();

	if (!res)
	{
		DisplayStatus(lvl_waring, query.lastError().text());
	}

	QByteArray iData;
	QDataStream iStream(&iData, QIODevice::WriteOnly);
	iStream.setByteOrder(QDataStream::BigEndian);
	iStream << (MsgID)LC_User << res;
	this->write(iData);
}

void ClientSocket::msg_CL_Password(QDataStream &oStream)
{
	QString user, pwd, newPwd;
	oStream >> user >> pwd >> newPwd;

	QSqlQuery query(db);
	QString sql = QString("UPDATE account SET `password` = '%3' WHERE `name` = '%1' AND `password` = '%2'")
		.arg(user).arg(pwd).arg(newPwd);
	query.prepare(sql);
	bool res = query.exec();

	DisplayStatus(lvl_waring, query.lastError().text());

	QByteArray iData;
	QDataStream iStream(&iData, QIODevice::WriteOnly);
	iStream.setByteOrder(QDataStream::BigEndian);
	iStream << (MsgID)LC_Password << res;
	this->write(iData);
}