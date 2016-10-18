#include "clientmanager.h"

ClientManager::ClientManager()
	: QObject(nullptr)
{

}

ClientManager::~ClientManager()
{

}

bool ClientManager::addClient(int socketDescriptor, QThread *cThread)
{
	lock.lock();
	clients.insert(socketDescriptor, cThread);
	lock.unlock();

	return true;
}
bool ClientManager::DeleteClient(int socketDescriptor)
{
	lock.lock();
	if (clients.contains(socketDescriptor))
	{
		auto tmp = clients.value(socketDescriptor);
		tmp->exit();
		tmp->deleteLater();

		clients.remove(socketDescriptor);
	}
	lock.unlock();
	return true;
}

void ClientManager::DeleteAll()
{
	lock.lock();

	foreach(auto tmp, clients)
	{
		tmp->exit();
		tmp->deleteLater();
	}
	clients.clear();

	lock.unlock();
}

int ClientManager::count()
{
	return clients.count();
}
