#pragma once
#include <string>
#include <vector>
#include <string.h>
namespace unl
{
    namespace util
    {
        std::vector<std::string> split(const std::string &src, const std::string &separator);
        int exec_cmd(std::string cmd, std::string &res);
    }
}