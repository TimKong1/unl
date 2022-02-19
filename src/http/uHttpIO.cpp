#include "http/uHttpIO.h"
#include <fmt/core.h>
#include <fmt/format.h>
#include <ctre.hpp>
#include <vector>
#include <string_view>
#include <fmt/chrono.h>
#include <chrono>
#include <array>
#include <stdio.h>

using namespace unl;

void uHttpIO::read_callback()
{
    std::string s = readall();
    request.make_request(s);
    // find a matching path
    for (auto i : server->server_map)
    {
       if(request.match_path(i.first)){
           i.second(this); // execute server function
           break;
       }
    }
    // not matching or server function have not call reply function
    if(!response_ok){
        reply_text(404, "Not Found", "<h1 align=\"center\">404 Not Found</h1><hr /><p align=\"center\">UNL HTTPServer</p>");
    }
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

std::string uHttpIO::get_gmt_date()
{
    std::time_t time = std::time(nullptr);
    return fmt::format("{:%a, %d %b %Y %H:%M:%S} GMT", fmt::localtime(time));
}

std::string_view uHttpIO::get_file_type(std::string_view path)
{
    static constexpr std::array<std::string_view, 10> types = {
        "txt", "html", "jpg", "gif", "png",
        "wav", "avi", "mov", "mp3", "mp4"
    };
    static constexpr std::array<std::string_view, 10> ret = {
        "text/plain; charset=utf-8", "text/plain; charset=utf-8",
        "image/jpeg", "image/gif", "image/png","audio/wav",
        "video/x-msvideo", "video/quicktime", "audio/mpeg", "audio/mpeg"
    };
    for (size_t i = 0; i < types.size(); i++)
    {
        if(path.compare(path.size() - types[i].size(), types[i].size(), types[i]) == 0){
            return ret[i];
        }
    }
    return "text/plain; charset=utf-8";
}

void uHttpIO::write_headers()
{
    for(auto &i : headers){
        write(i.first.data(),i.first.length());
        write(": ",2);
        write(i.second.data(),i.second.length());
        write("\r\n",2);
    }
    write("\r\n",2);
}

void uHttpIO::write_status(int status, std::string_view sstatus)
{
    std::string tstatus = fmt::format("HTTP/1.1 {} {}\r\n", status, sstatus);
    write(tstatus.c_str(), tstatus.length());
}

void uHttpIO::reply_text(int status, std::string_view sstatus, std::string_view txt)
{
    write_status(status, sstatus);
    headers["Date"] = get_gmt_date();
    headers["Content-Type"] = "text/html";
    write_headers();
    write(txt.data(), txt.length());
    response_ok = true;
}

void uHttpIO::reply_file(std::string_view file_name)
{
    
    fmt::print("fname:{}\r\n",file_name.data());
    FILE* fp = fopen(file_name.data(),"rb");
    if(fp == NULL){
        reply_text(500, "ERROR", "<h1 align=\"center\">500 ERROR</h1><hr /><p align=\"center\">UNL HTTPServer</p>");
        return;
    }
    write_status(200, "ok");
    headers["Date"] = get_gmt_date();
    headers["Content-Type"] = get_file_type(file_name);
    write_headers();
    char buff[1024];
    size_t len = 0;
    do
    {
        memset(buff, 0, sizeof(buff));
        len = fread(buff, 1, sizeof(buff), fp);
        write(buff,len);
    } while (len);
    fclose(fp);
    
    response_ok = true;
}