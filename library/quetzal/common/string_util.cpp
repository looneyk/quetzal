//------------------------------------------------------------------------------
// string_util.cpp
//------------------------------------------------------------------------------

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "string_util.hpp"
#include <locale>
#include <codecvt>

using namespace std;

//------------------------------------------------------------------------------
string quetzal::to_string(const string& str)
{
    return str;
}

//------------------------------------------------------------------------------
string quetzal::to_string(const wstring& wstr)
{
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

//------------------------------------------------------------------------------
string quetzal::to_string(const bool& b)
{
    return b ? "true" : "false";
}

//------------------------------------------------------------------------------
template<>
string quetzal::to_type<string>(const string& str)
{
    return str;
}

//------------------------------------------------------------------------------
template<>
wstring quetzal::to_type<wstring>(const string& str)
{
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

//------------------------------------------------------------------------------
template<>
bool quetzal::to_type<bool>(const string& str)
{
    return str == "true" || str == "1";
}
