#include "http/uHttpIO.h"

using namespace unl;

uHttpIO::uHttpIO(uBase *base, int fd) : uIO(base, fd){};

void uHttpIO::read_callback()
{
    std::string s = readall();
    write(s.c_str(),s.length());
}

void uHttpIO::event_callback(short what)
{
    if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT))
    {
        delete this;
    }
}

uHttpIO::~uHttpIO(){};