//--------------------------------------------------------------------------------------
// direct3d11
// EffectSkinned.cpp
//--------------------------------------------------------------------------------------

#include "EffectSkinned.hpp"

using namespace std;
using namespace DirectX;

using quetzal::direct3d11::IEffectLights;

namespace
{

    //--------------------------------------------------------------------------
    // Constant buffer layout. Must match the shader!
    struct EffectSkinnedConstants
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

        XMVECTOR bones[quetzal::direct3d11::EffectSkinned::MaxBones][3];
    };

    static_assert( ( sizeof(EffectSkinnedConstants) % 16 ) == 0, "CB size not padded correctly" );

    //--------------------------------------------------------------------------
    // Traits type describes our characteristics to the EffectCommon template.
    struct EffectSkinnedTraits
    {
        using ConstantBufferType = EffectSkinnedConstants;

        static constexpr int VertexShaderCount = 18;
        static constexpr int PixelShaderCount = 3;
        static constexpr int ShaderPermutationCount = 36;
    };
    
    } // namespace

//------------------------------------------------------------------------------
// Internal EffectSkinned implementation class.
class quetzal::direct3d11::EffectSkinned::Impl : public EffectCommon<EffectSkinnedTraits>
{
public:
    Impl(Renderer& renderer);

    bool preferPerPixelLighting;
    bool biasedVertexNormals;
    int weightsPerVertex;

    EffectLights lights;

    int shader_permutation() const noexcept;

    void apply(ID3D11DeviceContext* pDeviceContext);
};

// Include the precompiled shader code.
namespace
{
#if defined(_XBOX_ONE) && defined(_TITLE)
    #include "Shaders/Compiled/XboxOneSkinnedEffect_VSSkinnedVertexLightingOneBone.inc"
    #include "Shaders/Compiled/XboxOneSkinnedEffect_VSSkinnedVertexLightingTwoBones.inc"
    #include "Shaders/Compiled/XboxOneSkinnedEffect_VSSkinnedVertexLightingFourBones.inc"

    #include "Shaders/Compiled/XboxOneSkinnedEffect_VSSkinnedOneLightOneBone.inc"
    #include "Shaders/Compiled/XboxOneSkinnedEffect_VSSkinnedOneLightTwoBones.inc"
    #include "Shaders/Compiled/XboxOneSkinnedEffect_VSSkinnedOneLightFourBones.inc"

    #include "Shaders/Compiled/XboxOneSkinnedEffect_VSSkinnedPixelLightingOneBone.inc"
    #include "Shaders/Compiled/XboxOneSkinnedEffect_VSSkinnedPixelLightingTwoBones.inc"
    #include "Shaders/Compiled/XboxOneSkinnedEffect_VSSkinnedPixelLightingFourBones.inc"

    #include "Shaders/Compiled/XboxOneSkinnedEffect_VSSkinnedVertexLightingOneBoneBn.inc"
    #include "Shaders/Compiled/XboxOneSkinnedEffect_VSSkinnedVertexLightingTwoBonesBn.inc"
    #include "Shaders/Compiled/XboxOneSkinnedEffect_VSSkinnedVertexLightingFourBonesBn.inc"

    #include "Shaders/Compiled/XboxOneSkinnedEffect_VSSkinnedOneLightOneBoneBn.inc"
    #include "Shaders/Compiled/XboxOneSkinnedEffect_VSSkinnedOneLightTwoBonesBn.inc"
    #include "Shaders/Compiled/XboxOneSkinnedEffect_VSSkinnedOneLightFourBonesBn.inc"

    #include "Shaders/Compiled/XboxOneSkinnedEffect_VSSkinnedPixelLightingOneBoneBn.inc"
    #include "Shaders/Compiled/XboxOneSkinnedEffect_VSSkinnedPixelLightingTwoBonesBn.inc"
    #include "Shaders/Compiled/XboxOneSkinnedEffect_VSSkinnedPixelLightingFourBonesBn.inc"

