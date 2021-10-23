#if !defined(QUETZAL_MATH_VECTOR_HPP)
#define QUETZAL_MATH_VECTOR_HPP
//------------------------------------------------------------------------------
// math
// Vector.hpp
//------------------------------------------------------------------------------

#include "floating_point.hpp"
#include <concepts>
#include <iosfwd>
#include <span>
#include <cmath>
#include <cassert>

namespace quetzal::math
{

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Vector
    {
    public:

        using traits_type = Traits;
        using value_type = Traits::value_type;
        using size_type = Traits::size_type;
        static constexpr size_type dimension = Traits::dimension;
        using rep_type = Traits::rep_type;

        Vector();
        Vector(const rep_type& rep);
        Vector(std::span<value_type> s);

//        template<typename U> requires std::convertible_to<U, typename Traits::value_type>
//        Vector(const U (&components)[dimension]);

        template<typename U> requires (Traits::dimension == 1) && std::convertible_to<U, typename Traits::value_type>
        Vector(const U& x);

        template<typename U> requires (Traits::dimension == 2) && std::convertible_to<U, typename Traits::value_type>
        Vector(const U& x, const U& y);

        template<typename U> requires (Traits::dimension == 3) && std::convertible_to<U, typename Traits::value_type>
        Vector(const U& x, const U& y, const U& z);

        template<typename U> requires (Traits::dimension == 4) && std::convertible_to<U, typename Traits::value_type>
        Vector(const U& x, const U& y, const U& z, const U& w);

        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) = default;
        Vector& operator=(std::span<value_type> s);
//        Vector& operator=(const value_type(&components)[dimension]);

        Vector operator+() const;
        Vector operator-() const;
        Vector& operator+=(const Vector& other);
        Vector& operator-=(const Vector& other);
        Vector& operator*=(value_type f);
        Vector& operator/=(value_type f);

        value_type operator[](size_t i) const;
        value_type& operator[](size_t i);

        value_type x() const requires (Traits::dimension >= 1);
        value_type y() const requires (Traits::dimension >= 2);
        value_type z() const requires (Traits::dimension >= 3);
        value_type w() const requires (Traits::dimension >= 4);

        value_type& x() requires (Traits::dimension >= 1);
        value_type& y() requires (Traits::dimension >= 2);
        value_type& z() requires (Traits::dimension >= 3);
        value_type& w() requires (Traits::dimension >= 4);

        template<typename U> requires (Traits::dimension >= 1) && std::convertible_to<U, typename Traits::value_type>
        void set_x(const U& x);

        template<typename U> requires (Traits::dimension >= 2) && std::convertible_to<U, typename Traits::value_type>
        void set_y(const U& y);

        template<typename U> requires (Traits::dimension >= 3) && std::convertible_to<U, typename Traits::value_type>
        void set_z(const U& z);

        template<typename U> requires (Traits::dimension >= 4) && std::convertible_to<U, typename Traits::value_type>
        void set_w(const U& w);

        template<typename U> requires (Traits::dimension == 1) && std::convertible_to<U, typename Traits::value_type>
        void set(const U& x);

        template<typename U> requires (Traits::dimension == 2) && std::convertible_to<U, typename Traits::value_type>
        void set(const U& x, const U& y);

        template<typename U> requires (Traits::dimension == 3) && std::convertible_to<U, typename Traits::value_type>
        void set(const U& x, const U& y, const U& z);

        template<typename U> requires (Traits::dimension == 4) && std::convertible_to<U, typename Traits::value_type>
        void set(const U& x, const U& y, const U& z, const U& w);
                
        template<size_t I> requires (I < Traits::dimension)
        value_type get() const;

        template<size_t I> requires (I < Traits::dimension)
        value_type& get();

        template<size_t I, typename U> requires (I < Traits::dimension) && std::convertible_to<U, typename Traits::value_type>
        void set(const U& u);

        value_type norm() const;
        value_type norm_squared() const;

        void normalize();
        void round_zero(int ulp = ulpDefault);
        void clear();

        bool unit(int ulp = ulpDefault) const;
        bool float_zero(int ulp = ulpDefault) const;
        bool zero() const;

        // rep may not have data(), traits for data extraction? Traits::data(rep) ...
        // either a consistent raw form needs to be defined, or rep can only be used in the traits context ...
        /*
        const value_type* data() const;
        value_type* data();
        */

