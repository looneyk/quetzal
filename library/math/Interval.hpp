#if !defined(QUETZAL_MATH_INTERVAL_HPP)
#define QUETZAL_MATH_INTERVAL_HPP
//------------------------------------------------------------------------------
// math
// Interval.hpp
//------------------------------------------------------------------------------

// Closed interval
// Does not handle the concept of lower or upper bounds being open/closed

#include "math_util.hpp"

namespace quetzal
{

namespace math
{

    //--------------------------------------------------------------------------
    template<typename T>
    class Interval
    {
    public:

        using value_type = T;

        Interval();
        explicit Interval(const T& tLength);
        Interval(const T& tLower, const T& tUpper);
        Interval(const Interval&) = default;
        virtual ~Interval() = default;

        Interval& operator=(const Interval&) = default;

        T lower() const;
        T upper() const;
        void set_bounds(const T& tLower, const T& tUpper);

        T length() const;
        bool unit_length() const; // Length of Interval == 1
        bool proper() const;
        bool unit() const; // Specifically [0, 1]

        bool contains(T t) const;
        T lerp(T t) const;

    private:

        T m_tLower;
        T m_tUpper;
    };

    template<typename T>
    Interval<T> shift(const Interval<T>& interval, T t);

} // namespace math

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename T>
quetzal::math::Interval<T>::Interval() :
    Interval(T(0))
{
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::math::Interval<T>::Interval(const T& tLength) :
    Interval(T(0), tLength)
{
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::math::Interval<T>::Interval(const T& tLower, const T& tUpper) :
    m_tLower(tLower),
    m_tUpper(tUpper)
{
}

//------------------------------------------------------------------------------
template<typename T>
T quetzal::math::Interval<T>::lower() const
{
    return m_tLower;
}

//------------------------------------------------------------------------------
template<typename T>
T quetzal::math::Interval<T>::upper() const
{
    return m_tUpper;
}

//------------------------------------------------------------------------------
template<typename T>
void quetzal::math::Interval<T>::set_bounds(const T& tLower, const T & tUpper)
{
    m_tLower = tLower;
    m_tUpper = tUpper;
    return;
}

//------------------------------------------------------------------------------
template<typename T>
T quetzal::math::Interval<T>::length() const
{
    return m_tUpper - m_tLower;
}

//------------------------------------------------------------------------------
template<typename T>
bool quetzal::math::Interval<T>::unit_length() const
{
    if constexpr (std::is_floating_point_v<T>)
    {
        return float_eq(length(), T(1));
    }
    else
    {
        return length() == T(1);
    }
}

//------------------------------------------------------------------------------
template<typename T>
bool quetzal::math::Interval<T>::proper() const
{
    if constexpr (std::is_floating_point_v<T>)
    {
        return float_gt(m_tUpper, m_tLower);
    }
    else
    {
        return m_tUpper > m_tLower;
    }
}

//------------------------------------------------------------------------------
template<typename T>
bool quetzal::math::Interval<T>::unit() const
{
    if constexpr (std::is_floating_point_v<T>)
    {
        return float_eq(m_tLower, T(0)) && float_eq(m_tUpper, T(1));
    }
    else
    {
        return m_tLower == T(0) && m_tUpper == T(1);
    }
}

//------------------------------------------------------------------------------
template<typename T>
bool quetzal::math::Interval<T>::contains(T t) const
{
    if constexpr (std::is_floating_point_v<T>)
    {
        return float_ge(t, m_tLower) && float_le(t, m_tUpper);
    }
    else
    {
        return t >= m_tLower && t <= m_tUpper;
    }
}

//------------------------------------------------------------------------------
template<typename T>
T quetzal::math::Interval<T>::lerp(T t) const
{
    return math::lerp(m_tLower, m_tUpper, t);
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::math::Interval<T> quetzal::math::shift(const Interval<T>& interval, T t)
{
    return {interval.lower() + t, interval.upper() + t};
}

#endif // QUETZAL_MATH_INTERVAL_HPP
