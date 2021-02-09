#if !defined(QUETZAL_SYSTEM_PATH_HPP)
#define QUETZAL_SYSTEM_PATH_HPP
//------------------------------------------------------------------------------
// system_path.hpp
//------------------------------------------------------------------------------

#include <string>
#include <shlobj.h>

namespace quetzal
{

    std::string system_path(REFKNOWNFOLDERID rfid);

} // namespace quetzal

#endif // QUETZAL_SYSTEM_PATH_HPP
