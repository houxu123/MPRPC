#include "test.pb.h"
#include<iostream>
#include<string>
using namespace fixbug;

int test1(){
    //封装了login请求对象的数据
    LoginRequest req;
    req.set_name("zhangsan");
    req.set_pwd("123456");

    //对象数据序列化 -> char*
    std::string send_str;
    if(req.SerializeToString(&send_str)){
        std::cout << send_str << std::endl;
    }


    //从send_str反序列化一个login请求对象
    LoginRequest reqB;
    if(reqB.ParseFromString(send_str)){
        std::cout <<  reqB.name() << std::endl;
        std::cout << reqB.pwd() << std::endl;
    }
    return 0;
    
}
 
int test2(){
    LoginResponse rsp;
    ResultCode* rc = rsp.mutable_result();
    rc->set_errcode(1);
    rc->set_errmsg("登录处理失败了");
    return 0;
}
int main(){

}