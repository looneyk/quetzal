#if !defined(QUETZAL_DIRECT3D11_VERTEX_HPP)
#define QUETZAL_DIRECT3D11_VERTEX_HPP
//------------------------------------------------------------------------------
// direct3d11
// Vertex.hpp
//
// VertexPosition
// VertexPositionNormal
// VertexPositionTexture
// VertexPositionColor
// VertexPositionNormalTexture
// VertexPositionNormalColor
// VertexPositionTextureColor
// VertexPositionNormalTextureTangent
// VertexPositionNormalTextureColor
// VertexPositionNormalTextureTangentColor
// VertexPositionDualTexture
// VertexPositionNormalTangentColorTexture
// VertexPositionNormalTangentColorTextureSkinning
//
//------------------------------------------------------------------------------

#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include <d3d11.h>
#include <DirectXMath.h>
#include <array>

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    class VertexPosition
    {
    public:

        using vector2_type = direct3d11::Vector2;
        using vector3_type = direct3d11::Vector3;
        using input_elements_type = std::array<D3D11_INPUT_ELEMENT_DESC, 1>;

        VertexPosition() = default;
        VertexPosition(const vector3_type& position);
        VertexPosition(const VertexPosition&) = default;
        ~VertexPosition() = default;

        VertexPosition& operator=(const VertexPosition&) = default;

        vector3_type m_position;

        static const input_elements_type& input_elements();
        static size_t stride();

    private:

        static const input_elements_type m_input_elements;
    };

    //--------------------------------------------------------------------------
    class VertexPositionNormal
    {
    public:

        using vector2_type = direct3d11::Vector2;
        using vector3_type = direct3d11::Vector3;
        using input_elements_type = std::array<D3D11_INPUT_ELEMENT_DESC, 2>;

        VertexPositionNormal() = default;
        VertexPositionNormal(const vector3_type& position, const vector3_type& normal);
        VertexPositionNormal(const VertexPositionNormal&) = default;
        ~VertexPositionNormal() = default;

        VertexPositionNormal& operator=(const VertexPositionNormal&) = default;

        vector3_type m_position;
        vector3_type m_normal;

        static const input_elements_type& input_elements();
        static size_t stride();

    private:

        static const input_elements_type m_input_elements;
    };

    //--------------------------------------------------------------------------
    class VertexPositionTexture
    {
    public:

        using vector2_type = direct3d11::Vector2;
        using vector3_type = direct3d11::Vector3;
        using input_elements_type = std::array<D3D11_INPUT_ELEMENT_DESC, 2>;

        VertexPositionTexture() = default;
        VertexPositionTexture(const vector3_type& position, const vector2_type& texture);
        VertexPositionTexture(const VertexPositionTexture&) = default;
        ~VertexPositionTexture() = default;

        VertexPositionTexture& operator=(const VertexPositionTexture&) = default;

        vector3_type m_position;
        vector2_type m_texture;

        static const input_elements_type& input_elements();
        static size_t stride();

    private:

        static const input_elements_type m_input_elements;
    };

    //--------------------------------------------------------------------------
    class VertexPositionColor
    {
    public:

        using vector2_type = direct3d11::Vector2;
        using vector3_type = direct3d11::Vector3;
        using input_elements_type = std::array<D3D11_INPUT_ELEMENT_DESC, 2>;

        VertexPositionColor() = default;
        VertexPositionColor(const vector3_type& position, const DirectX::XMFLOAT4& color);
        VertexPositionColor(const VertexPositionColor&) = default;
        ~VertexPositionColor() = default;

        VertexPositionColor& operator=(const VertexPositionColor&) = default;

        vector3_type m_position;
        DirectX::XMFLOAT4 m_color;

        static const input_elements_type& input_elements();
        static size_t stride();

    private:

        static const input_elements_type m_input_elements;
    };

    //--------------------------------------------------------------------------
    class VertexPositionNormalTexture
    {
    public:

        using vector2_type = direct3d11::Vector2;
        using vector3_type = direct3d11::Vector3;
        using input_elements_type = std::array<D3D11_INPUT_ELEMENT_DESC, 3>;

        VertexPositionNormalTexture() = default;
        VertexPositionNormalTexture(const vector3_type& position, const vector3_type& normal, const vector2_type& texture);
        VertexPositionNormalTexture(const VertexPositionNormalTexture&) = default;
        ~VertexPositionNormalTexture() = default;

        VertexPositionNormalTexture& operator=(const VertexPositionNormalTexture&) = default;

        vector3_type m_position;
        vector3_type m_normal;
        vector2_type m_texture;

        static const input_elements_type& input_elements();
        static size_t stride();

    private:

        static const input_elements_type m_input_elements;
    };

    //--------------------------------------------------------------------------
    class VertexPositionNormalColor
    {
    public:

        using vector2_type = direct3d11::Vector2;
        using vector3_type = direct3d11::Vector3;
        using input_elements_type = std::array<D3D11_INPUT_ELEMENT_DESC, 3>;

        VertexPositionNormalColor() = default;
        VertexPositionNormalColor(const vector3_type& position, const vector3_type& normal, const DirectX::XMFLOAT4& color);
        VertexPositionNormalColor(const VertexPositionNormalColor&) = default;
        ~VertexPositionNormalColor() = default;

        VertexPositionNormalColor& operator=(const VertexPositionNormalColor&) = default;

        vector3_type m_position;
        vector3_type m_normal;
        DirectX::XMFLOAT4 m_color;

        static const input_elements_type& input_elements();
        static size_t stride();

    private:

        static const input_elements_type m_input_elements;
    };

    //--------------------------------------------------------------------------
    class VertexPositionTextureColor
    {
    public:

        using vector2_type = direct3d11::Vector2;
        using vector3_type = direct3d11::Vector3;
        using input_elements_type = std::array<D3D11_INPUT_ELEMENT_DESC, 3>;

        VertexPositionTextureColor() = default;
        VertexPositionTextureColor(const vector3_type& position, const vector2_type& texture, const DirectX::XMFLOAT4& color);
        VertexPositionTextureColor(const VertexPositionTextureColor&) = default;
        ~VertexPositionTextureColor() = default;

        VertexPositionTextureColor& operator=(const VertexPositionTextureColor&) = default;

        vector3_type m_position;
        vector2_type m_texture;
        DirectX::XMFLOAT4 m_color;

        static const input_elements_type& input_elements();
        static size_t stride();

    private:

        static const input_elements_type m_input_elements;
    };

    //--------------------------------------------------------------------------
    class VertexPositionNormalTextureTangent
    {
    public:

        using vector2_type = direct3d11::Vector2;
        using vector3_type = direct3d11::Vector3;
        using input_elements_type = std::array<D3D11_INPUT_ELEMENT_DESC, 4>;

        VertexPositionNormalTextureTangent() = default;
        VertexPositionNormalTextureTangent(const vector3_type& position, const vector3_type& normal, const vector2_type& texture, const vector3_type& tangent);
        VertexPositionNormalTextureTangent(const VertexPositionNormalTextureTangent&) = default;
        ~VertexPositionNormalTextureTangent() = default;

        VertexPositionNormalTextureTangent& operator=(const VertexPositionNormalTextureTangent&) = default;

        vector3_type m_position;
        vector3_type m_normal;
        vector2_type m_texture;
        vector3_type m_tangent;

        static const input_elements_type& input_elements();
        static size_t stride();

    private:

        static const input_elements_type m_input_elements;
    };

    //--------------------------------------------------------------------------
    class VertexPositionNormalTextureColor
    {
    public:

        using vector2_type = direct3d11::Vector2;
        using vector3_type = direct3d11::Vector3;
        using input_elements_type = std::array<D3D11_INPUT_ELEMENT_DESC, 4>;

        VertexPositionNormalTextureColor() = default;
        VertexPositionNormalTextureColor(const vector3_type& position, const vector3_type& normal, const vector2_type& texture, const DirectX::XMFLOAT4& color);
        VertexPositionNormalTextureColor(const VertexPositionNormalTextureColor&) = default;
        ~VertexPositionNormalTextureColor() = default;

        VertexPositionNormalTextureColor& operator=(const VertexPositionNormalTextureColor&) = default;

        vector3_type m_position;
        vector3_type m_normal;
        vector2_type m_texture;
        DirectX::XMFLOAT4 m_color;

        static const input_elements_type& input_elements();
        static size_t stride();

    private:

        static const input_elements_type m_input_elements;
    };

    //--------------------------------------------------------------------------
    class VertexPositionNormalTextureTangentColor
    {
    public:

        using vector2_type = direct3d11::Vector2;
        using vector3_type = direct3d11::Vector3;
        using input_elements_type = std::array<D3D11_INPUT_ELEMENT_DESC, 5>;

        VertexPositionNormalTextureTangentColor() = default;
        VertexPositionNormalTextureTangentColor(const vector3_type& position, const vector3_type& normal, const vector2_type& texture, const vector3_type& tangent, const DirectX::XMFLOAT4& color);
        VertexPositionNormalTextureTangentColor(const VertexPositionNormalTextureTangentColor&) = default;
        ~VertexPositionNormalTextureTangentColor() = default;

        VertexPositionNormalTextureTangentColor& operator=(const VertexPositionNormalTextureTangentColor&) = default;

        vector3_type m_position;
        vector3_type m_normal;
        vector2_type m_texture;
        vector3_type m_tangent;
        DirectX::XMFLOAT4 m_color;

        static const input_elements_type& input_elements();
        static size_t stride();

    private:

        static const input_elements_type m_input_elements;
    };

    //--------------------------------------------------------------------------
    class VertexPositionDualTexture
    {
    public:

        using vector2_type = direct3d11::Vector2;
        using vector3_type = direct3d11::Vector3;
        using input_elements_type = std::array<D3D11_INPUT_ELEMENT_DESC, 3>;

        VertexPositionDualTexture() = default;
        VertexPositionDualTexture(const vector3_type& position, const vector2_type& texture0, const vector2_type& texture1);
        VertexPositionDualTexture(const VertexPositionDualTexture&) = default;
        ~VertexPositionDualTexture() = default;

        VertexPositionDualTexture& operator=(const VertexPositionDualTexture&) = default;

        vector3_type m_position;
        vector2_type m_texture0;
        vector2_type m_texture1;

        static const input_elements_type& input_elements();
        static size_t stride();

    private:

        static const input_elements_type m_input_elements;
    };

    //--------------------------------------------------------------------------
    class VertexPositionNormalTangentColorTexture
    {
    public:

        using vector2_type = direct3d11::Vector2;
        using vector3_type = direct3d11::Vector3;
        using vector4_type = direct3d11::Vector4;
        using input_elements_type = std::array<D3D11_INPUT_ELEMENT_DESC, 5>;

        VertexPositionNormalTangentColorTexture() = default;
        VertexPositionNormalTangentColorTexture(const vector3_type& position, const vector3_type& normal, const vector4_type& tangent, const DirectX::XMFLOAT4& color, const vector2_type& texture);
        VertexPositionNormalTangentColorTexture(const vector3_type& position, const vector3_type& normal, const vector4_type& tangent, uint32_t color, const vector2_type& texture);
        VertexPositionNormalTangentColorTexture(const VertexPositionNormalTangentColorTexture&) = default;
        ~VertexPositionNormalTangentColorTexture() = default;

        VertexPositionNormalTangentColorTexture& operator=(const VertexPositionNormalTangentColorTexture&) = default;

        void set_color(const DirectX::XMFLOAT4& color);
        void XM_CALLCONV set_color(DirectX::FXMVECTOR color);

        vector3_type m_position;
        vector3_type m_normal;
        vector4_type m_tangent;
        uint32_t m_color;
        vector2_type m_texture;

        static const input_elements_type& input_elements();
        static size_t stride();

    private:

        static const input_elements_type m_input_elements;
    };

    //--------------------------------------------------------------------------
    class VertexPositionNormalTangentColorTextureSkinning : public VertexPositionNormalTangentColorTexture
    {
    public:

        using vector2_type = direct3d11::Vector2;
        using vector3_type = direct3d11::Vector3;
        using vector4_type = direct3d11::Vector4;
        using input_elements_type = std::array<D3D11_INPUT_ELEMENT_DESC, 7>;

        VertexPositionNormalTangentColorTextureSkinning() = default;
        VertexPositionNormalTangentColorTextureSkinning(const vector3_type& position, const vector3_type& normal, const vector4_type& tangent, const DirectX::XMFLOAT4& color, const vector2_type& texture, const DirectX::XMUINT4& indices, const DirectX::XMFLOAT4& weights);
        VertexPositionNormalTangentColorTextureSkinning(const vector3_type& position, const vector3_type& normal, const vector4_type& tangent, uint32_t color, const vector2_type& texture, const DirectX::XMUINT4& indices, const DirectX::XMFLOAT4& weights);
        VertexPositionNormalTangentColorTextureSkinning(const VertexPositionNormalTangentColorTextureSkinning&) = default;
        ~VertexPositionNormalTangentColorTextureSkinning() = default;

        VertexPositionNormalTangentColorTextureSkinning& operator=(const VertexPositionNormalTangentColorTextureSkinning&) = default;

        void set_blend_indices(const DirectX::XMUINT4& indices);
        void set_blend_weights(const DirectX::XMFLOAT4& weights);
        void XM_CALLCONV set_blend_weights(DirectX::FXMVECTOR weights);

        uint32_t m_indices;
        uint32_t m_weights;

        static const input_elements_type& input_elements();
        static size_t stride();

    private:

        static const input_elements_type m_input_elements;
    };

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_VERTEX_HPP
