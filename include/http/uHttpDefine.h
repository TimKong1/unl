#include <string_view>
#include <vector>
#include <unordered_map>

namespace unl
{
    namespace http
    {
        namespace method{
            constexpr int GET = 0;
            constexpr int POST = 1;
            constexpr int HEAD = 2;
            constexpr int PUT = 3;
            constexpr int DELETE = 4;
            constexpr int TRACE = 5;
            constexpr int CONNECT = 6;
            constexpr int OPTIONS = 7;
        };
        const std::unordered_map<std::string_view, int> method_name_dict = {{"GET", 0} , {"POST", 1}, {"HEAD", 2} , {"DELETE", 3} , {"TRACE", 4} , {"CONNECT", 5} , {"OPTIONS", 6} , {"GET", 7} };
        const std::vector<std::string_view> method_name_list = {"GET", "POST", "HEAD", "PUT", "DELETE", "TRACE", "CONNECT", "OPTIONS"};
    }
} 
