#pragma once
#include <map>
#include <string_view>
#include "uHttpDefine.h"
#include <string>

namespace unl
{
    class uHttpRequest
    {
    public:
        std::map<std::string_view, std::string_view> headers;
        std::map<std::string_view, std::string_view> parameter;
        std::string body;
        std::string path;
        std::string protocol;
        int method;
        int make_request(std::string_view s);
        int get_fromdata(std::map<std::string_view, std::string_view> &fromdata);
        bool match_path(const std::string &path);
    private:
        int parse_from_title(std::string_view title);
        int parse_add_headers(std::string_view headers);
        int parse_methos(std::string_view methon);
        int parse_protocol(std::string_view protocol);
        int parse_body(std::string_view body);
        int parse_path_parameter(std::string_view parameter);
    public:
        uHttpRequest();
        ~uHttpRequest();

    };
}

