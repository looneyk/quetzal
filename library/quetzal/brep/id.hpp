#if !defined(QUETZAL_BREP_ID_HPP)
#define QUETZAL_BREP_ID_HPP
//------------------------------------------------------------------------------
// brep
// id.hpp
//------------------------------------------------------------------------------

#include "quetzal/common/id.hpp"
#include <string>
#include <cstdint>

namespace quetzal::brep
{

    template<typename T>
    bool good(id_type id, const T& container);

    template<typename T>
    std::string id_string(id_type id, const T& container);

} // namespace quetzal::brep

//------------------------------------------------------------------------------
template<typename T>
bool quetzal::brep::good(id_type id, const T& container)
{
    return id != nullid && id < container.size() && !container[id].deleted();
}

//------------------------------------------------------------------------------
template<typename T>
std::string quetzal::brep::id_string(id_type id, const T& container)
{
    if (id == nullid)
    {
        return "null";
    }
    else if (!(id < container.size()))
    {
        return std::to_string(id) + " out of range";
    }
    else if (container[id].deleted())
    {
        return std::to_string(id) + " deleted";
    }

    return std::to_string(id);
}

#endif // QUETZAL_BREP_ID_HPP
