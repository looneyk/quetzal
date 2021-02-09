//------------------------------------------------------------------------------
// Direct3D 11
// Camera.cpp
//------------------------------------------------------------------------------

#include "Camera.hpp"
#include <cmath>

using namespace std;
using namespace DirectX;

//------------------------------------------------------------------------------
quetzal::direct3d11::Camera::Camera(float fov_nominal) :
    m_position(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)),
    m_up(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)),
    m_right(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)),
    m_forward(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)),
    m_fov_nominal(fov_nominal),
    m_fov(fov_nominal),
    m_view(XMMatrixIdentity()),
    m_projection(XMMatrixIdentity())
{
}

//------------------------------------------------------------------------------
XMVECTOR quetzal::direct3d11::Camera::position() const
{
    return m_position;
}

//------------------------------------------------------------------------------
XMVECTOR quetzal::direct3d11::Camera::right() const
{
    return m_right;
}

//------------------------------------------------------------------------------
XMVECTOR quetzal::direct3d11::Camera::up() const
{
    return m_up;
}

//------------------------------------------------------------------------------
XMVECTOR quetzal::direct3d11::Camera::forward() const
{
    return m_forward;
}

//------------------------------------------------------------------------------
float quetzal::direct3d11::Camera::theta(XMVECTOR up) const
{
    return asin(XMVectorGetX(XMVector3Dot(m_forward, up)));
}

//------------------------------------------------------------------------------
float quetzal::direct3d11::Camera::phi() const
{
// cache last valid phi and return that if both components 0 ...
    return static_cast<float>(atan2(XMVectorGetY(m_forward), XMVectorGetX(m_forward)));
}

//------------------------------------------------------------------------------
float quetzal::direct3d11::Camera::fov() const
{
    return m_fov;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Camera::set_fov(float fov)
{
    m_fov = fov;
    return;
}

//------------------------------------------------------------------------------
float quetzal::direct3d11::Camera::zoom() const
{
    return 1.0f / (tan(m_fov / 2.0f) * tan(m_fov_nominal));
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Camera::set_zoom(float f)
{
    m_fov = 2.0f * atan(1.0f / (f * tan(m_fov_nominal)));
    return;
}

//------------------------------------------------------------------------------
XMMATRIX quetzal::direct3d11::Camera::view() const
{
    return m_view;
}

//------------------------------------------------------------------------------
XMMATRIX quetzal::direct3d11::Camera::projection() const
{
    return m_projection;
}

//------------------------------------------------------------------------------
XMMATRIX quetzal::direct3d11::Camera::view_projection() const
{
    return m_view * m_projection;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Camera::set_position(DirectX::XMVECTOR position)
{
    m_position = position;
    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Camera::set_target(DirectX::XMVECTOR target)
{
    set_direction(target - m_position);
    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Camera::set_direction(DirectX::XMVECTOR direction)
{
    m_forward = XMVector3Normalize(direction);
    m_right = XMVector3Normalize(XMVector3Cross(m_forward, m_up)); // if up and m_forward are in line, don't recalcuate right ...
    m_up = XMVector3Normalize(XMVector3Cross(m_right, m_forward));
    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Camera::pan(float radians)
{
    // Rotate right and forward vectors around the internal up vector
    XMMATRIX rotation = XMMatrixRotationAxis(m_up, radians);
    m_right = XMVector3Normalize(XMVector3TransformNormal(m_right, rotation));
    m_forward = XMVector3Normalize(XMVector3TransformNormal(m_forward, rotation));
    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Camera::pan(float radians, XMVECTOR up)
{
    // Rotate right and forward vectors around the explicit up vector
    XMMATRIX rotation = XMMatrixRotationAxis(up, radians);
    m_right = XMVector3Normalize(XMVector3TransformNormal(m_right, rotation));
//assert(XMVectorGetZ(m_right) == 0.0f);
    m_forward = XMVector3Normalize(XMVector3TransformNormal(m_forward, rotation));
    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Camera::tilt(float radians)
{
    // Rotate up and forward vectors around the right vector
    XMMATRIX rotation = XMMatrixRotationAxis(m_right, radians);
    m_up = XMVector3Normalize(XMVector3TransformNormal(m_up, rotation));
    m_forward = XMVector3Normalize(XMVector3TransformNormal(m_forward, rotation));
    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Camera::roll(float radians)
{
    // Rotate right and up vectors around the forward vector
    XMMATRIX rotation = XMMatrixRotationAxis(m_forward, radians);
    m_right = XMVector3Normalize(XMVector3TransformNormal(m_right, rotation));
    m_up = XMVector3Normalize(XMVector3TransformNormal(m_up, rotation));
    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Camera::look(XMVECTOR target, XMVECTOR up)
{
    m_forward = XMVector3Normalize(target - m_position);
    m_right = XMVector3Normalize(XMVector3Cross(m_forward, up)); // if up and m_forward are in line, don't recalcuate right ...
//assert(XMVectorGetZ(m_right) == 0.0f);
    m_up = XMVector3Normalize(XMVector3Cross(m_right, m_forward));
    update_view();
    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Camera::set_lens(float fov, float aspect, float fnear, float ffar)
{
    m_fov = fov;
    m_projection = XMMatrixPerspectiveFovRH(m_fov, aspect, fnear, ffar);
    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Camera::set_lens(float aspect, float fnear, float ffar)
{
    set_lens(m_fov, aspect, fnear, ffar);
    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Camera::set_lens_ortho(float w, float h, float fnear, float ffar)
{
    m_projection = XMMatrixOrthographicRH(w, h, fnear, ffar);
    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Camera::update_view()
{
    // This needed for some reason, and with this it matches the result of XMMatrixLookAtRH
    // (To match result of XMMatrixLookAtLH, right = -m_right is needed instead)
    auto forward = -m_forward;

    // Build view matrix from right, up, and forward vectors which should already be orthonormal

    float x = -XMVectorGetX(XMVector3Dot(m_position, m_right));
    float y = -XMVectorGetX(XMVector3Dot(m_position, m_up));
    float z = -XMVectorGetX(XMVector3Dot(m_position, forward));

    XMFLOAT4X4 view;

//assert(XMVectorGetZ(m_right) == 0.0f);
    view(0, 0) = XMVectorGetX(m_right);
    view(1, 0) = XMVectorGetY(m_right);
    view(2, 0) = XMVectorGetZ(m_right);
    view(3, 0) = x;

    view(0, 1) = XMVectorGetX(m_up);
    view(1, 1) = XMVectorGetY(m_up);
    view(2, 1) = XMVectorGetZ(m_up);
    view(3, 1) = y;

    view(0, 2) = XMVectorGetX(forward);
    view(1, 2) = XMVectorGetY(forward);
    view(2, 2) = XMVectorGetZ(forward);
    view(3, 2) = z;

    view(0, 3) = 0.0f;
    view(1, 3) = 0.0f;
    view(2, 3) = 0.0f;
    view(3, 3) = 1.0f;

    m_view = XMLoadFloat4x4(&view);
    return;
}
