#if !defined(QUETZAL_MATH_FLOATING_POINT_HPP)
#define QUETZAL_MATH_FLOATING_POINT_HPP
//------------------------------------------------------------------------------
// math
// std::floating_point.hpp
//------------------------------------------------------------------------------

// originally based on cppreference.com epsilon example
// updated with Boost implementation of Knuth algorithm

#include <concepts>
#include <limits>
#include <cmath>

namespace quetzal::math
{

    constexpr int ulpDefault = 64;

    template<typename T> requires std::floating_point<T>
    bool float_eq(T lhs, T rhs, int ulp = ulpDefault);

    template<typename T> requires std::floating_point<T>
    bool float_ne(T lhs, T rhs, int ulp = ulpDefault);

    template<typename T> requires std::floating_point<T>
    bool float_lt(T lhs, T rhs, int ulp = ulpDefault);

    template<typename T> requires std::floating_point<T>
    bool float_le(T lhs, T rhs, int ulp = ulpDefault);

    template<typename T> requires std::floating_point<T>
    bool float_gt(T lhs, T rhs, int ulp = ulpDefault);

    template<typename T> requires std::floating_point<T>
    bool float_ge(T lhs, T rhs, int ulp = ulpDefault);

    template<typename T> requires std::floating_point<T>
    bool float_eq0(T t, int ulp = ulpDefault);

    template<typename T> requires std::floating_point<T>
    bool float_ne0(T t, int ulp = ulpDefault);

    template<typename T> requires std::floating_point<T>
    bool float_lt0(T t, int ulp = ulpDefault);

    template<typename T> requires std::floating_point<T>
    bool float_le0(T t, int ulp = ulpDefault);

    template<typename T> requires std::floating_point<T>
    bool float_gt0(T t, int ulp = ulpDefault);

    template<typename T> requires std::floating_point<T>
    bool float_ge0(T t, int ulp = ulpDefault);

    template<typename T> requires std::floating_point<T>
    bool float_clamped(T t, T tMin, T tMax, int ulp = ulpDefault);

    template<typename T> requires std::floating_point<T>
    bool float_clamped01(T t, int ulp = ulpDefault);

    template<typename T> requires std::floating_point<T>
    int float_sign(T t, int ulp = ulpDefault);

    template<typename T> requires std::floating_point<T>
    T round_zero(T t, int ulp = ulpDefault);

} // namespace quetzal::math

//------------------------------------------------------------------------------
template<typename T> requires std::floating_point<T>
bool quetzal::math::float_eq(T lhs, T rhs, int ulp)
{
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    // unless the result is subnormal
//    return std::abs(lhs - rhs) <= std::numeric_limits<T>::epsilon() * std::abs(lhs + rhs) * ulp
//           || std::abs(lhs - rhs) < std::numeric_limits<T>::min();

    if (float_eq0(lhs, ulp) || float_eq0(rhs, ulp))
    {
        return float_eq0(lhs - rhs, ulp);
    }

    if ((lhs < 0) != (rhs < 0))
    {
        return false;
    }

    T diff = std::abs(lhs - rhs);
    T elp = std::numeric_limits<T>::epsilon() * ulp;
    bool bMultiply = diff <= abs(lhs) * elp || diff <= abs(rhs) * elp;

//    bool bDivide = diff / abs(lhs) <= elp || diff / abs(rhs) <= elp; // ...
//    assert (bDivide == bMultiply); // ...

    return bMultiply;
}

//------------------------------------------------------------------------------
template<typename T> requires std::floating_point<T>
bool quetzal::math::float_ne(T lhs, T rhs, int ulp)
{
    return !float_eq(lhs, rhs, ulp);
}

//------------------------------------------------------------------------------
template<typename T> requires std::floating_point<T>
bool quetzal::math::float_lt(T lhs, T rhs, int ulp)
{
    return lhs <= rhs && !float_eq(lhs, rhs, ulp);
}

//------------------------------------------------------------------------------
template<typename T> requires std::floating_point<T>
bool quetzal::math::float_le(T lhs, T rhs, int ulp)
{
    return lhs <= rhs || float_eq(lhs, rhs, ulp);
}

//------------------------------------------------------------------------------
template<typename T> requires std::floating_point<T>
bool quetzal::math::float_gt(T lhs, T rhs, int ulp)
{
    return lhs > rhs && !float_eq(lhs, rhs, ulp);
}

//------------------------------------------------------------------------------
template<typename T> requires std::floating_point<T>
bool quetzal::math::float_ge(T lhs, T rhs, int ulp)
{
    return lhs >= rhs || float_eq(lhs, rhs, ulp);
}

//------------------------------------------------------------------------------
template<typename T> requires std::floating_point<T>
bool quetzal::math::float_eq0(T t, int ulp)
{
    return abs(t) <= std::numeric_limits<T>::epsilon() * ulp;
}

//------------------------------------------------------------------------------
template<typename T> requires std::floating_point<T>
bool quetzal::math::float_ne0(T t, int ulp)
{
    return !float_eq0(t, ulp);
}

//------------------------------------------------------------------------------
template<typename T> requires std::floating_point<T>
bool quetzal::math::float_lt0(T t, int ulp)
{
    return t < T(0) && !float_eq0(t, ulp);
}

//------------------------------------------------------------------------------
template<typename T> requires std::floating_point<T>
bool quetzal::math::float_le0(T t, int ulp)
{
    return t <= T(0) || float_eq0(t, ulp);
}

//------------------------------------------------------------------------------
template<typename T> requires std::floating_point<T>
bool quetzal::math::float_gt0(T t, int ulp)
{
    return t > T(0) && !float_eq0(t, ulp);
}

//------------------------------------------------------------------------------
template<typename T> requires std::floating_point<T>
bool quetzal::math::float_ge0(T t, int ulp)
{
    return t >= T(0) || float_eq0(t, ulp);
}

//------------------------------------------------------------------------------
template<typename T> requires std::floating_point<T>
bool quetzal::math::float_clamped(T t, T tMin, T tMax, int ulp)
{
    return float_ge(t, tMin, ulp) && float_le(t, tMax, ulp);
}

//------------------------------------------------------------------------------
template<typename T> requires std::floating_point<T>
bool quetzal::math::float_clamped01(T t, int ulp)
{
    return float_ge0(t, ulp) && float_le(t, T(1), ulp);
}

//------------------------------------------------------------------------------
template<typename T> requires std::floating_point<T>
int quetzal::math::float_sign(T t, int ulp)
{
    if (float_eq0(t, ulp))
    {
        return 0;
    }

    return t < 0 ? -1 : 1;
}

//------------------------------------------------------------------------------
template<typename T> requires std::floating_point<T>
T quetzal::math::round_zero(T t, int ulp)
{
    return float_eq0(t, ulp) ? T(0) : t;
}

#endif // QUETZAL_MATH_FLOATING_POINT_HPP
