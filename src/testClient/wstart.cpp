#include "wstart.h"

wStart::wStart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.lbl_status->setText("");

	ui.edit_Host->setText("127.0.0.1");
	ui.edit_Port->setText("8100");
	ui.edit_User->setText("root");
	ui.edit_pwd->setText("root@123456");	
}

wStart::~wStart()
{

}

void wStart::on_btn_Start_clicked()
{
	QString host = ui.edit_Host->text();
	qint32 port = ui.edit_Port->text().toInt();
	QString user = ui.edit_User->text();
	QString pwd = ui.edit_pwd->text();

	if (host.isNull() || host.isEmpty())
	{
		setStatusInfo(QStringLiteral("Host不可为空!"));
		return;
	}

	if (port <= 1024 || port > 65535)
	{
		setStatusInfo(QStringLiteral("Port不合理，有效范围为1024-65535!"));
		return;
	}
	
	emit inputOver(host, port, user, pwd);
}

void wStart::setStatusInfo(QString &str)
{
	ui.lbl_status->setText(str);
}