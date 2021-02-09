#if !defined(QUETZAL_DIRECT3D11_EFFECTDUALTEXTURE_HPP)
#define QUETZAL_DIRECT3D11_EFFECTDUALTEXTURE_HPP
//------------------------------------------------------------------------------
// direct3d11
// EffectDualTexture.hpp
//------------------------------------------------------------------------------

#include "EffectCommon.hpp"

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    // Built-in shader supports two layer multitexturing (eg. for lightmaps or detail textures).
    class EffectDualTexture : public IEffect, public IEffectMatrices, public IEffectFog
    {
    public:
        explicit EffectDualTexture(Renderer& renderer);
        EffectDualTexture(EffectDualTexture const&) = delete;
        EffectDualTexture(EffectDualTexture&& other);

        EffectDualTexture& operator=(EffectDualTexture const&) = delete;
        EffectDualTexture& operator=(EffectDualTexture&& other);

        virtual ~EffectDualTexture() = default;

        // IEffect methods.
        void apply(ID3D11DeviceContext* pDeviceContext) override;

        ShaderBytecode vertex_shader_bytecode() const override;

        // Camera settings.
        void XM_CALLCONV SetWorld(DirectX::FXMMATRIX value) override;
        void XM_CALLCONV SetView(DirectX::FXMMATRIX value) override;
        void XM_CALLCONV SetProjection(DirectX::FXMMATRIX value) override;
        void XM_CALLCONV SetMatrices(DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection) override;

        // Material settings.
        void XM_CALLCONV SetDiffuseColor(DirectX::FXMVECTOR value);
        void SetAlpha(float value);
        void XM_CALLCONV SetColorAndAlpha(DirectX::FXMVECTOR value);

        // Fog settings.
        void SetFogEnabled(bool bEnabled) override;
        void SetFogStart(float value) override;
        void SetFogEnd(float value) override;
        void XM_CALLCONV SetFogColor(DirectX::FXMVECTOR value) override;

        // Vertex color setting.
        void SetVertexColorEnabled(bool bEnabled);

        // Texture settings.
        void SetTexture(ID3D11ShaderResourceView* pShaderResourceView);
        void SetTexture2(ID3D11ShaderResourceView* pShaderResourceView);

    private:
        // Private implementation.
        class Impl;

        std::unique_ptr<Impl> pImpl;
    };

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_EFFECTDUALTEXTURE_HPP
