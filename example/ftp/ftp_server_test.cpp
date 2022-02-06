#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <iostream>
#ifndef _WIN32
#include <signal.h>
#include <arpa/inet.h>
#endif
#include "thread/uThreadPool.h"
#include "uFtpFactory.h"
#include "util/utstr.hpp"
#define SERVER_PORT 8899

void mlisten_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *addr, int socklen, void *arg)
{
    unl::uTask *task = unl::uFtpFactory::get()->create_task();
    task->sock_fd = fd;
    struct sockaddr_in *saddr = (struct sockaddr_in *)addr;
    std::cout << inet_ntoa(saddr->sin_addr) << ":" << ntohs(saddr->sin_port) << " connected!" << std::endl;
    unl::uThreadPool::get()->dispatch(task);
}

int main(int argc, char *argv[])
{
#ifdef _WIN32
    //init socket lib
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#else
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        return 1; //ignore pipe signal,send data to closed socked
    }
#endif
    unl::uThreadPool *tp = unl::uThreadPool::get();
    tp->init(4);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    struct event_base *base = event_base_new();
    if (!base)
    {
        printf("file:%s,line:%d event_base_new error", __FILE__, __LINE__);
    }

    struct evconnlistener *listener = evconnlistener_new_bind(
        base,
        mlisten_cb,
        (void *)base,
        LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,
        -1,
        (struct sockaddr *)&server_addr,
        sizeof(server_addr));
    if (!base)
    {
        printf("file:%s,line:%d evconnlistenet_new_bind error", __FILE__, __LINE__);
    }
    event_base_dispatch(base);
    evconnlistener_free(listener);
    event_base_free(base);

    return 0;
}