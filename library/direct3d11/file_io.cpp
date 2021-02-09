//------------------------------------------------------------------------------
// file_io.cpp
//------------------------------------------------------------------------------

#include "file_io.hpp"
#include <fstream>

using namespace std;

//------------------------------------------------------------------------------
vector<char> quetzal::direct3d11::read(const std::filesystem::path& pathname)
{
    vector<char> v;

    ifstream ifs(pathname, ios_base::binary);
    if (ifs)
    {
        ifs.seekg(0, ios_base::end);
        v.resize(static_cast<unsigned int>(ifs.tellg()));
        ifs.seekg(0, ios_base::beg);
        ifs.read(v.data(), v.size());
    }

    return v;
}
