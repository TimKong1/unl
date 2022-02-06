#include "http/uHttpServer.h"
using namespace unl;




int main(int argc, char *argv[])
{
#ifdef _WIN32
    //init socket lib
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#else
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        return 1; //ignore pipe signal,send data to closed socked
    }
#endif
    uHttpServer *server = new uHttpServer("0.0.0.0",8888);
    server->dispatch();
    delete server;
    return 0;
}