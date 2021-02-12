#if !defined(QUETZAL_BREP_ID_HPP)
#define QUETZAL_BREP_ID_HPP
//------------------------------------------------------------------------------
// brep
// id.hpp
//------------------------------------------------------------------------------

#include "quetzal/common/id.hpp"
#include <string>
#include <cstdint>

namespace quetzal
{

namespace brep
{

    template<typename T>
    bool good(id_type id, const T& container);

    template<typename T>
    std::string print(id_type id, const T& container);

} // namespace brep

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename T>
bool quetzal::brep::good(id_type id, const T& container)
{
    return id != nullid && id < container.size() && !container[id].deleted();
}

//------------------------------------------------------------------------------
template<typename T>
std::string quetzal::brep::print(id_type id, const T& container)
{
    if (id == nullid)
    {
        return "null";
    }
    else if (!(id < container.size()))
    {
        return "out of range";
    }
    else if (container[id].deleted())
    {
        return "deleted";
    }

    return std::to_string(id);
}

#endif // QUETZAL_BREP_ID_HPP
