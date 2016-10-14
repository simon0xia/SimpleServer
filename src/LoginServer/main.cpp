
#include "precompile.h"
#include <QtCore/QCoreApplication>

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



	return a.exec();
}
