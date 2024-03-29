//--------------------------------------------------------------------------------------
// direct3d11
// EffectEnvironmentMap.cpp
//--------------------------------------------------------------------------------------

#include "EffectEnvironmentMap.hpp"

using namespace std;
using namespace DirectX;

using quetzal::direct3d11::IEffectLights;

namespace
{

    //--------------------------------------------------------------------------
    // Constant buffer layout. Must match the shader!
    struct EffectEnvironmentMapConstants
    {
        XMVECTOR environmentMapSpecular;
        float environmentMapAmount;
        float fresnelFactor;
        float pad[2];

        XMVECTOR diffuseColor;
        XMVECTOR emissiveColor;

        XMVECTOR lightDirection[IEffectLights::MaxDirectionalLights];
        XMVECTOR lightDiffuseColor[IEffectLights::MaxDirectionalLights];

        XMVECTOR eyePosition;

        XMVECTOR fogColor;
        XMVECTOR fogVector;

        XMMATRIX world;
        XMVECTOR worldInverseTranspose[3];
        XMMATRIX worldViewProj;
    };

    //--------------------------------------------------------------------------
    static_assert( ( sizeof(EffectEnvironmentMapConstants) % 16 ) == 0, "CB size not padded correctly" );

    // Traits type describes our characteristics to the EffectCommon template.
    struct EffectEnvironmentMapTraits
    {
        using ConstantBufferType = EffectEnvironmentMapConstants;

        static constexpr int VertexShaderCount = 12;
        static constexpr int PixelShaderCount = 16;
        static constexpr int ShaderPermutationCount = 56;
        static constexpr int MappingCount = 3;
    };

} // namespace

//------------------------------------------------------------------------------
// Internal EffectEnvironmentMap implementation class.
class quetzal::direct3d11::EffectEnvironmentMap::Impl : public EffectCommon<EffectEnvironmentMapTraits>
{
public:
    Impl(Renderer& renderer);

    bool preferPerPixelLighting;
    bool fresnelEnabled;
    bool specularEnabled;
    bool biasedVertexNormals;
    EffectEnvironmentMap::Mapping mapping;

    EffectLights lights;

    com_ptr<ID3D11ShaderResourceView> environmentMap;

    int shader_permutation() const noexcept;

    void apply(ID3D11DeviceContext* pDeviceContext);
};

// Include the precompiled shader code.
namespace
{
#if defined(_XBOX_ONE) && defined(_TITLE)
    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_VSEnvMap.inc"
    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_VSEnvMapFresnel.inc"
    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_VSEnvMapOneLight.inc"
    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_VSEnvMapOneLightFresnel.inc"
    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_VSEnvMapPixelLighting.inc"

    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_VSEnvMapBn.inc"
    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_VSEnvMapFresnelBn.inc"
    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_VSEnvMapOneLightBn.inc"
    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_VSEnvMapOneLightFresnelBn.inc"
    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_VSEnvMapPixelLightingBn.inc"

    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_PSEnvMap.inc"
    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_PSEnvMapNoFog.inc"
    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_PSEnvMapSpecular.inc"
    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_PSEnvMapSpecularNoFog.inc"
    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_PSEnvMapPixelLighting.inc"
    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_PSEnvMapPixelLightingNoFog.inc"
    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_PSEnvMapPixelLightingFresnel.inc"
    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_PSEnvMapPixelLightingFresnelNoFog.inc"

    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_PSEnvMapSpherePixelLighting.inc"
    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_PSEnvMapSpherePixelLightingNoFog.inc"
    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_PSEnvMapSpherePixelLightingFresnel.inc"
    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_PSEnvMapSpherePixelLightingFresnelNoFog.inc"

    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_PSEnvMapDualParabolaPixelLighting.inc"
    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_PSEnvMapDualParabolaPixelLightingNoFog.inc"
    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_PSEnvMapDualParabolaPixelLightingFresnel.inc"
    #include "Shaders/Compiled/XboxOneEnvironmentMapEffect_PSEnvMapDualParabolaPixelLightingFresnelNoFog.inc"
#else
    #include "Shaders/Compiled/EnvironmentMapEffect_VSEnvMap.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_VSEnvMapFresnel.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_VSEnvMapOneLight.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_VSEnvMapOneLightFresnel.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_VSEnvMapPixelLighting.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_VSEnvMapPixelLightingSM4.inc"

