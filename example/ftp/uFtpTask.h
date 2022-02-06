#pragma once
#include "thread/uTask.h"
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <string>
#include "uconfig.h"
namespace unl
{
    class uFtpTask : public uTask
    {
    private:
    public:
        uFtpTask *cmd_task = 0;
        std::string cur_dir = "/";
        std::string root_dir = UNL_ROOT_DIR;
        //data channel
        std::string ip;
        int port = 0;
        void connect_port();
        void send(const std::string &msg);
        void send(const void *data, int size);
        void close();
        //parse cmd
        virtual void parse(const std::string &type, std::string &msg){};
        void res_cmd(const std::string &msg);
        virtual void read(bufferevent *bev){};
        virtual void write(bufferevent *bev){};
        virtual void event(struct bufferevent *bev, short what){};
        void set_callback(struct bufferevent *bev);
        virtual bool init() { return true; };
        uFtpTask();
        virtual ~uFtpTask();

    protected:
        struct bufferevent *bev = 0;
        FILE *fp = 0;
        static void event_cb(bufferevent *bev, short what, void *ctx);
        static void write_cb(bufferevent *bev, void *arg);
        static void read_cb(struct bufferevent *bev, void *arg);
    };
}