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
    std::string s = readall();
    request.make_request(s);
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