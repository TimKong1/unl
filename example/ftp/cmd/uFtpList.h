#pragma once
#include "../uFtpTask.h"

namespace unl
{
    class uFtpList : public uFtpTask
    {
    private:
        std::string get_list_data(const std::string &path);

    public:
        virtual void parse(const std::string &type, std::string &msg);
        virtual void read(bufferevent *bev);
        virtual void write(bufferevent *bev);
        virtual void event(struct bufferevent *bev, short what);
        uFtpList();
        ~uFtpList();
    };
}
