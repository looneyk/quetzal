//------------------------------------------------------------------------------
// error_util.cpp
//------------------------------------------------------------------------------

#include "error_util.hpp"
#include "string_util.hpp"
#include <debugapi.h>

using namespace std;

//------------------------------------------------------------------------------
std::string quetzal::to_error_string(DWORD error)
{
    wchar_t buffer[256] = {};

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, error, 0, buffer, static_cast<DWORD>(size(buffer)), nullptr);

    return to_string(wstring(buffer)); // should be a better way ...
}

//------------------------------------------------------------------------------
std::string quetzal::to_error_string(HRESULT hr)
{
    return to_error_string(static_cast<DWORD>(hr));
}

//------------------------------------------------------------------------------
void quetzal::write_debug_string(const std::string& s)
{
    OutputDebugString(to_type<wstring>(s).c_str());
    return;
}


/* 
    // Using FORMAT_MESSAGE_ALLOCATE_BUFFER:

    wchar_t* ptext = nullptr;
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, hr, 0, (LPTSTR)&ptext, 0, nullptr);
    //FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&ptext, 0, nullptr);

    if (ptext != nullptr)
    {
        m_text = to_string(ptext);
        HeapFree(GetProcessHeap(), 0, ptext);
    }
*/
