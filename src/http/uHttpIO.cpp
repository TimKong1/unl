#include "http/uHttpIO.h"
#include <fmt/core.h>
#include <fmt/format.h>
#include <ctre.hpp>
#include <vector>
#include <string_view>

using namespace unl;

uHttpIO::uHttpIO(uBase *base, int fd) : uIO(base, fd){};

void uHttpIO::read_callback()
{
    static constexpr auto pattern = ctll::fixed_string{ "\r\n" };
    std::string s = readall();
    auto sp = ctre::split<pattern>(s);
    int setp = 0;//setp=0:title setp=1:headers setp=2:body
    std::string_view title;
    std::string_view header;
    std::vector<std::string_view> headers;
    std::string_view body;
    for (auto &&e: sp){
        if(setp == 0){
            title = e.to_view();
            if(!title.length()){
                return;//error
            }
            setp++;
        }
        else if (setp == 1)
        {
            header = e.to_view();
            if(header.length() > 0){
                headers.push_back(header);
            }else{
                setp++;
            }
        }else{
            body = e.to_view();
            break;
        }
    }
    fmt::print("title:{}---\r\n",title);
    fmt::print("headers:{}---\r\n",fmt::join(headers,",\r\n"));
    fmt::print("body:{}---\r\n",body);
    std::string res = "HTTP/1.1 200 OK\r\n";
    res += "Content-Type: text/html\r\n";
    res += "Date: Sat, 07 Apr 2018 06:18:47 GMT\r\n";
    res += "Connection: close\r\n";
    res += "\r\n";
    res += "<h1>hello world</h1>\r\n\r\n";
    write(res.c_str(),res.length());
    this->response_ok = true;
}

void uHttpIO::write_callback()
{
    if(this->response_ok){
        delete this;
    }
}

void uHttpIO::event_callback(short what)
{
    if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT))
    {
        delete this;
    }
}

uHttpIO::~uHttpIO(){};