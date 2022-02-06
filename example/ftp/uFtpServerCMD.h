#pragma once
#include "uFtpTask.h"
#include <map>
#include <string>

namespace unl
{
    class uFtpServerCMD : public uFtpTask
    {
    private:
        std::map<std::string, uFtpTask *> calls;
        std::map<uFtpTask *, char> calls_del; //Avoid duplicate removal

    public:
        virtual bool init();
        virtual void read(bufferevent *bev);
        virtual void event(struct bufferevent *bev, short what);
        bool reg(const std::string &cmd, uFtpTask *call);
        uFtpServerCMD();
        ~uFtpServerCMD();
    };

}
