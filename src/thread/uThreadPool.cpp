#include "thread/uThreadPool.h"
#include "thread/uThread.h"
#include "thread/uTask.h"

using namespace unl;

/* create number of thread */
void uThreadPool::init(int num)
{
    for (int i = 0; i < num; i++)
    {
        uThread *thread = new uThread(i + 1);
        thread->start();
        this->threads.push_back(thread);
    }
    this->thread_num = num;
}

void uThreadPool::dispatch(uTask *task)
{
    if (!task)
    {
        return;
    }
    int tc = (this->last_thread_cnt + 1) % this->thread_num;
    this->last_thread_cnt = tc;
    uThread *t = this->threads[tc];
    t->add_task(task);
    t->activate();
}

uThreadPool::uThreadPool() {}
uThreadPool::~uThreadPool() {}