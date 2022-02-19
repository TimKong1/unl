#pragma once
#include "http/uHttpServer.h" 
#include "uIO.h"
#include "http/uHttpRequest.h"
#include "stdio.h"

namespace unl
{
    class uHttpServer;
    class uHttpIO : public uIO
    {
    public:
        uHttpRequest request;
        uHttpServer *server;
        std::map<std::string_view, std::string_view> headers{
            {"Server", "UNL/1.0.1"},
            {"Cache-Control", "no-cache"},
            {"Connection", "keep-alive"}
        };
    private:
        bool response_ok = 0;
        void read_callback() override;
        void write_callback() override;
        void event_callback(short what) override;
        std::string get_gmt_date();
        std::string_view get_file_type(std::string_view path);
        void write_headers();
        void write_status(int status, std::string_view sstatus);
        FILE *fp = 0;
    public:
        using uIO::uIO;
        ~uHttpIO() = default;
        void reply_text(int status, std::string_view sstatus, std::string_view txt);
        void reply_file(std::string_view file_name);
    };
}