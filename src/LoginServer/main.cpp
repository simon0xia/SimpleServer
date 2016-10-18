
#include "precompile.h"
#include <QtCore/QCoreApplication>

#include "ConnectionPool.h"
#include "loginserver.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	//确保只运行一次
	QSystemSemaphore sema("LoginServerSemaphore", 1, QSystemSemaphore::Open);
	//在临界区操作共享内存
	sema.acquire();
	QSharedMemory mem("LoginServerShareMemory");

	// 如果全局对象以存在则退出
	if (!mem.create(1))
	{
		qDebug() << "A server has already been running.";
		sema.release();
		return 0;
	}
	sema.release();

	qDebug() << "Start Login Server...";

	//初始化日志系统
	LogIns.init(lvl_info, "LoginServer.log");

	//启动服务器
	LoginServer svr;
	if (svr.startServer())
	{
	}

	//释放数据据连接池
	ConnectionPool::release();

	return a.exec();
}
