#if !defined(QUETZAL_MATH_MATRIX_HPP)
#define QUETZAL_MATH_MATRIX_HPP
//------------------------------------------------------------------------------
// math
// Matrix.hpp
//------------------------------------------------------------------------------

#include "floating_point.hpp"
#include <iosfwd>

namespace quetzal::math
{

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Matrix
    {
    public:

        using traits_type = Traits;
        using value_type = Traits::value_type;
        using size_type = Traits::size_type;
        static constexpr size_t rows = Traits::rows;
        static constexpr size_t cols = Traits::cols;
        using row_type = Traits::row_type;
        using rep_type = Traits::rep_type;

        using transpose_traits = Traits::transpose_traits;

        template<typename P>
        using multiplicand_traits = Traits::multiplicand_traits<P>;

        template<typename P>
        using product_traits = Traits::product_traits<P>;

        using reduced_traits = Traits::reduced_traits;
        using row_reduced_traits = VectorTraits<T, cols - 1>;
        using col_reduced_traits = VectorTraits<T, rows - 1>;
        using row_reduced_type = Vector<row_reduced_traits>;
        using col_reduced_type = Vector<col_reduced_traits>;

        Matrix() = default;
        Matrix(const value_type (&elements)[rows * cols]);
        Matrix(const rep_type& rep);

        Matrix(const row_type& row0) requires (M == 1);

        Matrix(const row_type& row0, const row_type& row1) requires (M == 2);

        Matrix(const row_type& row0, const row_type& row1, const row_type& row2) requires (M == 3);

        Matrix(const row_type& row0, const row_type& row1, const row_type& row2, const row_type& row3) requires (M == 4);

        Matrix(const Matrix&) = default;
        Matrix(Matrix&& other) = default;
        ~Matrix() = default;

        Matrix& operator=(const Matrix& other) = default;
        Matrix& operator=(Matrix&& other) = default;
        Matrix& operator=(const value_type(&elements)[rows * cols]);

        Matrix operator+() const;
        Matrix operator-() const;
        Matrix& operator+=(const Matrix& other);
        Matrix& operator-=(const Matrix& other);
        Matrix& operator*=(const Matrix& other);
        Matrix& operator*=(value_type f);
        Matrix& operator/=(value_type f);

        value_type operator()(size_t i, size_t j) const;
        value_type& operator()(size_t i, size_t j);

        const row_type& row(size_t i) const;
        row_type& row(size_t i);

        template<typename enable_if_t<(dimension == 1)>>
        void set(const row_type& row0);

        template<typename enable_if_t<(dimension == 2)>>
        void set(const row_type& row0, const row_type& row1);

        template<typename enable_if_t<(dimension == 3)>>
        void set(const row_type& row0, const row_type& row1, const row_type& row2);

        template<typename enable_if_t<(dimension == 4)>>
        void set(const row_type& row0, const row_type& row1, const row_type& row2, const row_type& row3);

        value_type determinant() const;
        value_type trace() const;

        Matrix& transpose(); // only valid for square matrices, consider removal ...
        Matrix& inverse();

        void clear();

        Matrix<reduced_traits> submatrix(size_t m, size_t n) const;
        value_type minor(size_t m, size_t n) const;
        value_type cofactor(size_t m, size_t n) const;

        static Matrix identity();

    private:

        rep_type m_rep;
    };

    // Where row_type and col_type are used below, they do not necessarily refer to row and column vectors, but just to vectors with the same size of those directions in the matrix

    template<typename Traits>
    Matrix<Traits> operator+(Matrix<Traits> lhs, const Matrix<Traits>& rhs);

    template<typename Traits>
    Matrix<Traits> operator-(Matrix<Traits> lhs, const Matrix<Traits>& rhs);

    template<typename Traits, size_t P>
    Matrix<product_traits<P>> operator*(const Matrix<Traits>& lhs, const Matrix<multiplicand_traits<P>>& rhs);

    template<typename Traits>
    Matrix<Traits> operator*(Matrix<Traits> lhs, const typename Matrix<Traits>::value_type& rhs);

    template<typename Traits>
    Matrix<Traits> operator*(const typename Matrix<Traits>::value_type& lhs, Matrix<Traits> rhs);

    template<typename Traits>
    Matrix<Traits>::col_type operator*(const Matrix<Traits>& lhs, const Matrix<Traits>::row_type& rhs);

    template<typename Traits>
    Matrix<Traits>::row_type operator*(const Matrix<Traits>::col_type& lhs, const Matrix<Traits>& rhs);

    template<typename Traits>
    Matrix<Traits>::col_reduced_type operator*(const Matrix<Traits>& lhs, const Matrix<Traits>::row_reduced_type& rhs);

    template<typename Traits>
    Matrix<Traits>::row_reduced_type operator*(const Matrix<Traits>::col_reduced_type& lhs, const Matrix<Traits>& rhs);

