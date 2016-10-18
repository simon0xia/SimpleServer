#include "loginserver.h"

LoginServer::LoginServer(QObject *parent)
	: QObject(parent)
{
	QSettings loginSet("Settings.ini");
	lsPort = loginSet.value("Login/Port", 8100).toInt();
	
}

LoginServer::~LoginServer()
{
	//run over
	qDebug() << "Login Server run over";
}

bool LoginServer::startServer()
{
	if (svr.listen(QHostAddress::Any, lsPort))
	{
		qDebug() << tr("server was listen on port:%1").arg(lsPort);
	}
	else
	{
		qDebug() << tr("Can not listen, because:").arg(svr.errorString());
		svr.deleteLater();
		return false;
	}

	return true;
}