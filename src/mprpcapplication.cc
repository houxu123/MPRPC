#include "mprpcapplication.h"
#include<iostream>
#include<unistd.h>
#include<string>

MprpcConfig MprpcApplication::m_config;

void showArgHelp(){
    std::cout << "format: command -i <configfile>" << std::endl;    
}

void MprpcApplication::Init(int argc, char** argv){
    if(argc < 2){
        showArgHelp();
        exit(EXIT_FAILURE);
    }

    int c = 0;
    std::string config_file;
    while ((c = getopt(argc, argv, "i:")) != -1)
    {
        switch (c)
        {
        case 'i':
            config_file = optarg;
            break;
        case '?':  //?：如果遇到一个无效的选项，getopt 返回?，并且optopt会包含无效选项的字符。
            showArgHelp();
            exit(EXIT_FAILURE);
        case ':': //:：如果某个选项缺少必要的参数，getopt 返回:，并且optopt会包含缺少参数的选项字符。
            showArgHelp();
            exit(EXIT_FAILURE);
        default:
            break;
        }
    }

    //开始加载配置文件rpcserver_ip =  rpcserver_port =  zookeeper_ip  =  zookeeper_port = 
   m_config.LoadConfigFile(config_file.c_str());
   //std::cout << "rpcserverip:" << m_config.Load("rpcserverip") << std::endl;
   //std::cout << "rpcserverport:" << m_config.Load("rpcserverport") << std::endl;
   //std::cout << "zookeeperip:" << m_config.Load("zookeeperip") << std::endl;
   //std::cout << "zookeeperport:" << m_config.Load("zookeeperport") << std::endl;
}

 MprpcApplication& MprpcApplication::GetInstance(){
    static MprpcApplication app;
    return app;
}

MprpcConfig& MprpcApplication::GetConfig(){
    return m_config;
}