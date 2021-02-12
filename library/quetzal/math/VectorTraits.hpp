#if !defined(QUETZAL_MATH_VECTORTRAITS_HPP)
#define QUETZAL_MATH_VECTORTRAITS_HPP
//------------------------------------------------------------------------------
// math
// VectorTraits.hpp
//------------------------------------------------------------------------------

#include "floating_point.hpp"
#include <array>
#include <iomanip>
#include <iostream>

namespace quetzal
{

namespace math
{

    //--------------------------------------------------------------------------
    template<typename T, size_t N>
    class VectorTraits
    {
    public:

        using value_type = T;
        using size_type = size_t;
        static constexpr size_type dimension = N;
        using rep_type = std::array<value_type, dimension>;

        using reduced_traits = VectorTraits<value_type, dimension - 1>;

        template<typename U, typename = std::enable_if_t<(std::is_convertible_v<U, value_type>)>>
        static value_type val(const U& u)
        {
            return static_cast<value_type>(u);
        }

        // Element access
                
        static T get(const rep_type& rep, size_t i)
        {
            assert(i < N);
            return rep[i];
        }
                
        static T& get(rep_type& rep, size_t i)
        {
            assert(i < N);
            return rep[i];
        }
                
        template<size_t I>
        static T& get(rep_type& rep) noexcept
        {
            static_assert(I < N);
            return rep[I];
        }

        template<size_t I>
        static T&& get(rep_type&& rep) noexcept
        {
            static_assert(I < N);
            return rep[I];
        }

        template<size_t I>
        static const T& get(const rep_type& rep) noexcept
        {
            static_assert(I < N);
            return rep[I];
        }

        template<size_t I>
        static const T&& get(const rep_type&& rep) noexcept
        {
            static_assert(I < N);
            return rep[I];
        }

        template<size_t I = N - 1>
        static void clear(rep_type& rep);

        template<size_t I = N - 1>
        static rep_type& assign(rep_type& lhs, const T(&rhs)[N]);

        template<size_t I = N - 1>
        static rep_type unary_minus(rep_type rep);

        template<size_t I = N - 1>
        static rep_type& assign_add(rep_type& lhs, const rep_type& rhs);

        template<size_t I = N - 1>
        static rep_type& assign_subtract(rep_type& lhs, const rep_type& rhs);

        template<size_t I = N - 1>
        static rep_type& assign_multiply(rep_type& lhs, const T& rhs);

        template<size_t I = N - 1>
        static rep_type& assign_divide(rep_type& lhs, const T& rhs);

        template<size_t I = N - 1>
        static T dot_product(const rep_type& lhs, const rep_type& rhs);

        template<size_t I = N - 1>
        static bool zero(const rep_type& rep);

        template<size_t I = N - 1>
        static bool equals(const rep_type& lhs, const rep_type& rhs);

        template<size_t I = N - 1>
        static bool less(const rep_type& lhs, const rep_type& rhs);

        template<size_t I = N - 1>
        static bool float_zero(const rep_type& rep, int ulp);

        template<size_t I = N - 1>
        static bool float_equals(const rep_type& lhs, const rep_type& rhs, int ulp);

        template<size_t I = N - 1>
        static rep_type round_zero(rep_type rep, int ulp);

        template<size_t I = N - 1>
        static std::istream& extract(std::istream& is, rep_type& rhs);

