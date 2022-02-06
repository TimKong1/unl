#pragma once
#include "uBase.h"

namespace unl
{
    class uTask : public uBase
    {
    private:
    public:
        int sock_fd = 0;
        int thread_id = 0;
        virtual bool init() = 0;
        uTask(){};
        virtual ~uTask(){};
    };
}
