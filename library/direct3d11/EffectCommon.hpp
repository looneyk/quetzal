#if !defined(QUETZAL_DIRECT3D11_EFFECTCOMMON_HPP)
#define QUETZAL_DIRECT3D11_EFFECTCOMMON_HPP
//------------------------------------------------------------------------------
// direct3d11
// EffectCommon.hpp
//------------------------------------------------------------------------------

#include "Effect.hpp"
#include "ConstantBuffer.hpp"
#include "VertexShaderManager.hpp"
#include "PixelShaderManager.hpp"
#include "Renderer.hpp"
#include "AlignedNew.hpp"
#include "common/ComException.hpp"
#include "common/com_ptr.hpp"
#include "DirectXHelpers.h"
#include <d3d11_1.h>

// breaks out common functionality into a set of helpers which can be assembled in different
// combinations to build up whatever subset is needed by each effect.

// Common classes that manage aspects of effect functionality

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    // Bitfield tracks which derived parameter values need to be recomputed.
    namespace EffectDirtyFlags
    {
        constexpr int ConstantBuffer        = 0x01;
        constexpr int WorldViewProj         = 0x02;
        constexpr int WorldInverseTranspose = 0x04;
        constexpr int EyePosition           = 0x08;
        constexpr int MaterialColor         = 0x10;
        constexpr int FogVector             = 0x20;
        constexpr int FogEnable             = 0x40;
        constexpr int AlphaTest             = 0x80;
    }

    //--------------------------------------------------------------------------
    // Helper stores matrix parameter values, and computes derived matrices.
    struct EffectMatrices
    {
        EffectMatrices() noexcept;

        DirectX::XMMATRIX world;
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX projection;
        DirectX::XMMATRIX worldView;

        void SetConstants(int& dirtyFlags, DirectX::XMMATRIX& worldViewProjConstant);
    };

    //--------------------------------------------------------------------------
    // Helper stores the current fog settings, and computes derived shader parameters.
    struct EffectFog
    {
        EffectFog() noexcept;

        bool enabled;
        float start;
        float end;

        void XM_CALLCONV SetConstants(int& dirtyFlags, DirectX::FXMMATRIX worldView, DirectX::XMVECTOR& fogVectorConstant);
    };

    //--------------------------------------------------------------------------
    // Helper stores material color settings, and computes derived parameters for shaders that do not support realtime lighting.
    struct EffectColor
    {
        EffectColor() noexcept;

        DirectX::XMVECTOR diffuseColor;
        float alpha;

        void SetConstants(int& dirtyFlags, DirectX::XMVECTOR& diffuseColorConstant);
    };

    //--------------------------------------------------------------------------
    // Helper stores the current light settings, and computes derived shader parameters.
    struct EffectLights : public EffectColor
    {
        EffectLights() noexcept;

        static constexpr int MaxDirectionalLights = IEffectLights::MaxDirectionalLights;

        // Fields.
        DirectX::XMVECTOR emissiveColor;
        DirectX::XMVECTOR ambientLightColor;

        bool lightEnabled[MaxDirectionalLights];
bool b[13]; // alignment warning ...
        DirectX::XMVECTOR lightDiffuseColor[MaxDirectionalLights];
        DirectX::XMVECTOR lightSpecularColor[MaxDirectionalLights];

        // Methods.
        void InitializeConstants(DirectX::XMVECTOR& specularColorAndPowerConstant, DirectX::XMVECTOR* lightDirectionConstant, DirectX::XMVECTOR* lightDiffuseConstant, DirectX::XMVECTOR* lightSpecularConstant) const;
        void SetConstants(int& dirtyFlags, EffectMatrices const& matrices, DirectX::XMMATRIX& worldConstant, DirectX::XMVECTOR worldInverseTransposeConstant[3], DirectX::XMVECTOR& eyePositionConstant, DirectX::XMVECTOR& diffuseColorConstant, DirectX::XMVECTOR& emissiveColorConstant, bool lightingEnabled);

        int SetLightEnabled(int nLight, bool bEnabled, DirectX::XMVECTOR* lightDiffuseConstant, DirectX::XMVECTOR* lightSpecularConstant);
        int XM_CALLCONV SetLightDiffuseColor(int nLight, DirectX::FXMVECTOR value, DirectX::XMVECTOR* lightDiffuseConstant);
        int XM_CALLCONV SetLightSpecularColor(int nLight, DirectX::FXMVECTOR value, DirectX::XMVECTOR* lightSpecularConstant);

        static void ValidateLightIndex(int nLight);
        static void EnableDefaultLighting(IEffectLights* effect);
    };

    //--------------------------------------------------------------------------
    // Templated base class provides functionality common to all the built-in effects.
    template<typename Traits>
    class EffectCommon : public AlignedNew<typename Traits::ConstantBufferType>
    {
    public:

        EffectCommon(Renderer& renderer) :
            m_renderer(renderer),
            m_mutex(),
            m_vertex_shaders(m_vertex_shader_manager, renderer),
            m_pixel_shaders(m_pixel_shader_manager, renderer),
            constants{},
            matrices(),
            fog(),
            m_pTexture(),
            dirtyFlags(INT_MAX),
            m_constant_buffer(renderer)
        {
        }

        //----------------------------------------------------------------------
        // Get the bytecode for the vertex shader for the given permutation
        // Client code needs this in order to create matching input layouts.
        ShaderBytecode vertex_shader_bytecode(int permutation) const
        {
            return m_vertex_shader_manager.bytecode(permutation);
        }

        //----------------------------------------------------------------------
        // Helper sets our shaders and constant buffers onto the D3D device.
        void apply_shaders(ID3D11DeviceContext* pDeviceContext, int permutation)
        {
            auto pVertexShader = m_vertex_shaders.open(permutation);
            auto pPixelShader = m_pixel_shaders.open(permutation);

            pDeviceContext->VSSetShader(pVertexShader, nullptr, 0);
            pDeviceContext->PSSetShader(pPixelShader, nullptr, 0);

            // Make sure the constant buffer is up to date.
            if (dirtyFlags & EffectDirtyFlags::ConstantBuffer)
            {
                m_constant_buffer.set(pDeviceContext, constants);

                dirtyFlags &= ~EffectDirtyFlags::ConstantBuffer;
            }

            // Set the constant buffer.
            ID3D11Buffer* pBuffer = m_constant_buffer.buffer();

            pDeviceContext->VSSetConstantBuffers(0, 1, &pBuffer);
            pDeviceContext->PSSetConstantBuffers(0, 1, &pBuffer);
        }

        //----------------------------------------------------------------------
        ID3D11ShaderResourceView* default_texture()
        {
            return checked_create(m_pTextureDefault, m_mutex,
                [&](ID3D11ShaderResourceView** ppShaderResourceView) -> HRESULT
                {
                    static constexpr uint32_t s_pixel = 0xffffffff;

                    D3D11_SUBRESOURCE_DATA initData = { &s_pixel, sizeof s_pixel, 0 };

                    D3D11_TEXTURE2D_DESC t2d_desc = {};
                    t2d_desc.Width = 1;
                    t2d_desc.Height = 1;
                    t2d_desc.MipLevels = 1;
                    t2d_desc.ArraySize = 1;
                    t2d_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                    t2d_desc.SampleDesc.Count = 1;
                    t2d_desc.Usage = D3D11_USAGE_IMMUTABLE;
                    t2d_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

                    com_ptr<ID3D11Texture2D> pTexture2D;
                    HRESULT hr = m_renderer.device()->CreateTexture2D(&t2d_desc, &initData, &pTexture2D.p());
                    if (SUCCEEDED(hr))
                    {
                        DirectX::SetDebugObjectName(pTexture2D.get(), "Texture2D");

                        D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
                        srv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
                        srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                        srv_desc.Texture2D.MipLevels = 1;

                        hr = m_renderer.device()->CreateShaderResourceView(pTexture2D, &srv_desc, ppShaderResourceView);
                        if (SUCCEEDED(hr))
                        {
                            DirectX::SetDebugObjectName(*ppShaderResourceView, "ShaderResourceView");
                        }
                    }

                    return hr;
                }
            );
        }

    protected:

        Renderer& m_renderer;
        std::mutex m_mutex;
        typename VertexShaderManager<Traits>::Shaders m_vertex_shaders;
        typename PixelShaderManager<Traits>::Shaders m_pixel_shaders;

public:
        typename Traits::ConstantBufferType constants;

        EffectMatrices matrices;
        EffectFog fog;

        com_ptr<ID3D11ShaderResourceView> m_pTexture;

        int dirtyFlags;

    private:

        // D3D constant buffer holds a copy of the same data as the public 'constants' field.
        ConstantBuffer<typename Traits::ConstantBufferType> m_constant_buffer;

        static VertexShaderManager<Traits> m_vertex_shader_manager;
        static PixelShaderManager<Traits> m_pixel_shader_manager;
        static com_ptr<ID3D11ShaderResourceView> m_pTextureDefault; // also per renderer? ...
    };

} // namespace quetzal::direct3d11

template<typename Traits>
quetzal::direct3d11::VertexShaderManager<Traits> quetzal::direct3d11::EffectCommon<Traits>::m_vertex_shader_manager;

template<typename Traits>
quetzal::direct3d11::PixelShaderManager<Traits> quetzal::direct3d11::EffectCommon<Traits>::m_pixel_shader_manager;

template<typename Traits>
quetzal::com_ptr<ID3D11ShaderResourceView> quetzal::direct3d11::EffectCommon<Traits>::m_pTextureDefault;

#endif // QUETZAL_DIRECT3D11_EFFECTCOMMON_HPP
