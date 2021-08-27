#if !defined(QUETZAL_MATH_MATRIXTRAITS_HPP)
#define QUETZAL_MATH_MATRIXTRAITS_HPP
//------------------------------------------------------------------------------
// math
// MatrixTraits.hpp
//------------------------------------------------------------------------------

#include "Vector.hpp"
#include "VectorTraits.hpp"
#include "floating_point.hpp"
#include <array>
#include <concepts>
#include <iomanip>
#include <iostream>

namespace quetzal::math
{

    //--------------------------------------------------------------------------
    template<typename T, size_t M, size_t N>
    class MatrixTraits
    {
    public:

        using value_type = T;
        using size_type = size_t;
        static constexpr size_type rows = M;
        static constexpr size_type cols = N;
        using row_traits = VectorTraits<T, N>
        using col_traits = VectorTraits<T, M>
        using row_type = Vector<row_traits>;
        using col_type = Vector<col_traits>;
        using rep_type = std::array<row_type, cols>;

        using transpose_traits = MatrixTraits<value_type, cols, rows>;

        template<typename P>
        using multiplicand_traits = MatrixTraits<value_type, cols, P>;

        template<typename P>
        using product_traits = MatrixTraits<value_type, rows, P>;

        using reduced_traits = MatrixTraits<value_type, rows - 1, cols - 1>;
        using row_reduced_traits = VectorTraits<T, cols - 1>;
        using col_reduced_traits = VectorTraits<T, rows - 1>;
        using row_reduced_type = Vector<row_reduced_traits>;
        using col_reduced_type = Vector<col_reduced_traits>;

        template<typename U> requires std::convertible_to<U, value_type>
        static value_type val(const U& u)
        {
            return static_cast<value_type>(u);
        }

        // Row and element access
                
        static row_type get_row(const rep_type& rep, size_t i)
        {
            assert(i < N);
            return rep[i];
        }
                
        static row_type& get_row(rep_type& rep, size_t i)
        {
            assert(i < N);
            return rep[i];
        }
                
        static T get(const rep_type& rep, size_t i, size_t j)
        {
            assert(i < N);
            return rep[i][j];
        }
                
        static T& get(rep_type& rep, size_t i, size_t j)
        {
            assert(i < N);
            return rep[i][j];
        }

        template<size_t I> requires (I < N)
        static row_type& get_row(rep_type& rep) noexcept
        {
            return rep[I];
        }

        template<size_t I> requires (I < N)
        static row_type&& get_row(rep_type&& rep) noexcept
        {
            return rep[I];
        }

        template<size_t I> requires (I < N)
        static row_typeonst T& get_row(const rep_type& rep) noexcept
        {
            return rep[I];
        }

        template<size_t I> requires (I < N)
        static row_typeonst T&& get_row(const rep_type&& rep) noexcept
        {
            return rep[I];
        }

        template<size_t I, size_t J> requires (I < N && J < N)
        static T& get(rep_type& rep) noexcept
        {
            return rep[I][J];
        }

        template<size_t I, size_t J> requires (I < N && J < N)
        static T&& get(rep_type&& rep) noexcept
        {
            return rep[I][J];
        }

        template<size_t I, size_t J> requires (I < N && J < N)
        static const T& get(const rep_type& rep) noexcept
        {
            return rep[I][J];
        }

        template<size_t I, size_t J> requires (I < N && J < N)
        static const T&& get(const rep_type&& rep) noexcept
        {
            return rep[I][J];
        }

// both compile and run time forms ...
// do we always want run time forms? ...
// see how they would be used in Matrix ...

        template<size_t I = M - 1>
        static void clear(rep_type& rep);

        template<size_t I = M * N - 1>
        static void assign(rep_type& lhs, const T(&rhs)[M * N]);

        template<size_t I = M - 1>
        static void assign(rep_type& lhs, const row_type(&rhs)[M]);

        template<size_t I = M - 1>
        static rep_type unary_minus(rep_type rep);

        template<size_t I = M - 1>
        static rep_type& assign_add(rep_type& lhs, const rep_type& rhs);

