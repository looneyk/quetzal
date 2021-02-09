#if !defined(QUETZAL_GEOMETRY_POINT_HPP)
#define QUETZAL_GEOMETRY_POINT_HPP
//------------------------------------------------------------------------------
// geometry
// Point.hpp
//------------------------------------------------------------------------------

#include "math/Vector.hpp"
#include "math/floating_point.hpp"

namespace quetzal
{

namespace geometry
{

    template<typename Traits>
    using Point = math::Vector<Traits>;

    template<typename Traits>
    bool colinear(const Point<Traits>& a, const Point<Traits>& b, const Point<Traits>& c);

} // namespace geometry

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::colinear(const Point<Traits>& a, const Point<Traits>& b, const Point<Traits>& c)
{
    if constexpr (Traits::dimension == 3)
    {
        return math::float_eq0(math::cross(b - a, c - a).norm_squared());
    }
    else
    {
        return math::float_eq0(math::dot_perp(b - a, c - a));
    }
}

#endif // QUETZAL_GEOMETRY_POINT_HPP
