# 典型的TCP服务器
这是一个典型的TCP服务器通讯模型。将来开发其他基于TCP通讯项目的时候，以此为蓝本。
项目使用Qt作为基础类库，网络通讯模块采用Qt类。程序使用多线程机制。
项目使用mysql作为后端数据库，使用连接池优化连接。