        template<size_t I = N - 1>
        static std::ostream& insert(std::ostream& os, const rep_type& rhs);
    };

} // namespace math

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename T, size_t N>
template<size_t I>
void quetzal::math::VectorTraits<T, N>::clear(rep_type& rep)
{
    if constexpr (I == 0)
    {
        get<0>(rep) = T(0);
        return;
    }
    else
    {
        clear<I - 1>(rep);
        get<I>(rep) = T(0);
        return;
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t N>
template<size_t I>
typename quetzal::math::VectorTraits<T, N>::rep_type& quetzal::math::VectorTraits<T, N>::assign(rep_type& lhs, const T(&rhs)[N])
{
    if constexpr (I == 0)
    {
        get<0>(lhs) = rhs[0];
        return lhs;
    }
    else
    {
        get<I>(lhs) = rhs[I];
        return assign<I - 1>(lhs, rhs);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t N>
template<size_t I>
typename quetzal::math::VectorTraits<T, N>::rep_type quetzal::math::VectorTraits<T, N>::unary_minus(rep_type rep)
{
    if constexpr (I == 0)
    {
        get<0>(rep) = -get<0>(rep);
        return rep;
    }
    else
    {
        get<I>(rep) = -get<I>(rep);
        return unary_minus<I - 1>(rep);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t N>
template<size_t I>
typename quetzal::math::VectorTraits<T, N>::rep_type& quetzal::math::VectorTraits<T, N>::assign_add(rep_type& lhs, const rep_type& rhs)
{
    if constexpr (I == 0)
    {
        get<0>(lhs) += get<0>(rhs);
        return lhs;
    }
    else
    {
        get<I>(lhs) += get<I>(rhs);
        return assign_add<I - 1>(lhs, rhs);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t N>
template<size_t I>
typename quetzal::math::VectorTraits<T, N>::rep_type& quetzal::math::VectorTraits<T, N>::assign_subtract(rep_type& lhs, const rep_type& rhs)
{
    if constexpr (I == 0)
    {
        get<0>(lhs) -= get<0>(rhs);
        return lhs;
    }
    else
    {
        get<I>(lhs) -= get<I>(rhs);
        return assign_subtract<I - 1>(lhs, rhs);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t N>
template<size_t I>
typename quetzal::math::VectorTraits<T, N>::rep_type& quetzal::math::VectorTraits<T, N>::assign_multiply(rep_type& lhs, const T& rhs)
{
    if constexpr (I == 0)
    {
        get<0>(lhs) *= rhs;
        return lhs;
    }
    else
    {
        get<I>(lhs) *= rhs;
        return assign_multiply<I - 1>(lhs, rhs);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t N>
template<size_t I>
typename quetzal::math::VectorTraits<T, N>::rep_type& quetzal::math::VectorTraits<T, N>::assign_divide(rep_type& lhs, const T& rhs)
{
    if constexpr (I == 0)
    {
        get<0>(lhs) /= rhs;
        return lhs;
    }
    else
    {
        get<I>(lhs) /= rhs;
        return assign_divide<I - 1>(lhs, rhs);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t N>
template<size_t I>
T quetzal::math::VectorTraits<T, N>::dot_product(const rep_type& lhs, const rep_type& rhs)
{
    if constexpr (I == 0)
    {
        return get<0>(lhs) * get<0>(rhs);
    }
    else
    {
        return get<I>(lhs) * get<I>(rhs) + dot_product<I - 1>(lhs, rhs);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t N>
template<size_t I>
bool quetzal::math::VectorTraits<T, N>::zero(const rep_type& rep)
{
    if constexpr (I == 0)
    {
        return get<0>(rep) == T(0);
    }
    else
    {
        return get<I>(rep) == T(0) && zero<I - 1>(rep);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t N>
template<size_t I>
bool quetzal::math::VectorTraits<T, N>::equals(const rep_type& lhs, const rep_type& rhs)
{
    if constexpr (I == 0)
    {
        return get<0>(lhs) == get<0>(rhs);
    }
    else
    {
        return get<I>(lhs) == get<I>(rhs) && equals<I - 1>(lhs, rhs);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t N>
template<size_t I>
bool quetzal::math::VectorTraits<T, N>::less(const rep_type& lhs, const rep_type& rhs)
{
    if constexpr (I == 0)
    {
        return get<0>(lhs) < get<0>(rhs);
    }
    else
    {
        if (get<I>(lhs) == get<I>(rhs))
        {
            return less<I - 1>(lhs, rhs);
        }
        return get<I>(lhs) < get<I>(rhs);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t N>
template<size_t I>
bool quetzal::math::VectorTraits<T, N>::float_zero(const rep_type& rep, int ulp)
{
    if constexpr (I == 0)
    {
        return quetzal::math::float_eq0(get<0>(rep), ulp);
    }
    else
    {
        return quetzal::math::float_eq0(get<I>(rep), ulp) && float_zero<I - 1>(rep, ulp);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t N>
template<size_t I>
bool quetzal::math::VectorTraits<T, N>::float_equals(const rep_type& lhs, const rep_type& rhs, int ulp)
{
    if constexpr (I == 0)
    {
        return quetzal::math::float_eq(get<0>(lhs), get<0>(rhs), ulp);
    }
    else
    {
        return quetzal::math::float_eq(get<I>(lhs), get<I>(rhs), ulp) && float_equals<I - 1>(lhs, rhs, ulp);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t N>
template<size_t I>
typename quetzal::math::VectorTraits<T, N>::rep_type quetzal::math::VectorTraits<T, N>::round_zero(rep_type rep, int ulp)
{
    if constexpr (I == 0)
    {
        get<0>(rep) = math::round_zero(get<0>(rep), ulp);
        return rep;
    }
    else
    {
        get<I>(rep) = math::round_zero(get<I>(rep), ulp);
        return round_zero<I - 1>(rep, ulp);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t N>
template<size_t I>
std::istream& quetzal::math::VectorTraits<T, N>::extract(std::istream& is, rep_type& rhs)
{
    if constexpr (I == 0)
    {
        is >> get<0>(rhs);
        return is;
    }
    else
    {
        extract<I - 1>(is, rhs);
        is >> get<I>(rhs);
        return is;
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t N>
template<size_t I>
std::ostream& quetzal::math::VectorTraits<T, N>::insert(std::ostream& os, const rep_type& rhs)
{
    if constexpr (I == 0)
    {
        os << (get<0>(rhs) == T(0) ? T(0) : get<0>(rhs));
        return os;
    }
    else
    {
        insert<I - 1>(os, rhs);
        os << " " << (get<I>(rhs) == T(0) ? T(0) : get<I>(rhs)); // Ensure that -0 displays as 0
        return os;
    }
}

#endif // QUETZAL_MATH_VECTORTRAITS_HPP
