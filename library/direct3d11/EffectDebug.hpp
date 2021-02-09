#if !defined(QUETZAL_DIRECT3D11_EFFECTDEBUG_HPP)
#define QUETZAL_DIRECT3D11_EFFECTDEBUG_HPP
//------------------------------------------------------------------------------
// direct3d11
// EffectDebug.hpp
//------------------------------------------------------------------------------

#include "EffectCommon.hpp"

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    // Built-in shader for debug visualization of normals, tangents, etc.
    class EffectDebug : public IEffect, public IEffectMatrices
    {
    public:

        enum Mode
        {
            Mode_Default = 0,   // Hemispherical ambient lighting
            Mode_Normals,       // RGB normals
            Mode_Tangents,      // RGB tangents
            Mode_BiTangents,    // RGB bi-tangents
        };

        explicit EffectDebug(Renderer& renderer);
        EffectDebug(EffectDebug const&) = delete;
        EffectDebug(EffectDebug&& other) noexcept;

        EffectDebug& operator=(EffectDebug const&) = delete;
        EffectDebug& operator=(EffectDebug&& other) noexcept;

        virtual ~EffectDebug() = default;

        // IEffect methods.
        void apply(ID3D11DeviceContext* pDeviceContext) override;

        ShaderBytecode vertex_shader_bytecode() const override;

        // Camera settings.
        void XM_CALLCONV SetWorld(DirectX::FXMMATRIX value) override;
        void XM_CALLCONV SetView(DirectX::FXMMATRIX value) override;
        void XM_CALLCONV SetProjection(DirectX::FXMMATRIX value) override;
        void XM_CALLCONV SetMatrices(DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection) override;

        // Debug Settings.
        void SetMode(Mode debugMode);
        void XM_CALLCONV SetHemisphericalAmbientColor(DirectX::FXMVECTOR upper, DirectX::FXMVECTOR lower);
        void SetAlpha(float value);

        // Vertex color setting.
        void SetVertexColorEnabled(bool value);

        // Normal compression settings.
        void SetBiasedVertexNormals(bool value);

    private:

        // Private implementation.
        class Impl;

        std::unique_ptr<Impl> pImpl;
    };

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_EFFECTDEBUG_HPP
