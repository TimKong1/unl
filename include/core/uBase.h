#pragma once
#include <event2/event.h>
#include <event2/listener.h>

namespace unl
{
    class uBase
    {
    public:
        struct event_base *base = 0;
    public:
        uBase(struct event_base *base);
        uBase(const uBase &base);
        uBase();
        ~uBase();
        int base_new();
        void base_free();        
    };
}