        // probably don't even expose rep here ...
        const rep_type& rep() const;
        rep_type& rep();

        // min and max are component-wise
        static Vector min();
        static Vector max();

    private:

        rep_type m_rep;
    };

    template<typename Traits>
    Vector<Traits> operator+(Vector<Traits> lhs, const Vector<Traits>& rhs);

    template<typename Traits>
    Vector<Traits> operator-(Vector<Traits> lhs, const Vector<Traits>& rhs);

    template<typename Traits>
    Vector<Traits> operator*(Vector<Traits> lhs, const typename Traits::value_type& rhs);

    template<typename Traits>
    Vector<Traits> operator*(const typename Traits::value_type& lhs, Vector<Traits> rhs);

    template<typename Traits>
    Vector<Traits> operator/(Vector<Traits> lhs, const typename Traits::value_type& rhs);

    template<typename Traits>
    bool operator==(const Vector<Traits>& lhs, const Vector<Traits>& rhs);

    template<typename Traits>
    bool operator!=(const Vector<Traits>& lhs, const Vector<Traits>& rhs);

    template<typename Traits>
    bool operator<(const Vector<Traits>& lhs, const Vector<Traits>& rhs);

    template<typename Traits>
    bool operator<=(const Vector<Traits>& lhs, const Vector<Traits>& rhs);

    template<typename Traits>
    bool operator>(const Vector<Traits>& lhs, const Vector<Traits>& rhs);

    template<typename Traits>
    bool operator>=(const Vector<Traits>& lhs, const Vector<Traits>& rhs);

    template<typename Traits>
    typename Traits::value_type dot(const Vector<Traits>& lhs, const Vector<Traits>& rhs);

    template<typename Traits> requires (Traits::dimension == 3)
    Vector<Traits> cross(const Vector<Traits>& lhs, const Vector<Traits>& rhs);

    template<typename Traits> requires (Traits::dimension == 2)
    Vector<Traits> perp(const Vector<Traits>& v);

    // dot(perp(lhs), b)
    template<typename Traits> requires (Traits::dimension == 2)
    typename Traits::value_type dot_perp(const Vector<Traits>& lhs, const Vector<Traits>& rhs);

    template<typename Traits>
    Vector<Traits> lerp(const Vector<Traits>& a, const Vector<Traits>& b, typename Traits::value_type t);

    template<typename Traits>
    Vector<Traits> normalize(const Vector<Traits>& v);

    template<typename Traits>
    bool parallel_unit(const Vector<Traits>& lhs, const Vector<Traits>& rhs);

    template<typename Traits>
    bool parallel(const Vector<Traits>& lhs, const Vector<Traits>& rhs);

    template<typename Traits>
    bool perpendicular(const Vector<Traits>& lhs, const Vector<Traits>& rhs);

    template<typename Traits>
    bool vector_eq(const Vector<Traits>& lhs, const Vector<Traits>& rhs, int ulp = ulpDefault);

    template<typename Traits>
    bool vector_eq0(const Vector<Traits>& v, int ulp = ulpDefault);

    template<typename Traits>
    bool vector_unit(const Vector<Traits>& v, int ulp = ulpDefault);

    template<typename Traits>
    Vector<Traits> round_zero(Vector<Traits> v, int ulp = ulpDefault);

    // min and max are component-wise

    template<typename Traits>
    Vector<Traits> min(const Vector<Traits>& lhs, const Vector<Traits>& rhs);

    template<typename Traits>
    Vector<Traits> max(const Vector<Traits>& lhs, const Vector<Traits>& rhs);

    template<typename Traits>
    std::istream& operator>>(std::istream& is, Vector<Traits>& v);

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, const Vector<Traits>& v);

} // namespace quetzal::math

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits>::Vector() :
    m_rep()
{
    clear();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits>::Vector(const rep_type& rep) :
    m_rep(rep)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits>::Vector(std::span<value_type> s) :
    m_rep()
{
    Traits::assign(m_rep, s);
}
/*
//------------------------------------------------------------------------------
template<typename Traits>
template<typename U> requires std::convertible_to<U, typename Traits::value_type>
quetzal::math::Vector<Traits>::Vector(const U (&components)[dimension]) :
    m_rep()
{
    Traits::assign(m_rep, components);
}
*/
//------------------------------------------------------------------------------
template<typename Traits>
template<typename U> requires (Traits::dimension == 1) && std::convertible_to<U, typename Traits::value_type>
quetzal::math::Vector<Traits>::Vector(const U& x) :
    m_rep()
{
    Traits::assign(m_rep, {value_type(x)});
}

//------------------------------------------------------------------------------
template<typename Traits>
template<typename U> requires (Traits::dimension == 2) && std::convertible_to<U, typename Traits::value_type>
quetzal::math::Vector<Traits>::Vector(const U& x, const U& y) :
    m_rep()
{
    Traits::assign(m_rep, {value_type(x), value_type(y)});
}

//------------------------------------------------------------------------------
template<typename Traits>
template<typename U> requires (Traits::dimension == 3) && std::convertible_to<U, typename Traits::value_type>
quetzal::math::Vector<Traits>::Vector(const U& x, const U& y, const U& z) :
    m_rep()
{
    Traits::assign(m_rep, {value_type(x), value_type(y), value_type(z)});
}

//------------------------------------------------------------------------------
template<typename Traits>
template<typename U> requires (Traits::dimension == 4) && std::convertible_to<U, typename Traits::value_type>
quetzal::math::Vector<Traits>::Vector(const U& x, const U& y, const U& z, const U& w) :
    m_rep()
{
    Traits::assign(m_rep, {value_type(x), value_type(y), value_type(z), value_type(w)});
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits>& quetzal::math::Vector<Traits>::operator=(std::span<value_type> s)
{
    Traits::assign(m_rep, s);
    return *this;
}
/*
//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits>& quetzal::math::Vector<Traits>::operator=(const value_type(&components)[dimension])
{
    Traits::assign(m_rep, components);
    return *this;
}
*/
//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits> quetzal::math::Vector<Traits>::operator+() const
{
    return *this;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits> quetzal::math::Vector<Traits>::operator-() const
{
    return Traits::unary_minus(m_rep);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits>& quetzal::math::Vector<Traits>::operator+=(const Vector& other)
{
    m_rep = Traits::assign_add(m_rep, other.m_rep);
    return *this;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits>& quetzal::math::Vector<Traits>::operator-=(const Vector& other)
{
    m_rep = Traits::assign_subtract(m_rep, other.m_rep);
    return *this;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits>& quetzal::math::Vector<Traits>::operator*=(value_type f)
{
    m_rep = Traits::assign_multiply(m_rep, f);
    return *this;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits>& quetzal::math::Vector<Traits>::operator/=(value_type f)
{
    m_rep = Traits::assign_divide(m_rep, f);
    return *this;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::math::Vector<Traits>::operator[](size_t i) const
{
    assert(i < dimension);
    return Traits::get(m_rep, i);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type& quetzal::math::Vector<Traits>::operator[](size_t i)
{
    assert(i < dimension);
    return Traits::get(m_rep, i);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::math::Vector<Traits>::x() const requires (Traits::dimension >= 1)
{
    return Traits::get<0>(m_rep);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::math::Vector<Traits>::y() const requires (Traits::dimension >= 2)
{
    return Traits::get<1>(m_rep);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::math::Vector<Traits>::z() const requires (Traits::dimension >= 3)
{
    return Traits::get<2>(m_rep);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::math::Vector<Traits>::w() const requires (Traits::dimension >= 4)
{
    return Traits::get<3>(m_rep);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type& quetzal::math::Vector<Traits>::x() requires (Traits::dimension >= 1)
{
    return Traits::get<0>(m_rep);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type& quetzal::math::Vector<Traits>::y() requires (Traits::dimension >= 2)
{
    return Traits::get<1>(m_rep);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type& quetzal::math::Vector<Traits>::z() requires (Traits::dimension >= 3)
{
    return Traits::get<2>(m_rep);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type& quetzal::math::Vector<Traits>::w() requires (Traits::dimension >= 4)
{
    return Traits::get<3>(m_rep);
}

//------------------------------------------------------------------------------
template<typename Traits>
template<typename U> requires (Traits::dimension >= 1) && std::convertible_to<U, typename Traits::value_type>
void quetzal::math::Vector<Traits>::set_x(const U& x)
{
    Traits::get<0>(m_rep) = value_type(x);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
template<typename U> requires (Traits::dimension >= 2) && std::convertible_to<U, typename Traits::value_type>
void quetzal::math::Vector<Traits>::set_y(const U& y)
{
    Traits::get<1>(m_rep) = value_type(y);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
template<typename U> requires (Traits::dimension >= 3) && std::convertible_to<U, typename Traits::value_type>
void quetzal::math::Vector<Traits>::set_z(const U& z)
{
    Traits::get<2>(m_rep) = value_type(z);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
template<typename U> requires (Traits::dimension >= 4) && std::convertible_to<U, typename Traits::value_type>
void quetzal::math::Vector<Traits>::set_w(const U& w)
{
    Traits::get<3>(m_rep) = value_type(w);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
template<typename U> requires (Traits::dimension == 1) && std::convertible_to<U, typename Traits::value_type>
void quetzal::math::Vector<Traits>::set(const U& x)
{
    Traits::assign(m_rep, {value_type(x)});
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
template<typename U> requires (Traits::dimension == 2) && std::convertible_to<U, typename Traits::value_type>
void quetzal::math::Vector<Traits>::set(const U& x, const U& y)
{
    Traits::assign(m_rep, {value_type(x), value_type(y)});
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
template<typename U> requires (Traits::dimension == 3) && std::convertible_to<U, typename Traits::value_type>
void quetzal::math::Vector<Traits>::set(const U& x, const U& y, const U& z)
{
    Traits::assign(m_rep, {value_type(x), value_type(y), value_type(z)});
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
template<typename U> requires (Traits::dimension == 4) && std::convertible_to<U, typename Traits::value_type>
void quetzal::math::Vector<Traits>::set(const U& x, const U& y, const U& z, const U& w)
{
    Traits::assign(m_rep, {value_type(x), value_type(y), value_type(z), value_type(w)});
    return;
}
                
//------------------------------------------------------------------------------
template<typename Traits>
template<size_t I> requires (I < Traits::dimension)
typename Traits::value_type quetzal::math::Vector<Traits>::get() const
{
    return Traits::get<I>(m_rep);
}

//------------------------------------------------------------------------------
template<typename Traits>
template<size_t I> requires (I < Traits::dimension)
typename Traits::value_type& quetzal::math::Vector<Traits>::get()
{
    return Traits::get<I>(m_rep);
}

//------------------------------------------------------------------------------
template<typename Traits>
template<size_t I, typename U> requires (I < Traits::dimension) && std::convertible_to<U, typename Traits::value_type>
void quetzal::math::Vector<Traits>::set(const U& u)
{
    Traits::get<I>(m_rep) = value_type(u);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::math::Vector<Traits>::norm() const
{
    return std::sqrt(norm_squared());
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::math::Vector<Traits>::norm_squared() const
{
    return Traits::dot_product(this->rep(), this->rep());
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::math::Vector<Traits>::normalize()
{
    if (!float_zero())
    {
        *this /= norm();
    }
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::math::Vector<Traits>::round_zero(int ulp)
{
    m_rep = Traits::round_zero(m_rep, ulp);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::math::Vector<Traits>::clear()
{
    Traits::clear(m_rep);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::math::Vector<Traits>::unit(int ulp) const
{
    return float_eq(norm_squared(), Traits::val(1), ulp);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::math::Vector<Traits>::float_zero(int ulp) const
{
    return Traits::float_zero(m_rep, ulp);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::math::Vector<Traits>::zero() const
{
    return Traits::zero(m_rep);
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename Traits::rep_type& quetzal::math::Vector<Traits>::rep() const
{
    return m_rep;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::rep_type& quetzal::math::Vector<Traits>::rep()
{
    return m_rep;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits> quetzal::math::Vector<Traits>::min()
{
    Vector<Traits> v;
    Traits::set_min(v.rep());
    return v;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits> quetzal::math::Vector<Traits>::max()
{
    Vector<Traits> v;
    Traits::set_max(v.rep());
    return v;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits> quetzal::math::operator+(Vector<Traits> lhs, const Vector<Traits>& rhs)
{
    return lhs += rhs;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits> quetzal::math::operator-(Vector<Traits> lhs, const Vector<Traits>& rhs)
{
    return lhs -= rhs;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits> quetzal::math::operator*(Vector<Traits> lhs, const typename Traits::value_type& rhs)
{
    return lhs *= rhs;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits> quetzal::math::operator*(const typename Traits::value_type& lhs, Vector<Traits> rhs)
{
    return rhs *= lhs;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits> quetzal::math::operator/(Vector<Traits> lhs, const typename Traits::value_type& rhs)
{
    return lhs /= rhs;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::math::operator==(const Vector<Traits>& lhs, const Vector<Traits>& rhs)
{
    return Traits::equals(lhs.rep(), rhs.rep());
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::math::operator!=(const Vector<Traits>& lhs, const Vector<Traits>& rhs)
{
    return !(lhs == rhs);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::math::operator<(const Vector<Traits>& lhs, const Vector<Traits>& rhs)
{
    return Traits::less(lhs.rep(), rhs.rep());
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::math::operator<=(const Vector<Traits>& lhs, const Vector<Traits>& rhs)
{
    return !(rhs < lhs);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::math::operator>(const Vector<Traits>& lhs, const Vector<Traits>& rhs)
{
    return rhs < lhs;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::math::operator>=(const Vector<Traits>& lhs, const Vector<Traits>& rhs)
{
    return !(lhs < rhs);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::math::dot(const Vector<Traits>& lhs, const Vector<Traits>& rhs)
{
    return Traits::dot_product(lhs.rep(), rhs.rep());
}

//------------------------------------------------------------------------------
template<typename Traits> requires (Traits::dimension == 3)
quetzal::math::Vector<Traits> quetzal::math::cross(const Vector<Traits>& lhs, const Vector<Traits>& rhs)
{
    return {lhs.y() * rhs.z() - lhs.z() * rhs.y(), lhs.z() * rhs.x() - lhs.x() * rhs.z(), lhs.x() * rhs.y() - lhs.y() * rhs.x()};
}

//------------------------------------------------------------------------------
template<typename Traits> requires (Traits::dimension == 2)
quetzal::math::Vector<Traits> quetzal::math::perp(const Vector<Traits>& v)
{
    return {-v.y(), v.x()};
}

//------------------------------------------------------------------------------
template<typename Traits> requires (Traits::dimension == 2)
typename Traits::value_type quetzal::math::dot_perp(const Vector<Traits>& lhs, const Vector<Traits>& rhs)
{
    return lhs.y() * rhs.x() - lhs.x() * rhs.y();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits> quetzal::math::lerp(const Vector<Traits>& a, const Vector<Traits>& b, typename Traits::value_type t)
{
    if (t == Traits::val(1))
    {
        return b;
    }

    return a + t * (b - a);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits> quetzal::math::normalize(const Vector<Traits>& v)
{
    return v / v.norm();
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::math::parallel_unit(const Vector<Traits>& lhs, const Vector<Traits>& rhs)
{
    assert(lhs.unit());
    assert(rhs.unit());

    return vector_eq(lhs, rhs) || vector_eq(lhs, -rhs);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::math::parallel(const Vector<Traits>& lhs, const Vector<Traits>& rhs)
{
    return vector_eq0(cross(lhs, rhs));
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::math::perpendicular(const Vector<Traits>& lhs, const Vector<Traits>& rhs)
{
    return float_eq0(dot(lhs, rhs));
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::math::vector_eq(const Vector<Traits>& lhs, const Vector<Traits>& rhs, int ulp)
{
    return Traits::float_equals(lhs.rep(), rhs.rep(), ulp);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::math::vector_eq0(const Vector<Traits>& v, int ulp)
{
    return v.float_zero(ulp);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::math::vector_unit(const Vector<Traits>& v, int ulp)
{
    return v.unit(ulp);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits> quetzal::math::round_zero(Vector<Traits> v, int ulp)
{
    return Vector<Traits>(Traits::round_zero(v.rep(), ulp));
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits> quetzal::math::min(const Vector<Traits>& lhs, const Vector<Traits>& rhs)
{
    return Vector<Traits>(Traits::min(lhs.rep(), rhs.rep()));
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<Traits> quetzal::math::max(const Vector<Traits>& lhs, const Vector<Traits>& rhs)
{
    return Vector<Traits>(Traits::max(lhs.rep(), rhs.rep()));
}

//------------------------------------------------------------------------------
template<typename Traits>
std::istream& quetzal::math::operator>>(std::istream& is, quetzal::math::Vector<Traits>& v)
{
    return Traits::extract(is, v.rep());
}

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::math::operator<<(std::ostream& os, const quetzal::math::Vector<Traits>& v)
{
    return Traits::insert(os, v.rep());
}

#endif // QUETZAL_MATH_VECTOR_HPP