    #include "Shaders/Compiled/XboxOneSkinnedEffect_PSSkinnedVertexLighting.inc"
    #include "Shaders/Compiled/XboxOneSkinnedEffect_PSSkinnedVertexLightingNoFog.inc"
    #include "Shaders/Compiled/XboxOneSkinnedEffect_PSSkinnedPixelLighting.inc"
#else
    #include "Shaders/Compiled/SkinnedEffect_VSSkinnedVertexLightingOneBone.inc"
    #include "Shaders/Compiled/SkinnedEffect_VSSkinnedVertexLightingTwoBones.inc"
    #include "Shaders/Compiled/SkinnedEffect_VSSkinnedVertexLightingFourBones.inc"

    #include "Shaders/Compiled/SkinnedEffect_VSSkinnedOneLightOneBone.inc"
    #include "Shaders/Compiled/SkinnedEffect_VSSkinnedOneLightTwoBones.inc"
    #include "Shaders/Compiled/SkinnedEffect_VSSkinnedOneLightFourBones.inc"

    #include "Shaders/Compiled/SkinnedEffect_VSSkinnedPixelLightingOneBone.inc"
    #include "Shaders/Compiled/SkinnedEffect_VSSkinnedPixelLightingTwoBones.inc"
    #include "Shaders/Compiled/SkinnedEffect_VSSkinnedPixelLightingFourBones.inc"

    #include "Shaders/Compiled/SkinnedEffect_VSSkinnedVertexLightingOneBoneBn.inc"
    #include "Shaders/Compiled/SkinnedEffect_VSSkinnedVertexLightingTwoBonesBn.inc"
    #include "Shaders/Compiled/SkinnedEffect_VSSkinnedVertexLightingFourBonesBn.inc"

    #include "Shaders/Compiled/SkinnedEffect_VSSkinnedOneLightOneBoneBn.inc"
    #include "Shaders/Compiled/SkinnedEffect_VSSkinnedOneLightTwoBonesBn.inc"
    #include "Shaders/Compiled/SkinnedEffect_VSSkinnedOneLightFourBonesBn.inc"

    #include "Shaders/Compiled/SkinnedEffect_VSSkinnedPixelLightingOneBoneBn.inc"
    #include "Shaders/Compiled/SkinnedEffect_VSSkinnedPixelLightingTwoBonesBn.inc"
    #include "Shaders/Compiled/SkinnedEffect_VSSkinnedPixelLightingFourBonesBn.inc"

    #include "Shaders/Compiled/SkinnedEffect_PSSkinnedVertexLighting.inc"
    #include "Shaders/Compiled/SkinnedEffect_PSSkinnedVertexLightingNoFog.inc"
    #include "Shaders/Compiled/SkinnedEffect_PSSkinnedPixelLighting.inc"
#endif
}

template<>
const quetzal::direct3d11::VertexShaderManager<EffectSkinnedTraits>::bytecode_array_type quetzal::direct3d11::VertexShaderManager<EffectSkinnedTraits>::m_bytecode =
{{
    { SkinnedEffect_VSSkinnedVertexLightingOneBone,     sizeof(SkinnedEffect_VSSkinnedVertexLightingOneBone)     },
    { SkinnedEffect_VSSkinnedVertexLightingTwoBones,    sizeof(SkinnedEffect_VSSkinnedVertexLightingTwoBones)    },
    { SkinnedEffect_VSSkinnedVertexLightingFourBones,   sizeof(SkinnedEffect_VSSkinnedVertexLightingFourBones)   },

    { SkinnedEffect_VSSkinnedOneLightOneBone,           sizeof(SkinnedEffect_VSSkinnedOneLightOneBone)           },
    { SkinnedEffect_VSSkinnedOneLightTwoBones,          sizeof(SkinnedEffect_VSSkinnedOneLightTwoBones)          },
    { SkinnedEffect_VSSkinnedOneLightFourBones,         sizeof(SkinnedEffect_VSSkinnedOneLightFourBones)         },

    { SkinnedEffect_VSSkinnedPixelLightingOneBone,      sizeof(SkinnedEffect_VSSkinnedPixelLightingOneBone)      },
    { SkinnedEffect_VSSkinnedPixelLightingTwoBones,     sizeof(SkinnedEffect_VSSkinnedPixelLightingTwoBones)     },
    { SkinnedEffect_VSSkinnedPixelLightingFourBones,    sizeof(SkinnedEffect_VSSkinnedPixelLightingFourBones)    },

    { SkinnedEffect_VSSkinnedVertexLightingOneBoneBn,   sizeof(SkinnedEffect_VSSkinnedVertexLightingOneBoneBn)   },
    { SkinnedEffect_VSSkinnedVertexLightingTwoBonesBn,  sizeof(SkinnedEffect_VSSkinnedVertexLightingTwoBonesBn)  },
    { SkinnedEffect_VSSkinnedVertexLightingFourBonesBn, sizeof(SkinnedEffect_VSSkinnedVertexLightingFourBonesBn) },

    { SkinnedEffect_VSSkinnedOneLightOneBoneBn,         sizeof(SkinnedEffect_VSSkinnedOneLightOneBoneBn)         },
    { SkinnedEffect_VSSkinnedOneLightTwoBonesBn,        sizeof(SkinnedEffect_VSSkinnedOneLightTwoBonesBn)        },
    { SkinnedEffect_VSSkinnedOneLightFourBonesBn,       sizeof(SkinnedEffect_VSSkinnedOneLightFourBonesBn)       },

    { SkinnedEffect_VSSkinnedPixelLightingOneBoneBn,    sizeof(SkinnedEffect_VSSkinnedPixelLightingOneBoneBn)    },
    { SkinnedEffect_VSSkinnedPixelLightingTwoBonesBn,   sizeof(SkinnedEffect_VSSkinnedPixelLightingTwoBonesBn)   },
    { SkinnedEffect_VSSkinnedPixelLightingFourBonesBn,  sizeof(SkinnedEffect_VSSkinnedPixelLightingFourBonesBn)  },

}};

