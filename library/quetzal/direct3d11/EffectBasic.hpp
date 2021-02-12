#if !defined(QUETZAL_DIRECT3D11_EFFECTBASIC_HPP)
#define QUETZAL_DIRECT3D11_EFFECTBASIC_HPP
//------------------------------------------------------------------------------
// direct3d11
// EffectBasic.hpp
//------------------------------------------------------------------------------

#include "EffectCommon.hpp"

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    // Built-in shader supports optional texture mapping, vertex coloring, directional lighting, and fog.
    class EffectBasic : public IEffect, public IEffectMatrices, public IEffectLights, public IEffectFog
    {
    public:

        explicit EffectBasic(Renderer& renderer);
        EffectBasic(EffectBasic const&) = delete;
        EffectBasic(EffectBasic&& other);

        EffectBasic& operator=(EffectBasic const&) = delete;
        EffectBasic& operator=(EffectBasic&& other);

        virtual ~EffectBasic() = default;

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
        void XM_CALLCONV SetEmissiveColor(DirectX::FXMVECTOR value);
        void XM_CALLCONV SetSpecularColor(DirectX::FXMVECTOR value);
        void SetSpecularPower(float value);
        void DisableSpecular();
        void SetAlpha(float value);
        void XM_CALLCONV SetColorAndAlpha(DirectX::FXMVECTOR value);

        // Light settings.
        void SetLightingEnabled(bool bEnabled) override;
        void SetPerPixelLighting(bool value) override;
        void XM_CALLCONV SetAmbientLightColor(DirectX::FXMVECTOR value) override;

        void SetLightEnabled(int nLight, bool bEnabled) override;
        void XM_CALLCONV SetLightDirection(int nLight, DirectX::FXMVECTOR value) override;
        void XM_CALLCONV SetLightDiffuseColor(int nLight, DirectX::FXMVECTOR value) override;
        void XM_CALLCONV SetLightSpecularColor(int nLight, DirectX::FXMVECTOR value) override;

        void EnableDefaultLighting() override;

        // Fog settings.
        void SetFogEnabled(bool bEnabled) override;
        void SetFogStart(float value) override;
        void SetFogEnd(float value) override;
        void XM_CALLCONV SetFogColor(DirectX::FXMVECTOR value) override;

        // Vertex color setting.
        void SetVertexColorEnabled(bool bEnabled);

        // Texture setting.
        void SetTextureEnabled(bool bEnabled);
        void SetTexture(ID3D11ShaderResourceView* pShaderResourceView);

        // Normal compression settings.
        void SetBiasedVertexNormals(bool value);

    private:

        // Private implementation.
        class Impl;

        std::unique_ptr<Impl> pImpl;
    };

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_EFFECTBASIC_HPP
