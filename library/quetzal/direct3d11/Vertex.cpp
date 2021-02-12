//------------------------------------------------------------------------------
// direct3d11
// Vertex.cpp
//------------------------------------------------------------------------------

#include "Vertex.hpp"
#include <DirectXPackedVector.h>

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

const quetzal::direct3d11::VertexPosition::input_elements_type quetzal::direct3d11::VertexPosition::m_input_elements
{{
    {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
}};

const quetzal::direct3d11::VertexPositionNormal::input_elements_type quetzal::direct3d11::VertexPositionNormal::m_input_elements
{{
    {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
}};

const quetzal::direct3d11::VertexPositionTexture::input_elements_type quetzal::direct3d11::VertexPositionTexture::m_input_elements
{{
    {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
}};

const quetzal::direct3d11::VertexPositionColor::input_elements_type quetzal::direct3d11::VertexPositionColor::m_input_elements
{{
    {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
}};

const quetzal::direct3d11::VertexPositionNormalTexture::input_elements_type quetzal::direct3d11::VertexPositionNormalTexture::m_input_elements
{{
    {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
}};

const quetzal::direct3d11::VertexPositionNormalColor::input_elements_type quetzal::direct3d11::VertexPositionNormalColor::m_input_elements
{{
    {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
}};

const quetzal::direct3d11::VertexPositionTextureColor::input_elements_type quetzal::direct3d11::VertexPositionTextureColor::m_input_elements
{{
    {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
}};

const quetzal::direct3d11::VertexPositionNormalTextureTangent::input_elements_type quetzal::direct3d11::VertexPositionNormalTextureTangent::m_input_elements
{{
    {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TANGENT",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
}};

const quetzal::direct3d11::VertexPositionNormalTextureColor::input_elements_type quetzal::direct3d11::VertexPositionNormalTextureColor::m_input_elements
{{
    {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
}};

const quetzal::direct3d11::VertexPositionNormalTextureTangentColor::input_elements_type quetzal::direct3d11::VertexPositionNormalTextureTangentColor::m_input_elements
{{
    {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TANGENT",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
}};

const quetzal::direct3d11::VertexPositionDualTexture::input_elements_type quetzal::direct3d11::VertexPositionDualTexture::m_input_elements
{{
    {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD",    1, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
}};

const quetzal::direct3d11::VertexPositionNormalTangentColorTexture::input_elements_type quetzal::direct3d11::VertexPositionNormalTangentColorTexture::m_input_elements
{{
    {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TANGENT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
}};

const quetzal::direct3d11::VertexPositionNormalTangentColorTextureSkinning::input_elements_type quetzal::direct3d11::VertexPositionNormalTangentColorTextureSkinning::m_input_elements
{{
    {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TANGENT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"COLOR",       0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"BLENDINDICES",0, DXGI_FORMAT_R8G8B8A8_UINT,      0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    {"BLENDWEIGHT", 0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
}};

//------------------------------------------------------------------------------
quetzal::direct3d11::VertexPosition::VertexPosition(const vector3_type& position) :
    m_position(position)
{
}

//------------------------------------------------------------------------------
const quetzal::direct3d11::VertexPosition::input_elements_type& quetzal::direct3d11::VertexPosition::input_elements()
{
    return m_input_elements;
}

//------------------------------------------------------------------------------
size_t quetzal::direct3d11::VertexPosition::stride()
{
    return sizeof (VertexPosition);
}

//------------------------------------------------------------------------------
quetzal::direct3d11::VertexPositionNormal::VertexPositionNormal(const vector3_type& position, const vector3_type& normal) :
    m_position(position),
    m_normal(normal)
{
}

//------------------------------------------------------------------------------
const quetzal::direct3d11::VertexPositionNormal::input_elements_type& quetzal::direct3d11::VertexPositionNormal::input_elements()
{
    return m_input_elements;
}

//------------------------------------------------------------------------------
size_t quetzal::direct3d11::VertexPositionNormal::stride()
{
    return sizeof (VertexPositionNormal);
}

//------------------------------------------------------------------------------
quetzal::direct3d11::VertexPositionTexture::VertexPositionTexture(const vector3_type& position, const vector2_type& texture) :
    m_position(position),
    m_texture(texture)
{
}

//------------------------------------------------------------------------------
const quetzal::direct3d11::VertexPositionTexture::input_elements_type& quetzal::direct3d11::VertexPositionTexture::input_elements()
{
    return m_input_elements;
}

//------------------------------------------------------------------------------
size_t quetzal::direct3d11::VertexPositionTexture::stride()
{
    return sizeof (VertexPositionTexture);
}

//------------------------------------------------------------------------------
quetzal::direct3d11::VertexPositionColor::VertexPositionColor(const vector3_type& position, const XMFLOAT4& color) :
    m_position(position),
    m_color(color)
{
}

//------------------------------------------------------------------------------
const quetzal::direct3d11::VertexPositionColor::input_elements_type& quetzal::direct3d11::VertexPositionColor::input_elements()
{
    return m_input_elements;
}

//------------------------------------------------------------------------------
size_t quetzal::direct3d11::VertexPositionColor::stride()
{
    return sizeof (VertexPositionColor);
}

//------------------------------------------------------------------------------
quetzal::direct3d11::VertexPositionNormalTexture::VertexPositionNormalTexture(const vector3_type& position, const vector3_type& normal, const vector2_type& texture) :
    m_position(position),
    m_normal(normal),
    m_texture(texture)
{
}

//------------------------------------------------------------------------------
const quetzal::direct3d11::VertexPositionNormalTexture::input_elements_type& quetzal::direct3d11::VertexPositionNormalTexture::input_elements()
{
    return m_input_elements;
}

//------------------------------------------------------------------------------
size_t quetzal::direct3d11::VertexPositionNormalTexture::stride()
{
    return sizeof (VertexPositionNormalTexture);
}

//------------------------------------------------------------------------------
quetzal::direct3d11::VertexPositionNormalColor::VertexPositionNormalColor(const vector3_type& position, const vector3_type& normal, const XMFLOAT4& color) :
    m_position(position),
    m_normal(normal),
    m_color(color)
{
}

//------------------------------------------------------------------------------
const quetzal::direct3d11::VertexPositionNormalColor::input_elements_type& quetzal::direct3d11::VertexPositionNormalColor::input_elements()
{
    return m_input_elements;
}

//------------------------------------------------------------------------------
size_t quetzal::direct3d11::VertexPositionNormalColor::stride()
{
    return sizeof (VertexPositionNormalColor);
}

//------------------------------------------------------------------------------
quetzal::direct3d11::VertexPositionTextureColor::VertexPositionTextureColor(const vector3_type& position, const vector2_type& texture, const XMFLOAT4& color) :
    m_position(position),
    m_texture(texture),
    m_color(color)
{
}

//------------------------------------------------------------------------------
const quetzal::direct3d11::VertexPositionTextureColor::input_elements_type& quetzal::direct3d11::VertexPositionTextureColor::input_elements()
{
    return m_input_elements;
}

//------------------------------------------------------------------------------
size_t quetzal::direct3d11::VertexPositionTextureColor::stride()
{
    return sizeof (VertexPositionTextureColor);
}

//------------------------------------------------------------------------------
quetzal::direct3d11::VertexPositionNormalTextureTangent::VertexPositionNormalTextureTangent(const vector3_type& position, const vector3_type& normal, const vector2_type& texture, const vector3_type& tangent) :
    m_position(position),
    m_normal(normal),
    m_texture(texture),
    m_tangent(tangent)
{
}

//------------------------------------------------------------------------------
const quetzal::direct3d11::VertexPositionNormalTextureTangent::input_elements_type& quetzal::direct3d11::VertexPositionNormalTextureTangent::input_elements()
{
    return m_input_elements;
}

//------------------------------------------------------------------------------
size_t quetzal::direct3d11::VertexPositionNormalTextureTangent::stride()
{
    return sizeof (VertexPositionNormalTextureTangent);
}

//------------------------------------------------------------------------------
quetzal::direct3d11::VertexPositionNormalTextureColor::VertexPositionNormalTextureColor(const vector3_type& position, const vector3_type& normal, const vector2_type& texture, const XMFLOAT4& color) :
    m_position(position),
    m_normal(normal),
    m_texture(texture),
    m_color(color)
{
}

//------------------------------------------------------------------------------
const quetzal::direct3d11::VertexPositionNormalTextureColor::input_elements_type& quetzal::direct3d11::VertexPositionNormalTextureColor::input_elements()
{
    return m_input_elements;
}

//------------------------------------------------------------------------------
size_t quetzal::direct3d11::VertexPositionNormalTextureColor::stride()
{
    return sizeof (VertexPositionNormalTextureColor);
}

//------------------------------------------------------------------------------
quetzal::direct3d11::VertexPositionNormalTextureTangentColor::VertexPositionNormalTextureTangentColor(const vector3_type& position, const vector3_type& normal, const vector2_type& texture, const vector3_type& tangent, const XMFLOAT4& color) :
    m_position(position),
    m_normal(normal),
    m_texture(texture),
    m_tangent(tangent),
    m_color(color)
{
}

//------------------------------------------------------------------------------
const quetzal::direct3d11::VertexPositionNormalTextureTangentColor::input_elements_type& quetzal::direct3d11::VertexPositionNormalTextureTangentColor::input_elements()
{
    return m_input_elements;
}

//------------------------------------------------------------------------------
size_t quetzal::direct3d11::VertexPositionNormalTextureTangentColor::stride()
{
    return sizeof (VertexPositionNormalTextureTangentColor);
}

//------------------------------------------------------------------------------
quetzal::direct3d11::VertexPositionDualTexture::VertexPositionDualTexture(const vector3_type& position, const vector2_type& texture0, const vector2_type& texture1) :
    m_position(position),
    m_texture0(texture0),
    m_texture1(texture1)
{
}

//------------------------------------------------------------------------------
const quetzal::direct3d11::VertexPositionDualTexture::input_elements_type& quetzal::direct3d11::VertexPositionDualTexture::input_elements()
{
    return m_input_elements;
}

//------------------------------------------------------------------------------
size_t quetzal::direct3d11::VertexPositionDualTexture::stride()
{
    return sizeof (VertexPositionDualTexture);
}

//------------------------------------------------------------------------------
quetzal::direct3d11::VertexPositionNormalTangentColorTexture::VertexPositionNormalTangentColorTexture(const vector3_type& position, const vector3_type& normal, const vector4_type& tangent, const XMFLOAT4& color, const vector2_type& texture) :
    m_position(position),
    m_normal(normal),
    m_tangent(tangent),
    m_texture(texture)
{
    set_color(color);
}

//------------------------------------------------------------------------------
quetzal::direct3d11::VertexPositionNormalTangentColorTexture::VertexPositionNormalTangentColorTexture(const vector3_type& position, const vector3_type& normal, const vector4_type& tangent, uint32_t color, const vector2_type& texture) :
    m_position(position),
    m_normal(normal),
    m_tangent(tangent),
    m_color(color),
    m_texture(texture)
{
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::VertexPositionNormalTangentColorTexture::set_color(const XMFLOAT4& color)
{
    set_color(XMLoadFloat4(&color));
    return;
}

//------------------------------------------------------------------------------
void XM_CALLCONV quetzal::direct3d11::VertexPositionNormalTangentColorTexture::set_color(FXMVECTOR color)
{
    XMUBYTEN4 rgba;
    XMStoreUByteN4(&rgba, color);
    m_color = rgba.v;
    return;
}

//------------------------------------------------------------------------------
const quetzal::direct3d11::VertexPositionNormalTangentColorTexture::input_elements_type& quetzal::direct3d11::VertexPositionNormalTangentColorTexture::input_elements()
{
    return m_input_elements;
}

//------------------------------------------------------------------------------
size_t quetzal::direct3d11::VertexPositionNormalTangentColorTexture::stride()
{
    return sizeof (VertexPositionNormalTangentColorTexture);
}

//------------------------------------------------------------------------------
quetzal::direct3d11::VertexPositionNormalTangentColorTextureSkinning::VertexPositionNormalTangentColorTextureSkinning(const vector3_type& position, const vector3_type& normal, const vector4_type& tangent, const XMFLOAT4& color, const vector2_type& texture, const XMUINT4& indices, const XMFLOAT4& weights) :
    VertexPositionNormalTangentColorTexture(position, normal, tangent, color, texture)
{
    set_blend_indices(indices);
    set_blend_weights(weights);
}

//------------------------------------------------------------------------------
quetzal::direct3d11::VertexPositionNormalTangentColorTextureSkinning::VertexPositionNormalTangentColorTextureSkinning(const vector3_type& position, const vector3_type& normal, const vector4_type& tangent, uint32_t color, const vector2_type& texture, const XMUINT4& indices, const XMFLOAT4& weights) :
    VertexPositionNormalTangentColorTexture(position, normal, tangent, color, texture)
{
    set_blend_indices(indices);
    set_blend_weights(weights);
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::VertexPositionNormalTangentColorTextureSkinning::set_blend_indices(const XMUINT4& indices)
{
    m_indices = ((indices.w & 0xff) << 24) | ((indices.z & 0xff) << 16) | ((indices.y & 0xff) << 8) | (indices.x & 0xff);
    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::VertexPositionNormalTangentColorTextureSkinning::set_blend_weights(const XMFLOAT4& weights)
{
    set_blend_weights(XMLoadFloat4(&weights));
    return;
}

//------------------------------------------------------------------------------
void XM_CALLCONV quetzal::direct3d11::VertexPositionNormalTangentColorTextureSkinning::set_blend_weights(FXMVECTOR weights)
{
    XMUBYTEN4 packed;
    XMStoreUByteN4(&packed, weights);
    m_weights = packed.v;
    return;
}

//------------------------------------------------------------------------------
const quetzal::direct3d11::VertexPositionNormalTangentColorTextureSkinning::input_elements_type& quetzal::direct3d11::VertexPositionNormalTangentColorTextureSkinning::input_elements()
{
    return m_input_elements;
}

//------------------------------------------------------------------------------
size_t quetzal::direct3d11::VertexPositionNormalTangentColorTextureSkinning::stride()
{
    return sizeof (VertexPositionNormalTangentColorTextureSkinning);
}
