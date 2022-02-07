cmake_minimum_required(VERSION 3.5)
project(http_server)
set(CMAKE_CXX_STANDARD 20)
add_executable(http_server
    http_server.cpp
)
#链接库和可执行文件，使用的是这个库的别名。PRIVATE 表示
target_link_libraries( http_server
    PRIVATE 
        unl::library
)