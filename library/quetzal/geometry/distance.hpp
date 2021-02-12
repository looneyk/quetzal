#if !defined(QUETZAL_GEOMETRY_DISTANCE_HPP)
#define QUETZAL_GEOMETRY_DISTANCE_HPP
//------------------------------------------------------------------------------
// geometry
// distance.hpp
//------------------------------------------------------------------------------

#include "Point.hpp"
#include "Line.hpp"
#include "Plane.hpp"
#include <cmath>

namespace quetzal
{

namespace geometry
{

    template<typename Traits>
    typename Traits::value_type distance(const Point<Traits>& pointA, const Point<Traits>& pointB);

    template<typename Traits>
    typename Traits::value_type distance(const Point<Traits>& point, const Line<Traits>& line);

    template<typename Traits>
    typename Traits::value_type distance(const Point<Traits>& point, const Plane<Traits>& plane);

    template<typename Traits>
    typename Traits::value_type distance(const Line<Traits>& line, const Point<Traits>& point);

    template<typename Traits>
    typename Traits::value_type distance(const Line<Traits>& lineA, const Line<Traits>& lineB);

    template<typename Traits>
    typename Traits::value_type distance(const Line<Traits>& line, const Plane<Traits>& plane);

    template<typename Traits>
    typename Traits::value_type distance(const Plane<Traits>& plane, const Point<Traits>& point);

    template<typename Traits>
    typename Traits::value_type distance(const Plane<Traits>& plane, const Line<Traits>& line);

    template<typename Traits>
    typename Traits::value_type distance(const Plane<Traits>& planeA, const Plane<Traits>& planeB);

} // namespace geometry

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::geometry::distance(const Point<Traits>& pointA, const Point<Traits>& pointB)
{
    return (pointB - pointA).norm();
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::geometry::distance(const Point<Traits>& point, const Line<Traits>& line)
{
    return (point - line.projection(point)).norm();
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::geometry::distance(const Point<Traits>& point, const Plane<Traits>& plane)
{
    return std::abs(dot(plane.normal(), (point - plane.point())));
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::geometry::distance(const Line<Traits>& line, const Point<Traits>& point)
{
    return distance(point, line);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::geometry::distance(const Line<Traits>& lineA, const Line<Traits>& lineB)
{
    if (parallel(lineA.direction(), lineB.direction()))
    {
        return distance(lineA.point(), lineB);
    }

    if constexpr (Traits::dimension == 3)
    {
/*
auto [s, t] = closest_point_parameters(lineA, lineB);
std::cout << "closest s t\t" << s << "\t" << t << "\t" << lineA << "\t" << lineB << std::endl;
auto da = (lineA.point(s) - lineB.point(t)).norm();
auto db = abs(dot(lineB.point() - lineA.point(), normalize(cross(lineA.direction(), lineB.direction()))));
assert(math::float_eq(da, db));
std::cout << "worked" << std::endl;
*/
        return abs(dot(lineB.point() - lineA.point(), normalize(cross(lineA.direction(), lineB.direction()))));
    }
    else if constexpr (Traits::dimension == 2)
    {
        return Traits::val(0);
    }
    else
    {
        static_assert(false, "Dimension out of range.");
    }
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::geometry::distance(const Line<Traits>& line, const Plane<Traits>& plane)
{
    if (perpendicular(line.direction(), plane.normal()))
    {
        return distance(line.point(), plane);
    }

    return Traits::val(0);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::geometry::distance(const Plane<Traits>& plane, const Point<Traits>& point)
{
    return distance(point, plane);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::geometry::distance(const Plane<Traits>& plane, const Line<Traits>& line)
{
    return distance(line, plane);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::geometry::distance(const Plane<Traits>& planeA, const Plane<Traits>& planeB)
{
    if (parallel(planeA.normal(), planeB.normal()))
    {
        return distance(planeA.point(), planeB);
    }

    return Traits::val(0);
}

#endif // QUETZAL_GEOMETRY_DISTANCE_HPP
