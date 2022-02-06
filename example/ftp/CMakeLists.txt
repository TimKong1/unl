cmake_minimum_required(VERSION 3.5)
project(ftp_server)

add_executable(ftp_server
    ftp_server_test.cpp
    uFtpFactory.cpp
    uFtpServerCMD.cpp
    uFtpTask.cpp
    cmd/uFtpList.cpp
    cmd/uFtpPort.cpp
    cmd/uFtpRETR.cpp
    cmd/uFtpSTOR.cpp
    cmd/uFtpUSER.cpp
)

target_include_directories(unl
    PRIVATE
        cmd
)

target_link_libraries( ftp_server
    PRIVATE
        unl::library
)