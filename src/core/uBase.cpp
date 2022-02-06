#include "uBase.h"

using namespace unl;

int uBase::base_new()
{
    this->base = event_base_new();
    if (!this->base)
    {
        printf("file:%s,line:%d event_base_new error", __FILE__, __LINE__);
        return -1;
    }
    return 0;
}

void uBase::base_free()
{
    if(!base){
        event_base_free(base);
    }
}

uBase::uBase(struct event_base *base)
{
    this->base = base;
}

uBase::uBase(const uBase &base)
{
    this->base = base.base;
}

uBase::uBase(){}
uBase::~uBase(){}