    #include "Shaders/Compiled/EnvironmentMapEffect_VSEnvMapBn.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_VSEnvMapFresnelBn.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_VSEnvMapOneLightBn.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_VSEnvMapOneLightFresnelBn.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_VSEnvMapPixelLightingBn.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_VSEnvMapPixelLightingBnSM4.inc"

    #include "Shaders/Compiled/EnvironmentMapEffect_PSEnvMap.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_PSEnvMapNoFog.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_PSEnvMapSpecular.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_PSEnvMapSpecularNoFog.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_PSEnvMapPixelLighting.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_PSEnvMapPixelLightingNoFog.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_PSEnvMapPixelLightingFresnel.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_PSEnvMapPixelLightingFresnelNoFog.inc"

    #include "Shaders/Compiled/EnvironmentMapEffect_PSEnvMapSpherePixelLighting.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_PSEnvMapSpherePixelLightingNoFog.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_PSEnvMapSpherePixelLightingFresnel.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_PSEnvMapSpherePixelLightingFresnelNoFog.inc"

    #include "Shaders/Compiled/EnvironmentMapEffect_PSEnvMapDualParabolaPixelLighting.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_PSEnvMapDualParabolaPixelLightingNoFog.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_PSEnvMapDualParabolaPixelLightingFresnel.inc"
    #include "Shaders/Compiled/EnvironmentMapEffect_PSEnvMapDualParabolaPixelLightingFresnelNoFog.inc"
#endif
}

template<>
const quetzal::direct3d11::VertexShaderManager<EffectEnvironmentMapTraits>::bytecode_array_type quetzal::direct3d11::VertexShaderManager<EffectEnvironmentMapTraits>::m_bytecode =
{{
    { EnvironmentMapEffect_VSEnvMap,                   sizeof(EnvironmentMapEffect_VSEnvMap)                   },
    { EnvironmentMapEffect_VSEnvMapFresnel,            sizeof(EnvironmentMapEffect_VSEnvMapFresnel)            },
    { EnvironmentMapEffect_VSEnvMapOneLight,           sizeof(EnvironmentMapEffect_VSEnvMapOneLight)           },
    { EnvironmentMapEffect_VSEnvMapOneLightFresnel,    sizeof(EnvironmentMapEffect_VSEnvMapOneLightFresnel)    },
    { EnvironmentMapEffect_VSEnvMapPixelLighting,      sizeof(EnvironmentMapEffect_VSEnvMapPixelLighting)      },

    { EnvironmentMapEffect_VSEnvMapBn,                 sizeof(EnvironmentMapEffect_VSEnvMapBn)                 },
    { EnvironmentMapEffect_VSEnvMapFresnelBn,          sizeof(EnvironmentMapEffect_VSEnvMapFresnelBn)          },
    { EnvironmentMapEffect_VSEnvMapOneLightBn,         sizeof(EnvironmentMapEffect_VSEnvMapOneLightBn)         },
    { EnvironmentMapEffect_VSEnvMapOneLightFresnelBn,  sizeof(EnvironmentMapEffect_VSEnvMapOneLightFresnelBn)  },
    { EnvironmentMapEffect_VSEnvMapPixelLightingBn,    sizeof(EnvironmentMapEffect_VSEnvMapPixelLightingBn)    },

#if defined(_XBOX_ONE) && defined(_TITLE)
    { EnvironmentMapEffect_VSEnvMapPixelLighting,      sizeof(EnvironmentMapEffect_VSEnvMapPixelLighting)      },
    { EnvironmentMapEffect_VSEnvMapPixelLightingBn,    sizeof(EnvironmentMapEffect_VSEnvMapPixelLightingBn)    },
#else
    { EnvironmentMapEffect_VSEnvMapPixelLightingSM4,   sizeof(EnvironmentMapEffect_VSEnvMapPixelLightingSM4)   },
    { EnvironmentMapEffect_VSEnvMapPixelLightingBnSM4, sizeof(EnvironmentMapEffect_VSEnvMapPixelLightingBnSM4) },
#endif
}};

