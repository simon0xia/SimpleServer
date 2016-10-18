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
		static ClientManager instance;   //�ֲ���̬����  
		return instance;
	}

	//��Client�̼߳������Ԫ
	bool addClient(int socketDescriptor, QThread *cThread);

	//����Client�����ӹ���Ԫ��ɾ����
	bool DeleteClient(int socketDescriptor);

	//��������Client������չ���Ԫ�����ã�
	void DeleteAll();

	//��ѯClientThread����
	int count();	

private:
	//���캯����˽�е�  
	ClientManager();
	ClientManager(const ClientManager &);
	ClientManager & operator = (const ClientManager &);

private:
	std::mutex lock;

	QHash<int, QThread*> clients;
};

#define ClientManagerIns ClientManager::GetInstance()

#endif // CLIENTMANAGER_H
