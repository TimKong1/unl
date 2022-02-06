#pragma once
#include <list>
#include <mutex>
#include <event2/event.h>
namespace unl
{
    class uTask;
    class uThread
    {
    private:
        int id = 0;
        int notify_send_fd = 0;
        struct event_base *base = nullptr;
        std::list<uTask *> tasks;
        std::mutex tasks_mutex;

    public:
        void start();
        //thread entry
        void main();
        //install thread
        bool setup();
        void notify(evutil_socket_t fd, short which);
        void activate();
        //one thread handles multiple tasks,share a event base
        void add_task(uTask *task);
        uThread(int id = 0);
        ~uThread();
    };

}
