#include "uFtpServerCMD.h"
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <iostream>
#include <string>
#include "util/utstr.hpp"
#include <vector>
#include "uconfig.h"

using namespace unl;

bool uFtpServerCMD::reg(const std::string &cmd, uFtpTask *call)
{
    if (!call || cmd.empty())
    {
        return false;
    }
    if (this->calls.find(cmd) != this->calls.end())
    {
        std::cout << "uFtpServerCMD::reg cmd is alread register!" << std::endl;
        return false;
    }
    this->calls[cmd] = call;
    this->calls_del[call] = 0;
    return true;
}

bool uFtpServerCMD::init()
{
    bufferevent *bev = bufferevent_socket_new(this->base, this->sock_fd, BEV_OPT_CLOSE_ON_FREE);
    if (!bev)
    {
        delete this;
        return false;
    }
    this->bev = bev;
    this->set_callback(bev);

    timeval t = {UNL_TIME_OUT_VAL, 0};
    bufferevent_set_timeouts(bev, &t, 0);

    std::string msg("220 Welcome to unl FtpServer\r\n");
    bufferevent_write(bev, msg.c_str(), msg.length());
    return true;
}

void uFtpServerCMD::read(bufferevent *bev)
{
    char buff[1024];
    while (true)
    {
        memset(buff, 0, sizeof(buff));
        int len = bufferevent_read(bev, buff, sizeof(buff));
        if (len <= 0)
        {
            break;
        }
        std::string s(buff);
        std::cout << "recv:" << s << std::endl;
        auto v = util::split(s, " \r\n");
        std::string type = v[0];
        std::string msg;
        if (v.size() >= 2)
        {
            msg = v[1];
        }
        else
        {
            msg = "";
        }
        if (this->calls.find(type) != this->calls.end())
        {
            uFtpTask *t = this->calls[type];
            t->base = base;
            t->cmd_task = this;
            t->ip = this->ip;
            t->port = this->port;
            t->parse(type, msg);
        }
        else
        {
            std::string msg = "200 OK\r\n";
            bufferevent_write(bev, msg.c_str(), msg.length());
        }
    }
}

void uFtpServerCMD::event(struct bufferevent *bev, short what)
{
    if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT))
    {
        std::cout << "connect closed!" << std::endl;
        delete this;
    }
}

uFtpServerCMD::uFtpServerCMD()
{
}

uFtpServerCMD::~uFtpServerCMD()
{
    for (auto t : this->calls_del)
    {
        if (t.second)
        {
            std::cout << "aaaa" << std::endl;
            t.first->close();
            delete t.first;
        }
    }
    this->close();
}