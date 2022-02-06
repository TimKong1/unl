#include "thread/uThread.h"
#include <thread>
#include <iostream>
#include <event2/event.h>
#include "thread/uTask.h"
#ifdef _WIN32
#else
#include <unistd.h>
#endif
using namespace unl;

static void pipe_read_cb(evutil_socket_t fd, short which, void *arg)
{
    uThread *t = (uThread *)arg;
    t->notify(fd, which);
}

void uThread::start()
{
    this->setup();
    std::thread _thread(&uThread::main, this);
    _thread.detach();
}

void uThread::main()
{
    std::cout << "thread" << id << "run main" << std::endl;
    event_base_dispatch(this->base);
    std::cout << "thread" << id << "end main" << std::endl;
    event_base_free(this->base);
}

bool uThread::setup()
{
#ifdef _WIN32
    evtuil_socket_t fds[2];
    if (evutil_socketpair(AF_INET, SOCK_STREAM, 0, fds) < 0)
    {
        return false;
    }
    evutil_make_socket_nonblocking(fds[0]);
    evutil_make_socket_nonblocking(fds[1]);
#else
    int fds[2];
    if (pipe(fds))
    {
        std::cerr << "pipe init error" << std::endl;
        return false;
    }
#endif
    this->notify_send_fd = fds[1];
    event_config *ev_config = event_config_new();
    event_config_set_flag(ev_config, EVENT_BASE_FLAG_NOLOCK); //TODO:Delete when using multiple processes
    this->base = event_base_new_with_config(ev_config);
    event_config_free(ev_config);
    if (!this->base)
    {
        return false;
    }
    event *ev = event_new(this->base, fds[0], EV_READ | EV_PERSIST, pipe_read_cb, this);
    event_add(ev, NULL);
    return true;
}

void uThread::notify(evutil_socket_t fd, short which)
{
    char buf[2] = {0};
#ifdef _WIN32
    int len = recv(fd, buf, 1, 0);
#else
    int len = read(fd, buf, 1);
#endif
    if (len <= 0)
    {
        std::cerr << "uThread::notify read error!" << std::endl;
        return;
    }
    tasks_mutex.lock();
    if (tasks.empty())
    {
        tasks_mutex.unlock();
        return;
    }
    uTask *task = tasks.front();
    this->tasks.pop_front();
    this->tasks_mutex.unlock();
    task->init();
}

void uThread::activate()
{
#ifdef _WIN32
    int len = send(this->notify_send_fd, "c", 1, 0);
#else
    int len = write(this->notify_send_fd, "c", 1);
#endif
    if (len <= 0)
    {
        std::cerr << "uThread::activate() error!" << std::endl;
        return;
    }
}

void uThread::add_task(uTask *task)
{
    if (!task)
    {
        return;
    }
    task->base = this->base;
    task->thread_id = this->id;
    this->tasks_mutex.lock();
    this->tasks.push_back(task);
    this->tasks_mutex.unlock();
}

uThread::uThread(int id)
{
    this->id = id;
}

uThread::~uThread()
{
}