#pragma once

#include <vector>

namespace unl
{
    class uThread;
    class uTask;
    class uThreadPool
    {
    private:
        uThreadPool();
        int thread_num = 0;
        int last_thread_cnt = -1;
        std::vector<uThread *> threads;

    public:
        static uThreadPool *get()
        {
            static uThreadPool p;
            return &p;
        }
        void init(int num);
        void dispatch(uTask *task);
        ~uThreadPool();
    };

}