template<>
const quetzal::direct3d11::VertexShaderManager<EffectEnvironmentMapTraits>::shader_array_type quetzal::direct3d11::VertexShaderManager<EffectEnvironmentMapTraits>::m_nShader =
{
    0,      // basic
    0,      // basic, no fog
    1,      // fresnel
    1,      // fresnel, no fog
    0,      // specular
    0,      // specular, no fog
    1,      // fresnel + specular
    1,      // fresnel + specular, no fog

    2,      // one light
    2,      // one light, no fog
    3,      // one light, fresnel
    3,      // one light, fresnel, no fog
    2,      // one light, specular
    2,      // one light, specular, no fog
    3,      // one light, fresnel + specular
    3,      // one light, fresnel + specular, no fog

    4,      // pixel lighting
    4,      // pixel lighting, no fog
    4,      // pixel lighting, fresnel
    4,      // pixel lighting, fresnel, no fog

    5,      // basic (biased vertex normals)
    5,      // basic (biased vertex normals), no fog
    6,      // fresnel (biased vertex normals)
    6,      // fresnel (biased vertex normals), no fog
    5,      // specular (biased vertex normals)
    5,      // specular (biased vertex normals), no fog
    6,      // fresnel + specular (biased vertex normals)
    6,      // fresnel + specular (biased vertex normals), no fog

    7,      // one light (biased vertex normals)
    7,      // one light (biased vertex normals), no fog
    8,      // one light (biased vertex normals), fresnel
    8,      // one light (biased vertex normals), fresnel, no fog
    7,      // one light (biased vertex normals), specular
    7,      // one light (biased vertex normals), specular, no fog
    8,      // one light (biased vertex normals), fresnel + specular
    8,      // one light (biased vertex normals), fresnel + specular, no fog

    9,      // pixel lighting (biased vertex normals)
    9,      // pixel lighting (biased vertex normals), no fog
    9,      // pixel lighting (biased vertex normals), fresnel
    9,      // pixel lighting (biased vertex normals), fresnel, no fog

    4,      // spheremap pixel lighting
    4,      // spheremap pixel lighting, no fog
    4,      // spheremap pixel lighting, fresnel
    4,      // spheremap pixel lighting, fresnel, no fog

    9,      // spheremap pixel lighting (biased vertex normals)
    9,      // spheremap pixel lighting (biased vertex normals), no fog
    9,      // spheremap pixel lighting (biased vertex normals), fresnel
    9,      // spheremap pixel lighting (biased vertex normals), fresnel, no fog

    10,     // dual-parabola pixel lighting
    10,     // dual-parabola pixel lighting, no fog
    10,     // dual-parabola pixel lighting, fresnel
    10,     // dual-parabola pixel lighting, fresnel, no fog

    11,     // dual-parabola pixel lighting (biased vertex normals)
    11,     // dual-parabola pixel lighting (biased vertex normals), no fog
    11,     // dual-parabola pixel lighting (biased vertex normals), fresnel
    11,     // dual-parabola pixel lighting (biased vertex normals), fresnel, no fog
};

