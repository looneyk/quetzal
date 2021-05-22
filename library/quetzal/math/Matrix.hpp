#if !defined(QUETZAL_MATH_MATRIX_HPP)
#define QUETZAL_MATH_MATRIX_HPP
//------------------------------------------------------------------------------
// math
// Matrix.hpp
//------------------------------------------------------------------------------

#include "Vector.hpp"
#include "VectorTraits.hpp"
#include "floating_point.hpp"
#include <array>
#include <iostream>

namespace quetzal::math
{

    //--------------------------------------------------------------------------
    template<typename T, size_t M = 4, size_t N = M>
    class Matrix
    {
    public:

        static_assert(M > 0 && N > 0);

        using value_type = T;
        using row_traits = VectorTraits<T, N>;
        using row_type = Vector<row_traits>;
        using rows_type = std::array<row_type, M>;
        static const size_t rows = M;
        static const size_t cols = N;

        Matrix() = default;
        Matrix(std::initializer_list<value_type> elements);

        Matrix(const row_type& row0) requires (M == 1);

        Matrix(const row_type& row0, const row_type& row1) requires (M == 2);

        Matrix(const row_type& row0, const row_type& row1, const row_type& row2) requires (M == 3);

        Matrix(const row_type& row0, const row_type& row1, const row_type& row2, const row_type& row3) requires (M == 4);

        Matrix(const Matrix&) = default;
        Matrix(Matrix&& other) = default;
        ~Matrix() = default;

        Matrix& operator=(const Matrix& other) = default;
        Matrix& operator=(Matrix&& other) = default;
        Matrix operator+() const;
        Matrix operator-() const;
        Matrix& operator+=(const Matrix& other);
        Matrix& operator-=(const Matrix& other);
        Matrix& operator*=(value_type f);
        Matrix& operator/=(value_type f);
        value_type operator()(size_t m, size_t n) const;
        value_type& operator()(size_t m, size_t n);

        Matrix& operator*=(const Matrix& other); // square only ...

        const row_type& row(size_t m) const;
        row_type& row(size_t m);

        Matrix<T, M - 1, N - 1> submatrix(size_t m, size_t n) const;
        void clear();

        // the following are only valid for square matrices, consider removal ...
        value_type determinant() const;
        value_type trace() const;

        Matrix& transpose();
        Matrix& inverse();

        value_type minor(size_t m, size_t n) const;
        value_type cofactor(size_t m, size_t n) const;

        static Matrix<T, M, N> identity(); // square only ...

    private:

        rows_type m_rows;
    };

    template<typename T, size_t M, size_t N>
    Matrix<T, M, N> operator+(Matrix<T, M, N> lhs, const Matrix<T, M, N>& rhs);

    template<typename T, size_t M, size_t N>
    Matrix<T, M, N> operator-(Matrix<T, M, N> lhs, const Matrix<T, M, N>& rhs);

    template<typename T, size_t M, size_t N, size_t P>
    Matrix<T, M, P> operator*(const Matrix<T, M, N>& lhs, const Matrix<T, N, P>& rhs);

    template<typename T, size_t M, size_t N>
    Matrix<T, M, N> operator*(Matrix<T, M, N> lhs, const typename Matrix<T, M, N>::value_type& rhs);

    template<typename T, size_t M, size_t N>
    Matrix<T, M, N> operator*(const typename Matrix<T, M, N>::value_type& lhs, Matrix<T, M, N> rhs);

    template<typename T, size_t M, size_t N>
    Vector<VectorTraits<T, M>> operator*(const Matrix<T, M, N>& lhs, const Vector<VectorTraits<T, N>>& rhs);

    template<typename T, size_t M, size_t N>
    Vector<VectorTraits<T, N>> operator*(const Vector<VectorTraits<T, M>>& lhs, const Matrix<T, M, N>& rhs);

    template<typename T, size_t M, size_t N>
    Vector<VectorTraits<T, M - 1>> operator*(const Matrix<T, M, N>& lhs, const Vector<VectorTraits<T, N - 1>>& rhs);

