#pragma once
#include "thread/uTask.h"

namespace unl
{
    class uFtpFactory
    {
    private:
        uFtpFactory();

    public:
        static uFtpFactory *get()
        {
            static uFtpFactory f;
            return &f;
        }
        uTask *create_task();

        ~uFtpFactory();
    };
}