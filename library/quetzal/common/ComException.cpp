//------------------------------------------------------------------------------
// ComException.cpp
//------------------------------------------------------------------------------

#include "ComException.hpp"
#include "error_util.hpp"
#include <sstream>
#include <iomanip>

using namespace std;

namespace
{

    //--------------------------------------------------------------------------
    string hr_message(HRESULT hr, const string& message)
    {
        ostringstream oss;
        oss << message
            << " " << hex << showbase << setw(10) << setfill('0') << hr << dec << " "
            << quetzal::to_error_string(hr);
        return oss.str();
    }

} // namespace

//------------------------------------------------------------------------------
quetzal::ComException::ComException(HRESULT hr, const string& message, const string& file, int line) :
    quetzal::Exception(hr_message(hr, message), file, line)
{
}