    template<typename T, size_t M, size_t N>
    Vector<VectorTraits<T, N - 1>> operator*(const Vector<VectorTraits<T, M - 1>>& lhs, const Matrix<T, M, N>& rhs);

    template<typename T, size_t M, size_t N>
    Matrix<T, M, N> operator/(Matrix<T, M, N> lhs, const typename Matrix<T, M, N>::value_type& rhs);

    template<typename T, size_t M, size_t N>
    bool operator==(const Matrix<T, M, N>& lhs, const Matrix<T, M, N>& rhs);

    template<typename T, size_t M, size_t N>
    bool operator!=(const Matrix<T, M, N>& lhs, const Matrix<T, M, N>& rhs);

    template<typename T, size_t M>
    Vector<VectorTraits<T, M>>& operator*=(Vector<VectorTraits<T, M>>& lhs, const Matrix<T, M, M>& rhs);

    template<typename T, size_t M>
    Vector<VectorTraits<T, M - 1>>& operator*=(Vector<VectorTraits<T, M - 1>>& lhs, const Matrix<T, M, M>& rhs);

    template<typename T, size_t M, size_t N>
    Matrix<T, N, M> transpose(Matrix<T, M, N> matrix);

    template<typename T, size_t M>
    Matrix<T, M, M> comatrix(const Matrix<T, M, M>& matrix);

    template<typename T, size_t M>
    Matrix<T, M, M> adjugate(const Matrix<T, M, M>& matrix);

    template<typename T, size_t M>
    Matrix<T, M, M> inverse(const Matrix<T, M, M>& matrix);

    template<typename T, size_t M>
    Matrix<T, M, M> inverse(const Matrix<T, M, M>& matrix, typename Matrix<T, M, M>::value_type det);

    template<typename T, size_t M, size_t N>
    bool matrix_eq(const Matrix<T, M, N>& lhs, const Matrix<T, M, N>& rhs, int ulp = 128);

