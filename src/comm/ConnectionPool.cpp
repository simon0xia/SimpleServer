#include "ConnectionPool.h"
#include <QDebug>

QMutex ConnectionPool::mutex;
QWaitCondition ConnectionPool::waitConnection;
ConnectionPool* ConnectionPool::instance = NULL;

ConnectionPool::ConnectionPool() 
{
	QSettings dbSet("DBSetting.ini");

    hostName	= dbSet.value("Info/Host", "127.0.0.1").toString();
	DBName		= dbSet.value("Info/database", "mirDB").toString();
	port		= dbSet.value("Info/Port", 3306).toInt();
	username	= dbSet.value("Info/User", "root").toString();
	password	= dbSet.value("Info/Password", "intel@nvidia").toString();
    
	maxWaitTime = dbSet.value("Set/WaitTime", 2000).toInt();;
	waitInterval = dbSet.value("Set/WaitInterval", 200).toInt();;
	maxConnectionCount = dbSet.value("Set/MaxConnection", 100).toInt();;

	databaseType = "QMYSQL";
    testOnBorrow = true;
    testOnBorrowSql = "SELECT 1";   
}

ConnectionPool::~ConnectionPool() {
    // �������ӳص�ʱ��ɾ�����е�����
    foreach(QString connectionName, usedConnectionNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }

    foreach(QString connectionName, unusedConnectionNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }
}

ConnectionPool& ConnectionPool::getInstance() {
    if (NULL == instance) {
        QMutexLocker locker(&mutex);

        if (NULL == instance) {
            instance = new ConnectionPool();
        }
    }

    return *instance;
}

void ConnectionPool::release() {
    QMutexLocker locker(&mutex);
    delete instance;
    instance = NULL;
}

QSqlDatabase ConnectionPool::openConnection() {
    ConnectionPool& pool = ConnectionPool::getInstance();
    QString connectionName;

    QMutexLocker locker(&mutex);

    // �Ѵ���������
    int connectionCount = pool.unusedConnectionNames.size() + pool.usedConnectionNames.size();

    // ��������Ѿ����꣬�ȴ� waitInterval ���뿴���Ƿ��п������ӣ���ȴ� maxWaitTime ����
    for (int i = 0;
         i < pool.maxWaitTime
         && pool.unusedConnectionNames.size() == 0 && connectionCount == pool.maxConnectionCount;
         i += pool.waitInterval) {
        waitConnection.wait(&mutex, pool.waitInterval);

        // ���¼����Ѵ���������
        connectionCount = pool.unusedConnectionNames.size() + pool.usedConnectionNames.size();
    }

    if (pool.unusedConnectionNames.size() > 0) {
        // ���Ѿ����յ����ӣ���������
        connectionName = pool.unusedConnectionNames.dequeue();
    } else if (connectionCount < pool.maxConnectionCount) {
        // û���Ѿ����յ����ӣ�����û�дﵽ������������򴴽��µ�����
        connectionName = QString("Connection-%1").arg(connectionCount + 1);
    } else {
        // �Ѿ��ﵽ���������
        qDebug() << "Cannot create more connections.";
        return QSqlDatabase();
    }

    // ��������
    QSqlDatabase db = pool.createConnection(connectionName);

    // ��Ч�����Ӳŷ��� usedConnectionNames
    if (db.isOpen()) {
        pool.usedConnectionNames.enqueue(connectionName);
    }

    return db;
}

void ConnectionPool::closeConnection(QSqlDatabase connection) {
    ConnectionPool& pool = ConnectionPool::getInstance();
    QString connectionName = connection.connectionName();

    // ��������Ǵ��������ӣ��� used ��ɾ�������� unused ��
    if (pool.usedConnectionNames.contains(connectionName)) {
        QMutexLocker locker(&mutex);
        pool.usedConnectionNames.removeOne(connectionName);
        pool.unusedConnectionNames.enqueue(connectionName);
        waitConnection.wakeOne();
    }
}

QSqlDatabase ConnectionPool::createConnection(const QString &connectionName) {
    // �����Ѿ��������ˣ������������������´���
    if (QSqlDatabase::contains(connectionName)) {
        QSqlDatabase db1 = QSqlDatabase::database(connectionName);

        if (testOnBorrow) {
            // ��������ǰ�������ݿ⣬������ӶϿ������½�������
            qDebug() << "Test connection on borrow, execute:" << testOnBorrowSql << ", for" << connectionName;
            QSqlQuery query(testOnBorrowSql, db1);

            if (query.lastError().type() != QSqlError::NoError && !db1.open()) {
                qDebug() << "Open datatabase error:" << db1.lastError().text();
                return QSqlDatabase();
            }
        }

        return db1;
    }

    // ����һ���µ�����
    QSqlDatabase db = QSqlDatabase::addDatabase(databaseType, connectionName);
    db.setHostName(hostName);
	db.setPort(port);
    db.setDatabaseName(DBName);
    db.setUserName(username);
    db.setPassword(password);

    if (!db.open()) {
        qDebug() << "Open datatabase error:" << db.lastError().text();
        return QSqlDatabase();
    }

    return db;
}