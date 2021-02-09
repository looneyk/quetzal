#if !defined(QUETZAL_LOGLEVEL_HPP)
#define QUETZAL_LOGLEVEL_HPP
//------------------------------------------------------------------------------
// LogLevel.hpp
//------------------------------------------------------------------------------

#include <string>

namespace quetzal
{

    //--------------------------------------------------------------------------
    enum class LogLevel
    {
        None,
        Fatal,
        Error,
        Warn,
        Info,
        Debug,
        Trace,
        All
    };

    std::string to_string(LogLevel level);

} // namespace quetzal

#endif // QUETZAL_LOGLEVEL_HPP
