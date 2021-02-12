#if !defined(QUETZAL_DIRECT3D11_EFFECT_HPP)
#define QUETZAL_DIRECT3D11_EFFECT_HPP
//------------------------------------------------------------------------------
// direct3d11
// Effect.hpp
//------------------------------------------------------------------------------

#include "ShaderBytecode.hpp"
#include <DirectXMath.h>
#include <d3d11_1.h>

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    // Abstract interface representing any effect which can be applied onto a D3D device context
    class IEffect
    {
    public:

        virtual ~IEffect() = default;

        virtual ShaderBytecode vertex_shader_bytecode() const = 0;
        virtual void apply(ID3D11DeviceContext* pDeviceContext) = 0;
    };

    //--------------------------------------------------------------------------
    // Abstract interface for effects with world, view, and projection matrices
    class IEffectMatrices
    {
    public:

        virtual ~IEffectMatrices() = default;

        virtual void XM_CALLCONV SetWorld(DirectX::FXMMATRIX value) = 0;
        virtual void XM_CALLCONV SetView(DirectX::FXMMATRIX value) = 0;
        virtual void XM_CALLCONV SetProjection(DirectX::FXMMATRIX value) = 0;
        virtual void XM_CALLCONV SetMatrices(DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection);
    };

    //--------------------------------------------------------------------------
    // Abstract interface for effects which support directional lighting
    class IEffectLights
    {
    public:

        virtual ~IEffectLights() = default;

        virtual void SetLightingEnabled(bool bEnabled) = 0;
        virtual void SetPerPixelLighting(bool value) = 0;
        virtual void XM_CALLCONV SetAmbientLightColor(DirectX::FXMVECTOR value) = 0;

        virtual void SetLightEnabled(int nLight, bool bEnabled) = 0;
        virtual void XM_CALLCONV SetLightDirection(int nLight, DirectX::FXMVECTOR value) = 0;
        virtual void XM_CALLCONV SetLightDiffuseColor(int nLight, DirectX::FXMVECTOR value) = 0;
        virtual void XM_CALLCONV SetLightSpecularColor(int nLight, DirectX::FXMVECTOR value) = 0;

        virtual void EnableDefaultLighting() = 0;

        static constexpr int MaxDirectionalLights = 3;
    };

    //--------------------------------------------------------------------------
    // Abstract interface for effects which support fog
    class IEffectFog
    {
    public:

        virtual ~IEffectFog() = default;

        virtual void SetFogEnabled(bool bEnabled) = 0;
        virtual void SetFogStart(float value) = 0;
        virtual void SetFogEnd(float value) = 0;
        virtual void XM_CALLCONV SetFogColor(DirectX::FXMVECTOR value) = 0;
    };

    //--------------------------------------------------------------------------
    // Abstract interface for effects which support skinning
    class IEffectSkinning
    {
    public:

        virtual ~IEffectSkinning() = default;

        virtual void SetWeightsPerVertex(int value) = 0;
        virtual void SetBoneTransforms(DirectX::XMMATRIX const* value, size_t count) = 0;
        virtual void ResetBoneTransforms() = 0;

        static constexpr int MaxBones = 72;
    };

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_EFFECT_HPP
