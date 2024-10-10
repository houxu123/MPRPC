#include "mprpcconfig.h"
#include <iostream>
#include<string>


void MprpcConfig::LoadConfigFile(const char* confige_file){
    FILE* pf = fopen(confige_file, "r");
    if(pf == nullptr){
        std::cout << confige_file << "is not exist!" << std::endl;
        exit(EXIT_FAILURE);
    }
    //1. 注释 2.正确地配置项=  3.去掉开头的多余的空格
    while(!feof(pf)){
        char buf[512] = {0};
        //一行一行的读取数据
        fgets(buf, 512, pf);

        std::string read_buf(buf);
        //去掉字符串前后多余的空格
        Trim(read_buf);

        //判断#的注释
        if(read_buf[0] == '#' || read_buf.empty())
            continue; //处理下一行
        
        //解析配置项
        int idx = read_buf.find('=');
        if(idx == -1){
            //配置项不合法
            continue;
        }
        
        std::string key;
        std::string value;
        key = read_buf.substr(0,idx);
        Trim(key); //去掉等号左边的空格
        int endidx = read_buf.find('\n', idx);
        value = read_buf.substr(idx + 1, endidx - idx - 1);
        Trim(value); //去掉等号右边的空格
        m_configMap.insert({key, value});
    }
        
}
    //查询配置项信息
std::string MprpcConfig::Load(const std::string& key){
    auto it = m_configMap.find(key);
    if(it == m_configMap.end())
        return "";
    return it->second;
}

//去掉字符串前后多余的空格
void MprpcConfig::Trim(std::string& src_buf){
                
    //去掉字符串前面多余的空格
    int idx = src_buf.find_first_not_of(' ');
    if(idx != -1){
        //说明字符串前面有空格
        src_buf = src_buf.substr(idx, src_buf.size() - idx);
    }
    //去掉字符串后面多余的空格
    idx = src_buf.find_last_not_of(' ');
    if(idx != -1){
        //说明字符串后面有空格
        src_buf = src_buf.substr(0, idx + 1);
    }
}