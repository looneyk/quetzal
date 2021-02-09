#if !defined(QUETZAL_DIRECT3D11_EFFECTENVIRONMENTMAP_HPP)
#define QUETZAL_DIRECT3D11_EFFECTENVIRONMENTMAP_HPP
//------------------------------------------------------------------------------
// direct3d11
// EffectEnvironmentMap.hpp
//------------------------------------------------------------------------------

#include "EffectCommon.hpp"

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    // Built-in shader supports cubic environment mapping.
    class EffectEnvironmentMap : public IEffect, public IEffectMatrices, public IEffectLights, public IEffectFog
    {
    public:

        enum Mapping
        {
            Mapping_Cube = 0,       // Cubic environment map
            Mapping_Sphere,         // Spherical environment map
            Mapping_DualParabola,   // Dual-parabola environment map (requires Feature Level 10.0)
        };

        explicit EffectEnvironmentMap(Renderer& renderer);
        EffectEnvironmentMap(EffectEnvironmentMap const&) = delete;
        EffectEnvironmentMap(EffectEnvironmentMap&& other);

        EffectEnvironmentMap& operator=(EffectEnvironmentMap const&) = delete;
        EffectEnvironmentMap& operator=(EffectEnvironmentMap&& other);

        virtual ~EffectEnvironmentMap() = default;

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
        void SetAlpha(float value);
        void XM_CALLCONV SetColorAndAlpha(DirectX::FXMVECTOR value);

        // Light settings.
        void XM_CALLCONV SetAmbientLightColor(DirectX::FXMVECTOR value) override;

        void SetPerPixelLighting(bool value) override;
        void SetLightEnabled(int nLight, bool bEnabled) override;
        void XM_CALLCONV SetLightDirection(int nLight, DirectX::FXMVECTOR value) override;
        void XM_CALLCONV SetLightDiffuseColor(int nLight, DirectX::FXMVECTOR value) override;

        void EnableDefaultLighting() override;

        // Fog settings.
        void SetFogEnabled(bool bEnabled) override;
        void SetFogStart(float value) override;
        void SetFogEnd(float value) override;
        void XM_CALLCONV SetFogColor(DirectX::FXMVECTOR value) override;

        // Texture setting.
        void SetTexture(ID3D11ShaderResourceView* pShaderResourceView);

        // Environment map settings.
        void SetEnvironmentMap(ID3D11ShaderResourceView* pShaderResourceView);
        void SetEnvironmentMapAmount(float value);
        void XM_CALLCONV SetEnvironmentMapSpecular(DirectX::FXMVECTOR value);
        void SetFresnelFactor(float value);

        // Normal compression settings.
        void SetBiasedVertexNormals(bool value);

    private:
        // Private implementation.
        class Impl;

        std::unique_ptr<Impl> pImpl;

        // Unsupported interface methods.
        void SetLightingEnabled(bool bEnabled) override;
        void XM_CALLCONV SetLightSpecularColor(int nLight, DirectX::FXMVECTOR value) override;
    };

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_EFFECTENVIRONMENTMAP_HPP