    template<typename T, size_t M, size_t N>
    std::ostream& operator<<(std::ostream& os, const Matrix<T, M, N>& matrix);

} // namespace quetzal::math

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Matrix<T, M, N>::Matrix(std::initializer_list<value_type> elements) :
    m_rows{}
{
    auto k = elements.begin();
    for (size_t i = 0; i < M; ++i)
    {
        for (size_t j = 0; j < N; ++j)
        {
            m_rows[i][j] = *k++;
        }
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Matrix<T, M, N>::Matrix(const row_type& row0) requires (M == 1) :
    m_rows{row0}
{
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Matrix<T, M, N>::Matrix(const row_type& row0, const row_type& row1) requires (M == 2) :
    m_rows{row0, row1}
{
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Matrix<T, M, N>::Matrix(const row_type& row0, const row_type& row1, const row_type& row2) requires (M == 3) :
    m_rows{row0, row1, row2}
{
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Matrix<T, M, N>::Matrix(const row_type& row0, const row_type& row1, const row_type& row2, const row_type& row3) requires (M == 4) :
    m_rows{row0, row1, row2, row3}
{
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Matrix<T, M, N> quetzal::math::Matrix<T, M, N>::operator+() const
{
    return *this;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Matrix<T, M, N> quetzal::math::Matrix<T, M, N>::operator-() const
{
    return {-m_rows[0], -m_rows[1], -m_rows[2], -m_rows[3]};
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Matrix<T, M, N>& quetzal::math::Matrix<T, M, N>::operator+=(const Matrix& other)
{
    for (size_t i = 0; i < M; ++i)
    {
        m_rows[i] += other.m_rows[i];
    }

    return *this;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Matrix<T, M, N>& quetzal::math::Matrix<T, M, N>::operator-=(const Matrix& other)
{
    for (size_t i = 0; i < M; ++i)
    {
        m_rows[i] -= other.m_rows[i];
    }

    return *this;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Matrix<T, M, N>& quetzal::math::Matrix<T, M, N>::operator*=(value_type f)
{
    for (size_t i = 0; i < M; ++i)
    {
        m_rows[i] *= f;
    }

    return *this;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Matrix<T, M, N>& quetzal::math::Matrix<T, M, N>::operator/=(value_type f)
{
    for (size_t i = 0; i < M; ++i)
    {
        m_rows[i] /= f;
    }

    return *this;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
typename quetzal::math::Matrix<T, M, N>::value_type quetzal::math::Matrix<T, M, N>::operator()(size_t m, size_t n) const
{
    return m_rows[m][n];
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
typename quetzal::math::Matrix<T, M, N>::value_type& quetzal::math::Matrix<T, M, N>::operator()(size_t m, size_t n)
{
    return m_rows[m][n];
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Matrix<T, M, N>& quetzal::math::Matrix<T, M, N>::operator*=(const Matrix<T, M, N>& other)
{
    static_assert(M == N);

    *this = *this * other;
    return *this;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
const typename quetzal::math::Matrix<T, M, N>::row_type& quetzal::math::Matrix<T, M, N>::row(size_t m) const
{
    return m_rows[m];
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
typename quetzal::math::Matrix<T, M, N>::row_type& quetzal::math::Matrix<T, M, N>::row(size_t m)
{
    return m_rows[m];
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Matrix<T, M - 1, N - 1> quetzal::math::Matrix<T, M, N>::submatrix(size_t m, size_t n) const
{
    static_assert(M > 1 && N > 1);
    assert(m < M);
    assert(n < N);

    quetzal::math::Matrix<T, M - 1, N - 1> matrix;

    for (size_t i = 0; i < M - 1; ++i)
    {
        size_t ii = i < m ? i : i + 1;

        for (size_t j = 0; j < N - 1; ++j)
        {
            size_t jj = j < n ? j : j + 1;

            matrix(i, j) = m_rows[ii][jj];
        }
    }

    return matrix;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
void quetzal::math::Matrix<T, M, N>::clear()
{
    for (size_t i = 0; i < M; ++i)
    {
        m_rows[i].clear();
    }

    return;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
typename quetzal::math::Matrix<T, M, N>::value_type quetzal::math::Matrix<T, M, N>::determinant() const
{
    static_assert(M == N);
    static_assert(M > 0);

    if constexpr (M == 1)
    {
        return m_rows[0][0];
    }
    else if constexpr (M == 2)
    {
        return m_rows[0][0] * m_rows[1][1] - m_rows[0][1] * m_rows[1][0];
    }
    else
    {
        value_type sign = 1;
        value_type d = 0;

        for (size_t j = 0; j < N; ++j)
        {
            d += sign * m_rows[0][j] * minor(0, j);
            sign = -sign;
        }

        return d;
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
typename quetzal::math::Matrix<T, M, N>::value_type quetzal::math::Matrix<T, M, N>::trace() const
{
    static_assert(M == N);

    value_type t = 0;

    for (size_t i = 0; i < M; ++i)
    {
        t += m_rows[i][i];
    }

    return t;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Matrix<T, M, N>& quetzal::math::Matrix<T, M, N>::transpose()
{
    static_assert(M == N);

    *this = transpose(*this);
    return *this;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Matrix<T, M, N>& quetzal::math::Matrix<T, M, N>::inverse()
{
    static_assert(M == N);
    *this = inverse(*this);
    return *this;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
typename quetzal::math::Matrix<T, M, N>::value_type quetzal::math::Matrix<T, M, N>::minor(size_t m, size_t n) const
{
    static_assert(M == N);
    assert(m < M);
    assert(n < N);

    if constexpr (M > 1)
    {
        return submatrix(m, n).determinant();
    }
    else
    {
        return m_rows[0][0];
    }
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
typename quetzal::math::Matrix<T, M, N>::value_type quetzal::math::Matrix<T, M, N>::cofactor(size_t m, size_t n) const
{
    static_assert(M == N);

    return (((m + n) & 0x01) == 0 ? 1 : -1) * minor(m, n);
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Matrix<T, M, N> quetzal::math::Matrix<T, M, N>::identity()
{
    static_assert(M == N);

    Matrix<T, M> matrix;
    for (size_t i = 0; i < M; ++i)
    {
        matrix(i, i) = 1;
    }

    return matrix;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Matrix<T, M, N> quetzal::math::operator+(Matrix<T, M, N> lhs, const Matrix<T, M, N>& rhs)
{
    for (size_t i = 0; i < M; ++i)
    {
        lhs.row(i) += rhs.row(i);
    }

    return lhs;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Matrix<T, M, N> quetzal::math::operator-(Matrix<T, M, N> lhs, const Matrix<T, M, N>& rhs)
{
    for (size_t i = 0; i < M; ++i)
    {
        lhs.row(i) -= rhs.row(i);
    }

    return lhs;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N, size_t P>
quetzal::math::Matrix<T, M, P> quetzal::math::operator*(const Matrix<T, M, N>& lhs, const Matrix<T, N, P>& rhs)
{
    Matrix<T, M, P> matrix;

    for (size_t i = 0; i < M; ++i)
    {
        for (size_t j = 0; j < P; ++j)
        {
            typename Matrix<T, M, N>::value_type a = 0;

            for (size_t k = 0; k < N; ++k)
            {
                a += lhs(i, k) * rhs(k, j);
            }

            matrix(i, j) = a;
        }
    }

    return matrix;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Matrix<T, M, N> quetzal::math::operator*(Matrix<T, M, N> lhs, const typename Matrix<T, M, N>::value_type& rhs)
{
    for (size_t i = 0; i < M; ++i)
    {
        lhs.row(i) *= rhs;
    }

    return lhs;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Matrix<T, M, N> quetzal::math::operator*(const typename Matrix<T, M, N>::value_type& lhs, Matrix<T, M, N> rhs)
{
    for (size_t i = 0; i < M; ++i)
    {
        rhs.row(i) *= lhs;
    }

    return rhs;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Vector<quetzal::math::VectorTraits<T, M>> quetzal::math::operator*(const Matrix<T, M, N>& lhs, const Vector<VectorTraits<T, N>>& rhs)
{
    Vector<VectorTraits<T, M>> v;

    for (size_t i = 0; i < M; ++i)
    {
        v[i] = dot(lhs.row(i), rhs);
    }

    return v;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Vector<quetzal::math::VectorTraits<T, N>> quetzal::math::operator*(const Vector<VectorTraits<T, M>>& lhs, const Matrix<T, M, N>& rhs)
{
    Vector<VectorTraits<T, N>> v;

    for (size_t j = 0; j < N; ++j)
    {
        for (size_t i = 0; i < M; ++i)
        {
            v[j] += lhs[i] * rhs(i, j);
        }
    }

    return v;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Vector<quetzal::math::VectorTraits<T, M - 1>> quetzal::math::operator*(const Matrix<T, M, N>& lhs, const Vector<VectorTraits<T, N - 1>>& rhs)
{
    Vector<VectorTraits<T, M - 1>> v;

    for (size_t i = 0; i < M - 1; ++i)
    {
        for (size_t j = 0; j < N - 1; ++j)
        {
            v[i] += lhs(i, j) * rhs[j];
        }

        v[i] += lhs(i, N - 1);
    }

    return v;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Vector<quetzal::math::VectorTraits<T, N - 1>> quetzal::math::operator*(const Vector<VectorTraits<T, M - 1>>& lhs, const Matrix<T, M, N>& rhs)
{
    Vector<VectorTraits<T, N - 1>> v;

    for (size_t j = 0; j < N - 1; ++j)
    {
        for (size_t i = 0; i < M - 1; ++i)
        {
            v[j] += lhs[i] * rhs(i, j);
        }

        v[j] += rhs(M - 1, j);
    }

    return v;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Matrix<T, M, N> quetzal::math::operator/(Matrix<T, M, N> lhs, const typename Matrix<T, M, N>::value_type& rhs)
{
    for (size_t i = 0; i < M; ++i)
    {
        lhs.row(i) /= rhs;
    }

    return lhs;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
bool quetzal::math::operator==(const Matrix<T, M, N>& lhs, const Matrix<T, M, N>& rhs)
{
    for (size_t i = 0; i < M; ++i)
    {
        if (lhs.row(i) != rhs.row(i))
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
bool quetzal::math::operator!=(const Matrix<T, M, N>& lhs, const Matrix<T, M, N>& rhs)
{
    return !operator==(lhs, rhs);
}

//------------------------------------------------------------------------------
template<typename T, size_t M>
quetzal::math::Vector<quetzal::math::VectorTraits<T, M>>& quetzal::math::operator*=(Vector<VectorTraits<T, M>>& lhs, const Matrix<T, M, M>& rhs)
{
    Vector<VectorTraits<T, M>> v;

    for (size_t j = 0; j < M; ++j)
    {
        for (size_t i = 0; i < M; ++i)
        {
            v[j] += lhs[i] * rhs(i, j);
        }
    }

//    swap(lhs, v);
    lhs = v; // ...

    return lhs;
}

//------------------------------------------------------------------------------
template<typename T, size_t M>
quetzal::math::Vector<quetzal::math::VectorTraits<T, M - 1>>& quetzal::math::operator*=(Vector<VectorTraits<T, M - 1>>& lhs, const Matrix<T, M, M>& rhs)
{
    Vector<VectorTraits<T, M>> lhs1;
    for (size_t i = 0; i < M - 1; ++i)
    {
        lhs1[i] = lhs[i];
    }
    lhs1[M - 1] = T(1);
    
    lhs1 *= rhs;

//    lhs.resize(M - 1);

    for (size_t j = 0; j < M - 1; ++j)
    {
        lhs[j] = lhs1[j];
    }

    return lhs;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
quetzal::math::Matrix<T, N, M> quetzal::math::transpose(Matrix<T, M, N> matrix)
{
    Matrix<T, N, M> m;

    for (size_t i = 0; i < M; ++i)
    {
        for (size_t j = 0; j < N; ++j)
        {
            m(j, i) = matrix(i, j);
        }
    }

    return m;
}

//------------------------------------------------------------------------------
template<typename T, size_t M>
quetzal::math::Matrix<T, M, M> quetzal::math::comatrix(const Matrix<T, M, M>& matrix)
{
    Matrix<T, M, M> m;

    for (size_t i = 0; i < M; ++i)
    {
        for (size_t j = 0; j < M; ++j)
        {
            m(i, j) = matrix.cofactor(i, j);
        }
    }

    return m;
}

//------------------------------------------------------------------------------
template<typename T, size_t M>
quetzal::math::Matrix<T, M, M> quetzal::math::adjugate(const Matrix<T, M, M>& matrix)
{
    return transpose(comatrix(matrix));
}

//------------------------------------------------------------------------------
template<typename T, size_t M>
quetzal::math::Matrix<T, M, M> quetzal::math::inverse(const Matrix<T, M, M>& matrix)
{
    return inverse(matrix, matrix.determinant());
}

//------------------------------------------------------------------------------
template<typename T, size_t M>
quetzal::math::Matrix<T, M, M> quetzal::math::inverse(const Matrix<T, M, M>& matrix, typename Matrix<T, M, M>::value_type det)
{
    if (math::float_eq0(det))
    {
        return Matrix<T, M, M>(); // fill with infinity ...
    }

    return adjugate(matrix) * (1 / det);
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
bool quetzal::math::matrix_eq(const Matrix<T, M, N>& lhs, const Matrix<T, M, N>& rhs, int ulp)
{
    for (size_t i = 0; i < M; ++i)
    {
        if (!vector_eq(lhs.row(i), rhs.row(i), ulp))
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
template<typename T, size_t M, size_t N>
std::ostream& quetzal::math::operator<<(std::ostream& os, const Matrix<T, M, N>& matrix)
{
    for (size_t i = 0; i < M; ++i)
    {
        os << matrix.row(i) << "\n";
    }

    return os;
}

#endif // QUETZAL_MATH_MATRIX_HPP
