#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <event2/bufferevent.h>
#include <iostream>
#ifndef _WIN32
#include <signal.h>
#include <arpa/inet.h>
#endif

#include "uBase.h"

namespace unl
{
    class uListener : public uBase
    {
    private:
        struct sockaddr_in addr;
        struct evconnlistener *listener;
    public:
        uListener(in_addr_t addr = INADDR_ANY, in_port_t port = 8888);
        uListener(const char *addr, uint16_t port);
        virtual ~uListener();
        int dispatch();
        virtual void on_listen(int fd, const char *ip, uint16_t port) = 0;
    private:
        int listener_new();
    };
    
} // namespace unl
