#if !defined(QUETZAL_GEOMETRY_TRIANGLE_UTIL_HPP)
#define QUETZAL_GEOMETRY_TRIANGLE_UTIL_HPP
//------------------------------------------------------------------------------
// geometry
// triangle_util.hpp
//------------------------------------------------------------------------------

#include "quetzal/math/Vector.hpp"
#include "Point.hpp"
#include "Segment.hpp"
#include <cmath>

namespace quetzal
{

namespace geometry
{

    template<typename V>
    bool triangle_contains(const V& a, const V& b, const V& c, const V& point);

    template<typename V, typename = std::enable_if_t<(V::dimension == 2)>>
    bool triangle_ccw(const V& a, const V& b, const V& c);

    template<typename V, typename = std::enable_if_t<(V::dimension == 3)>>
    bool triangle_ccw(const V& a, const V& b, const V& c, const V& normal);

    template<typename V>
    bool triangle_degenerate(const V& a, const V& b, const V& c);

    // Based on implied winding around positive z-axis
    template<typename V, typename = std::enable_if_t<(V::dimension == 2)>>
    typename V::value_type triangle_area(const V& a, const V& b, const V& c);

    template<typename V, typename = std::enable_if_t<(V::dimension == 3)>>
    typename V::value_type triangle_area(const V& a, const V& b, const V& c, const V& normal);

    // Based on implied winding around positive z-axis
    template<typename V, typename = std::enable_if_t<(V::dimension == 2)>>
    typename V::value_type triangle_signed_area(const V& a, const V& b, const V& c);