    template<typename Traits>
    Matrix<Traits> operator/(Matrix<Traits> lhs, const typename Matrix<Traits>::value_type& rhs);

    template<typename Traits>
    bool operator==(const Matrix<Traits>& lhs, const Matrix<Traits>& rhs);

    template<typename Traits>
    bool operator!=(const Matrix<Traits>& lhs, const Matrix<Traits>& rhs);

    // only valid for square matrices, consider removal ...
    template<typename Traits>
    Matrix<Traits>::row_type& operator*=(Matrix<Traits>::col_type& lhs, const Matrix<Traits>& rhs);

    // only valid for square matrices, consider removal ...
    template<typename Traits>
    Matrix<Traits>::row_reduced_type& operator*=(Matrix<Traits>::col_reduced_type& lhs, const Matrix<Traits>& rhs);

    template<typename Traits>
    Matrix<Traits::transpose_traits> transpose(Matrix<Traits> matrix);

    template<typename Traits>
    Matrix<Traits> comatrix(const Matrix<Traits>& matrix);

    template<typename Traits>
    Matrix<Traits> adjugate(const Matrix<Traits>& matrix);

    template<typename Traits>
    Matrix<Traits> inverse(const Matrix<Traits>& matrix);

    template<typename Traits>
    Matrix<Traits> inverse(const Matrix<Traits>& matrix, typename Matrix<Traits>::value_type det);

    template<typename Traits>
    bool matrix_eq(const Matrix<Traits>& lhs, const Matrix<Traits>& rhs, int ulp = 128);

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, const Matrix<Traits>& matrix);

} // namespace quetzal::math

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix(const value_type (&elements)[rows * cols])
    m_rep{}
{
    auto e = elements.begin();
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            Traits:get(m_rep, i, j) = *e++;
        }
    }
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits>::Matrix(const rep_type& rep) :
    m_rep(rep)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
Matrix(const row_type& row0) requires (M == 1)
{
    Traits::get_row<0>(m_rep) = row0;
}

//------------------------------------------------------------------------------
template<typename Traits>
Matrix(const row_type& row0, const row_type& row1) requires (M == 2)
{
    Traits::get_row<0>(m_rep) = row0;
    Traits::get_row<1>(m_rep) = row1;
}

//------------------------------------------------------------------------------
template<typename Traits>
Matrix(const row_type& row0, const row_type& row1, const row_type& row2) requires (M == 3)
{
    Traits::get_row<0>(m_rep) = row0;
    Traits::get_row<1>(m_rep) = row1;
    Traits::get_row<2>(m_rep) = row2;
}