template<>
const quetzal::direct3d11::VertexShaderManager<EffectSkinnedTraits>::shader_array_type quetzal::direct3d11::VertexShaderManager<EffectSkinnedTraits>::m_nShader =
{{
    0,      // vertex lighting, one bone
    0,      // vertex lighting, one bone, no fog
    1,      // vertex lighting, two bones
    1,      // vertex lighting, two bones, no fog
    2,      // vertex lighting, four bones
    2,      // vertex lighting, four bones, no fog

    3,      // one light, one bone
    3,      // one light, one bone, no fog
    4,      // one light, two bones
    4,      // one light, two bones, no fog
    5,      // one light, four bones
    5,      // one light, four bones, no fog

    6,      // pixel lighting, one bone
    6,      // pixel lighting, one bone, no fog
    7,      // pixel lighting, two bones
    7,      // pixel lighting, two bones, no fog
    8,      // pixel lighting, four bones
    8,      // pixel lighting, four bones, no fog

    9,      // vertex lighting (biased vertex normals), one bone
    9,      // vertex lighting (biased vertex normals), one bone, no fog
    10,     // vertex lighting (biased vertex normals), two bones
    10,     // vertex lighting (biased vertex normals), two bones, no fog
    11,     // vertex lighting (biased vertex normals), four bones
    11,     // vertex lighting (biased vertex normals), four bones, no fog

    12,     // one light (biased vertex normals), one bone
    12,     // one light (biased vertex normals), one bone, no fog
    13,     // one light (biased vertex normals), two bones
    13,     // one light (biased vertex normals), two bones, no fog
    14,     // one light (biased vertex normals), four bones
    14,     // one light (biased vertex normals), four bones, no fog

    15,     // pixel lighting (biased vertex normals), one bone
    15,     // pixel lighting (biased vertex normals), one bone, no fog
    16,     // pixel lighting (biased vertex normals), two bones
    16,     // pixel lighting (biased vertex normals), two bones, no fog
    17,     // pixel lighting (biased vertex normals), four bones
    17,     // pixel lighting (biased vertex normals), four bones, no fog
}};

template<>
const quetzal::direct3d11::PixelShaderManager<EffectSkinnedTraits>::bytecode_array_type quetzal::direct3d11::PixelShaderManager<EffectSkinnedTraits>::m_bytecode =
{{
    { SkinnedEffect_PSSkinnedVertexLighting,      sizeof(SkinnedEffect_PSSkinnedVertexLighting)      },
    { SkinnedEffect_PSSkinnedVertexLightingNoFog, sizeof(SkinnedEffect_PSSkinnedVertexLightingNoFog) },
    { SkinnedEffect_PSSkinnedPixelLighting,       sizeof(SkinnedEffect_PSSkinnedPixelLighting)       },
}};

