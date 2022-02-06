#include "uFtpPort.h"
#include <iostream>
#include "util/utstr.hpp"

using namespace unl;

void uFtpPort::parse(const std::string &type, std::string &msg)
{
	printf("%s\n",msg.c_str());
    auto v = util::split(msg, ",");
printf("%lu\n",v.size());
for(auto i : v){
printf("%s\n",i.c_str());
}
    if (v.size() != 6)
    {
        this->res_cmd("501 Syntax error in parameters or arguments.\r\n");
        return;
    }
    this->cmd_task->ip = v[0] + "." + v[1] + "." + v[2] + "." + v[3];
    this->cmd_task->port = (atoi(v[4].c_str()) << 8) + atoi(v[5].c_str());
    this->ip = this->cmd_task->ip;
    this->port = this->cmd_task->port;
    std::cout << this->ip << ":" << this->port << std::endl;
    this->res_cmd("200 PORT command successful.\r\n");
}

uFtpPort::uFtpPort() {}
uFtpPort::~uFtpPort() {}
