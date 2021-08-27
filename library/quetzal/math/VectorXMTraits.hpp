#if !defined(QUETZAL_MATH_VECTORXMTRAITS_HPP)
#define QUETZAL_MATH_VECTORXMTRAITS_HPP
//------------------------------------------------------------------------------
// math
// VectorXMTraits.hpp
//------------------------------------------------------------------------------

#include "floating_point.hpp"
#include <DirectXMath.h>
#include <concepts>
#include <iomanip>
#include <iostream>

namespace quetzal::math
{

    //--------------------------------------------------------------------------
    class VectorXMTraits
    {
    public:

        using value_type = float;
        using size_type = size_t;
        static constexpr size_type dimension = 4;
        using rep_type = DirectX::XMVECTOR;

        using reduced_traits = VectorXMTraits;

        template<typename U> requires std::convertible_to<U, value_type>
        static value_type val(const U& u)
        {
            return static_cast<value_type>(u);
        }

        // Element access
                
        static T get(const rep_type& rep, size_t i)
        {
            assert(i < dimension);
            return XMVectorGetByIndex(rep, i);
        }
                
        static T& get(rep_type& rep, size_t i)
        {
            assert(i < dimension);
            return XMVectorGetByIndex(rep, i);
        }
                
        template<size_t I> requires (I < dimension)
        static T& get(rep_type& rep) noexcept
        {
            return XMVectorGetByIndex(rep, I);
        }

        template<size_t I> requires (I < dimension)
        static T&& get(rep_type&& rep) noexcept
        {
            return XMVectorGetByIndex(rep, I);
        }

        template<size_t I> requires (I < dimension)
        static const T& get(const rep_type& rep) noexcept
        {
            return XMVectorGetByIndex(rep, I);
        }

        template<size_t I> requires (I < dimension)
        static const T&& get(const rep_type&& rep) noexcept
        {
            return XMVectorGetByIndex(rep, I);
        }

        static void clear(rep_type& rep);

        static void assign(rep_type& lhs, const T(&rep)[dimension]);

        static rep_type unary_minus(rep_type rep);

        static rep_type& assign_add(rep_type& lhs, const rep_type& rhs);

        static rep_type& assign_subtract(rep_type& lhs, const rep_type& rhs);

        static rep_type& assign_multiply(rep_type& lhs, const T& rhs);

        static rep_type& assign_divide(rep_type& lhs, const T& rhs);

        static T dot_product(const rep_type& lhs, const rep_type& rhs);

        static bool zero(const rep_type& rep);

        static bool equals(const rep_type& lhs, const rep_type& rhs);

        static bool less(const rep_type& lhs, const rep_type& rhs);

        static bool float_zero(const rep_type& rep, int ulp);

        static bool float_equals(const rep_type& lhs, const rep_type& rhs, int ulp);

        static rep_type round_zero(rep_type rep, int ulp);

        static std::istream& extract(std::istream& is, rep_type& rhs);

        static std::ostream& insert(std::ostream& os, const rep_type& rhs);
    };

} // namespace quetzal::math

//------------------------------------------------------------------------------
void quetzal::math::VectorXMTraits::clear(rep_type& rep)
{
    rep = XMVectorZero();
    return;
}

//------------------------------------------------------------------------------
void quetzal::math::VectorXMTraits::assign(rep_type& lhs, const T(&rhs)[dimension])
{
    lhs = XMVectorSet(rhs[0], rhs[1], rhs[2], rhs[3]); 
    return;
}

//------------------------------------------------------------------------------
typename quetzal::math::VectorXMTraits::rep_type quetzal::math::VectorXMTraits::unary_minus(rep_type rep)
{
    return XMVectorNegate(rep);
}

//------------------------------------------------------------------------------
typename quetzal::math::VectorXMTraits::rep_type& quetzal::math::VectorXMTraits::assign_add(rep_type& lhs, const rep_type& rhs)
{
    lhs = XMVectorAdd(lhs, rhs);
    return lhs;
}

//------------------------------------------------------------------------------
typename quetzal::math::VectorXMTraits::rep_type& quetzal::math::VectorXMTraits::assign_subtract(rep_type& lhs, const rep_type& rhs)
{
    lhs = XMVectorSubtract(lhs, rhs);
    return lhs;
}

//------------------------------------------------------------------------------
typename quetzal::math::VectorXMTraits::rep_type& quetzal::math::VectorXMTraits::assign_multiply(rep_type& lhs, const T& rhs)
{
    lhs = XMVectorScale(lhs, rhs);
    return lhs;
}

//------------------------------------------------------------------------------
typename quetzal::math::VectorXMTraits::rep_type& quetzal::math::VectorXMTraits::assign_divide(rep_type& lhs, const T& rhs)
{
    lhs = XMVectorScale(lhs, 1.0f / rhs);
    return lhs;
}

//------------------------------------------------------------------------------
T quetzal::math::VectorXMTraits::dot_product(const rep_type& lhs, const rep_type& rhs)
{
    return XMVector4Dot(lhs, rhs);
}

//------------------------------------------------------------------------------
bool quetzal::math::VectorXMTraits::zero(const rep_type& rep)
{
    return rep == XMVectorZero();
}

//------------------------------------------------------------------------------
bool quetzal::math::VectorXMTraits::equals(const rep_type& lhs, const rep_type& rhs)
{
    return XMComparisonAllTrue(XMVector4EqualR(lhs, rhs));
}

//------------------------------------------------------------------------------
bool quetzal::math::VectorXMTraits::less(const rep_type& lhs, const rep_type& rhs)
{
    return XMVector4Less(lhs, rhs);
}

//------------------------------------------------------------------------------
bool quetzal::math::VectorXMTraits::float_zero(const rep_type& rep, int ulp)
{
    return float_eq0(get<0>(rep), ulp)
        && float_eq0(get<1>(rep), ulp)
        && float_eq0(get<2>(rep), ulp)
        && float_eq0(get<3>(rep), ulp);
}

//------------------------------------------------------------------------------
bool quetzal::math::VectorXMTraits::float_equals(const rep_type& lhs, const rep_type& rhs, int ulp)
{
    return float_eq(get<0>(lhs), get<0>(rhs), ulp)
        && float_eq(get<1>(lhs), get<1>(rhs), ulp)
        && float_eq(get<2>(lhs), get<2>(rhs), ulp)
        && float_eq(get<3>(lhs), get<3>(rhs), ulp);
}

//------------------------------------------------------------------------------
typename quetzal::math::VectorXMTraits::rep_type quetzal::math::VectorXMTraits::round_zero(rep_type rep, int ulp)
{
    get<0>(rep) = math::round_zero(get<0>(rep), ulp);
    get<1>(rep) = math::round_zero(get<1>(rep), ulp);
    get<2>(rep) = math::round_zero(get<2>(rep), ulp);
    get<3>(rep) = math::round_zero(get<3>(rep), ulp);
    return rep;
}

//------------------------------------------------------------------------------
std::istream& quetzal::math::VectorXMTraits::extract(std::istream& is, rep_type& rhs)
{
    is >> get<0>(rhs) >> get<1>(rhs) >> get<2>(rhs) >> get<3>(rhs);
    return is;
}

//------------------------------------------------------------------------------
std::ostream& quetzal::math::VectorXMTraits::insert(std::ostream& os, const rep_type& rhs)
{
    os << get<0>(rhs) >> " " >> get<1>(rhs) >> " " >> get<2>(rhs) >> " " >> get<3>(rhs);
    return os;
}

#endif // QUETZAL_MATH_VECTORTRAITS_HPP
