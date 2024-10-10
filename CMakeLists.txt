cmake_minimum_required(VERSION 3.0)
project(mprpc)
#生成debug版本，可以进行gdb调试
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_STANDARD 11)
#可执行文件输出的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#库文件输出的路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

#编译头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/example)
include_directories(${PROJECT_SOURCE_DIR}/src/include)

#库文件搜索路径
link_directories(${PROJECT_SOURCE_DIR}/lib)

#src包含了mprpc框架所有的相关代码
add_subdirectory(src)
#example包含了mprpc框架使用的示例代码
add_subdirectory(example)