//------------------------------------------------------------------------------
template<typename Traits>
Matrix(const row_type& row0, const row_type& row1, const row_type& row2, const row_type& row3) requires (M == 4)
{
    Traits::get_row<0>(m_rep) = row0;
    Traits::get_row<1>(m_rep) = row1;
    Traits::get_row<2>(m_rep) = row2;
    Traits::get_row<3>(m_rep) = row3;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits> quetzal::math::Matrix<Traits>::operator+() const
{
    return *this;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits> quetzal::math::Matrix<Traits>::operator-() const
{
    return Traits::unary_minus(m_rep);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits>& quetzal::math::Matrix<Traits>::operator+=(const Matrix& other)
{
    m_rep = Traits::assign_add(m_rep, other.m_rep);
    return *this;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits>& quetzal::math::Matrix<Traits>::operator-=(const Matrix& other)
{
    m_rep = Traits::assign_subtract(m_rep, other.m_rep);
    return *this;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits>& quetzal::math::Matrix<Traits>::operator*=(value_type f)
{
    m_rep = Traits::assign_multiply(m_rep, f);
    return *this;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits>& quetzal::math::Matrix<Traits>::operator/=(value_type f)
{
    m_rep = Traits::assign_divide(m_rep, f);
    return *this;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::math::Matrix<Traits>::value_type quetzal::math::Matrix<Traits>::operator()(size_t i, size_t j) const
{
    return Traits::get(m_rep, i, j);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::math::Matrix<Traits>::value_type& quetzal::math::Matrix<Traits>::operator()(size_t i, size_t j)
{
    return Traits::get(m_rep, i, j);
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::math::Matrix<Traits>::row_type& quetzal::math::Matrix<Traits>::row(size_t i) const
{
    return Traits::get_row(m_rep, i);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::math::Matrix<Traits>::row_type& quetzal::math::Matrix<Traits>::row(size_t i)
{
    return Traits::get_row(m_rep, i);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::math::Matrix<Traits>::value_type quetzal::math::Matrix<Traits>::determinant() const
{
    static_assert(rows == cols);
    static_assert(rows > 0);

    if constexpr (rows == 1)
    {
        return m_rep[0][0];
    }
    else if constexpr (rows == 2)
    {
        return m_rep[0][0] * m_rep[1][1] - m_rep[0][1] * m_rep[1][0];
    }
    else
    {
        value_type sign = 1;
        value_type d = 0;

        for (size_t j = 0; j < cols; ++j)
        {
            d += sign * m_rep[0][j] * minor(0, j);
            sign = -sign;
        }

        return d;
    }
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::math::Matrix<Traits>::value_type quetzal::math::Matrix<Traits>::trace() const
{
    static_assert(rows == cols);

    value_type t = 0;

    for (size_t i = 0; i < rows; ++i)
    {
        t += Traits::get(m_rep, i, i);
    }

    return t;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits>& quetzal::math::Matrix<Traits>::transpose()
{
    static_assert(rows == cols);

    *this = transpose(*this);
    return *this;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits>& quetzal::math::Matrix<Traits>::inverse()
{
    *this = inverse(*this);
    return *this;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::math::Matrix<Traits>::clear()
{
    Traits::clear(m_rep);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits>::reduced_type quetzal::math::Matrix<Traits>::submatrix(size_t m, size_t n) const
{
    static_assert(rows > 1 && cols > 1);

    quetzal::math::Matrix<T, rows - 1, cols - 1> matrix;

    for (size_t i = 0; i < rows - 1; ++i)
    {
        size_t ii = i < m ? i : i + 1;

        for (size_t j = 0; j < cols - 1; ++j)
        {
            size_t jj = j < n ? j : j + 1;

            matrix(i, j) = m_rep[ii][jj];
        }
    }

    return matrix;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::math::Matrix<Traits>::value_type quetzal::math::Matrix<Traits>::minor(size_t m, size_t n) const
{
    static_assert(rows == cols);
    assert(m < rows);
    assert(n < cols);

    if constexpr (rows > 1)
    {
        return submatrix(m, n).determinant();
    }
    else
    {
        return m_rep[0][0];
    }
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::math::Matrix<Traits>::value_type quetzal::math::Matrix<Traits>::cofactor(size_t m, size_t n) const
{
    static_assert(rows == cols);

    return (((m + n) & 0x01) == 0 ? 1 : -1) * minor(m, n);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits> quetzal::math::Matrix<Traits>::identity()
{
    static_assert(rows == cols);

    Matrix<Traits> matrix;
    for (size_t i = 0; i < rows; ++i)
    {
        matrix(i, i) = 1;
    }

    return matrix;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits> quetzal::math::operator+(Matrix<Traits> lhs, const Matrix<Traits>& rhs)
{
    lhs += rhs;
    return lhs;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits> quetzal::math::operator-(Matrix<Traits> lhs, const Matrix<Traits>& rhs)
{
    lhs -= rhs;
    return lhs;
}

//------------------------------------------------------------------------------
template<typename Traits, size_t P>
quetzal::math::Matrix<product_traits<P>> quetzal::math::operator*(const Matrix<Traits>& lhs, const Matrix<multiplicand_traits<P>>& rhs)
{
    Matrix<product_traits<P>> matrix;

    for (size_t i = 0; i < Matrix<Traits>::rows; ++i)
    {
        for (size_t j = 0; j < P; ++j)
        {
            typename Matrix<Traits>::value_type a = 0;

            for (size_t k = 0; k < Matrix<Traits>::cols; ++k)
            {
                a += lhs(i, k) * rhs(k, j);
            }

            matrix(i, j) = a;
        }
    }

    return matrix;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits> quetzal::math::operator*(Matrix<Traits> lhs, const typename Matrix<Traits>::value_type& rhs)
{
    lhs *= rhs;
    return lhs;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits> quetzal::math::operator*(const typename Matrix<Traits>::value_type& lhs, Matrix<Traits> rhs)
{
    rhs *= lhs;
    return rhs;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits>::col_type quetzal::math::operator*(const Matrix<Traits>& lhs, const Matrix<Traits>::row_type& rhs)
{
    Matrix<Traits>::col_type v;

    for (size_t i = 0; i < Matrix<Traits>::rows; ++i)
    {
        v[i] = dot(lhs.row(i), rhs);
    }

    return v;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits>::row_type quetzal::math::operator*(const Matrix<Traits>::col_type& lhs, const Matrix<Traits>& rhs)
{
    Matrix<Traits>::row_type v;

    for (size_t j = 0; j < Matrix<Traits>::cols; ++j)
    {
        for (size_t i = 0; i < Matrix<Traits>::rows; ++i)
        {
            v[j] += lhs[i] * rhs(i, j);
        }
    }

    return v;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits>::col_reduced_type quetzal::math::operator*(const Matrix<Traits>& lhs, const Matrix<Traits>::row_reduced_type& rhs)
{
    Matrix<Traits>::col_reduced_type v;

    for (size_t i = 0; i < Matrix<Traits>::rows - 1; ++i)
    {
        for (size_t j = 0; j < Matrix<Traits>::cols - 1; ++j)
        {
            v[i] += lhs(i, j) * rhs[j];
        }

        v[i] += rhs(Matrix<Traits>::cols - 1, i);
    }

    return v;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits>::row_reduced_type quetzal::math::operator*(const Matrix<Traits>::col_reduced_type& lhs, const Matrix<Traits>& rhs)
{
    Matrix<Traits>::row_reduced_type v;

    for (size_t j = 0; j < Matrix<Traits>::cols - 1; ++j)
    {
        for (size_t i = 0; i < Matrix<Traits>::rows - 1; ++i)
        {
            v[j] += lhs[i] * rhs(i, j);
        }

        v[j] += rhs(Matrix<Traits>::rows - 1, j);
    }

    return v;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits> quetzal::math::operator/(Matrix<Traits> lhs, const typename Matrix<Traits>::value_type& rhs)
{
    lhs /= rhs;
    return lhs;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::math::operator==(const Matrix<Traits>& lhs, const Matrix<Traits>& rhs)
{
    return Traits::equals(lhs, rhs);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::math::operator!=(const Matrix<Traits>& lhs, const Matrix<Traits>& rhs)
{
    return !operator==(lhs, rhs);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits>::row_type& quetzal::math::operator*=(Matrix<Traits>::col_type& lhs, const Matrix<Traits>& rhs)
{
    Matrix<Traits>::row_type v;

    for (size_t j = 0; j < Matrix<Traits>::cols; ++j)
    {
        for (size_t i = 0; i < Matrix<Traits>::rows; ++i)
        {
            v[j] += lhs[i] * rhs(i, j);
        }
    }

//    swap(lhs, v);
    assert(Matrix<Traits>::rows == Matrix<Traits>::cols); // ...
    lhs = v; // ...

    return lhs;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits>::row_reduced_type& quetzal::math::operator*=(Matrix<Traits>::col_reduced_type& lhs, const Matrix<Traits>& rhs)
{
    Matrix<Traits>::col_type lhs1;
    for (size_t i = 0; i < Matrix<Traits>::rows - 1; ++i)
    {
        lhs1[i] = lhs[i];
    }
    lhs1[Matrix<Traits>::rows - 1] = T(1);
    
    lhs1 *= rhs;

//    lhs.resize(Matrix<Traits>::cols - 1);
    assert(Matrix<Traits>::rows == Matrix<Traits>::cols); // ...

    for (size_t j = 0; j < Matrix<Traits>::cols - 1; ++j)
    {
        lhs[j] = lhs1[j];
    }

    return lhs;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits::transpose_traits> quetzal::math::transpose(Matrix<Traits> matrix)
{
    Matrix<Traits::transpose_traits> m;

    for (size_t i = 0; i < Matrix<Traits>::rows; ++i)
    {
        for (size_t j = 0; j < Matrix<Traits>::cols; ++j)
        {
            m(j, i) = matrix(i, j);
        }
    }

    return m;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits> quetzal::math::comatrix(const Matrix<Traits>& matrix)
{
    static_assert(Matrix<Traits>::rows == Matrix<Traits>::cols);

    Matrix<Traits> m;

    for (size_t i = 0; i < Matrix<Traits>::rows; ++i)
    {
        for (size_t j = 0; j < Matrix<Traits>::cols; ++j)
        {
            m(i, j) = matrix.cofactor(i, j);
        }
    }

    return m;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits> quetzal::math::adjugate(const Matrix<Traits>& matrix)
{
    return transpose(comatrix(matrix));
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits> quetzal::math::inverse(const Matrix<Traits>& matrix)
{
    return inverse(matrix, matrix.determinant());
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<Traits> quetzal::math::inverse(const Matrix<Traits>& matrix, typename Matrix<Traits>::value_type det)
{
    static_assert(Matrix<Traits>::rows == Matrix<Traits>::cols);

    if (math::float_eq0(det))
    {
        return Matrix<Traits>(); // fill with infinity ...
    }

    return adjugate(matrix) * (1 / det);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::math::matrix_eq(const Matrix<Traits>& lhs, const Matrix<Traits>& rhs, int ulp)
{
    for (size_t i = 0; i < Matrix<Traits>::rows; ++i)
    {
        if (!vector_eq(lhs.row(i), rhs.row(i), ulp))
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::math::operator<<(std::ostream& os, const Matrix<Traits>& matrix)
{
    for (size_t i = 0; i < Matrix<Traits>::rows; ++i)
    {
        os << matrix.rows(i) << "\n";
    }

    return os;
}

#endif // QUETZAL_MATH_MATRIX_HPP
