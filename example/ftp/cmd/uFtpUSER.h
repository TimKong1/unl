#pragma once
#include "../uFtpTask.h"
namespace unl
{
    class uFtpUSER : public uFtpTask
    {
    private:
    public:
        virtual void parse(const std::string &type, std::string &msg);
        uFtpUSER();
        ~uFtpUSER();
    };
}