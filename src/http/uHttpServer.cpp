#include "http/uHttpServer.h"
#include "http/uHttpIO.h"

using namespace unl;

void uHttpServer::on_listen(int fd, const char *ip, uint16_t port)
{
    std::cout << ip << ":" << port << std::endl;
    uHttpIO *io = new uHttpIO(this,fd); // connect client
    timeval t = {60, 0};
    io->set_timeouts(&t);
    io->server = this;
    io->callback_enable(EV_WRITE|EV_READ|EV_TIMEOUT);
}


uHttpServer::uHttpServer(const char *addr, uint16_t port): uListener(addr, port){}

uHttpServer::~uHttpServer(){}