#if !defined(QUETZAL_ID_HPP)
#define QUETZAL_ID_HPP
//------------------------------------------------------------------------------
// common
// id.hpp
//------------------------------------------------------------------------------

#include <cstdint>

namespace quetzal
{

    using id_type = size_t;

    constexpr id_type nullid = SIZE_MAX;

} // namespace quetzal

#endif // QUETZAL_ID_HPP