template<>
const quetzal::direct3d11::PixelShaderManager<EffectEnvironmentMapTraits>::bytecode_array_type quetzal::direct3d11::PixelShaderManager<EffectEnvironmentMapTraits>::m_bytecode =
{{
    { EnvironmentMapEffect_PSEnvMap,                          sizeof(EnvironmentMapEffect_PSEnvMap)                          },
    { EnvironmentMapEffect_PSEnvMapNoFog,                     sizeof(EnvironmentMapEffect_PSEnvMapNoFog)                     },
    { EnvironmentMapEffect_PSEnvMapSpecular,                  sizeof(EnvironmentMapEffect_PSEnvMapSpecular)                  },
    { EnvironmentMapEffect_PSEnvMapSpecularNoFog,             sizeof(EnvironmentMapEffect_PSEnvMapSpecularNoFog)             },
    { EnvironmentMapEffect_PSEnvMapPixelLighting,             sizeof(EnvironmentMapEffect_PSEnvMapPixelLighting)             },
    { EnvironmentMapEffect_PSEnvMapPixelLightingNoFog,        sizeof(EnvironmentMapEffect_PSEnvMapPixelLightingNoFog)        },
    { EnvironmentMapEffect_PSEnvMapPixelLightingFresnel,      sizeof(EnvironmentMapEffect_PSEnvMapPixelLightingFresnel)      },
    { EnvironmentMapEffect_PSEnvMapPixelLightingFresnelNoFog, sizeof(EnvironmentMapEffect_PSEnvMapPixelLightingFresnelNoFog) },

    { EnvironmentMapEffect_PSEnvMapSpherePixelLighting,             sizeof(EnvironmentMapEffect_PSEnvMapSpherePixelLighting) },
    { EnvironmentMapEffect_PSEnvMapSpherePixelLightingNoFog,        sizeof(EnvironmentMapEffect_PSEnvMapSpherePixelLightingNoFog) },
    { EnvironmentMapEffect_PSEnvMapSpherePixelLightingFresnel,      sizeof(EnvironmentMapEffect_PSEnvMapSpherePixelLightingFresnel) },
    { EnvironmentMapEffect_PSEnvMapSpherePixelLightingFresnelNoFog, sizeof(EnvironmentMapEffect_PSEnvMapSpherePixelLightingFresnelNoFog) },

    { EnvironmentMapEffect_PSEnvMapDualParabolaPixelLighting,             sizeof(EnvironmentMapEffect_PSEnvMapDualParabolaPixelLighting) },
    { EnvironmentMapEffect_PSEnvMapDualParabolaPixelLightingNoFog,        sizeof(EnvironmentMapEffect_PSEnvMapDualParabolaPixelLightingNoFog) },
    { EnvironmentMapEffect_PSEnvMapDualParabolaPixelLightingFresnel,      sizeof(EnvironmentMapEffect_PSEnvMapDualParabolaPixelLightingFresnel) },
    { EnvironmentMapEffect_PSEnvMapDualParabolaPixelLightingFresnelNoFog, sizeof(EnvironmentMapEffect_PSEnvMapDualParabolaPixelLightingFresnelNoFog) },
}};

