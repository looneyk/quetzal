#if !defined(QUETZAL_STRING_UTIL_HPP)
#define QUETZAL_STRING_UTIL_HPP
//------------------------------------------------------------------------------
// string_util.hpp
//------------------------------------------------------------------------------

#include <string>
#include <sstream>

namespace quetzal
{

    //--------------------------------------------------------------------------
    template<typename T>
    std::string to_string(const T& t)
    {
        std::ostringstream oss;
        oss << t;
        return oss.str();
    }

    std::string to_string(const std::string& str);
    std::string to_string(const std::wstring& wstr);
    std::string to_string(const bool& b);

    //--------------------------------------------------------------------------
    template<typename T>
    T to_type(const std::string& str)
    {
        std::istringstream iss(str);
        T t;
        iss >> t;
        return t;
    }

    template<>
    std::string to_type<std::string>(const std::string& str);

    template<>
    std::wstring to_type<std::wstring>(const std::string& str);

    template<>
    bool to_type<bool>(const std::string& str);

// to_utf8 ...
//    std::string to_utf8(char16_t c);
//    std::string to_utf8(char32_t c);

} // namespace quetzal

#endif // QUETZAL_STRING_UTIL_HPP
