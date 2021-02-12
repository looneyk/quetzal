#if !defined(QUETZAL_DIRECT3D11_EFFECTPHYSICALLYBASED_HPP)
#define QUETZAL_DIRECT3D11_EFFECTPHYSICALLYBASED_HPP
//------------------------------------------------------------------------------
// direct3d11
// EffectPhysicallyBased.hpp
//------------------------------------------------------------------------------

#include "EffectCommon.hpp"

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    // Built-in shader for Physically-Based Rendering (Roughness/Metalness) with Image-based lighting
    class EffectPhysicallyBased : public IEffect, public IEffectMatrices, public IEffectLights
    {
    public:
        explicit EffectPhysicallyBased(Renderer& renderer);
        EffectPhysicallyBased(EffectPhysicallyBased const&) = delete;
        EffectPhysicallyBased(EffectPhysicallyBased&& other) noexcept;

        EffectPhysicallyBased& operator=(EffectPhysicallyBased const&) = delete;
        EffectPhysicallyBased& operator=(EffectPhysicallyBased&& other) noexcept;

        virtual ~EffectPhysicallyBased() = default;

        // IEffect methods.
        void apply(ID3D11DeviceContext* pDeviceContext) override;

        ShaderBytecode vertex_shader_bytecode() const override;

        // Camera settings.
        void XM_CALLCONV SetWorld(DirectX::FXMMATRIX value) override;
        void XM_CALLCONV SetView(DirectX::FXMMATRIX value) override;
        void XM_CALLCONV SetProjection(DirectX::FXMMATRIX value) override;
        void XM_CALLCONV SetMatrices(DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection) override;

        // Light settings.
        void SetLightEnabled(int nLight, bool bEnabled) override;
        void XM_CALLCONV SetLightDirection(int nLight, DirectX::FXMVECTOR value) override;
        void XM_CALLCONV SetLightDiffuseColor(int nLight, DirectX::FXMVECTOR value) override;

        void EnableDefaultLighting() override;

        // PBR Settings.
        void SetAlpha(float value);
        void XM_CALLCONV SetConstantAlbedo(DirectX::FXMVECTOR value);
        void SetConstantMetallic(float value);
        void SetConstantRoughness(float value);

        // Texture settings.
        void SetAlbedoTexture(ID3D11ShaderResourceView* value);
        void SetNormalTexture(ID3D11ShaderResourceView* value);
        void SetRMATexture(ID3D11ShaderResourceView* value);

        void SetEmissiveTexture(ID3D11ShaderResourceView* value);

        void SetSurfaceTextures(
            ID3D11ShaderResourceView* albedo,
            ID3D11ShaderResourceView* normal,
            ID3D11ShaderResourceView* roughnessMetallicAmbientOcclusion);

        void SetIBLTextures(
            ID3D11ShaderResourceView* radiance,
            int numRadianceMips,
            ID3D11ShaderResourceView* irradiance);

        // Normal compression settings.
        void SetBiasedVertexNormals(bool value);

        // Velocity buffer settings.
        void SetVelocityGeneration(bool value);

        // Render target size, required for velocity buffer output.
        void SetRenderTargetSizeInPixels(int width, int height);

    private:
        // Private implementation.
        class Impl;

        std::unique_ptr<Impl> pImpl;

        // Unsupported interface methods.
        void SetLightingEnabled(bool bEnabled) override;
        void SetPerPixelLighting(bool value) override;
        void XM_CALLCONV SetAmbientLightColor(DirectX::FXMVECTOR value) override;
        void XM_CALLCONV SetLightSpecularColor(int whichLight, DirectX::FXMVECTOR value) override;
    };

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_EFFECTPHYSICALLYBASED_HPP
