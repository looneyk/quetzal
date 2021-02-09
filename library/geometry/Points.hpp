#if !defined(QUETZAL_GEOMETRY_POINTS_HPP)
#define QUETZAL_GEOMETRY_POINTS_HPP
//------------------------------------------------------------------------------
// geometry
// Point.hpp
//------------------------------------------------------------------------------

#include "Point.hpp"
#include <vector>

namespace quetzal
{

namespace geometry
{

    template<typename Traits>
    using Points = std::vector<Point<Traits>>;

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, const Points<Traits>& points);

} // namespace geometry

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, const Points<Traits>& points)
{
    os << points.size() << ":";
    for (const auto& point : points)
    {
        os << " {" << point << "}";
    }
    os << std::endl;
    return os;
}

#endif // QUETZAL_GEOMETRY_POINTS_HPP