        template<size_t I = M - 1>
        static rep_type& assign_subtract(rep_type& lhs, const rep_type& rhs);

        template<size_t I = M - 1>
        static rep_type& assign_multiply(rep_type& lhs, const T& rhs);

        template<size_t I = M - 1>
        static rep_type& assign_divide(rep_type& lhs, const T& rhs);

        template<size_t I = M - 1>
        static bool zero(const rep_type& rep);

        template<size_t I = M - 1>
        static bool equals(const rep_type& lhs, const rep_type& rhs);

        template<size_t I = M - 1>
        static bool less(const rep_type& lhs, const rep_type& rhs);

        template<size_t I = M - 1>
        static bool float_zero(const rep_type& rep, int ulp);

        template<size_t I = M - 1>
        static bool float_equals(const rep_type& lhs, const rep_type& rhs, int ulp);

        template<size_t I = M - 1>
        static rep_type round_zero(rep_type rep, int ulp);

        template<size_t I = M - 1>
        static std::istream& extract(std::istream& is, rep_type& rhs);

        template<size_t I = M - 1>
        static std::ostream& insert(std::ostream& os, const rep_type& rhs);
    };

} // namespace quetzal::math

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
template<size_t I>
void quetzal::math::MatrixTraits<T, M, N>::clear(rep_type& rep)
{
    if constexpr (I == 0)
    {
        get_row<0>(rep).clear();
        return;
    }
    else
    {
        clear<I - 1>(rep);
        get_row<I>(rep).clear();
        return;
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
template<size_t I>
void quetzal::math::MatrixTraits<T, M, N>::assign(rep_type& lhs, const T(&rhs)[M * N])
{
    // ...

    if constexpr (I == 0)
    {
        get<0>(lhs) = rhs[0];
        return;
    }
    else
    {
        assign<I - 1>(lhs, rhs);
        get<I>(lhs) = rhs[I];
        return;
    }
}

//------------------------------------------------------------------------------
template<size_t I = M - 1>
template<size_t I>
void assign(rep_type& lhs, const row_type(&rhs)[M])
{
    if constexpr (I == 0)
    {
        get_row<0>(lhs) = rhs[0];
        return;
    }
    else
    {
        assign<I - 1>(lhs, rhs);
        get_row<I>(lhs) = rhs[I];
        return;
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
template<size_t I>
typename quetzal::math::MatrixTraits<T, M, N>::rep_type quetzal::math::MatrixTraits<T, M, N>::unary_minus(rep_type rep)
{
    if constexpr (I == 0)
    {
        get_row<0>(rep) = -get_row<0>(rep);
        return rep;
    }
    else
    {
        get_row<I>(rep) = -get_row<I>(rep);
        return unary_minus<I - 1>(rep);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
template<size_t I>
typename quetzal::math::MatrixTraits<T, M, N>::rep_type& quetzal::math::MatrixTraits<T, M, N>::assign_add(rep_type& lhs, const rep_type& rhs)
{
    if constexpr (I == 0)
    {
        get_row<0>(lhs) += get_row<0>(rhs);
        return lhs;
    }
    else
    {
        get_row<I>(lhs) += get_row<I>(rhs);
        return assign_add<I - 1>(lhs, rhs);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
template<size_t I>
typename quetzal::math::MatrixTraits<T, M, N>::rep_type& quetzal::math::MatrixTraits<T, M, N>::assign_subtract(rep_type& lhs, const rep_type& rhs)
{
    if constexpr (I == 0)
    {
        get_row<0>(lhs) -= get_row<0>(rhs);
        return lhs;
    }
    else
    {
        get_row<I>(lhs) -= get_row<I>(rhs);
        return assign_subtract<I - 1>(lhs, rhs);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
template<size_t I>
typename quetzal::math::MatrixTraits<T, M, N>::rep_type& quetzal::math::MatrixTraits<T, M, N>::assign_multiply(rep_type& lhs, const T& rhs)
{
    if constexpr (I == 0)
    {
        get_row<0>(lhs) *= rhs;
        return lhs;
    }
    else
    {
        get_row<I>(lhs) *= rhs;
        return assign_multiply<I - 1>(lhs, rhs);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
template<size_t I>
typename quetzal::math::MatrixTraits<T, M, N>::rep_type& quetzal::math::MatrixTraits<T, M, N>::assign_divide(rep_type& lhs, const T& rhs)
{
    if constexpr (I == 0)
    {
        get_row<0>(lhs) /= rhs;
        return lhs;
    }
    else
    {
        get_row<I>(lhs) /= rhs;
        return assign_divide<I - 1>(lhs, rhs);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
template<size_t I>
bool quetzal::math::MatrixTraits<T, M, N>::zero(const rep_type& rep)
{
    if constexpr (I == 0)
    {
        return get_row<0>(rep) == T(0);
    }
    else
    {
        return get_row<I>(rep) == T(0) && zero<I - 1>(rep);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
template<size_t I>
bool quetzal::math::MatrixTraits<T, M, N>::equals(const rep_type& lhs, const rep_type& rhs)
{
    if constexpr (I == 0)
    {
        return get_row<0>(lhs) == get_row<0>(rhs);
    }
    else
    {
        return get_row<I>(lhs) == get_row<I>(rhs) && equals<I - 1>(lhs, rhs);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
template<size_t I>
bool quetzal::math::MatrixTraits<T, M, N>::less(const rep_type& lhs, const rep_type& rhs)
{
    if constexpr (I == 0)
    {
        return get_row<0>(lhs) < get_row<0>(rhs);
    }
    else
    {
        if (get_row<I>(lhs) == get_row<I>(rhs))
        {
            return less<I - 1>(lhs, rhs);
        }
        return get_row<I>(lhs) < get_row<I>(rhs);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
template<size_t I>
bool quetzal::math::MatrixTraits<T, M, N>::float_zero(const rep_type& rep, int ulp)
{
    if constexpr (I == 0)
    {
        return quetzal::math::vector_eq0(get_row<0>(rep), ulp);
    }
    else
    {
        return quetzal::math::vector_eq0(get_row<I>(rep), ulp) && float_zero<I - 1>(rep, ulp);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
template<size_t I>
bool quetzal::math::MatrixTraits<T, M, N>::float_equals(const rep_type& lhs, const rep_type& rhs, int ulp)
{
    if constexpr (I == 0)
    {
        return quetzal::math::vector_eq(get_row<0>(lhs), get_row<0>(rhs), ulp);
    }
    else
    {
        return quetzal::math::vector_eq(get_row<I>(lhs), ge_rowt<I>(rhs), ulp) && float_equals<I - 1>(lhs, rhs, ulp);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
template<size_t I>
typename quetzal::math::MatrixTraits<T, M, N>::rep_type quetzal::math::MatrixTraits<T, M, N>::round_zero(rep_type rep, int ulp)
{
    if constexpr (I == 0)
    {
        get<0>(rep) = math::round_zero(get_row<0>(rep), ulp);
        return rep;
    }
    else
    {
        get<I>(rep) = math::round_zero(get_row<I>(rep), ulp);
        return round_zero<I - 1>(rep, ulp);
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
template<size_t I>
std::istream& quetzal::math::MatrixTraits<T, M, N>::extract(std::istream& is, rep_type& rhs)
{
    if constexpr (I == 0)
    {
        is >> get_row<0>(rhs);
        return is;
    }
    else
    {
        extract<I - 1>(is, rhs);
        is >> get_row<I>(rhs);
        return is;
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
template<size_t I>
std::ostream& quetzal::math::MatrixTraits<T, M, N>::insert(std::ostream& os, const rep_type& rhs)
{
    if constexpr (I == 0)
    {
        os << get_row<0>(rhs);
        return os;
    }
    else
    {
        insert<I - 1>(os, rhs);
        os << "\n" << get_row<I>(rhs);
        return os;
    }
}

#endif // QUETZAL_MATH_MATRIXTRAITS_HPP
