
#include "precompile.h"
#include <QtCore/QCoreApplication>

#include "ConnectionPool.h"
#include "loginserver.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	//ȷ��ֻ����һ��
	QSystemSemaphore sema("LoginServerSemaphore", 1, QSystemSemaphore::Open);
	//���ٽ������������ڴ�
	sema.acquire();
	QSharedMemory mem("LoginServerShareMemory");

	// ���ȫ�ֶ����Դ������˳�
	if (!mem.create(1))
	{
		qDebug() << "A server has already been running.";
		sema.release();
		return 0;
	}
	sema.release();

	qDebug() << "Start Login Server...";

	//��ʼ����־ϵͳ
	LogIns.init(lvl_info, "LoginServer.log");

	//����������
	LoginServer svr;
	if (svr.startServer())
	{
	}

	//�ͷ����ݾ����ӳ�
	ConnectionPool::release();

	return a.exec();
}
