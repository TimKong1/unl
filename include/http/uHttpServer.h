#include "uListener.h"

namespace unl{
    class uHttpServer : public uListener
    {
    public:
        void on_listen(int fd, const char *ip, uint16_t port);
        uHttpServer(const char *addr, uint16_t port);
        ~uHttpServer();
    };
}