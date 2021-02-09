//------------------------------------------------------------------------------
// direct3d11
// Vector4.cpp
//------------------------------------------------------------------------------

#include "Vector4.hpp"
#include "math/floating_point.hpp"
#include <iostream>

using namespace std;
using namespace DirectX;

//------------------------------------------------------------------------------
void quetzal::direct3d11::XMFLOAT4_Traits::clear(rep_type& rep)
{
    rep.x = 0.0f;
    rep.y = 0.0f;
    rep.z = 0.0f;
    rep.w = 0.0f;
    return;
}

//------------------------------------------------------------------------------
typename quetzal::direct3d11::XMFLOAT4_Traits::rep_type& quetzal::direct3d11::XMFLOAT4_Traits::assign(rep_type& lhs, const float(&rhs)[4])
{
    lhs.x = rhs[0];
    lhs.y = rhs[1];
    lhs.z = rhs[2];
    lhs.w = rhs[2];
    return lhs;
}

//------------------------------------------------------------------------------
typename quetzal::direct3d11::XMFLOAT4_Traits::rep_type quetzal::direct3d11::XMFLOAT4_Traits::unary_minus(rep_type rep)
{
    rep.x = -rep.x;
    rep.y = -rep.y;
    rep.z = -rep.z;
    rep.w = -rep.w;
    return rep;
}

//------------------------------------------------------------------------------
typename quetzal::direct3d11::XMFLOAT4_Traits::rep_type& quetzal::direct3d11::XMFLOAT4_Traits::assign_add(rep_type& lhs, const rep_type& rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    lhs.w += rhs.w;
    return lhs;
}

//------------------------------------------------------------------------------
typename quetzal::direct3d11::XMFLOAT4_Traits::rep_type& quetzal::direct3d11::XMFLOAT4_Traits::assign_subtract(rep_type& lhs, const rep_type& rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    lhs.w -= rhs.w;
    return lhs;
}

//------------------------------------------------------------------------------
typename quetzal::direct3d11::XMFLOAT4_Traits::rep_type& quetzal::direct3d11::XMFLOAT4_Traits::assign_multiply(rep_type& lhs, const float& rhs)
{
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.z *= rhs;
    lhs.w *= rhs;
    return lhs;
}

//------------------------------------------------------------------------------
typename quetzal::direct3d11::XMFLOAT4_Traits::rep_type& quetzal::direct3d11::XMFLOAT4_Traits::assign_divide(rep_type& lhs, const float& rhs)
{
    lhs.x /= rhs;
    lhs.y /= rhs;
    lhs.z /= rhs;
    lhs.w /= rhs;
    return lhs;
}

//------------------------------------------------------------------------------
float quetzal::direct3d11::XMFLOAT4_Traits::dot_product(const rep_type& lhs, const rep_type& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

//------------------------------------------------------------------------------
bool quetzal::direct3d11::XMFLOAT4_Traits::zero(const rep_type& rep)
{
    return rep.x == 0.0f && rep.y == 0.0f && rep.z == 0.0f && rep.w == 0.0f;
}

//------------------------------------------------------------------------------
bool quetzal::direct3d11::XMFLOAT4_Traits::equals(const rep_type& lhs, const rep_type& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

//------------------------------------------------------------------------------
bool quetzal::direct3d11::XMFLOAT4_Traits::less(const rep_type& lhs, const rep_type& rhs)
{
    if (lhs.w == rhs.w)
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
    return lhs.w < rhs.w;
}

//------------------------------------------------------------------------------
bool quetzal::direct3d11::XMFLOAT4_Traits::float_zero(const rep_type& rep, int ulp)
{
    return math::float_eq0(rep.x, ulp) && math::float_eq0(rep.y, ulp) && math::float_eq0(rep.z, ulp) && math::float_eq0(rep.w, ulp);
}

//------------------------------------------------------------------------------
bool quetzal::direct3d11::XMFLOAT4_Traits::float_equals(const rep_type& lhs, const rep_type& rhs, int ulp)
{
    return math::float_eq(lhs.x, rhs.x, ulp) && math::float_eq(lhs.y, rhs.y, ulp) && math::float_eq(lhs.z, rhs.z, ulp) && math::float_eq(lhs.w, rhs.w, ulp);
}

//------------------------------------------------------------------------------
typename quetzal::direct3d11::XMFLOAT4_Traits::rep_type quetzal::direct3d11::XMFLOAT4_Traits::round_zero(rep_type rep, int ulp)
{
    rep.x = math::round_zero(rep.x, ulp);
    rep.y = math::round_zero(rep.y, ulp);
    rep.z = math::round_zero(rep.z, ulp);
    rep.w = math::round_zero(rep.w, ulp);
    return rep;
}

//------------------------------------------------------------------------------
std::istream& quetzal::direct3d11::XMFLOAT4_Traits::extract(std::istream& is, rep_type& rhs)
{
    is >> rhs.x >> rhs.y >> rhs.z >> rhs.w;
    return is;
}

//------------------------------------------------------------------------------
std::ostream& quetzal::direct3d11::XMFLOAT4_Traits::insert(std::ostream& os, const rep_type& rhs)
{
    os << (rhs.x == 0.0f ? 0.0f : rhs.x) << " " << (rhs.y == 0.0f ? 0.0f : rhs.y) << " " << (rhs.z == 0.0f ? 0.0f : rhs.z) << " " << (rhs.w == 0.0f ? 0.0f : rhs.w);
    return os;
}
