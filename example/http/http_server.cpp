#include "http/uHttpServer.h"
#include <nlohmann/json.hpp>
#include <fmt/format.h>
#include <cstring>

using json = nlohmann::json;
using namespace unl;

int getNum(std::string_view s){
    return atoi(std::string(s).c_str());
}

void website(uHttpIO *io)
{
    using namespace std::string_literals;
    io->reply_file("./"s + io->request.path);
}

void add(uHttpIO *io)
{
    auto request = io->request;
    if(request.method == http::method::GET){
        int num1 = getNum(request.parameter["num1"]);
        int num2 = getNum(request.parameter["num2"]);
        io->reply_text(200,"ok",fmt::format("{}+{}={}",num1,num2,num1+num2));   
    }else if(request.headers["Content-Type"] == "application/json"){
        json data = json::parse(request.body);
        int num1 = data["num1"];
        int num2 = data["num2"];
        json ret = {
            {"code" , 200},
            {"msg", "ok"},
            {"data", {
                {"num1", num1},
                {"num2", num2},
                {"sum", num1+num2}
            }}
        };
        io->reply_text(200, "ok", ret.dump(), "application/json");
    }else{
        std::map<std::string_view,std::string_view> from_data;
        request.get_fromdata(from_data);
        int num1 = getNum(from_data["num1"]);
        int num2 = getNum(from_data["num2"]);
        io->reply_text(200, "ok", fmt::format("{}+{}={}",num1,num2,num1+num2));
    }
}

void helloworld(uHttpIO *io)
{
    io->reply_text(200, "OK", "<h1>hello world</h1>");
}

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
    server->server_map["^/website.*"] = website;
    server->server_map["^/add.*"] = add;
    server->server_map["^/hello.*"] = helloworld;
    server->dispatch();
    delete server;
    return 0;
}