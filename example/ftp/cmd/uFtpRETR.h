#pragma once
#include "../uFtpTask.h"

namespace unl
{
    class uFtpRETR : public uFtpTask
    {
    private:
        FILE *fp = 0;

    public:
        virtual void parse(const std::string &type, std::string &msg);
        virtual void write(bufferevent *bev);
        virtual void event(struct bufferevent *bev, short what);
        uFtpRETR();
        ~uFtpRETR();
    };

}
