#include <string>
#include <string_view>

namespace unl
{
    namespace util
    {
        std::string url_encode(std::string_view str);
        std::string url_decode(std::string_view str);
    }
    
} // namespace unl
