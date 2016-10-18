#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "precompile.h"
#include <mutex>

class ClientManager : public QObject
{
	Q_OBJECT

public:
	~ClientManager();
	static ClientManager &GetInstance()
	{
		static ClientManager instance;   //局部静态变量  
		return instance;
	}

	//将Client线程加入管理单元
	bool addClient(int socketDescriptor, QThread *cThread);

	//结束Client，并从管理单元中删除。
	bool DeleteClient(int socketDescriptor);

	//结束所有Client，并清空管理单元。慎用！
	void DeleteAll();

	//查询ClientThread数量
	int count();	

private:
	//构造函数是私有的  
	ClientManager();
	ClientManager(const ClientManager &);
	ClientManager & operator = (const ClientManager &);

private:
	std::mutex lock;

	QHash<int, QThread*> clients;
};

#define ClientManagerIns ClientManager::GetInstance()

#endif // CLIENTMANAGER_H
