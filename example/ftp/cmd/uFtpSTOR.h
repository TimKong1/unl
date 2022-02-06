#pragma once

#include "../uFtpTask.h"

namespace unl
{
    class uFtpSTOR : public uFtpTask
    {
    private:
        FILE *fp = 0;

    public:
        virtual void parse(const std::string &type, std::string &msg);
        virtual void event(struct bufferevent *bev, short what);
        virtual void read(bufferevent *bev);
    };
}