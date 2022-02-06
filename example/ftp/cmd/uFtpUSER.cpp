#include "uFtpUSER.h"
#include <string.h>
#include <iostream>

using namespace unl;

void uFtpUSER::parse(const std::string &type, std::string &msg)
{
    std::cout << type << ":" << msg << std::endl;
    this->res_cmd("230 Login successful.\r\n");
}

uFtpUSER::uFtpUSER()
{
}

uFtpUSER::~uFtpUSER()
{
}