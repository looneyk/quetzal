#if !defined(QUETZAL_DIRECT3D11_EFFECTSKINNED_HPP)
#define QUETZAL_DIRECT3D11_EFFECTSKINNED_HPP
//------------------------------------------------------------------------------
// direct3d11
// EffectSkinned.hpp
//------------------------------------------------------------------------------

#include "EffectCommon.hpp"

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    // Built-in shader supports skinned animation.
    class EffectSkinned : public IEffect, public IEffectMatrices, public IEffectLights, public IEffectFog, public IEffectSkinning
    {
    public:

        explicit EffectSkinned(Renderer& renderer);
        EffectSkinned(EffectSkinned const&) = delete;
        EffectSkinned(EffectSkinned&& other);

        EffectSkinned& operator=(EffectSkinned const&) = delete;
        EffectSkinned& operator=(EffectSkinned&& other);

        virtual ~EffectSkinned() = default;

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

        // Texture setting.
        void SetTexture(ID3D11ShaderResourceView* pShaderResourceView);

        // Animation settings.
        void SetWeightsPerVertex(int value) override;
        void SetBoneTransforms(DirectX::XMMATRIX const* value, size_t count) override;
        void ResetBoneTransforms() override;

        // Normal compression settings.
        void SetBiasedVertexNormals(bool value);

    private:
        // Private implementation.
        class Impl;

        std::unique_ptr<Impl> pImpl;

        // Unsupported interface method.
        void SetLightingEnabled(bool bEnabled) override;
    };

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_EFFECTSKINNED_HPP