template<>
const quetzal::direct3d11::PixelShaderManager<EffectEnvironmentMapTraits>::shader_array_type quetzal::direct3d11::PixelShaderManager<EffectEnvironmentMapTraits>::m_nShader =
{
    0,      // basic
    1,      // basic, no fog
    0,      // fresnel
    1,      // fresnel, no fog
    2,      // specular
    3,      // specular, no fog
    2,      // fresnel + specular
    3,      // fresnel + specular, no fog

    0,      // one light
    1,      // one light, no fog
    0,      // one light, fresnel
    1,      // one light, fresnel, no fog
    2,      // one light, specular
    3,      // one light, specular, no fog
    2,      // one light, fresnel + specular
    3,      // one light, fresnel + specular, no fog

    4,      // per pixel lighting
    5,      // per pixel lighting, no fog
    6,      // per pixel lighting, fresnel
    7,      // per pixel lighting, fresnel, no fog

    0,      // basic (biased vertex normals)
    1,      // basic (biased vertex normals), no fog
    0,      // fresnel (biased vertex normals)
    1,      // fresnel (biased vertex normals), no fog
    2,      // specular (biased vertex normals)
    3,      // specular (biased vertex normals), no fog
    2,      // fresnel + specular (biased vertex normals)
    3,      // fresnel + specular (biased vertex normals), no fog

    0,      // one light (biased vertex normals)
    1,      // one light (biased vertex normals), no fog
    0,      // one light (biased vertex normals), fresnel
    1,      // one light (biased vertex normals), fresnel, no fog
    2,      // one light (biased vertex normals), specular
    3,      // one light (biased vertex normals), specular, no fog
    2,      // one light (biased vertex normals), fresnel + specular
    3,      // one light (biased vertex normals), fresnel + specular, no fog

    4,      // per pixel lighting (biased vertex normals)
    5,      // per pixel lighting (biased vertex normals), no fog
    6,      // per pixel lighting (biased vertex normals), fresnel
    7,      // per pixel lighting (biased vertex normals), fresnel, no fog

    8,      // spheremap pixel lighting
    9,      // spheremap pixel lighting, no fog
    10,     // spheremap pixel lighting, fresnel
    11,     // spheremap pixel lighting, fresnel, no fog

    8,      // spheremap pixel lighting (biased vertex normals)
    9,      // spheremap pixel lighting (biased vertex normals), no fog
    10,     // spheremap pixel lighting (biased vertex normals), fresnel
    11,     // spheremap pixel lighting (biased vertex normals), fresnel, no fog

    12,     // dual-parabola pixel lighting
    13,     // dual-parabola pixel lighting, no fog
    14,     // dual-parabola pixel lighting, fresnel
    15,     // dual-parabola pixel lighting, fresnel, no fog

    12,     // dual-parabola pixel lighting (biased vertex normals)
    13,     // dual-parabola pixel lighting (biased vertex normals), no fog
    14,     // dual-parabola pixel lighting (biased vertex normals), fresnel
    15,     // dual-parabola pixel lighting (biased vertex normals), fresnel, no fog
};

template<>
quetzal::com_ptr<ID3D11ShaderResourceView> quetzal::direct3d11::EffectCommon<EffectEnvironmentMapTraits>::m_pTextureDefault;

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectEnvironmentMap::Impl::Impl(Renderer& renderer) :
    EffectCommon(renderer),
    preferPerPixelLighting(false),
    fresnelEnabled(true),
    specularEnabled(false),
    biasedVertexNormals(false)
{
// shader manager arrays are sized with traits anyway ...
//    static_assert(std::size(EffectCommon<EffectEnvironmentMapTraits>::VertexShaderIndices) == EffectEnvironmentMapTraits::ShaderPermutationCount, "array/max mismatch");
//    static_assert(std::size(EffectCommon<EffectEnvironmentMapTraits>::VertexShaderBytecode) == EffectEnvironmentMapTraits::VertexShaderCount, "array/max mismatch");
//    static_assert(std::size(EffectCommon<EffectEnvironmentMapTraits>::PixelShaderBytecode) == EffectEnvironmentMapTraits::PixelShaderCount, "array/max mismatch");
//    static_assert(std::size(EffectCommon<EffectEnvironmentMapTraits>::PixelShaderIndices) == EffectEnvironmentMapTraits::ShaderPermutationCount, "array/max mismatch");

    constants.environmentMapAmount = 1;
    constants.fresnelFactor = 1;

    XMVECTOR unwantedOutput[MaxDirectionalLights];

    lights.InitializeConstants(unwantedOutput[0], constants.lightDirection, constants.lightDiffuseColor, unwantedOutput);
}

int quetzal::direct3d11::EffectEnvironmentMap::Impl::shader_permutation() const noexcept
{
    int permutation = 0;

    // Use optimized shaders if fog is disabled.
    if (!fog.enabled)
    {
        permutation += 1;
    }

    // Support fresnel?
    if (fresnelEnabled)
    {
        permutation += 2;
    }

    if (mapping == Mapping_Sphere)
    {
        permutation += 40;

        if (biasedVertexNormals)
        {
            permutation += 4;
        }
    }
    else if (mapping == Mapping_DualParabola)
    {
        permutation += 48;

        if (biasedVertexNormals)
        {
            permutation += 4;
        }
    }
    else // Mapping_Cube
    {
    if (preferPerPixelLighting)
    {
        permutation += 16;
    }
    else
    {
        // Support specular?
        if (specularEnabled)
        {
            permutation += 4;
        }

        // Use the only-bother-with-the-first-light shader optimization?
        if (!lights.lightEnabled[1] && !lights.lightEnabled[2])
        {
            permutation += 8;
        }
    }

    if (biasedVertexNormals)
    {
        // Compressed normals need to be scaled and biased in the vertex shader.
        permutation += 20;
        }
    }

    return permutation;
}

