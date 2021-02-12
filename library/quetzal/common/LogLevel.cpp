//------------------------------------------------------------------------------
// LogLevel.hpp
//------------------------------------------------------------------------------

#include "LogLevel.hpp"

using namespace std;

//------------------------------------------------------------------------------
string quetzal::to_string(LogLevel level)
{
    switch (level)
    {
        case LogLevel::None:
            return "[None ] ";

        case LogLevel::Fatal:
            return "[Fatal]";

        case LogLevel::Error:
            return "[Error]";

        case LogLevel::Warn:
            return "[Warn ] ";

        case LogLevel::Info:
            return "[Info ] ";

        case LogLevel::Debug:
            return "[Debug]";

        case LogLevel::Trace:
            return "[Trace]";
    }

    return "";
}
