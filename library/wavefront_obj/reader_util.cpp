//------------------------------------------------------------------------------
// wavefront_obj
// reader_util.cpp
//------------------------------------------------------------------------------

#include "reader_util.hpp"
#include "common/string_util.hpp"

using namespace std;

//--------------------------------------------------------------------------
vector<string> quetzal::wavefront_obj::tokenize(const string& s, string delimiters, empties e)
{
    vector<string> tokens;
    size_t pos;
    size_t pos_next = numeric_limits<size_t>::max();

    do
    {
        if (e == empties::rejected)
        {
            pos_next = s.find_first_not_of(delimiters, pos_next + 1);
            if (pos_next == string::npos)
            {
                break;
            }
            pos_next -= 1;
        }

        pos = pos_next + 1;
        pos_next = s.find_first_of(delimiters, pos);
        tokens.push_back(s.substr(pos, pos_next - pos));
    }
    while (pos_next != string::npos);

    return tokens;
}

//--------------------------------------------------------------------------
bool quetzal::wavefront_obj::parse_vertex_reference(const string& s, vertex_reference_type& vr)
{
    auto tokens = tokenize(s, "/", empties::allowed);
    if (tokens.empty() || tokens.size() > 3)
    {
        return false;
    }

    vr[0] = to_type<long>(tokens[0]);

    if (tokens.size() > 1)
    {
        vr[1] = to_type<long>(tokens[1]);
    }

    if (tokens.size() > 2)
    {
        vr[2] = to_type<long>(tokens[2]);
    }

    return true;
}
