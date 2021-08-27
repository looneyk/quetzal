#if !defined(QUETZAL_GEOMETRY_TRIANGLE_UTIL_HPP)
#define QUETZAL_GEOMETRY_TRIANGLE_UTIL_HPP
//------------------------------------------------------------------------------
// geometry
// triangle_util.hpp
//------------------------------------------------------------------------------

#include "quetzal/math/DimensionReducer.hpp"
#include "quetzal/math/Vector.hpp"
#include "Point.hpp"
#include "Segment.hpp"
#include <cmath>

namespace quetzal::geometry
{

    // Returns -1, 0, 1: interior, on, exterior
    // Exterior is defined by the direction of the normal as defined by the winding order
    // This is more accurate than contains for points on the polygon edges
    template<typename V>
    int triangle_compare(const V& a, const V& b, const V& c, const V& point);

    // Point is in the region defined by the triangle
    // For points on triangle edges, containment is not defined
    template<typename V>
    bool triangle_contains(const V& a, const V& b, const V& c, const V& point);

    template<typename V> requires (V::dimension == 2)
    bool triangle_ccw(const V& a, const V& b, const V& c);

    template<typename V> requires (V::dimension == 3)
    bool triangle_ccw(const V& a, const V& b, const V& c, const V& normal);

    template<typename V>
    bool triangle_degenerate(const V& a, const V& b, const V& c);

    template<typename V>
    V triangle_centroid(const V& a, const V& b, const V& c);

    template<typename V> requires (V::dimension == 2)
    typename V::value_type triangle_area(const V& a, const V& b, const V& c);

    template<typename V> requires (V::dimension == 3)
    typename V::value_type triangle_area(const V& a, const V& b, const V& c, const V& normal);

    // Based on implied winding around positive z-axis
    template<typename V> requires (V::dimension == 2)
    typename V::value_type triangle_signed_area(const V& a, const V& b, const V& c);

    template<typename V> requires (V::dimension == 3)
    typename V::value_type triangle_signed_area(const V& a, const V& b, const V& c, const V& normal);

namespace internal
{

    // Check if point is in region defined by triangle
    template<typename V> requires (V::dimension == 2)
    bool triangle_region_contains(const V& a, const V& b, const V& c, const V& point);

} // namespace internal

} // namespace quetzal::geometry

//------------------------------------------------------------------------------
template<typename V>
int quetzal::geometry::triangle_compare(const V& a, const V& b, const V& c, const V& point)
{
    using segment_type = Segment<typename V::traits_type>;
    if (segment_type(a, b).contains(point) || segment_type(b, c).contains(point) || segment_type(c, a).contains(point))
    {
        return 0;
    }

    if (triangle_degenerate(a, b, c))
    {
        return 1;
    }

    if constexpr (V::dimension == 2)
    {
        return internal::triangle_region_contains(a, b, c, point) ? -1 : 1;
    }
    else if constexpr (V::dimension == 3)
    {
        math::DimensionReducer<typename V::traits_type> dr(normalize(cross(b - a, c - a)));
        return internal::triangle_region_contains(dr.reduce(a), dr.reduce(b), dr.reduce(c), dr.reduce(point)) ? -1 : 1;
    }
}

//------------------------------------------------------------------------------
template<typename V>
bool quetzal::geometry::triangle_contains(const V& a, const V& b, const V& c, const V& point)
{
    if (triangle_degenerate(a, b, c))
    {
        return false;
    }

    if constexpr (V::dimension == 2)
    {
        return internal::triangle_region_contains(a, b, c, point);
    }
    else if constexpr (V::dimension == 3)
    {
        V normal = normalize(cross(b - a, c - a));
        Plane<typename V::traits_type> plane(a, normal);
        if (!plane.contains(point))
        {
            return false;
        }

        math::DimensionReducer<typename V::traits_type> dr(normal);
        return internal::triangle_region_contains(dr.reduce(a), dr.reduce(b), dr.reduce(c), dr.reduce(point));
    }
}

//------------------------------------------------------------------------------
template<typename V> requires (V::dimension == 2)
bool quetzal::geometry::triangle_ccw(const V& a, const V& b, const V& c)
{
    return dot(c - a, perp(b - a)) >= 0;
}

//------------------------------------------------------------------------------
template<typename V> requires (V::dimension == 3)
bool quetzal::geometry::triangle_ccw(const V& a, const V& b, const V& c, const V& normal)
{
    return dot(c - a, cross(normal, b - a)) >= 0;
}

//------------------------------------------------------------------------------
template<typename V>
bool quetzal::geometry::triangle_degenerate(const V& a, const V& b, const V& c)
{
    return colinear(a, b, c);
}

//------------------------------------------------------------------------------
template<typename V>
V quetzal::geometry::triangle_centroid(const V& a, const V& b, const V& c)
{
    return (a + b + c) / V::value_type(3);
}

//------------------------------------------------------------------------------
template<typename V> requires (V::dimension == 2)
typename V::value_type quetzal::geometry::triangle_area(const V& a, const V& b, const V& c)
{
    return std::abs(triangle_signed_area(a, b, c));
}

//------------------------------------------------------------------------------
template<typename V> requires (V::dimension == 3)
typename V::value_type quetzal::geometry::triangle_area(const V& a, const V& b, const V& c, const V& normal)
{
    return cross(b - a, c - a).norm() / typename V::value_type(2);
}

//------------------------------------------------------------------------------
template<typename V> requires (V::dimension == 2)
typename V::value_type quetzal::geometry::triangle_signed_area(const V& a, const V& b, const V& c)
{
    V v0 = b - a;
    V v1 = c - a;
    return (v0.x() * v1.y() - v0.y() * v1.x()) / typename V::value_type(2);
}

//------------------------------------------------------------------------------
template<typename V> requires (V::dimension == 3)
typename V::value_type quetzal::geometry::triangle_signed_area(const V& a, const V& b, const V& c, const V& normal)
{
    V cross_product = cross(b - a, c - a);
    typename V::value_type area = cross_product.norm() / typename V::value_type(2);
    return dot(cross_product, normal) >= typename V::value_type(0) ? area : -area;
}

//------------------------------------------------------------------------------
// Area signs implementation
template<typename V> requires (V::dimension == 2)
bool quetzal::geometry::internal::triangle_region_contains(const V& a, const V& b, const V& c, const V& point)
{
    using T = V::value_type;

    T signa = dot(perp(b - a), point - a);
    T signb = dot(perp(c - b), point - b);
    T signc = dot(perp(a - c), point - c);

    return (signa > T(0) && signb > T(0) && signc > T(0)) || (signa < T(0) && signb < T(0) && signc < T(0));
}

/*
//------------------------------------------------------------------------------
// Barycentric implementation
template<typename V> requires (V::dimension == 2)
bool quetzal::geometry::internal::triangle_region_contains(const V& a, const V& b, const V& c, const V& point)
{
    using T = V::value_type;

    V v0 = a;
    V v1 = b - a;
    V v2 = c - a;

    T denomFactor = T(1) / (v1.x() * v2.y() - v1.y() * v2.x());
    T u = ((point.x() - v0.x()) * v2.y() - (point.y() - v0.y()) * v2.x()) * denomFactor;
    T v = ((point.y() - v0.y()) * v1.x() - (point.x() - v0.x()) * v1.y()) * denomFactor;

    return u > 0 && v > 0 && u + v < 1;
}
*/

#endif // QUETZAL_GEOMETRY_TRIANGLE_UTIL_HPP
