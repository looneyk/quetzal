//------------------------------------------------------------------------------
// direct3d11
// EffectNormalMap.cpp
//------------------------------------------------------------------------------

#include "EffectNormalMap.hpp"

using namespace std;
using namespace DirectX;

using quetzal::direct3d11::IEffectLights;

namespace
{

    //--------------------------------------------------------------------------
    // Constant buffer layout. Must match the shader!
    struct EffectNormalMapConstants
    {
        XMVECTOR diffuseColor;
        XMVECTOR emissiveColor;
        XMVECTOR specularColorAndPower;

        XMVECTOR lightDirection[IEffectLights::MaxDirectionalLights];
        XMVECTOR lightDiffuseColor[IEffectLights::MaxDirectionalLights];
        XMVECTOR lightSpecularColor[IEffectLights::MaxDirectionalLights];

        XMVECTOR eyePosition;

        XMVECTOR fogColor;
        XMVECTOR fogVector;

        XMMATRIX world;
        XMVECTOR worldInverseTranspose[3];
        XMMATRIX worldViewProj;
    };

    static_assert( ( sizeof(EffectNormalMapConstants) % 16 ) == 0, "CB size not padded correctly" );

    //--------------------------------------------------------------------------
    // Traits type describes our characteristics to the EffectCommon template.
    struct EffectNormalMapTraits
    {
        using ConstantBufferType = EffectNormalMapConstants;

        static constexpr int VertexShaderCount = 4;
        static constexpr int PixelShaderCount = 4;
        static constexpr int ShaderPermutationCount = 16;
    };

} // namespace

//------------------------------------------------------------------------------
// Internal EffectNormalMap implementation class.
class quetzal::direct3d11::EffectNormalMap::Impl : public EffectCommon<EffectNormalMapTraits>
{
public:

    Impl(Renderer& renderer);

    com_ptr<ID3D11ShaderResourceView> specularTexture;
    com_ptr<ID3D11ShaderResourceView> normalTexture;

    bool vertexColorEnabled;
    bool biasedVertexNormals;

    EffectLights lights;

    int shader_permutation() const noexcept;

    void apply(ID3D11DeviceContext* pDeviceContext);
};

// Include the precompiled shader code.
namespace
{
#if defined(_XBOX_ONE) && defined(_TITLE)
    #include "Shaders/Compiled/XboxOneNormalMapEffect_VSNormalPixelLightingTx.inc"
    #include "Shaders/Compiled/XboxOneNormalMapEffect_VSNormalPixelLightingTxVc.inc"

    #include "Shaders/Compiled/XboxOneNormalMapEffect_VSNormalPixelLightingTxBn.inc"
    #include "Shaders/Compiled/XboxOneNormalMapEffect_VSNormalPixelLightingTxVcBn.inc"

    #include "Shaders/Compiled/XboxOneNormalMapEffect_PSNormalPixelLightingTx.inc"
    #include "Shaders/Compiled/XboxOneNormalMapEffect_PSNormalPixelLightingTxNoFog.inc"
    #include "Shaders/Compiled/XboxOneNormalMapEffect_PSNormalPixelLightingTxNoSpec.inc"
    #include "Shaders/Compiled/XboxOneNormalMapEffect_PSNormalPixelLightingTxNoFogSpec.inc"
#else
    #include "Shaders/Compiled/NormalMapEffect_VSNormalPixelLightingTx.inc"
    #include "Shaders/Compiled/NormalMapEffect_VSNormalPixelLightingTxVc.inc"

    #include "Shaders/Compiled/NormalMapEffect_VSNormalPixelLightingTxBn.inc"
    #include "Shaders/Compiled/NormalMapEffect_VSNormalPixelLightingTxVcBn.inc"

    #include "Shaders/Compiled/NormalMapEffect_PSNormalPixelLightingTx.inc"
    #include "Shaders/Compiled/NormalMapEffect_PSNormalPixelLightingTxNoFog.inc"
    #include "Shaders/Compiled/NormalMapEffect_PSNormalPixelLightingTxNoSpec.inc"
    #include "Shaders/Compiled/NormalMapEffect_PSNormalPixelLightingTxNoFogSpec.inc"
#endif
}

