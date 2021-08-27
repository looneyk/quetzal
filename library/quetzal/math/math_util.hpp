#if !defined(QUETZAL_MATH_MATH_UTIL_HPP)
#define QUETZAL_MATH_MATH_UTIL_HPP
//------------------------------------------------------------------------------
// math
// math_util.hpp
//
// Angles in radians
//------------------------------------------------------------------------------

#include "floating_point.hpp"
#include <algorithm>
#include <concepts>
#include <cmath>

namespace quetzal::math
{

    template<typename T>
    constexpr T Pi = T(3.1415926535897932384626433832795);

    template<typename T>
    constexpr T PiTwo = T(6.283185307179586476925286766559);

    template<typename T>
    constexpr T PiHalf = T(1.5707963267948966192313216916398);

    template<typename T>
    constexpr T PiThird = T(1.0471975511965977461542144610932);

    template<typename T>
    constexpr T PiFourth = T(0.78539816339744830961566084581988);

    template<typename T>
    constexpr T PiFifth = T(0.6283185307179586476925286766559);

    template<typename T>
    constexpr T PiTwoThird = T(2.0943951023931954923084289221863);

    template<typename T>
    constexpr T GoldenRatio = T(1.6180339887498948482045868343656);

    template<typename T> requires std::integral<T>
	bool even(T t);

    template<typename T> requires std::integral<T>
	bool odd(T t);

    template<typename T>
    int signum(T t);

    template<typename T>
    T clamp0(T t);

    template<typename T>
    T clamp01(T t);

    template<typename T>
    bool clamped(T t, T tMin, T tMax);

    template<typename T>
    bool clamped01(T t);

    template<typename X, typename T>
    X lerp(const X& x0, const X& x1, T t);

    // |arc| == 2 * Pi
    template<typename T>
    bool full_circle(T arc);

    // |phi1 - phi0| == 2 * Pi
    template<typename T>
    bool full_circle(T phi0, T phi1);

    // Angle between a and b
    template<typename V>
    typename V::value_type angle(const V& a, const V& b);

    // Angle between a and b where a and b are unit vectors
    template<typename V>
    typename V::value_type angle_unit(const V& a, const V& b);

    // Angle from from to to around axis
    template<typename V>
    typename V::value_type angle(const V& from, const V& to, const V& axis);

    // Angle from from to to around axis where all vectors are unit vectors
    template<typename V>
    typename V::value_type angle_unit(const V& from, const V& to, const V& axis);

    template<typename V>
    typename V::value_type azimuth_angle(const V& v);

    template<typename V>
    typename V::value_type elevation_angle(const V& v);

    template<typename V> requires (V::dimension == 3)
    V cartesian_to_spherical(const V& v);

    template<typename V> requires (V::dimension == 3)
    V cartesian_to_cylindrical(const V& v);

    template<typename V> requires (V::dimension == 3)
    V spherical_to_cartesian(const V& v);

    template<typename V> requires (V::dimension == 3)
    V cylindrical_to_cartesian(const V& v);

} // namespace quetzal::math

//------------------------------------------------------------------------------
template<typename T> requires std::integral<T>
bool quetzal::math::even(T t)
{
	return (t & 1) == 0;
}

//------------------------------------------------------------------------------
template<typename T> requires std::integral<T>
bool quetzal::math::odd(T t)
{
	return (t & 1) == 1;
}

//------------------------------------------------------------------------------
template<typename T>
int quetzal::math::signum(T t)
{
    return (T(0) < t) - (t < T(0));
}

//------------------------------------------------------------------------------
template<typename T>
T quetzal::math::clamp0(T t)
{
    return std::max(t, T(0));
}

//------------------------------------------------------------------------------
template<typename T>
T quetzal::math::clamp01(T t)
{
    return std::clamp(t, T(0), T(1));
}

//------------------------------------------------------------------------------
template<typename T>
bool quetzal::math::clamped(T t, T tMin, T tMax)
{
    return t >= tMin && t <= tMax;
}

//------------------------------------------------------------------------------
template<typename T>
bool quetzal::math::clamped01(T t)
{
    return t >= T(0) && t <= T(1);
}

//------------------------------------------------------------------------------
template<typename X, typename T>
X quetzal::math::lerp(const X& x0, const X& x1, T t)
{
/*
    if (t == T(1))
    {
        return x1;
    }

    return x0 + (x1 - x0) * t;
*/
    return (T(1) - t) * x0 + t * x1;
}

//------------------------------------------------------------------------------
template<typename T>
bool quetzal::math::full_circle(T arc)
{
    return math::float_eq(abs(arc), PiTwo<T>);
}

//------------------------------------------------------------------------------
template<typename T>
bool quetzal::math::full_circle(T phi0, T phi1)
{
    return full_circle(phi1 - phi0);
}

//------------------------------------------------------------------------------
template<typename V>
typename V::value_type quetzal::math::angle(const V& a, const V& b)
{
    // why not just normalize and use angle_unit? ...
    auto a1 = V::value_type(1) / a.norm();
    auto b1 = V::value_type(1) / b.norm();
    return std::acos(std::clamp(dot(a, b) * (a1 * b1), V::value_type(-1), V::value_type(1)));
}

//------------------------------------------------------------------------------
template<typename V>
typename V::value_type quetzal::math::angle_unit(const V& a, const V& b)
{
    assert(a.unit());
    assert(b.unit());

    return std::acos(std::clamp(dot(a, b), V::value_type(-1), V::value_type(1)));
}

//------------------------------------------------------------------------------
template<typename V>
typename V::value_type quetzal::math::angle(const V& from, const V& to, const V& axis)
{
    typename V::value_type theta = angle(from, to);
    return dot(cross(from, to), axis) < typename V::value_type(0) ? math::PiTwo<typename V::value_type> - theta : theta;
}

//------------------------------------------------------------------------------
template<typename V>
typename V::value_type quetzal::math::angle_unit(const V& from, const V& to, const V& axis)
{
    typename V::value_type theta = angle_unit(from, to);
    return dot(cross(from, to), axis) < typename V::value_type(0) ? math::PiTwo<typename V::value_type> - theta : theta;
}

//------------------------------------------------------------------------------
template<typename V>
typename V::value_type quetzal::math::azimuth_angle(const V& v)
{
    return atan2(v.y(), v.x());
}

//------------------------------------------------------------------------------
template<typename V>
typename V::value_type quetzal::math::elevation_angle(const V& v)
{
    return std::atan2(v.z(), std::sqrt(v.x() * v.x() + v.y() * v.y()));
}

//------------------------------------------------------------------------------
template<typename V> requires (V::dimension == 3)
V quetzal::math::cartesian_to_spherical(const V& v)
{
    return {norm(v), azimuth_angle(v), elevation_angle(v)};
}

//------------------------------------------------------------------------------
template<typename V> requires (V::dimension == 3)
V quetzal::math::cartesian_to_cylindrical(const V& v)
{
    return {norm({v.x(), v.y(), 0}), azimuth_angle(v), v.z()};
}

//------------------------------------------------------------------------------
template<typename V> requires (V::dimension == 3)
V quetzal::math::spherical_to_cartesian(const V& v)
{
    return {v[0] * sin(v[2]) * cos(v[1]), v[0] * sin(v[2]) * sin(v[1]), v[0] * cos(v[2])};
}

//------------------------------------------------------------------------------
template<typename V> requires (V::dimension == 3)
V quetzal::math::cylindrical_to_cartesian(const V& v)
{
    return {v[0] * cos(v[1]), v[0] * sin(v[1]), v[2]};
}

#endif // QUETZAL_MATH_MATH_UTIL_HPP
