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

namespace quetzal
{

namespace geometry
{

    // Client responsibility to ensure that the direction vector is unit length

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Line
    {
    public:

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
    // not lines any more, elsewhere? ...
    template<typename Traits>
    std::array<typename Traits::value_type, 2> closest_point_parameters(const Point<Traits>& pointA, const math::Vector<Traits>& vectorA, const Point<Traits>& pointB, const math::Vector<Traits>& vectorB);

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, Line<Traits> line);

} // namespace geometry

} // namespace quetzal

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
    T sd = denom;
    T td = denom;

    if (math::float_eq0(denom)) // Parallel
    {
        return {T(0), (uv > vv ? uw / uv : vw / vv)}; // Use the largest denominator
    }

    T sn = (uv * vw - vv * uw);
    T s = math::float_eq0(sn) ? T(0) : sn / sd;

    T tn = (uu * vw - uv * uw);
    T t = math::float_eq0(sn) ? T(0) : tn / td;

    return {s, t};
}

/*
void DistanceSegments3Console::dist3D_Segment_to_Segment(Vector3<double> const& P0, Vector3<double> const& P1, Vector3<double> const& Q0, Vector3<double> const& Q1, double& sqrDistance, double& s, double& t, Vector3<double> closest[2])
{
    double const SMALL_NUM = 0.00000001;
    Vector3<double>   u = P1 - P0;
    Vector3<double>   v = Q1 - Q0;
    Vector3<double>   w = P0 - Q0;
    double    uu = Dot(u, u);         // always >= 0
    double    uv = Dot(u, v);
    double    vv = Dot(v, v);         // always >= 0
    double    uw = Dot(u, w);
    double    vw = Dot(v, w);
    double    D = uu * vv - uv * uv;        // always >= 0
    double    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
    double    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0

    // compute the line parameters of the two closest points
    if (D < SMALL_NUM) { // the lines are almost parallel
        sN = 0.0;         // force using point P0 on segment S1
        sD = 1.0;         // to prevent possible division by 0.0 later
        tN = vw;
        tD = vv;
    }
    else {                 // get the closest points on the infinite lines
        sN = (uv * vw - vv * uw);
        tN = (uu * vw - uv * uw);
        if (sN < 0.0) {        // sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = vw;
            tD = vv;
        }
        else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
            sN = sD;
            tN = vw + uv;
            tD = vv;
        }
    }

    if (tN < 0.0) {            // tc < 0 => the t=0 edge is visible
        tN = 0.0;
        // recompute sc for this edge
        if (-uw < 0.0)
            sN = 0.0;
        else if (-uw > uu)
            sN = sD;
        else {
            sN = -uw;
            sD = uu;
        }
    }
    else if (tN > tD) {      // tc > 1  => the t=1 edge is visible
        tN = tD;
        // recompute sc for this edge
        if ((-uw + uv) < 0.0)
            sN = 0;
        else if ((-uw + uv) > uu)
            sN = sD;
        else {
            sN = (-uw + uv);
            sD = uu;
        }
    }
    // finally do the division to get sc and tc
    sc = (std::fabs(sN) < SMALL_NUM ? 0.0 : sN / sD);
    tc = (std::fabs(tN) < SMALL_NUM ? 0.0 : tN / tD);

    // get the difference of the two closest points
    s = sc;
    t = tc;
    closest[0] = (1.0 - sc) * P0 + sc * P1;
    closest[1] = (1.0 - tc) * Q0 + tc * Q1;
    Vector3<double> diff = closest[0] - closest[1];
    sqrDistance = Dot(diff, diff);
}
*/

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, Line<Traits> line)
{
    os << "[" << line.point() << "], [" << line.direction() << "]";
    return os;
}

#endif // QUETZAL_GEOMETRY_LINE_HPP
