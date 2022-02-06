#include "uFtpTask.h"
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <iostream>
#include "uconfig.h"

using namespace unl;

void uFtpTask::connect_port()
{
    if (this->ip.empty() || this->port <= 0 || !this->base)
    {
        std::cout << "Connect port failed" << std::endl;
        return;
    }
    this->close();
    this->bev = bufferevent_socket_new(this->base, -1, BEV_OPT_CLOSE_ON_FREE);
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(this->port);
    evutil_inet_pton(AF_INET, this->ip.c_str(), &sin.sin_addr.s_addr);
    this->set_callback(this->bev);
    timeval t = {UNL_TIME_OUT_VAL, 0};
    bufferevent_set_timeouts(this->bev, &t, 0);
    int ret = bufferevent_socket_connect(this->bev, (sockaddr *)&sin, sizeof(sin));
    printf("%d\n", ret);
}

void uFtpTask::send(const std::string &msg)
{
    this->send(msg.c_str(), msg.length());
}

void uFtpTask::send(const void *data, int size)
{
    if (!this->bev)
    {
        return;
    }
    bufferevent_write(this->bev, data, size);
}

void uFtpTask::close()
{
    if (fp)
    {
        fclose(fp);
    }
    if (bev)
    {
        bufferevent_free(bev);
        bev = 0;
    }
}

void uFtpTask::res_cmd(const std::string &msg)
{
    if (!cmd_task || !cmd_task->bev)
        return;
    std::cout << "res msg:" << msg << std::endl;
    bufferevent_write(cmd_task->bev, msg.c_str(), msg.length());
}

void uFtpTask::event_cb(struct bufferevent *bev, short what, void *ctx)
{
    uFtpTask *t = (uFtpTask *)ctx;
    t->event(bev, what);
}

void uFtpTask::write_cb(bufferevent *bev, void *arg)
{
    uFtpTask *t = (uFtpTask *)arg;
    t->write(bev);
}

void uFtpTask::read_cb(bufferevent *bev, void *arg)
{
    uFtpTask *t = (uFtpTask *)arg;
    t->read(bev);
}

void uFtpTask::set_callback(struct bufferevent *bev)
{
    bufferevent_setcb(bev, read_cb, write_cb, event_cb, this);
    bufferevent_enable(bev, EV_READ | EV_WRITE | EV_TIMEOUT);
}

uFtpTask::uFtpTask() {}
uFtpTask::~uFtpTask() {}