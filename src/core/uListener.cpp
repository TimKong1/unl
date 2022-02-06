#include "uListener.h"

using namespace unl;

void listen_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *addr, int socklen, void *arg)
{
    uListener *__this = (uListener*)arg;
    struct sockaddr_in *saddr = (struct sockaddr_in *)addr;
    __this->on_listen(fd, inet_ntoa(saddr->sin_addr), ntohs(saddr->sin_port));
}

uListener::uListener(in_addr_t addr, in_port_t port)
{
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(port);
    this->addr.sin_addr.s_addr = htonl(addr);
    this->listener_new();
}

int uListener::listener_new(){
    this->base_new();
    this->listener = evconnlistener_new_bind(
        this->base,
        listen_cb,
        (void*)this,
        LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,
        -1,
        (struct sockaddr *)&this->addr,
        sizeof(this->addr));
    if (!this->listener)
    {
        printf("file:%s,line:%d evconnlistenet_new_bind error", __FILE__, __LINE__);
        return -1;
    }
    return 0;
}

uListener::uListener(const char *addr, uint16_t port){
    int dst;
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(port);
    inet_pton(AF_INET, addr, (void* )&dst);
    this->addr.sin_addr.s_addr = htonl(dst);
    this->listener_new();
}

uListener::~uListener()
{
    evconnlistener_free(listener);
    this->base_free();
    std::cout << "uListener end " << std::endl;
}

int uListener::dispatch(){
    return event_base_dispatch(this->base);
}