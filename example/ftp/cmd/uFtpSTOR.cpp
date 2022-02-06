#include "uFtpSTOR.h"
#include <iostream>

using namespace unl;

void uFtpSTOR::parse(const std::string &type, std::string &msg)
{
    std::string path = cmd_task->root_dir + cmd_task->cur_dir;
    if (path[path.size() - 1] != '/')
    {
        path += "/";
    }
    path += msg;

    this->fp = fopen(path.c_str(), "wb");
    if (this->fp)
    {
        this->connect_port();
        this->res_cmd("125 file ok\r\n");
        bufferevent_trigger(bev, EV_READ, 0);
    }
    else
    {
        this->res_cmd("450 file open failed!\r\n");
    }
}

void uFtpSTOR::event(struct bufferevent *bev, short what)
{
    if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT))
    {
        this->close();
        if (fp)
        {
            fclose(fp);
            fp = 0;
        }
        std::cout << "stor connect closed!" << std::endl;
        this->res_cmd("226 Transfer complete\r\n");
    }
    else if (what & BEV_EVENT_CONNECTED)
    {
        std::cout << "stor  connected!" << std::endl;
    }
}

void uFtpSTOR::read(bufferevent *bev)
{
    if (!fp)
    {
        return;
    }
    while (true)
    {
        char buf[1024];
        int len = bufferevent_read(bev, buf, sizeof(buf));
        if (len <= 0)
        {
            return;
        }
        int size = fwrite(buf, 1, len, fp);
        if (len != size)
        {
            this->close();
            this->res_cmd("450 file write failed!\r\n");
            return;
        }
    }
}