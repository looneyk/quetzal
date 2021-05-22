#if !defined(QUETZAL_GEOMETRY_LINE_HPP)
#define QUETZAL_GEOMETRY_LINE_HPP
//------------------------------------------------------------------------------
// geometry
// Line.hpp
//------------------------------------------------------------------------------

#include "Point.hpp"
#include "quetzal/math/Vector.hpp"
#include "quetzal/math/floating_point.hpp"
#include <array>
#include <iostream>
#include <cassert>

namespace quetzal::geometry
{

    // Client responsibility to ensure that the direction vector is unit length

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Line
    {
    public:

        using traits_type = Traits;
        using value_type = typename Traits::value_type;
        using vector_type = math::Vector<Traits>;
        using point_type = Point<Traits>;

        Line() = default;
        Line(const point_type& point, const vector_type& direction);
        Line(const Line&) = default;
        Line(Line&&) = default;
        ~Line() = default;

        Line& operator=(const Line&) = default;
        Line& operator=(Line&&) = default;

        point_type point() const;
        void set_point(const point_type& point);

        vector_type direction() const;
        void set_direction(const vector_type& direction);

        point_type point(value_type t) const;

        bool contains(const point_type& point) const;

        Point<Traits> projection(const Point<Traits>& point) const;
        value_type projection_parameter(const Point<Traits>& point) const;

    private:

        point_type m_point;
        vector_type m_direction;
    };

    // Compute the line parameters of the two closest points on lines A and B
    template<typename Traits>
    std::array<typename Traits::value_type, 2> closest_point_parameters(const Line<Traits>& lineA, const Line<Traits>& lineB);

    // For the case where one or both are actually segments which require vector instead of direction ...
    template<typename Traits>
    std::array<typename Traits::value_type, 2> closest_point_parameters(const Point<Traits>& pointA, const math::Vector<Traits>& vectorA, const Point<Traits>& pointB, const math::Vector<Traits>& vectorB);

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, Line<Traits> line);

} // namespace quetzal::geometry

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Line<Traits>::Line(const point_type& point, const vector_type& direction) :
    m_point(point),
    m_direction(direction)
{
    assert(vector_unit(direction));
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Line<Traits>::point_type quetzal::geometry::Line<Traits>::point() const
{
    return m_point;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Line<Traits>::set_point(const point_type& point)
{
    m_point = point;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Line<Traits>::vector_type quetzal::geometry::Line<Traits>::direction() const
{
    return m_direction;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Line<Traits>::set_direction(const vector_type& direction)
{
    assert(vector_unit(direction));

    m_direction = direction;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Line<Traits>::point_type quetzal::geometry::Line<Traits>::point(value_type t) const
{
    return m_point + t * m_direction;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Line<Traits>::contains(const point_type& point) const
{
    return vector_eq(point, projection(point));
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Line<Traits>::point_type quetzal::geometry::Line<Traits>::projection(const Point<Traits>& point) const
{
    return this->point(projection_parameter(point));
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Line<Traits>::value_type quetzal::geometry::Line<Traits>::projection_parameter(const Point<Traits>& point) const
{
    return math::dot(point - m_point, m_direction);
}

//------------------------------------------------------------------------------
template<typename Traits>
std::array<typename Traits::value_type, 2> quetzal::geometry::closest_point_parameters(const Line<Traits>& lineA, const Line<Traits>& lineB)
{
    using T = typename Traits::value_type;
    using V = math::Vector<Traits>;

    V u = lineA.direction();
    V v = lineB.direction();
    V w = lineA.point() - lineB.point();

    T uv = dot(u, v);
    T uw = dot(u, w);
    T vw = dot(v, w);

// with these Line's, uu and vv should be 1 ...

    // In the common case where directions are unit vectors, avoid potential precision problems
    T uu = dot(u, u); // >= 0
    if (math::float_eq(uu, T(1))) uu = T(1);
    T vv = dot(v, v); // >= 0
    if (math::float_eq(vv, T(1))) vv = T(1);

    T denom = uu * vv - uv * uv; // >= 0
    if (math::float_eq0(denom)) // Parallel
    {
        return {T(0), (uv > vv ? uw / uv : vw / vv)}; // Use the largest denominator
    }

    T sn = (uv * vw - vv * uw);
    T sd = denom;
    T s = math::float_eq0(sn) ? T(0) : sn / sd;

    T tn = (uu * vw - uv * uw);
    T td = denom;
    T t = math::float_eq0(tn) ? T(0) : tn / td;

    return {s, t};
}

//------------------------------------------------------------------------------
template<typename Traits>
std::array<typename Traits::value_type, 2> quetzal::geometry::closest_point_parameters(const Point<Traits>& pointA, const math::Vector<Traits>& vectorA, const Point<Traits>& pointB, const math::Vector<Traits>& vectorB)
{
    using T = typename Traits::value_type;
    using V = math::Vector<Traits>;

    V u = vectorA;
    V v = vectorB;
    V w = pointA - pointB;

    T uv = dot(u, v);
    T uw = dot(u, w);
    T vw = dot(v, w);

    // In the common case where directions are unit vectors, avoid potential precision problems
    T uu = dot(u, u); // >= 0
    if (math::float_eq(uu, T(1))) uu = T(1);
    T vv = dot(v, v); // >= 0
    if (math::float_eq(vv, T(1))) vv = T(1);

    T denom = uu * vv - uv * uv; // >= 0
    if (math::float_eq0(denom)) // Parallel
    {
        return {T(0), (uv > vv ? uw / uv : vw / vv)}; // Use the largest denominator
    }

    T sn = (uv * vw - vv * uw);
    T sd = denom;
    T s = math::float_eq0(sn) ? T(0) : sn / sd;

    T tn = (uu * vw - uv * uw);
    T td = denom;
    T t = math::float_eq0(tn) ? T(0) : tn / td;

    return {s, t};
}

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, Line<Traits> line)
{
    os << "[" << line.point() << "], [" << line.direction() << "]";
    return os;
}

#endif // QUETZAL_GEOMETRY_LINE_HPP