    template<typename V, typename = std::enable_if_t<(V::dimension == 3)>>
    typename V::value_type triangle_signed_area(const V& a, const V& b, const V& c, const V& normal);

} // namespace geometry

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename V>
bool quetzal::geometry::triangle_contains(const V& a, const V& b, const V& c, const V& point)
{
    if (triangle_degenerate(a, b, c))
    {
        return Segment<typename V::traits_type>(a, b).contains(point) || Segment<typename V::traits_type>(b, c).contains(point);
    }

    using T = typename V::value_type;

    if constexpr (V::dimension == 3)
    {
        V normal = normalize(cross(b - a, c - a)); // except for plane normal, shouldn't need to be unit since only sign of results are being used ...
        Plane<typename V::traits_type> plane(a, normal);
        if (!plane.contains(point))
        {
            return false;
        }

        V pa = cross(normal, b - a);
        V pb = cross(normal, c - b);
        V pc = cross(normal, a - c);

        // common below here ...

        T da = dot(pa, point - a);
        T db = dot(pb, point - b);
        T dc = dot(pc, point - c);

        // floating point rounding is handled by the degenerate case
        T dref = dot(pa, c - a);
        if (dref > 0)
        {
            return da >= T(0) && db >= T(0) && dc >= T(0);
        }
        else
        {
            return da <= T(0) && db <= T(0) && dc <= T(0);
        }
    }
    else if constexpr (V::dimension == 2)
    {
        V pa = perp(b - a);
        V pb = perp(c - b);
        V pc = perp(a - c);

        T da = dot(pa, point - a);
        T db = dot(pb, point - b);
        T dc = dot(pc, point - c);

        // floating point rounding is handled by the degenerate case
        T dref = dot(pa, c - a);
        if (dref > 0)
        {
            return da > T(0) && db > T(0) && dc > T(0);
        }
        else
        {
            return da < T(0) && db < T(0) && dc < T(0);
        }
/*
// http://totologic.blogspot.com/2014/01/accurate-point-in-triangle-test.html
// perp? ...
v1 = <y2 - y1, -x2 + x1>
v2 = <y3 - y2, -x3 + x2>
v3 = <y1 - y3, -x1 + x3>

Then we get the 3 following vectors :

v1' = <x - x1, y - y1>
v2' = <x - x2, y - y2>
v3' = <x - x3, y - y3>

At last, we compute the 3 dot products :
dot1 = v1 . v1' = (y2 - y1)*(x - x1) + (-x2 + x1)*(y - y1)
dot2 = v1 . v2' = (y3 - y2)*(x - x2) + (-x3 + x2)*(y - y2)
dot3 = v3 . v3' = (y1 - y3)*(x - x3) + (-x1 + x3)*(y - y3)

Finally, we can apply the interesting property :

p lies in T if and only if 0 <= dot1 and 0 <= dot2 and 0 <= dot3

Code sample :

function side(x1, y1, x2, y2, x, y:Number):Number
{
 return (y2 - y1)*(x - x1) + (-x2 + x1)*(y - y1);
}

function pointInTriangle(x1, y1, x2, y2, x3, y3, x, y:Number):Boolean
{
 var checkSide1:Boolean = side(x1, y1, x2, y2, x, y) >= 0;
 var checkSide2:Boolean = side(x2, y2, x3, y3, x, y) >= 0;
 var checkSide3:Boolean = side(x3, y3, x1, y1, x, y) >= 0;
 return checkSide1 && checkSide2 && checkSide3;
}


float sign (fPoint p1, fPoint p2, fPoint p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool PointInTriangle (fPoint pt, fPoint v1, fPoint v2, fPoint v3)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(pt, v1, v2);
    d2 = sign(pt, v2, v3);
    d3 = sign(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

https://mathworld.wolfram.com/TriangleInterior.html

old:
    V v0 = c - a;
    V v1 = b - a;

    // Compute dot products
    typename V::value_type dot00 = dot(v0, v0);
    typename V::value_type dot01 = dot(v0, v1);
    typename V::value_type dot11 = dot(v1, v1);

    typename V::value_type denom = dot00 * dot11 - dot01 * dot01;
    if (math::float_eq0(denom))
    {
        return false;
    }

    V v2 = p - a;
    typename V::value_type invDenom = V::traits_type::val(1) / denom;
    typename V::value_type dot02 = dot(v0, v2);
    typename V::value_type dot12 = dot(v1, v2);

    typename V::value_type u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    if (float_lt0(u))
    {
        return false;
    }

    typename V::value_type v = (dot00 * dot12 - dot01 * dot02) * invDenom;
    if (float_lt0(v))
    {
        return false;
    }

    typename V::value_type w = u + v - V::traits_type::val(1);
    if (float_lt0(w))
    {
        return false;
    }

    return true;
*/
    }
    else
    {
        static_assert(false);
    }
}

//------------------------------------------------------------------------------
template<typename V, typename>
bool quetzal::geometry::triangle_ccw(const V& a, const V& b, const V& c)
{
    return dot(c - b, perp(b - a)) >= 0;
}

//------------------------------------------------------------------------------
template<typename V, typename>
bool quetzal::geometry::triangle_ccw(const V& a, const V& b, const V& c, const V& normal)
{
    return dot(c - b, cross(normal, b - a)) >= 0;
}

//------------------------------------------------------------------------------
template<typename V>
bool quetzal::geometry::triangle_degenerate(const V& a, const V& b, const V& c)
{
    return colinear(a, b, c);
}

//------------------------------------------------------------------------------
template<typename V, typename>
typename V::value_type quetzal::geometry::triangle_area(const V& a, const V& b, const V& c)
{
    return std::abs(triangle_signed_area(a, b, c));
}

//------------------------------------------------------------------------------
template<typename V, typename>
typename V::value_type quetzal::geometry::triangle_area(const V& a, const V& b, const V& c, const V& normal)
{
    return std::abs(triangle_signed_area(a, b, c, normal));
}

//------------------------------------------------------------------------------
template<typename V, typename>
typename V::value_type quetzal::geometry::triangle_signed_area(const V& a, const V& b, const V& c)
{
    V v0 = b - a;
    V v1 = c - a;

    typename V::value_type area = std::abs(v0.x() * v1.y() - v0.y() * v1.x()) / typename V::value_type(2);
    return triangle_ccw(a, b, c) ? area : -area;
}

//------------------------------------------------------------------------------
template<typename V, typename>
typename V::value_type quetzal::geometry::triangle_signed_area(const V& a, const V& b, const V& c, const V& normal)
{
    V cross_product = cross(b - a, c - a);
    typename V::value_type area = cross_product.norm() / typename V::value_type(2);
    return dot(cross_product, normal) >= typename V::value_type(0) ? area : -area;
}

#endif // QUETZAL_GEOMETRY_TRIANGLE_UTIL_HPP
