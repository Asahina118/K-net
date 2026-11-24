#include <string_view>

namespace knet
{
    template <typename T>
    inline std::string_view get_err_msg(T error)
    {
        return get_msg(error);
    }
}
