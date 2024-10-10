#设置使用的源文件
#set(SRC_LIST calluserservice.cc ../user.pb.cc)
set(SRC_LIST callfriendservice.cc ../friend.pb.cc)
#生成可执行文件
add_executable(consumer ${SRC_LIST})
target_link_libraries(consumer mprpc protobuf)