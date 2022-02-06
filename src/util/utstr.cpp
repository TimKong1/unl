#include "util/utstr.hpp"
#include <iostream>
#include <string>
#include <stdio.h>

std::vector<std::string> unl::util::split(const std::string &src, const std::string &separator)
{
    char *pNext;
    std::vector<std::string> v;
    if (src.empty())
        return v;
    if (separator.empty())
        return v;
    int len = src.length() + 1;
    char *buff = new char[len];
	memset(buff,0,len);
    src.copy(buff, len);
    pNext = strtok(buff, separator.c_str());
    while (pNext != NULL)
    {
        v.push_back(std::string(pNext));
        pNext = strtok(NULL, separator.c_str());
    }
    delete[] buff;
    return v;
}

int unl::util::exec_cmd(std::string cmd, std::string &res)
{
    if (cmd.size() == 0)
    { //cmd is empty
        return -1;
    }

    char buffer[1024] = {0};
    std::string result = "";
    FILE *pin = popen(cmd.c_str(), "r");
    if (!pin)
    { //popen failed
        return -1;
    }

    res.clear();
    while (!feof(pin))
    {
        if (fgets(buffer, sizeof(buffer), pin) != NULL)
        {
            result += buffer;
        }
    }

    res = result;
    return pclose(pin); //-1:pclose failed; else shell ret
}
