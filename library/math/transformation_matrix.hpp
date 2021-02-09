#if !defined(QUETZAL_MATH_TRANSFORMATION_MATRIX_HPP)
#define QUETZAL_MATH_TRANSFORMATION_MATRIX_HPP
//------------------------------------------------------------------------------
// math
// transformation_matrix.hpp
//
// Transformation matrices for pre-multiplication: v' = v * M
//
//------------------------------------------------------------------------------

#include "Matrix.hpp"
#include "Vector.hpp"
#include "floating_point.hpp"
#include "math_util.hpp"
#include "math_xm.hpp"
#include <cmath>
#include <cassert>

namespace quetzal
{

namespace math
{

    template<typename V>
    Matrix<typename V::value_type> translation(const V& v);

    // should use enable_if_t, but lacking N ...
    template<typename T>
    Matrix<T> translation(T tx, T ty, T tz);

    // should use enable_if_t, but lacking N ...
    template<typename T>
    Matrix<T> translation(T tx, T ty);

    template<typename T>
    Matrix<T> scaling(T factor);

    // should use enable_if_t, but lacking N ...
    template<typename T>
    Matrix<T> scaling(T sx, T sy, T sz);

    // should use enable_if_t, but lacking N ...
    template<typename T>
    Matrix<T> scaling(T sx, T sy);

    template<typename V>
    Matrix<typename V::value_type> scaling_axis(const V& axis, typename V::value_type sDirectional, typename V::value_type sTransverse);

    template<typename T>
    Matrix<T> rotation_x(T angle);

    template<typename T>
    Matrix<T> rotation_y(T angle);

    template<typename T>
    Matrix<T> rotation_z(T angle);

    template<typename V>
    Matrix<typename V::value_type> rotation_axis(const V& axis, typename V::value_type angle);

    template<typename V>
    Matrix<typename V::value_type> rotation_axis_unit(const V& axis, typename V::value_type angle);

    // Rotation matrix resulting from rotation of unit vector from a to b
    template<typename V>
    Matrix<typename V::value_type> rotation_lerp_unit(const V& a, const V& b, typename V::value_type t);

    // Transformation matrix for rotating upFrom and rightFrom to coincide with upTo and rightTo
    // up and right vectors need to be unit length
    template<typename V>
    Matrix<typename V::value_type> alignment(const V& upFrom, V rightFrom, const V& upTo, V rightTo);

    // Fewer operations, would expect to be more accurate ...
    template<typename V>
    Matrix<typename V::value_type> alignment2(const V& upFrom, const V& rightFrom, const V& upTo, const V& rightTo);

    template<typename Traits>
    Matrix<typename Traits::value_type, 3, 3> change_of_basis(const Vector<Traits>& a0, const Vector<Traits>& a1, const Vector<Traits>& a2, const Vector<Traits>& b0, const Vector<Traits>& b1, const Vector<Traits>& b2);

