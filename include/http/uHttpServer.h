#pragma once
#include "uListener.h"
#include <functional>
#include <map>
#include <string>
#include "http/uHttpRequest.h"
#include "http/uHttpIO.h"

namespace unl{
    class uHttpIO;
    using server_function_t = std::function<void(uHttpIO*)>;
    class uHttpServer : public uListener
    {
    public:
        std::map<std::string, server_function_t> server_map;
        void on_listen(int fd, const char *ip, uint16_t port);
        uHttpServer(const char *addr, uint16_t port);
        ~uHttpServer();
    };
}