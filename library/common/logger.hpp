#if !defined(QUETZAL_LOGGER_HPP)
#define QUETZAL_LOGGER_HPP
//------------------------------------------------------------------------------
// logger.hpp
//------------------------------------------------------------------------------

#include "Log.hpp"
#include <fstream>

namespace quetzal
{

    extern Log<std::ofstream> log;

} // namespace quetzal

#endif // QUETZAL_LOGGER_HPP
