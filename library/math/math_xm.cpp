//------------------------------------------------------------------------------
// math
// math_xm.cpp
//------------------------------------------------------------------------------

#include "math_xm.hpp"
#include <DirectXMath.h>

using namespace std;
using namespace DirectX;

//------------------------------------------------------------------------------
float quetzal::math::determinant_xm(const Matrix<float, 4, 4>& matrix)
{
    return XMVectorGetX(XMMatrixDeterminant(math_xm_internal::to_xmmatrix(matrix)));
}

//------------------------------------------------------------------------------
quetzal::math::Matrix<float, 4, 4> quetzal::math::inverse_xm(const Matrix<float, 4, 4>& matrix)
{
    return math_xm_internal::to_matrix(XMMatrixInverse(nullptr, math_xm_internal::to_xmmatrix(matrix)));
}

//------------------------------------------------------------------------------
XMMATRIX quetzal::math::math_xm_internal::to_xmmatrix(const quetzal::math::Matrix<float, 4, 4>& matrix)
{
    return XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(&matrix.row(0)));
}

//------------------------------------------------------------------------------
quetzal::math::Matrix<float, 4, 4> quetzal::math::math_xm_internal::to_matrix(const XMMATRIX& u)
{
    quetzal::math::Matrix<float, 4, 4> matrix;
    XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&matrix.row(0)), u); // matrix.rep() ...
    return matrix;
}
