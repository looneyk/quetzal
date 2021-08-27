#if !defined(QUETZAL_MATH_QUATERNION_HPP)
#define QUETZAL_MATH_QUATERNION_HPP
//------------------------------------------------------------------------------
// math
// Quaternion.hpp
//
// Little more than storage functionality at present
// base this off Vector? ...
//
//------------------------------------------------------------------------------

#include "floating_point.hpp"
#include <array>
#include <cassert>

namespace quetzal::math
{

    //--------------------------------------------------------------------------
    template<typename T>
    class Quaternion
    {
    public:

        typedef T value_type;
        static constexpr size_type dimension = 4;
        using rep_type = std::array<value_type, dimension>;

        Quaternion() = default;
        Quaternion(const value_type(&components)[dimension]);
        Quaternion(const rep_type& rep);
        Quaternion(value_type a, value_type b, value_type c, value_type d);
        Quaternion(const Quaternion&) = default;
        Quaternion(Quaternion&&) = default;
        ~Quaternion() = default;

        Quaternion& operator=(const Quaternion&) = default;
        Quaternion& operator=(Quaternion&& other) = default;

        void init(value_type a, value_type b, value_type c, value_type d);

        value_type a() const;
        value_type b() const;
        value_type c() const;
        value_type d() const;

        bool zero() const;

    private:

        rep_type m_rep;
    };

} // namespace quetzal::math

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Quaternion<Traits>::Quaternion(const value_type(&components)[dimension]) :
    m_rep(components)
{
}

//--------------------------------------------------------------------------
template<typename T>
quetzal::math::Quaternion<T>::Quaternion(const rep_type& rep) :
    m_rep{rep}
{
}

//--------------------------------------------------------------------------
template<typename T>
quetzal::math::Quaternion<T>::Quaternion(value_type a, value_type b, value_type c, value_type d) :
    m_rep{a, b, c, d}
{
}

//--------------------------------------------------------------------------
template<typename T>
void quetzal::math::Quaternion<T>::init(value_type a, value_type b, value_type c, value_type d)
{
    m_rep = {a, b, c, d};
    return;
}

//--------------------------------------------------------------------------
template<typename T>
T quetzal::math::Quaternion<T>::a() const
{
    return m_rep[0];
}

//--------------------------------------------------------------------------
template<typename T>
T quetzal::math::Quaternion<T>::b() const
{
    return m_rep[1];
}

//--------------------------------------------------------------------------
template<typename T>
T quetzal::math::Quaternion<T>::c() const
{
    return m_rep[2];
}

//--------------------------------------------------------------------------
template<typename T>
T quetzal::math::Quaternion<T>::d() const
{
    return m_rep[3];
}

//--------------------------------------------------------------------------
template<typename T>
bool quetzal::math::Quaternion<T>::zero() const
{
    if constexpr (std::is_floating_point_v<T>)
    {
        return float_eq0(m_rep[0]) && float_eq0(m_rep[1]) && float_eq0(m_rep[2]) && float_eq0(m_rep[3]);
    }
    else
    {
        return m_rep[0] == T(0) && m_rep[1] == T(0) && m_rep[2] == T(0) && m_rep[3] == T(0);
    }
}

#endif // QUETZAL_MATH_QUATERNION_HPP
