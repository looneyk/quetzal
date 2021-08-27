#if !defined(QUETZAL_MATH_MATH_XM_HPP)
#define QUETZAL_MATH_MATH_XM_HPP
//------------------------------------------------------------------------------
// math
// math_xm.hpp
//------------------------------------------------------------------------------

#include "Matrix.hpp"
#include <DirectXMath.h>

namespace quetzal::math
{

    template<typename V>
    Matrix<float, 4, 4> rotation_axis_unit_xm(const V& axis, float angle);

    float determinant_xm(const Matrix<float, 4, 4>& matrix);

    Matrix<float, 4, 4> inverse_xm(const Matrix<float, 4, 4>& matrix);

namespace math_xm_internal
{

    template<typename V>
    DirectX::XMVECTOR to_xmvector(const V& v);

    DirectX::XMMATRIX to_xmmatrix(const Matrix<float, 4, 4>& matrix);

    Matrix<float, 4, 4> to_matrix(const DirectX::XMMATRIX& u);

} // namespace math_xm_internal

} // namespace quetzal::math

//------------------------------------------------------------------------------
template<typename V>
quetzal::math::Matrix<float, 4, 4> quetzal::math::rotation_axis_unit_xm(const V& axis, float angle)
{
    return math_xm_internal::to_matrix(DirectX::XMMatrixRotationNormal(math_xm_internal::to_xmvector(axis), angle));
}

//------------------------------------------------------------------------------
template<typename V>
DirectX::XMVECTOR quetzal::math::math_xm_internal::to_xmvector(const V& v)
{
    if constexpr (V::dimension == 3)
    {
        return DirectX::XMLoadFloat3(reinterpret_cast<const DirectX::XMFLOAT3*>(&v.rep()));
    }
    else if constexpr (V::dimension == 4)
    {
        return DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4*>(&v.rep()));
    }
}

#endif // QUETZAL_MATH_MATH_XM_HPP