template<>
const quetzal::direct3d11::PixelShaderManager<EffectSkinnedTraits>::shader_array_type quetzal::direct3d11::PixelShaderManager<EffectSkinnedTraits>::m_nShader =
{{
    0,      // vertex lighting, one bone
    1,      // vertex lighting, one bone, no fog
    0,      // vertex lighting, two bones
    1,      // vertex lighting, two bones, no fog
    0,      // vertex lighting, four bones
    1,      // vertex lighting, four bones, no fog

    0,      // one light, one bone
    1,      // one light, one bone, no fog
    0,      // one light, two bones
    1,      // one light, two bones, no fog
    0,      // one light, four bones
    1,      // one light, four bones, no fog

    2,      // pixel lighting, one bone
    2,      // pixel lighting, one bone, no fog
    2,      // pixel lighting, two bones
    2,      // pixel lighting, two bones, no fog
    2,      // pixel lighting, four bones
    2,      // pixel lighting, four bones, no fog

    0,      // vertex lighting (biased vertex normals), one bone
    1,      // vertex lighting (biased vertex normals), one bone, no fog
    0,      // vertex lighting (biased vertex normals), two bones
    1,      // vertex lighting (biased vertex normals), two bones, no fog
    0,      // vertex lighting (biased vertex normals), four bones
    1,      // vertex lighting (biased vertex normals), four bones, no fog

    0,      // one light (biased vertex normals), one bone
    1,      // one light (biased vertex normals), one bone, no fog
    0,      // one light (biased vertex normals), two bones
    1,      // one light (biased vertex normals), two bones, no fog
    0,      // one light (biased vertex normals), four bones
    1,      // one light (biased vertex normals), four bones, no fog

    2,      // pixel lighting (biased vertex normals), one bone
    2,      // pixel lighting (biased vertex normals), one bone, no fog
    2,      // pixel lighting (biased vertex normals), two bones
    2,      // pixel lighting (biased vertex normals), two bones, no fog
    2,      // pixel lighting (biased vertex normals), four bones
    2,      // pixel lighting (biased vertex normals), four bones, no fog
}};

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectSkinned::Impl::Impl(Renderer& renderer) :
    EffectCommon(renderer),
    preferPerPixelLighting(false),
    biasedVertexNormals(false),
    weightsPerVertex(4)
{
// shader manager arrays are sized with traits anyway ...
//    static_assert(std::size(EffectCommon<EffectSkinnedTraits>::VertexShaderIndices) == EffectSkinnedTraits::ShaderPermutationCount, "array/max mismatch");
//    static_assert(std::size(EffectCommon<EffectSkinnedTraits>::VertexShaderBytecode) == EffectSkinnedTraits::VertexShaderCount, "array/max mismatch");
//    static_assert(std::size(EffectCommon<EffectSkinnedTraits>::PixelShaderBytecode) == EffectSkinnedTraits::PixelShaderCount, "array/max mismatch");
//    static_assert(std::size(EffectCommon<EffectSkinnedTraits>::PixelShaderIndices) == EffectSkinnedTraits::ShaderPermutationCount, "array/max mismatch");

    lights.InitializeConstants(constants.specularColorAndPower, constants.lightDirection, constants.lightDiffuseColor, constants.lightSpecularColor);

    for (int i = 0; i < MaxBones; i++)
    {
        constants.bones[i][0] = g_XMIdentityR0;
        constants.bones[i][1] = g_XMIdentityR1;
        constants.bones[i][2] = g_XMIdentityR2;
    }
}

int quetzal::direct3d11::EffectSkinned::Impl::shader_permutation() const noexcept
{
    int permutation = 0;

    // Use optimized shaders if fog is disabled.
    if (!fog.enabled)
    {
        permutation += 1;
    }

    // Evaluate 1, 2, or 4 weights per vertex?
    if (weightsPerVertex == 2)
    {
        permutation += 2;
    }
    else if (weightsPerVertex == 4)
    {
        permutation += 4;
    }

    if (preferPerPixelLighting)
    {
        // Do lighting in the pixel shader.
        permutation += 12;
    }
    else if (!lights.lightEnabled[1] && !lights.lightEnabled[2])
    {
        // Use the only-bother-with-the-first-light shader optimization.
        permutation += 6;
    }

    if (biasedVertexNormals)
    {
        // Compressed normals need to be scaled and biased in the vertex shader.
        permutation += 18;
    }

    return permutation;
}

