//------------------------------------------------------------------------------
// system_path.cpp
//------------------------------------------------------------------------------

#include "system_path.hpp"
#include "string_util.hpp"

using namespace std;

//------------------------------------------------------------------------------
string quetzal::system_path(REFKNOWNFOLDERID rfid)
{
    string s;

    wchar_t* pwc = nullptr;
    HRESULT hr = SHGetKnownFolderPath(rfid, 0, nullptr, &pwc);
    if (SUCCEEDED(hr) && pwc != nullptr)
    {
        s = to_string(wstring(pwc));
        CoTaskMemFree(pwc);
    }

    return s;
}
