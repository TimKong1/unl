cmake_minimum_required(VERSION 3.5)
project(unl)
#添加第三方库
add_subdirectory(tripartite_src/libevent)
add_subdirectory(tripartite_src/fmt)
add_subdirectory(tripartite_src/compile-time-regular-expressions)
#C++20
set(CMAKE_CXX_STANDARD 20)
############################################################
# Create a library
############################################################
#根据生成unl动态库
add_library(unl SHARED 
    src/core/uBase.cpp
    src/core/uIO.cpp
    src/core/uListener.cpp
    src/http/uHttpIO.cpp
    src/http/uHttpServer.cpp
    src/thread/uTask.cpp
    src/thread/uThread.cpp
    src/thread/uThreadPool.cpp
    src/util/utstr.cpp
)
#给动态库unl起一个别的名字unl::library
add_library(unl::library ALIAS unl)
#为这个库目标，添加头文件路径，PUBLIC表示包含了这个库的目标也会包含这个路径
target_include_directories(unl
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/core
)
#链接libevent和pthread
target_link_libraries(unl event pthread fmt ctre)
#编译 example里的http_server
add_subdirectory(example/http)
#编译 example里的ftp_server
add_subdirectory(example/ftp)