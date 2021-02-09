//------------------------------------------------------------------------------
// Exception.cpp
//------------------------------------------------------------------------------

#include "Exception.hpp"
#include <sstream>

using namespace std;

namespace
{

    //--------------------------------------------------------------------------
    string file_line_message(const string& message, const string& file, int line)
    {
        ostringstream oss;
        oss << message << " (" << file << " " << line << ")";
        return oss.str();
    }

} // namespace

//------------------------------------------------------------------------------
quetzal::Exception::Exception(const string& message, const string& file, int line) :
    runtime_error(file_line_message(message, file, line))
{
}
