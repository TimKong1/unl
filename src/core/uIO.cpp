#include "uIO.h"

using namespace unl;

static void read_cb(struct bufferevent *bev, void *ctx)
{
    uIO *io = (uIO*)ctx;
    io->read_callback();
}

static void write_cb(struct bufferevent *bev, void *ctx)
{
    uIO *io = (uIO*)ctx;
    io->write_callback();
}

static void event_cb(struct bufferevent *bev, short what, void *ctx)
{
    uIO *io = (uIO*)ctx;
    io->event_callback(what);
}

int uIO::write(const char *data, size_t len)
{
    return bufferevent_write(bev, data, len);
}

size_t uIO::read(void *data, size_t len)
{
    return bufferevent_read(bev, data, len);
}

std::string uIO::readall()
{
    std::string s;
    char buff[1024];
    while (true)
    {
        bzero(buff, sizeof(buff));
        size_t len = read(buff,sizeof(buff));
        if(len <= 0) {
            break;
        }
        s += buff;
    }
    return s;
}

void uIO::close()
{
    if (bev)
    {
        bufferevent_free(bev);
        bev = nullptr;
    }
}

int uIO::connect(const char *ip, uint16_t port)
{
    this->bev = bufferevent_socket_new(this->base, -1, BEV_OPT_CLOSE_ON_FREE);
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    evutil_inet_pton(AF_INET, ip, &sin.sin_addr.s_addr);
    set_callback();
    return bufferevent_socket_connect(this->bev, (sockaddr *)&sin, sizeof(sin));
}

int uIO::set_timeouts(const struct timeval *time)
{
    return bufferevent_set_timeouts(bev, time, 0);
}

int uIO::callback_enable(short what)
{
    return bufferevent_enable(bev, what);
}

void uIO::set_callback()
{
    bufferevent_setcb(bev, read_cb, write_cb, event_cb, this);
}

uIO::uIO(){}

uIO::uIO(uBase *base, int fd)
{
    bev = bufferevent_socket_new(base->base, fd, BEV_OPT_CLOSE_ON_FREE);
    set_callback();
}

uIO::uIO(uBase *base, const char *ip, uint16_t port)
{
    this->base = base->base;
    this->connect(ip, port);
}

uIO::~uIO()
{
    close();
}