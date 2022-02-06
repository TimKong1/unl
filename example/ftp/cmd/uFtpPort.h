#pragma once
#include "../uFtpTask.h"
#include <string>

namespace unl
{
    class uFtpPort : public uFtpTask
    {
    private:
    public:
        void parse(const std::string &type, std::string &msg);
        uFtpPort();
        ~uFtpPort();
    };

}