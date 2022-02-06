#include "uFtpList.h"
#include <string.h>
#include <iostream>
#include "util/utstr.hpp"

using namespace unl;

void uFtpList::parse(const std::string &type, std::string &msg)
{
    std::string res_msg = "";
    if (type == "PWD") //Show current directory
    {
        if (this->cmd_task->cur_dir == "")
        {
            this->cmd_task->cur_dir = "/";
        }
        msg = "257 \"";
        msg += this->cmd_task->cur_dir;
        msg += "\" is current dir.\r\n";
        this->res_cmd(msg);
    }
    else if (type == "LIST") //list all file and directory
    {
        this->connect_port();
        this->res_cmd("150 Here comes the directory listing.\r\n");
        std::string ll = this->get_list_data(this->cmd_task->root_dir + this->cmd_task->cur_dir);
        this->send(ll);
    }
    else if (type == "CWD") //switch directory
    {
        int pos = msg.find("\r\n");
        std::string path = msg.substr(0, pos - 1);
        if (path[0] == '/') //Absolute path
        {
            cmd_task->cur_dir = path;
        }
        else
        {
            if (cmd_task->cur_dir[cmd_task->cur_dir.length() - 1] != '/')
            {
                cmd_task->cur_dir += "/";
            }
            cmd_task->cur_dir += path;
        }
        this->res_cmd("250 Directory success chanaged,\r\n");
    }
    else if (type == "CDUP")
    {
        std::string path = cmd_task->cur_dir;
        if (path == "/")
        { //root dir can`t cdup
            this->res_cmd("250 Directory success chanaged,\r\n");
            return;
        }
        if (path[path.size() - 1] == '/')
        {
            path.pop_back();
        }
        int pos = path.rfind("/");
        path = path.substr(0, pos);
        cmd_task->cur_dir = path;
        this->res_cmd("250 Directory success chanaged,\r\n");
    }
}

std::string uFtpList::get_list_data(const std::string &path)
{
    std::string cmd = "ls -l " + path;
    std::string ret;
    util::exec_cmd(cmd, ret);
    std::cout << cmd << std::endl;
    return ret + "\r\n";
}

void uFtpList::event(struct bufferevent *bev, short what)
{
    if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT))
    {
        this->close();
        std::cout << "list connect closed!" << std::endl;
    }
    else if (what & BEV_EVENT_CONNECTED)
    {
        std::cout << "uFtpList bev_event connected!" << std::endl;
    }
}

void uFtpList::read(bufferevent *bev)
{
}

void uFtpList::write(bufferevent *bev)
{
    this->res_cmd("226 Transfer complete\r\n");
    this->close();
}

uFtpList::uFtpList()
{
}

uFtpList::~uFtpList()
{
}