// Sets our state onto the D3D device.
void quetzal::direct3d11::EffectSkinned::Impl::apply(ID3D11DeviceContext* pDeviceContext)
{
    // Compute derived parameter values.
    matrices.SetConstants(dirtyFlags, constants.worldViewProj);

    fog.SetConstants(dirtyFlags, matrices.worldView, constants.fogVector);

    lights.SetConstants(dirtyFlags, matrices, constants.world, constants.worldInverseTranspose, constants.eyePosition, constants.diffuseColor, constants.emissiveColor, true);

    // Set the texture.
    auto textures = m_pTexture.get();
    if ( !textures )
        textures = default_texture();

    pDeviceContext->PSSetShaderResources(0, 1, &textures );

    // Set shaders and constant buffers.
    apply_shaders(pDeviceContext, shader_permutation());
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectSkinned::EffectSkinned(Renderer& renderer) :
    pImpl(new Impl(renderer))
{
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectSkinned::EffectSkinned(EffectSkinned&& other) :
    pImpl(std::move(other.pImpl))
{
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectSkinned& quetzal::direct3d11::EffectSkinned::operator=(EffectSkinned&& other)
{
    pImpl = std::move(other.pImpl);
    return *this;
}

// IEffect methods.
void quetzal::direct3d11::EffectSkinned::apply(ID3D11DeviceContext* pDeviceContext)
{
    pImpl->apply(pDeviceContext);
}

quetzal::direct3d11::ShaderBytecode quetzal::direct3d11::EffectSkinned::vertex_shader_bytecode() const
{
    return pImpl->vertex_shader_bytecode(pImpl->shader_permutation());
}

// Camera settings.
void XM_CALLCONV quetzal::direct3d11::EffectSkinned::SetWorld(FXMMATRIX value)
{
    pImpl->matrices.world = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::WorldInverseTranspose | EffectDirtyFlags::FogVector;
}

void XM_CALLCONV quetzal::direct3d11::EffectSkinned::SetView(FXMMATRIX value)
{
    pImpl->matrices.view = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::EyePosition | EffectDirtyFlags::FogVector;
}

void XM_CALLCONV quetzal::direct3d11::EffectSkinned::SetProjection(FXMMATRIX value)
{
    pImpl->matrices.projection = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj;
}

void XM_CALLCONV quetzal::direct3d11::EffectSkinned::SetMatrices(FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection)
{
    pImpl->matrices.world = world;
    pImpl->matrices.view = view;
    pImpl->matrices.projection = projection;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::WorldInverseTranspose | EffectDirtyFlags::EyePosition | EffectDirtyFlags::FogVector;
}

// Material settings.
void XM_CALLCONV quetzal::direct3d11::EffectSkinned::SetDiffuseColor(FXMVECTOR value)
{
    pImpl->lights.diffuseColor = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}

void XM_CALLCONV quetzal::direct3d11::EffectSkinned::SetEmissiveColor(FXMVECTOR value)
{
    pImpl->lights.emissiveColor = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}

void XM_CALLCONV quetzal::direct3d11::EffectSkinned::SetSpecularColor(FXMVECTOR value)
{
    // Set xyz to new value, but preserve existing w (specular power).
    pImpl->constants.specularColorAndPower = XMVectorSelect(pImpl->constants.specularColorAndPower, value, g_XMSelect1110);

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

void quetzal::direct3d11::EffectSkinned::SetSpecularPower(float value)
{
    // Set w to new value, but preserve existing xyz (specular color).
    pImpl->constants.specularColorAndPower = XMVectorSetW(pImpl->constants.specularColorAndPower, value);

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

void quetzal::direct3d11::EffectSkinned::DisableSpecular()
{
    // Set specular color to black, power to 1
    // Note: Don't use a power of 0 or the shader will generate strange highlights on non-specular materials

    pImpl->constants.specularColorAndPower = g_XMIdentityR3;

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

void quetzal::direct3d11::EffectSkinned::SetAlpha(float value)
{
    pImpl->lights.alpha = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}

void XM_CALLCONV quetzal::direct3d11::EffectSkinned::SetColorAndAlpha(FXMVECTOR value)
{
    pImpl->lights.diffuseColor = value;
    pImpl->lights.alpha = XMVectorGetW(value);

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}

// Light settings.
void quetzal::direct3d11::EffectSkinned::SetLightingEnabled(bool bEnabled)
{
    if (!bEnabled)
    {
        throw std::exception("EffectSkinned does not support turning off lighting");
    }
}

void quetzal::direct3d11::EffectSkinned::SetPerPixelLighting(bool value)
{
    pImpl->preferPerPixelLighting = value;
}

void XM_CALLCONV quetzal::direct3d11::EffectSkinned::SetAmbientLightColor(FXMVECTOR value)
{
    pImpl->lights.ambientLightColor = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}

void quetzal::direct3d11::EffectSkinned::SetLightEnabled(int nLight, bool bEnabled)
{
    pImpl->dirtyFlags |= pImpl->lights.SetLightEnabled(nLight, bEnabled, pImpl->constants.lightDiffuseColor, pImpl->constants.lightSpecularColor);
}

void XM_CALLCONV quetzal::direct3d11::EffectSkinned::SetLightDirection(int nLight, FXMVECTOR value)
{
    EffectLights::ValidateLightIndex(nLight);

    pImpl->constants.lightDirection[nLight] = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

void XM_CALLCONV quetzal::direct3d11::EffectSkinned::SetLightDiffuseColor(int nLight, FXMVECTOR value)
{
    pImpl->dirtyFlags |= pImpl->lights.SetLightDiffuseColor(nLight, value, pImpl->constants.lightDiffuseColor);
}

void XM_CALLCONV quetzal::direct3d11::EffectSkinned::SetLightSpecularColor(int nLight, FXMVECTOR value)
{
    pImpl->dirtyFlags |= pImpl->lights.SetLightSpecularColor(nLight, value, pImpl->constants.lightSpecularColor);
}

void quetzal::direct3d11::EffectSkinned::EnableDefaultLighting()
{
    EffectLights::EnableDefaultLighting(this);
}

// Fog settings.
void quetzal::direct3d11::EffectSkinned::SetFogEnabled(bool bEnabled)
{
    pImpl->fog.enabled = bEnabled;

    pImpl->dirtyFlags |= EffectDirtyFlags::FogEnable;
}

void quetzal::direct3d11::EffectSkinned::SetFogStart(float value)
{
    pImpl->fog.start = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::FogVector;
}

void quetzal::direct3d11::EffectSkinned::SetFogEnd(float value)
{
    pImpl->fog.end = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::FogVector;
}

void XM_CALLCONV quetzal::direct3d11::EffectSkinned::SetFogColor(FXMVECTOR value)
{
    pImpl->constants.fogColor = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

// Texture settings.
void quetzal::direct3d11::EffectSkinned::SetTexture(ID3D11ShaderResourceView* pShaderResourceView)
{
    pImpl->m_pTexture = pShaderResourceView;
}

// Animation settings.
void quetzal::direct3d11::EffectSkinned::SetWeightsPerVertex(int value)
{
    if ((value != 1) &&
        (value != 2) &&
        (value != 4))
    {
        throw std::out_of_range("WeightsPerVertex must be 1, 2, or 4");
    }

    pImpl->weightsPerVertex = value;
}

void quetzal::direct3d11::EffectSkinned::SetBoneTransforms(XMMATRIX const* value, size_t count)
{
    if (count > MaxBones)
        throw std::out_of_range("count parameter out of range");

    auto boneConstant = pImpl->constants.bones;

    for (size_t i = 0; i < count; i++)
    {
        XMMATRIX boneMatrix = XMMatrixTranspose(value[i]);

        boneConstant[i][0] = boneMatrix.r[0];
        boneConstant[i][1] = boneMatrix.r[1];
        boneConstant[i][2] = boneMatrix.r[2];
    }

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

void quetzal::direct3d11::EffectSkinned::ResetBoneTransforms()
{
    auto boneConstant = pImpl->constants.bones;

    for(size_t i = 0; i < MaxBones; ++i)
    {
        boneConstant[i][0] = g_XMIdentityR0;
        boneConstant[i][1] = g_XMIdentityR1;
        boneConstant[i][2] = g_XMIdentityR2;
    }

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

// Normal compression settings.
void quetzal::direct3d11::EffectSkinned::SetBiasedVertexNormals(bool value)
{
    pImpl->biasedVertexNormals = value;
}
