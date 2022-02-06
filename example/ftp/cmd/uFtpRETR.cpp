#include "uFtpRETR.h"
#include <iostream>

using namespace unl;

void uFtpRETR::parse(const std::string &type, std::string &msg)
{
    std::string path = cmd_task->root_dir + cmd_task->cur_dir;
    if (path[path.size() - 1] != '/')
    {
        path += "/";
    }
    path += msg;
    this->fp = fopen(path.c_str(), "rb");
    if (this->fp)
    {
        this->connect_port();
        this->res_cmd("150 file ok\r\n");
        bufferevent_trigger(bev, EV_WRITE, 0);
    }
    else
    {
        this->res_cmd("450 file open failed!\r\n");
    }
}

void uFtpRETR::write(bufferevent *bev)
{
    if (!fp)
    {
        return;
    }
    char buf[1024];
    int len = fread(buf, 1, sizeof(buf), fp);
    if (len <= 0)
    {
        fclose(fp);
        fp = 0;
        this->res_cmd("226 Transfer complete\r\n");
        this->close();
        return;
    }
    this->send(buf, len);
}

void uFtpRETR::event(struct bufferevent *bev, short what)
{
    if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT))
    {
        this->close();
        if (fp)
        {
            fclose(fp);
            fp = 0;
        }
        std::cout << "retr connect closed!" << std::endl;
    }
    else if (what & BEV_EVENT_CONNECTED)
    {
        std::cout << "retr  connected!" << std::endl;
    }
}

uFtpRETR::uFtpRETR()
{
}

uFtpRETR::~uFtpRETR()
{
}