template<>
const quetzal::direct3d11::VertexShaderManager<EffectNormalMapTraits>::bytecode_array_type quetzal::direct3d11::VertexShaderManager<EffectNormalMapTraits>::m_bytecode =
{{
    { NormalMapEffect_VSNormalPixelLightingTx,     sizeof(NormalMapEffect_VSNormalPixelLightingTx)     },
    { NormalMapEffect_VSNormalPixelLightingTxVc,   sizeof(NormalMapEffect_VSNormalPixelLightingTxVc)   },

    { NormalMapEffect_VSNormalPixelLightingTxBn,   sizeof(NormalMapEffect_VSNormalPixelLightingTxBn)   },
    { NormalMapEffect_VSNormalPixelLightingTxVcBn, sizeof(NormalMapEffect_VSNormalPixelLightingTxVcBn) },
}};

template<>
const quetzal::direct3d11::VertexShaderManager<EffectNormalMapTraits>::shader_array_type quetzal::direct3d11::VertexShaderManager<EffectNormalMapTraits>::m_nShader =
{{
    0,      // pixel lighting + texture
    0,      // pixel lighting + texture, no fog
    1,      // pixel lighting + texture + vertex color
    1,      // pixel lighting + texture + vertex color, no fog

    0,      // pixel lighting + texture, no specular
    0,      // pixel lighting + texture, no fog or specular
    1,      // pixel lighting + texture + vertex color, no specular
    1,      // pixel lighting + texture + vertex color, no fog or specular

    2,      // pixel lighting (biased vertex normal/tangent) + texture
    2,      // pixel lighting (biased vertex normal/tangent) + texture, no fog
    3,      // pixel lighting (biased vertex normal/tangent) + texture + vertex color
    3,      // pixel lighting (biased vertex normal/tangent) + texture + vertex color, no fog

    2,      // pixel lighting (biased vertex normal/tangent) + texture, no specular
    2,      // pixel lighting (biased vertex normal/tangent) + texture, no fog or specular
    3,      // pixel lighting (biased vertex normal/tangent) + texture + vertex color, no specular
    3,      // pixel lighting (biased vertex normal/tangent) + texture + vertex color, no fog or specular
}};

template<>
const quetzal::direct3d11::PixelShaderManager<EffectNormalMapTraits>::bytecode_array_type quetzal::direct3d11::PixelShaderManager<EffectNormalMapTraits>::m_bytecode =
{{
    { NormalMapEffect_PSNormalPixelLightingTx,          sizeof(NormalMapEffect_PSNormalPixelLightingTx)          },
    { NormalMapEffect_PSNormalPixelLightingTxNoFog,     sizeof(NormalMapEffect_PSNormalPixelLightingTxNoFog)     },
    { NormalMapEffect_PSNormalPixelLightingTxNoSpec,    sizeof(NormalMapEffect_PSNormalPixelLightingTxNoSpec)    },
    { NormalMapEffect_PSNormalPixelLightingTxNoFogSpec, sizeof(NormalMapEffect_PSNormalPixelLightingTxNoFogSpec) },
}};

template<>
const quetzal::direct3d11::PixelShaderManager<EffectNormalMapTraits>::shader_array_type quetzal::direct3d11::PixelShaderManager<EffectNormalMapTraits>::m_nShader =
{{
    0,      // pixel lighting + texture
    1,      // pixel lighting + texture, no fog
    0,      // pixel lighting + texture + vertex color
    1,      // pixel lighting + texture + vertex color, no fog

    2,      // pixel lighting + texture, no specular
    3,      // pixel lighting + texture, no fog or specular
    2,      // pixel lighting + texture + vertex color, no specular
    3,      // pixel lighting + texture + vertex color, no fog or specular

    0,      // pixel lighting (biased vertex normal/tangent) + texture
    1,      // pixel lighting (biased vertex normal/tangent) + texture, no fog
    0,      // pixel lighting (biased vertex normal/tangent) + texture + vertex color
    1,      // pixel lighting (biased vertex normal/tangent) + texture + vertex color, no fog

    2,      // pixel lighting (biased vertex normal/tangent) + texture, no specular
    3,      // pixel lighting (biased vertex normal/tangent) + texture, no fog or specular
    2,      // pixel lighting (biased vertex normal/tangent) + texture + vertex color, no specular
    3,      // pixel lighting (biased vertex normal/tangent) + texture + vertex color, no fog or specular
}};

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectNormalMap::Impl::Impl(Renderer& renderer) :
    EffectCommon(renderer),
    vertexColorEnabled(false),
    biasedVertexNormals(false)
{
// shader manager arrays are sized with traits anyway ...
//    static_assert(std::size(EffectCommon<EffectNormalMapTraits>::VertexShaderIndices) == EffectNormalMapTraits::ShaderPermutationCount, "array/max mismatch");
//    static_assert(std::size(EffectCommon<EffectNormalMapTraits>::VertexShaderBytecode) == EffectNormalMapTraits::VertexShaderCount, "array/max mismatch");
//    static_assert(std::size(EffectCommon<EffectNormalMapTraits>::PixelShaderBytecode) == EffectNormalMapTraits::PixelShaderCount, "array/max mismatch");
//    static_assert(std::size(EffectCommon<EffectNormalMapTraits>::PixelShaderIndices) == EffectNormalMapTraits::ShaderPermutationCount, "array/max mismatch");

    lights.InitializeConstants(constants.specularColorAndPower, constants.lightDirection, constants.lightDiffuseColor, constants.lightSpecularColor);
}

