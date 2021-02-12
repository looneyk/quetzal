//------------------------------------------------------------------------------
// error_util.hpp
//------------------------------------------------------------------------------

#if !defined(QUETZAL_ERROR_UTIL_HPP)
#define QUETZAL_ERROR_UTIL_HPP

#include <string>
#define NOMINMAX
#include <windows.h>
#undef NOMINMAX

namespace quetzal
{

    std::string to_error_string(DWORD error);
    std::string to_error_string(HRESULT hr);

    void write_debug_string(const std::string& s);

} // namespace quetzal

#endif // QUETZAL_ERROR_UTIL_HPP
