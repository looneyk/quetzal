//------------------------------------------------------------------------------
// direct3d11
// math_util.cpp
//------------------------------------------------------------------------------

#include "math_util.hpp"
#include <cfloat>
#include <cmath>

using namespace std;
using namespace DirectX;

//------------------------------------------------------------------------------
float quetzal::direct3d11::math::RandF()
{
    return (float)(rand()) / (float)RAND_MAX;
}

//------------------------------------------------------------------------------
float quetzal::direct3d11::math::RandF(float a, float b)
{
    return a + RandF()*(b-a);
}

//------------------------------------------------------------------------------
float quetzal::direct3d11::math::AngleFromXY(float x, float y)
{
    float theta = 0.0f;

    // Quadrant I or IV
    if (x >= 0.0f)
    {
        // If x = 0, then atan(y / x) = +pi/2 if y > 0
        //                atan(y / x) = -pi/2 if y < 0
        theta = atan(y / x); // in [-pi/2, +pi/2]

        if (theta < 0.0f)
        {
            theta += XM_2PI; // in [0, 2*pi).
        }
    }

    // Quadrant II or III
    else
    {
        theta = atan(y / x) + XM_PI; // in [0, 2*pi).
    }

    return theta;
}

//------------------------------------------------------------------------------
XMMATRIX quetzal::direct3d11::math::InverseTranspose(CXMMATRIX M)
{
    // Inverse-transpose is just applied to normals.
    // So zero out translation row so that it doesn't get into our inverse-transpose calculation
    // we don't want the inverse-transpose of the translation.
    XMMATRIX A = M;
    A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

    XMVECTOR det = XMMatrixDeterminant(A);
    return XMMatrixTranspose(XMMatrixInverse(&det, A));
}

//------------------------------------------------------------------------------
XMVECTOR quetzal::direct3d11::math::RandUnitVec3()
{
    XMVECTOR One  = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
    XMVECTOR Zero = XMVectorZero();

    // Keep trying until we get a point on/in the hemisphere.
    while(true)
    {
        // Generate random point in the cube [-1,1]^3.
        XMVECTOR v = XMVectorSet(RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), 0.0f);

        // Ignore points outside the unit sphere in order to get an even distribution
        // over the unit sphere.  Otherwise points will clump more on the sphere near
        // the corners of the cube.

        if( XMVector3Greater( XMVector3LengthSq(v), One) )
            continue;

        return XMVector3Normalize(v);
    }
}

//------------------------------------------------------------------------------
XMVECTOR quetzal::direct3d11::math::RandHemisphereUnitVec3(XMVECTOR n)
{
    XMVECTOR One  = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
    XMVECTOR Zero = XMVectorZero();

    // Keep trying until we get a point on/in the hemisphere.
    while(true)
    {
        // Generate random point in the cube [-1,1]^3.
        XMVECTOR v = XMVectorSet(RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), 0.0f);

        // Ignore points outside the unit sphere in order to get an even distribution
        // over the unit sphere.  Otherwise points will clump more on the sphere near
        // the corners of the cube.

        if( XMVector3Greater( XMVector3LengthSq(v), One) )
            continue;

        // Ignore points in the bottom hemisphere.
        if( XMVector3Less( XMVector3Dot(n, v), Zero ) )
            continue;

        return XMVector3Normalize(v);
    }
}


/*
angle = atan2(length(cross(a, b)), dot(a, b))

Which has the advantages of robustness and of not requiring that the input vectors be unit length.

Put together, you get something like this (pseudocode, untested):

vector3 c = cross(a, b);
float l = length(c);
if (l > epsilon)
{
    axis = c / l;
    angle = atan2(l, dot(a, b));
}
else if (dot(a, b) > 0f)
{
    // Nearly positively aligned; skip rotation, or compute axis and angle using other means
}
else
{
    // Nearly negatively aligned; axis is any vector perpendicular to either vector, and angle is 180 degrees
}


C = B x A (the cross-product of B and A). C is the axis of rotation, and arcsin(|C|) is the necessary rotation angle. 
*/

//------------------------------------------------------------------------------
float quetzal::direct3d11::math::rotation_angle(XMVECTOR from, XMVECTOR to)
{
// XMVector3AngleBetweenVectors? only if issue below doesn't apply ...
    return rotation_angle_normal(XMVector3Normalize(from), XMVector3Normalize(to));
}

//------------------------------------------------------------------------------
float quetzal::direct3d11::math::rotation_angle_normal(XMVECTOR from_normal, XMVECTOR to_normal)
{
    // First need to check if angle == 0 or 180 (normal.x or y == 0 for {0, 0, 1}, cross product length 0, ...) ...
    // does XMVector3AngleBetweenNormals handle this? ...
    auto vangle = XMVector3AngleBetweenNormals(from_normal, to_normal);
    return XMVectorGetX(vangle);
}

//------------------------------------------------------------------------------
bool operator==(const XMFLOAT2& lhs, const XMFLOAT2& rhs)
{
    return rhs.x == lhs.x && rhs.y == lhs.y;
}

//------------------------------------------------------------------------------
bool operator!=(const XMFLOAT2& lhs, const XMFLOAT2& rhs)
{
    return !operator==(rhs, lhs);
}

//------------------------------------------------------------------------------
bool operator==(const XMFLOAT3& lhs, const XMFLOAT3& rhs)
{
    return rhs.x == lhs.x && rhs.y == lhs.y && rhs.z == lhs.z;
}

//------------------------------------------------------------------------------
bool operator!=(const XMFLOAT3& lhs, const XMFLOAT3& rhs)
{
    return !operator==(rhs, lhs);
}

//------------------------------------------------------------------------------
XMFLOAT3 operator+(const XMFLOAT3& lhs, const XMFLOAT3& rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

//------------------------------------------------------------------------------
XMFLOAT3 operator-(const XMFLOAT3& lhs, const XMFLOAT3& rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

//------------------------------------------------------------------------------
XMFLOAT3 operator*(const XMFLOAT3& lhs, float f)
{
    return {lhs.x * f, lhs.y * f, lhs.z * f};
}

//------------------------------------------------------------------------------
XMFLOAT3 operator*(float f, const XMFLOAT3& rhs)
{
    return {f * rhs.x, f * rhs.y, f * rhs.z};
}

//------------------------------------------------------------------------------
ostream& operator<<(ostream& os, const XMVECTOR& v)
{
    os << XMVectorGetX(v) << " " << XMVectorGetY(v) << " " << XMVectorGetZ(v);
    return os;
}

//------------------------------------------------------------------------------
ostream& operator<<(ostream& os, const XMFLOAT2& f2)
{
    os << f2.x << " " << f2.y;
    return os;
}

//------------------------------------------------------------------------------
ostream& operator<<(ostream& os, const XMFLOAT3& f3)
{
    os << f3.x << " " << f3.y << " " << f3.z;
    return os;
}

//------------------------------------------------------------------------------
ostream& operator<<(ostream& os, const XMFLOAT4& f4)
{
    os << f4.x << " " << f4.y << " " << f4.z << " " << f4.w;
    return os;
}

//------------------------------------------------------------------------------
XMFLOAT3 to_xmfloat3(XMVECTOR v)
{
    XMFLOAT3 f3;
    XMStoreFloat3(&f3, v);
    return f3;
}

//------------------------------------------------------------------------------
XMFLOAT4 to_xmfloat4(XMVECTOR v)
{
    XMFLOAT4 f4;
    XMStoreFloat4(&f4, v);
    return f4;
}
