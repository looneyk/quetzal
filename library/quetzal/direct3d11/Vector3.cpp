//------------------------------------------------------------------------------
// direct3d11
// Vector3.cpp
//------------------------------------------------------------------------------

#include "Vector3.hpp"
#include "quetzal/math/floating_point.hpp"
#include <iostream>

using namespace std;
using namespace DirectX;

//------------------------------------------------------------------------------
void quetzal::direct3d11::XMFLOAT3_Traits::clear(rep_type& rep)
{
    rep.x = 0.0f;
    rep.y = 0.0f;
    rep.z = 0.0f;
    return;
}

//------------------------------------------------------------------------------
typename quetzal::direct3d11::XMFLOAT3_Traits::rep_type& quetzal::direct3d11::XMFLOAT3_Traits::assign(rep_type& lhs, const float(&rhs)[3])
{
    lhs.x = rhs[0];
    lhs.y = rhs[1];
    lhs.z = rhs[2];
    return lhs;
}

//------------------------------------------------------------------------------
typename quetzal::direct3d11::XMFLOAT3_Traits::rep_type quetzal::direct3d11::XMFLOAT3_Traits::unary_minus(rep_type rep)
{
    rep.x = -rep.x;
    rep.y = -rep.y;
    rep.z = -rep.z;
    return rep;
}

//------------------------------------------------------------------------------
typename quetzal::direct3d11::XMFLOAT3_Traits::rep_type& quetzal::direct3d11::XMFLOAT3_Traits::assign_add(rep_type& lhs, const rep_type& rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

//------------------------------------------------------------------------------
typename quetzal::direct3d11::XMFLOAT3_Traits::rep_type& quetzal::direct3d11::XMFLOAT3_Traits::assign_subtract(rep_type& lhs, const rep_type& rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
}

//------------------------------------------------------------------------------
typename quetzal::direct3d11::XMFLOAT3_Traits::rep_type& quetzal::direct3d11::XMFLOAT3_Traits::assign_multiply(rep_type& lhs, const float& rhs)
{
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.z *= rhs;
    return lhs;
}

//------------------------------------------------------------------------------
typename quetzal::direct3d11::XMFLOAT3_Traits::rep_type& quetzal::direct3d11::XMFLOAT3_Traits::assign_divide(rep_type& lhs, const float& rhs)
{
    lhs.x /= rhs;
    lhs.y /= rhs;
    lhs.z /= rhs;
    return lhs;
}

//------------------------------------------------------------------------------
float quetzal::direct3d11::XMFLOAT3_Traits::dot_product(const rep_type& lhs, const rep_type& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

//------------------------------------------------------------------------------
bool quetzal::direct3d11::XMFLOAT3_Traits::zero(const rep_type& rep)
{
    return rep.x == 0.0f && rep.y == 0.0f && rep.z == 0.0f;
}

//------------------------------------------------------------------------------
bool quetzal::direct3d11::XMFLOAT3_Traits::equals(const rep_type& lhs, const rep_type& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

//------------------------------------------------------------------------------
bool quetzal::direct3d11::XMFLOAT3_Traits::less(const rep_type& lhs, const rep_type& rhs)
{
    if (lhs.z == rhs.z)
    {
        if (lhs.y == rhs.y)
        {
            return lhs.x < rhs.x;
        }
        return lhs.y < rhs.y;
    }
    return lhs.z < rhs.z;
}

//------------------------------------------------------------------------------
bool quetzal::direct3d11::XMFLOAT3_Traits::float_zero(const rep_type& rep, int ulp)
{
    return math::float_eq0(rep.x, ulp) && math::float_eq0(rep.y, ulp) && math::float_eq0(rep.z, ulp);
}

//------------------------------------------------------------------------------
bool quetzal::direct3d11::XMFLOAT3_Traits::float_equals(const rep_type& lhs, const rep_type& rhs, int ulp)
{
    return math::float_eq(lhs.x, rhs.x, ulp) && math::float_eq(lhs.y, rhs.y, ulp) && math::float_eq(lhs.z, rhs.z, ulp);
}

//------------------------------------------------------------------------------
typename quetzal::direct3d11::XMFLOAT3_Traits::rep_type quetzal::direct3d11::XMFLOAT3_Traits::round_zero(rep_type rep, int ulp)
{
    rep.x = math::round_zero(rep.x, ulp);
    rep.y = math::round_zero(rep.y, ulp);
    rep.z = math::round_zero(rep.z, ulp);
    return rep;
}

//------------------------------------------------------------------------------
std::istream& quetzal::direct3d11::XMFLOAT3_Traits::extract(std::istream& is, rep_type& rhs)
{
    is >> rhs.x >> rhs.y >> rhs.z;
    return is;
}

//------------------------------------------------------------------------------
std::ostream& quetzal::direct3d11::XMFLOAT3_Traits::insert(std::ostream& os, const rep_type& rhs)
{
    os << (rhs.x == 0.0f ? 0.0f : rhs.x) << " " << (rhs.y == 0.0f ? 0.0f : rhs.y) << " " << (rhs.z == 0.0f ? 0.0f : rhs.z);
    return os;
}