// Sets our state onto the D3D device.
void quetzal::direct3d11::EffectEnvironmentMap::Impl::apply(ID3D11DeviceContext* pDeviceContext)
{
    // Compute derived parameter values.
    matrices.SetConstants(dirtyFlags, constants.worldViewProj);

    fog.SetConstants(dirtyFlags, matrices.worldView, constants.fogVector);

    lights.SetConstants(dirtyFlags, matrices, constants.world, constants.worldInverseTranspose, constants.eyePosition, constants.diffuseColor, constants.emissiveColor, true);

    // Set the textures.
    ID3D11ShaderResourceView* textures[2] =
    {
        m_pTexture,
        environmentMap,
    };

    pDeviceContext->PSSetShaderResources(0, 2, textures);

    // Set shaders and constant buffers.
    apply_shaders(pDeviceContext, shader_permutation());
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectEnvironmentMap::EffectEnvironmentMap(Renderer& renderer) :
    pImpl(new Impl(renderer))
{
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectEnvironmentMap::EffectEnvironmentMap(EffectEnvironmentMap&& other) :
    pImpl(std::move(other.pImpl))
{
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectEnvironmentMap& quetzal::direct3d11::EffectEnvironmentMap::operator=(EffectEnvironmentMap&& other)
{
    pImpl = std::move(other.pImpl);
    return *this;
}

// IEffect methods.
void quetzal::direct3d11::EffectEnvironmentMap::apply(ID3D11DeviceContext* pDeviceContext)
{
    pImpl->apply(pDeviceContext);
}

quetzal::direct3d11::ShaderBytecode quetzal::direct3d11::EffectEnvironmentMap::vertex_shader_bytecode() const
{
    return pImpl->vertex_shader_bytecode(pImpl->shader_permutation());
}

// Camera settings.
void XM_CALLCONV quetzal::direct3d11::EffectEnvironmentMap::SetWorld(FXMMATRIX value)
{
    pImpl->matrices.world = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::WorldInverseTranspose | EffectDirtyFlags::FogVector;
}

void XM_CALLCONV quetzal::direct3d11::EffectEnvironmentMap::SetView(FXMMATRIX value)
{
    pImpl->matrices.view = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::EyePosition | EffectDirtyFlags::FogVector;
}

void XM_CALLCONV quetzal::direct3d11::EffectEnvironmentMap::SetProjection(FXMMATRIX value)
{
    pImpl->matrices.projection = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj;
}

void XM_CALLCONV quetzal::direct3d11::EffectEnvironmentMap::SetMatrices(FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection)
{
    pImpl->matrices.world = world;
    pImpl->matrices.view = view;
    pImpl->matrices.projection = projection;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::WorldInverseTranspose | EffectDirtyFlags::EyePosition | EffectDirtyFlags::FogVector;
}

// Material settings.
void XM_CALLCONV quetzal::direct3d11::EffectEnvironmentMap::SetDiffuseColor(FXMVECTOR value)
{
    pImpl->lights.diffuseColor = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}

void XM_CALLCONV quetzal::direct3d11::EffectEnvironmentMap::SetEmissiveColor(FXMVECTOR value)
{
    pImpl->lights.emissiveColor = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}

void quetzal::direct3d11::EffectEnvironmentMap::SetAlpha(float value)
{
    pImpl->lights.alpha = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}

void XM_CALLCONV quetzal::direct3d11::EffectEnvironmentMap::SetColorAndAlpha(FXMVECTOR value)
{
    pImpl->lights.diffuseColor = value;
    pImpl->lights.alpha = XMVectorGetW(value);

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}

// Light settings.
void quetzal::direct3d11::EffectEnvironmentMap::SetLightingEnabled(bool value)
{
    if (!value)
    {
        throw std::exception("EffectEnvironmentMap does not support turning off lighting");
    }
}

void quetzal::direct3d11::EffectEnvironmentMap::SetPerPixelLighting(bool value)
{
    pImpl->preferPerPixelLighting = value;
}

void XM_CALLCONV quetzal::direct3d11::EffectEnvironmentMap::SetAmbientLightColor(FXMVECTOR value)
{
    pImpl->lights.ambientLightColor = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}

void quetzal::direct3d11::EffectEnvironmentMap::SetLightEnabled(int nLight, bool bEnabled)
{
    XMVECTOR unwantedOutput[MaxDirectionalLights] = {};

    pImpl->dirtyFlags |= pImpl->lights.SetLightEnabled(nLight, bEnabled, pImpl->constants.lightDiffuseColor, unwantedOutput);
}

void XM_CALLCONV quetzal::direct3d11::EffectEnvironmentMap::SetLightDirection(int nLight, FXMVECTOR value)
{
    EffectLights::ValidateLightIndex(nLight);

    pImpl->constants.lightDirection[nLight] = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

void XM_CALLCONV quetzal::direct3d11::EffectEnvironmentMap::SetLightDiffuseColor(int nLight, FXMVECTOR value)
{
    pImpl->dirtyFlags |= pImpl->lights.SetLightDiffuseColor(nLight, value, pImpl->constants.lightDiffuseColor);
}

void XM_CALLCONV quetzal::direct3d11::EffectEnvironmentMap::SetLightSpecularColor(int, FXMVECTOR)
{
    // Unsupported interface method.
}

void quetzal::direct3d11::EffectEnvironmentMap::EnableDefaultLighting()
{
    EffectLights::EnableDefaultLighting(this);
}

// Fog settings.
void quetzal::direct3d11::EffectEnvironmentMap::SetFogEnabled(bool bEnabled)
{
    pImpl->fog.enabled = bEnabled;

    pImpl->dirtyFlags |= EffectDirtyFlags::FogEnable;
}

void quetzal::direct3d11::EffectEnvironmentMap::SetFogStart(float value)
{
    pImpl->fog.start = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::FogVector;
}

void quetzal::direct3d11::EffectEnvironmentMap::SetFogEnd(float value)
{
    pImpl->fog.end = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::FogVector;
}

void XM_CALLCONV quetzal::direct3d11::EffectEnvironmentMap::SetFogColor(FXMVECTOR value)
{
    pImpl->constants.fogColor = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

// Texture settings.
void quetzal::direct3d11::EffectEnvironmentMap::SetTexture(ID3D11ShaderResourceView* pShaderResourceView)
{
    pImpl->m_pTexture = pShaderResourceView;
}

void quetzal::direct3d11::EffectEnvironmentMap::SetEnvironmentMap(ID3D11ShaderResourceView* pShaderResourceView)
{
    pImpl->environmentMap = pShaderResourceView;
}

// Additional settings.
void quetzal::direct3d11::EffectEnvironmentMap::SetEnvironmentMapAmount(float value)
{
    pImpl->constants.environmentMapAmount = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

void XM_CALLCONV quetzal::direct3d11::EffectEnvironmentMap::SetEnvironmentMapSpecular(FXMVECTOR value)
{
    pImpl->constants.environmentMapSpecular = value;

    pImpl->specularEnabled = !XMVector3Equal(value, XMVectorZero());

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

void quetzal::direct3d11::EffectEnvironmentMap::SetFresnelFactor(float value)
{
    pImpl->constants.fresnelFactor = value;

    pImpl->fresnelEnabled = (value != 0);

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

// Normal compression settings.
void quetzal::direct3d11::EffectEnvironmentMap::SetBiasedVertexNormals(bool value)
{
    pImpl->biasedVertexNormals = value;
}
