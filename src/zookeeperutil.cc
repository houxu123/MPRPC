#include "zookeeperutil.h"
#include "mprpcapplication.h"
#include <semaphore.h>
#include <iostream>

/*
zookeeper在RPC框架中的作用

服务注册和发现
    服务注册：当一个服务提供者（服务端）启动时，它会将自己的网络地址（IP和端口）以及其他信息（例如服务名称、版本等）注册到 Zookeeper 上。
            这样，Zookeeper 会保存该服务的状态及其可用性。
            
    服务发现：当一个服务消费者（客户端）需要调用某个服务时，它会向 Zookeeper 查询该服务的地址，Zookeeper 返回可用服务列表。
    这样，客户端就可以直接与服务提供者进行通信，而不需要手动管理服务的地址。这种机制解除了客户端和服务端之间的耦合，
    使得服务端可以动态上下线，同时客户端能够根据服务状态调整调用逻辑。

 故障恢复
    Zookeeper 通过 心跳检测 和 会话机制 实现服务的健康检查和故障恢复：
    如果某个服务实例发生故障或宕机，Zookeeper 可以检测到该服务不再响应心跳，并自动将其从可用服务列表中移除。
    客户端从 Zookeeper 获取到的服务列表是动态的，当服务实例变化（如服务上下线、实例崩溃等）时，Zookeeper 会通知客户端更新服务列表，
    避免调用无效的服务实例。

分布式配置管理
    在 RPC 框架中，有时需要对多个服务进行统一的配置管理，Zookeeper 可以用于 分布式配置管理。
    Zookeeper 能够存储服务的配置信息，并为多个服务提供者或消费者提供统一的访问入口，保证配置数据的一致性。
    修改配置时，Zookeeper 会实时更新，并通知所有依赖该配置的节点进行更新。
*/

// 全局的watcher观察器   zkserver给zkclient的通知
void global_watcher(zhandle_t *zh, int type,
                   int state, const char *path, void *watcherCtx)
{
    if (type == ZOO_SESSION_EVENT)  // 回调的消息类型是和会话相关的消息类型
        {
                if (state == ZOO_CONNECTED_STATE)  // zkclient和zkserver连接成功
                {
                        sem_t *sem = (sem_t*)zoo_get_context(zh);
            sem_post(sem);
                }
        }
}

ZkClient::ZkClient() : m_zhandle(nullptr)
{
}

ZkClient::~ZkClient()
{
    if (m_zhandle != nullptr)
    {
        zookeeper_close(m_zhandle); // 关闭句柄，释放资源  MySQL_Conn
    }
}

// 连接zkserver
void ZkClient::Start()
{
    std::string host = MprpcApplication::GetInstance().GetConfig().Load("zookeeperip");
    std::string port = MprpcApplication::GetInstance().GetConfig().Load("zookeeperport");
    std::string connstr = host + ":" + port;

        /*
        zookeeper_mt：多线程版本
        zookeeper的API客户端程序提供了三个线程
        API调用线程 
        网络I/O线程  pthread_create  poll
        watcher回调线程 pthread_create
        */
    m_zhandle = zookeeper_init(connstr.c_str(), global_watcher, 30000, nullptr, nullptr, 0);
    if (nullptr == m_zhandle)
    {
        std::cout << "zookeeper_init error!" << std::endl;
        exit(EXIT_FAILURE);
    }

    sem_t sem;
    sem_init(&sem, 0, 0);
    zoo_set_context(m_zhandle, &sem);

    sem_wait(&sem);
    std::cout << "zookeeper_init success!" << std::endl;
}


void ZkClient::Create(const char *path, const char *data, int datalen, int state)
{
    char path_buffer[128];
    int bufferlen = sizeof(path_buffer);
    int flag;
        // 先判断path表示的znode节点是否存在，如果存在，就不再重复创建了
        flag = zoo_exists(m_zhandle, path, 0, nullptr);
        if (ZNONODE == flag) // 表示path的znode节点不存在
        {
                // 创建指定path的znode节点了
                flag = zoo_create(m_zhandle, path, data, datalen,
                        &ZOO_OPEN_ACL_UNSAFE, state, path_buffer, bufferlen);
                if (flag == ZOK)
                {
                        std::cout << "znode create success... path:" << path << std::endl;
                }
                else
                {
                        std::cout << "flag:" << flag << std::endl;
                        std::cout << "znode create error... path:" << path << std::endl;
                        exit(EXIT_FAILURE);
                }
        }
}

// 根据指定的path，获取znode节点的值
std::string ZkClient::GetData(const char *path)
{
    char buffer[64];
        int bufferlen = sizeof(buffer);
        int flag = zoo_get(m_zhandle, path, 0, buffer, &bufferlen, nullptr);
        if (flag != ZOK)
        {
                std::cout << "get znode error... path:" << path << std::endl;
                return "";
        }
        else
        {
                return buffer;
        }
}