    template<typename Traits>
    Matrix<typename Traits::value_type, 2, 2> change_of_basis(const Vector<Traits>& a0, const Vector<Traits>& a1, const Vector<Traits>& b0, const Vector<Traits>& b1);

} // namespace math

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename V>
quetzal::math::Matrix<typename V::value_type> quetzal::math::translation(const V& v)
{
    static_assert(V::dimension >= 2);

    Matrix<typename V::value_type> matrix = Matrix<typename V::value_type>::identity();
    matrix(3, 0) = v[0];
    matrix(3, 1) = v[1];

    if constexpr (V::dimension >= 3)
    {
        matrix(3, 2) = v[2];
    }

    return matrix;
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::math::Matrix<T> quetzal::math::translation(T tx, T ty, T tz)
{
    Matrix<T> matrix = Matrix<T>::identity();
    matrix(3, 0) = tx;
    matrix(3, 1) = ty;
    matrix(3, 2) = tz;
    return matrix;
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::math::Matrix<T> quetzal::math::translation(T tx, T ty)
{
    Matrix<T> matrix = Matrix<T>::identity();
    matrix(3, 0) = tx;
    matrix(3, 1) = ty;
    return matrix;
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::math::Matrix<T> quetzal::math::scaling(T factor)
{
    Matrix<T> matrix = Matrix<T>::identity();
    matrix(0, 0) = factor;
    matrix(1, 1) = factor;
    matrix(2, 2) = factor;
    return matrix;
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::math::Matrix<T> quetzal::math::scaling(T sx, T sy, T sz)
{
    Matrix<T> matrix = Matrix<T>::identity();
    matrix(0, 0) = sx;
    matrix(1, 1) = sy;
    matrix(2, 2) = sz;
    return matrix;
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::math::Matrix<T> quetzal::math::scaling(T sx, T sy)
{
    Matrix<T> matrix = Matrix<T>::identity();
    matrix(0, 0) = sx;
    matrix(1, 1) = sy;
    return matrix;
}

//------------------------------------------------------------------------------
template<typename V>
quetzal::math::Matrix<typename V::value_type> quetzal::math::scaling_axis(const V& axis, typename V::value_type sDirectional, typename V::value_type sTransverse)
{
    assert(axis.unit());

    typename V::value_type sDiff = sDirectional - sTransverse;

    Matrix<typename V::value_type> matrix = Matrix<typename V::value_type>::identity();
    matrix(0, 0) = sTransverse + sDiff * axis.x() * axis.x();
    matrix(1, 0) = sDiff * axis.x() * axis.y();
    matrix(2, 0) = sDiff * axis.x() * axis.z();
    matrix(0, 1) = sDiff * axis.y() * axis.x();
    matrix(1, 1) = sTransverse + sDiff * axis.y() * axis.y();
    matrix(2, 1) = sDiff * axis.y() * axis.z();
    matrix(0, 2) = sDiff * axis.z() * axis.x();
    matrix(1, 2) = sDiff * axis.z() * axis.y();
    matrix(2, 2) = sTransverse + sDiff * axis.z() * axis.z();
    return matrix;
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::math::Matrix<T> quetzal::math::rotation_x(T angle)
{
    T c = cos(angle);
    T s = sin(angle);

    Matrix<T> matrix = Matrix<T>::identity();
    matrix(1, 1) = c;
    matrix(1, 2) = s;
    matrix(2, 1) = -s;
    matrix(2, 2) = c;

    return matrix;
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::math::Matrix<T> quetzal::math::rotation_y(T angle)
{
    T c = cos(angle);
    T s = sin(angle);

    Matrix<T> matrix = Matrix<T>::identity();
    matrix(2, 2) = c;
    matrix(2, 0) = s;
    matrix(0, 2) = -s;
    matrix(0, 0) = c;

    return matrix;
}

//------------------------------------------------------------------------------
template<typename T>
quetzal::math::Matrix<T> quetzal::math::rotation_z(T angle)
{
    T c = cos(angle);
    T s = sin(angle);

    Matrix<T> matrix = Matrix<T>::identity();
    matrix(0, 0) = c;
    matrix(0, 1) = s;
    matrix(1, 0) = -s;
    matrix(1, 1) = c;

    return matrix;
}

//------------------------------------------------------------------------------
template<typename V>
quetzal::math::Matrix<typename V::value_type> quetzal::math::rotation_axis(const V& axis, typename V::value_type angle)
{
    return rotation_axis_unit(normalize(axis), angle);
}

//------------------------------------------------------------------------------
template<typename V>
quetzal::math::Matrix<typename V::value_type> quetzal::math::rotation_axis_unit(const V& axis, typename V::value_type angle)
{
    assert(axis.unit());

    if constexpr (std::is_same_v<typename V::value_type, float>)
    {
        // DirectXMath is about 15 times faster. Try using SSE directly ...
        return rotation_axis_unit_xm(axis, angle);
    }
    else
    {
        // Rotation matrix using Rodrigues' rotation formula

        using T = typename V::value_type;

        Matrix<T> mcross = {
            {T(0), axis[2], -axis[1], T(0)},
            {-axis[2], T(0), axis[0], T(0)},
            {axis[1], -axis[0], T(0), T(0)},
            {T(0), T(0), T(0), T(0)}
        };

        Matrix<T> mcross2 = {
            {-(axis[1] * axis[1] + axis[2] * axis[2]), axis[0] * axis[1], axis[2] * axis[0], T(0)},
            {axis[0] * axis[1], -(axis[2] * axis[2] + axis[0] * axis[0]), axis[1] * axis[2], T(0)},
            {axis[2] * axis[0], axis[1] * axis[2], -(axis[0] * axis[0] + axis[1] * axis[1]), T(0)},
            {T(0), T(0), T(0), T(0)}
        };

        return Matrix<T>::identity() + sin(angle) * mcross + (1 - cos(angle)) * mcross2;
    }
}

//------------------------------------------------------------------------------
template<typename V>
quetzal::math::Matrix<typename V::value_type> quetzal::math::rotation_lerp_unit(const V& a, const V& b, typename V::value_type t)
{
    if (vector_eq(a, b))
    {
        return Matrix<typename V::value_type>::identity();
    }

    return rotation_axis(cross(a, b), t * angle_unit(a, b));
}

//------------------------------------------------------------------------------
template<typename V>
quetzal::math::Matrix<typename V::value_type> quetzal::math::alignment(const V& upFrom, V rightFrom, const V& upTo, V rightTo)
{
    assert(upFrom.unit());
    assert(rightFrom.unit());
    assert(upTo.unit());
    assert(rightTo.unit());
    assert(perpendicular(upFrom, rightFrom));
    assert(perpendicular(upTo, rightTo));

    using T = typename V::value_type;

    V axisZ = {T(0), T(0), T(1)};

    // Transformation to rotate up and right From vectors to z-axis and x-axis respectively

    T thetaFrom = math::angle_unit(upFrom, axisZ);

    V axisFrom;
    if (float_eq(thetaFrom, Pi<T>))
    {
        axisFrom = rightFrom;
    }
    else if (float_ne0(thetaFrom))
    {
        axisFrom = normalize(cross(upFrom, axisZ));
        rightFrom *= math::rotation_axis_unit(axisFrom, thetaFrom);
    }

    assert(float_eq0(rightFrom.z()));
    T phiFrom = atan2(rightFrom.y(), rightFrom.x());

    // Transformation to rotate up and right To vectors to z-axis and x-axis respectively

    T thetaTo = math::angle_unit(upTo, axisZ);

    V axisTo;
    if (float_eq(thetaTo, Pi<T>))
    {
        axisTo = rightTo;
    }
    else if (float_ne0(thetaTo))
    {
        axisTo = normalize(cross(upTo, axisZ));
        rightTo *= math::rotation_axis_unit(axisTo, thetaTo);
    }

    assert(float_eq0(rightTo.z()));
    T phiTo = atan2(rightTo.y(), rightTo.x());

    T dphi = phiTo - phiFrom;

    // Transformation matrix

    Matrix<T> matrix = Matrix<T>::identity();

    // Rotate From to z-axis orientation
    if (float_ne0(thetaFrom))
    {
        matrix = math::rotation_axis_unit(axisFrom, thetaFrom);
    }

    // z-axis rotation
    if (float_ne0(dphi))
    {
        matrix *= math::rotation_z(dphi);
    }

    // Rotate To from z-axis orientation
    if (float_ne0(thetaTo))
    {
        matrix *= math::rotation_axis_unit(axisTo, -thetaTo);
    }

    return matrix;
}

//------------------------------------------------------------------------------
template<typename V>
quetzal::math::Matrix<typename V::value_type> quetzal::math::alignment2(const V& upFrom, const V& rightFrom, const V& upTo, const V& rightTo)
{
    assert(upFrom.unit());
    assert(rightFrom.unit());
    assert(upTo.unit());
    assert(rightTo.unit());
    assert(perpendicular(upFrom, rightFrom));
    assert(perpendicular(upTo, rightTo));

    using T = typename V::value_type;

    Matrix<T> matrix = Matrix<T>::identity();
    V rightFromAligned = rightFrom;

    T dtheta = math::angle_unit(upFrom, upTo);
    if (float_ne0(dtheta))
    {
        V axis;
        if (float_eq(dtheta, Pi<T>))
        {
            axis = rightTo;
        }
        else
        {
            axis = normalize(cross(upFrom, upTo));
        }

        matrix = rotation_axis_unit(axis, dtheta);
        rightFromAligned = normalize(rightFrom * matrix);
    }

    assert(perpendicular(upTo, rightFromAligned));

    T dphi = math::angle_unit(rightFromAligned, rightTo, upTo);
    if (float_ne0(dphi))
    {
        matrix *= rotation_axis_unit(upTo, dphi);
    }

    assert(vector_eq(upFrom * matrix, upTo));
    assert(vector_eq(rightFrom * matrix, rightTo));

    return matrix;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<typename Traits::value_type, 3, 3> quetzal::math::change_of_basis(const Vector<Traits>& a0, const Vector<Traits>& a1, const Vector<Traits>& a2, const Vector<Traits>& b0, const Vector<Traits>& b1, const Vector<Traits>& b2)
{
    using matrix_type = Matrix<typename Traits::value_type, 3, 3>;

    matrix_type a(a0, a1, a2);
    matrix_type b(b0, b1, b2);

    return inverse(a) * b;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Matrix<typename Traits::value_type, 2, 2> quetzal::math::change_of_basis(const Vector<Traits>& a0, const Vector<Traits>& a1, const Vector<Traits>& b0, const Vector<Traits>& b1)
{
    using matrix2_type = Matrix<typename Traits::value_type, 2, 2>;

    matrix2_type a(a0, a1);
    matrix2_type b(b0, b1);

    return inverse(a) * b;
}

#endif // QUETZAL_MATH_TRANSFORMATION_MATRIX_HPP
