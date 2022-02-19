#include "http/uHttpRequest.h"
#include <ctre.hpp>
#include <regex>

using namespace unl;

int uHttpRequest::parse_from_title(std::string_view title)
{
    static constexpr auto pattern = ctll::fixed_string{ "^(GET|POST|HEAD|PUT|DELETE|TRACE|CONNECT|OPTIONS)\\s+(\\/\\w+.*)\\s+(HTTP.*)$" };
    

    auto matcher = ctre::match<pattern>(title);
    if(!matcher.get<0>()){
        return -1;
    }
    parse_methos(matcher.get<1>());
    parse_path_parameter(matcher.get<2>());
    parse_protocol(matcher.get<3>());
    return 0;
}

int uHttpRequest::parse_add_headers(std::string_view headers)
{
    size_t pos = headers.find(':');
    if(pos == std::string_view::npos){
        return -1;
    }
    this->headers[headers.substr(0, pos)] = headers.substr(pos + 2);
    return 0;
}

int uHttpRequest::parse_methos(std::string_view method)
{
    this->method = http::method_name_dict.find(method)->second;
    return 0;
}

int uHttpRequest::parse_protocol(std::string_view protocol)
{
    this->protocol = protocol;
    return 0;
}

int uHttpRequest::parse_body(std::string_view body)
{
    this->body = body;
    return 0;
}

int uHttpRequest::parse_path_parameter(std::string_view parameter)
{
    static constexpr auto pattern_and = ctll::fixed_string{ "&" };
    std::string_view uri = parameter;
    size_t pos = uri.find('?');
    if(pos == std::string_view::npos){
        this->path = uri;
    }else{
        this->path = uri.substr(0, pos);
        std::string_view sparm =  uri.substr(pos + 1);
        auto plist = ctre::split<pattern_and>(sparm);
        for (auto &&e : plist)
        {
            auto pv = e.to_view();
            size_t npos = pv.find('=');
            if(npos != std::string_view::npos){
                this->parameter[pv.substr(0,npos)] = pv.substr(npos + 1);
            }
        }
    }
    return 0;
}


int uHttpRequest::get_fromdata(std::map<std::string_view, std::string_view> &fromdata)
{
    static constexpr auto pattern_and = ctll::fixed_string{ "&" };
    auto plist = ctre::split<pattern_and>(this->body);
    for (auto &&e : plist)
    {
        auto pv = e.to_view();
        size_t npos = pv.find('=');
        if(npos != std::string_view::npos){
            fromdata[pv.substr(0,npos)] = pv.substr(npos + 1);
        }else{
            return -1;
        }
    }
    return 0;
}


int uHttpRequest::make_request(std::string_view s)
{
    static constexpr auto pattern = ctll::fixed_string{ "\r\n" };
    auto sp = ctre::split<pattern>(s);
    int setp = 0;//setp=0:title setp=1:headers setp=2:body
    std::string_view body;
    for (auto &&e: sp){
        if(setp == 0){
            std::string_view title = e.to_view();
            if(!title.length()){
                return -1;
            }
            parse_from_title(title);
            setp++;
        }
        else if (setp == 1)
        {
            std::string_view header = e.to_view();
            if(header.length() > 0){
                if(parse_add_headers(header) < 0){
                    setp++;
                }
            }else{
                setp++;
            }
        }else{
            parse_body(e.to_view());
            break;
        }
    }
    return 0;
}

bool uHttpRequest::match_path(const std::string &path)
{
    const std::regex pattern(path);
    return std::regex_match(this->path, pattern);
}

uHttpRequest::uHttpRequest(){}
uHttpRequest::~uHttpRequest(){}