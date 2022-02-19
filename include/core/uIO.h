#pragma once
#include "uBase.h"
#include <event2/bufferevent.h>
#include <string>

namespace unl
{
    class uIO : public uBase
    {
    public:
        bufferevent *bev;
        int fd;
    public:
        int write(const char * data, size_t len);
        int flush(short iotype, enum bufferevent_flush_mode state);
        size_t read(void *data, size_t len);
        std::string readall();
        void close();
        int connect(const char *ip, uint16_t port);
        int set_timeouts(const struct timeval *time);
        int callback_enable(short event);
        void set_callback();
        virtual void write_callback(){};
        virtual void read_callback(){};
        virtual void event_callback(short what){};
        uIO();
        uIO(uBase *base, int fd);
        uIO(uBase *base, const char *ip, uint16_t port);
        virtual ~uIO();
    };
}