int quetzal::direct3d11::EffectNormalMap::Impl::shader_permutation() const noexcept
{
    int permutation = 0;

    // Use optimized shaders if fog is disabled.
    if (!fog.enabled)
    {
        permutation += 1;
    }

    // Support vertex coloring?
    if (vertexColorEnabled)
    {
        permutation += 2;
    }

    // Specular map?
    if (!specularTexture)
    {
        permutation += 4;
    }

    if (biasedVertexNormals)
    {
        // Compressed normals & tangents need to be scaled and biased in the vertex shader.
        permutation += 8;
    }

    return permutation;
}

// Sets our state onto the D3D device.
void quetzal::direct3d11::EffectNormalMap::Impl::apply(ID3D11DeviceContext* pDeviceContext)
{
    // Compute derived parameter values.
    matrices.SetConstants(dirtyFlags, constants.worldViewProj);

    fog.SetConstants(dirtyFlags, matrices.worldView, constants.fogVector);

    lights.SetConstants(dirtyFlags, matrices, constants.world, constants.worldInverseTranspose, constants.eyePosition, constants.diffuseColor, constants.emissiveColor, true);

    // Set the textures
    ID3D11ShaderResourceView* textures[] = { m_pTexture, specularTexture, normalTexture};
    pDeviceContext->PSSetShaderResources(0, std::size(textures), textures);

    // Set shaders and constant buffers.
    apply_shaders(pDeviceContext, shader_permutation());
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectNormalMap::EffectNormalMap(Renderer& renderer) :
    pImpl(new Impl(renderer))
{
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectNormalMap::EffectNormalMap(EffectNormalMap&& other) :
    pImpl(std::move(other.pImpl))
{
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectNormalMap& quetzal::direct3d11::EffectNormalMap::operator=(EffectNormalMap&& other)
{
    pImpl = std::move(other.pImpl);
    return *this;
}

// IEffect methods.
void quetzal::direct3d11::EffectNormalMap::apply(ID3D11DeviceContext* pDeviceContext)
{
    pImpl->apply(pDeviceContext);
}

quetzal::direct3d11::ShaderBytecode quetzal::direct3d11::EffectNormalMap::vertex_shader_bytecode() const
{
    return pImpl->vertex_shader_bytecode(pImpl->shader_permutation());
}

// Camera settings.
void XM_CALLCONV quetzal::direct3d11::EffectNormalMap::SetWorld(FXMMATRIX value)
{
    pImpl->matrices.world = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::WorldInverseTranspose | EffectDirtyFlags::FogVector;
}

void XM_CALLCONV quetzal::direct3d11::EffectNormalMap::SetView(FXMMATRIX value)
{
    pImpl->matrices.view = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::EyePosition | EffectDirtyFlags::FogVector;
}

void XM_CALLCONV quetzal::direct3d11::EffectNormalMap::SetProjection(FXMMATRIX value)
{
    pImpl->matrices.projection = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj;
}

void XM_CALLCONV quetzal::direct3d11::EffectNormalMap::SetMatrices(FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection)
{
    pImpl->matrices.world = world;
    pImpl->matrices.view = view;
    pImpl->matrices.projection = projection;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::WorldInverseTranspose | EffectDirtyFlags::EyePosition | EffectDirtyFlags::FogVector;
}

// Material settings.
void XM_CALLCONV quetzal::direct3d11::EffectNormalMap::SetDiffuseColor(FXMVECTOR value)
{
    pImpl->lights.diffuseColor = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}

void XM_CALLCONV quetzal::direct3d11::EffectNormalMap::SetEmissiveColor(FXMVECTOR value)
{
    pImpl->lights.emissiveColor = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}

void XM_CALLCONV quetzal::direct3d11::EffectNormalMap::SetSpecularColor(FXMVECTOR value)
{
    // Set xyz to new value, but preserve existing w (specular power).
    pImpl->constants.specularColorAndPower = XMVectorSelect(pImpl->constants.specularColorAndPower, value, g_XMSelect1110);

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

void quetzal::direct3d11::EffectNormalMap::SetSpecularPower(float value)
{
    // Set w to new value, but preserve existing xyz (specular color).
    pImpl->constants.specularColorAndPower = XMVectorSetW(pImpl->constants.specularColorAndPower, value);

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

void quetzal::direct3d11::EffectNormalMap::DisableSpecular()
{
    // Set specular color to black, power to 1
    // Note: Don't use a power of 0 or the shader will generate strange highlights on non-specular materials

    pImpl->constants.specularColorAndPower = g_XMIdentityR3;

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

void quetzal::direct3d11::EffectNormalMap::SetAlpha(float value)
{
    pImpl->lights.alpha = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}

void XM_CALLCONV quetzal::direct3d11::EffectNormalMap::SetColorAndAlpha(FXMVECTOR value)
{
    pImpl->lights.diffuseColor = value;
    pImpl->lights.alpha = XMVectorGetW(value);

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}

// Light settings.
void quetzal::direct3d11::EffectNormalMap::SetLightingEnabled(bool bEnabled)
{
    if (!bEnabled)
    {
        throw std::exception("EffectNormalMap does not support turning off lighting");
    }
}

void quetzal::direct3d11::EffectNormalMap::SetPerPixelLighting(bool)
{
    // Unsupported interface method.
}

void XM_CALLCONV quetzal::direct3d11::EffectNormalMap::SetAmbientLightColor(FXMVECTOR value)
{
    pImpl->lights.ambientLightColor = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}

void quetzal::direct3d11::EffectNormalMap::SetLightEnabled(int nLight, bool bEnabled)
{
    pImpl->dirtyFlags |= pImpl->lights.SetLightEnabled(nLight, bEnabled, pImpl->constants.lightDiffuseColor, pImpl->constants.lightSpecularColor);
}

void XM_CALLCONV quetzal::direct3d11::EffectNormalMap::SetLightDirection(int nLight, FXMVECTOR value)
{
    EffectLights::ValidateLightIndex(nLight);

    pImpl->constants.lightDirection[nLight] = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

void XM_CALLCONV quetzal::direct3d11::EffectNormalMap::SetLightDiffuseColor(int nLight, FXMVECTOR value)
{
    pImpl->dirtyFlags |= pImpl->lights.SetLightDiffuseColor(nLight, value, pImpl->constants.lightDiffuseColor);
}

void XM_CALLCONV quetzal::direct3d11::EffectNormalMap::SetLightSpecularColor(int nLight, FXMVECTOR value)
{
    pImpl->dirtyFlags |= pImpl->lights.SetLightSpecularColor(nLight, value, pImpl->constants.lightSpecularColor);
}

void quetzal::direct3d11::EffectNormalMap::EnableDefaultLighting()
{
    EffectLights::EnableDefaultLighting(this);
}

// Fog settings.
void quetzal::direct3d11::EffectNormalMap::SetFogEnabled(bool bEnabled)
{
    pImpl->fog.enabled = bEnabled;

    pImpl->dirtyFlags |= EffectDirtyFlags::FogEnable;
}

void quetzal::direct3d11::EffectNormalMap::SetFogStart(float value)
{
    pImpl->fog.start = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::FogVector;
}

void quetzal::direct3d11::EffectNormalMap::SetFogEnd(float value)
{
    pImpl->fog.end = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::FogVector;
}

void XM_CALLCONV quetzal::direct3d11::EffectNormalMap::SetFogColor(FXMVECTOR value)
{
    pImpl->constants.fogColor = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

// Vertex color setting.
void quetzal::direct3d11::EffectNormalMap::SetVertexColorEnabled(bool bEnabled)
{
    pImpl->vertexColorEnabled = bEnabled;
}

// Texture settings.
void quetzal::direct3d11::EffectNormalMap::SetTexture(ID3D11ShaderResourceView* pShaderResourceView)
{
    pImpl->m_pTexture = pShaderResourceView;
}

void quetzal::direct3d11::EffectNormalMap::SetNormalTexture(ID3D11ShaderResourceView* pShaderResourceView)
{
    pImpl->normalTexture = pShaderResourceView;
}

void quetzal::direct3d11::EffectNormalMap::SetSpecularTexture(ID3D11ShaderResourceView* pShaderResourceView)
{
    pImpl->specularTexture = pShaderResourceView;
}

// Normal compression settings.
void quetzal::direct3d11::EffectNormalMap::SetBiasedVertexNormalsAndTangents(bool value)
{
    pImpl->biasedVertexNormals = value;
}