//------------------------------------------------------------------------------
// direct3d11
// EffectPhysicallyBased.cpp
//------------------------------------------------------------------------------

#include "EffectPhysicallyBased.hpp"

using namespace std;
using namespace DirectX;

using quetzal::direct3d11::IEffectLights;

namespace
{

    //--------------------------------------------------------------------------
    // Constant buffer layout. Must match the shader!
    struct EffectPhysicallyBasedConstants
    {
        XMVECTOR eyePosition;
        XMMATRIX world;
        XMVECTOR worldInverseTranspose[3];
        XMMATRIX worldViewProj;
        XMMATRIX prevWorldViewProj; // for velocity generation

        XMVECTOR lightDirection[IEffectLights::MaxDirectionalLights];
        XMVECTOR lightDiffuseColor[IEffectLights::MaxDirectionalLights];

        // PBR Parameters
        XMVECTOR Albedo;
        float    Metallic;
        float    Roughness;
        int      numRadianceMipLevels;

        // Size of render target
        float   targetWidth;
        float   targetHeight;
    };

    static_assert((sizeof(EffectPhysicallyBasedConstants) % 16) == 0, "CB size not padded correctly");

    //--------------------------------------------------------------------------
    // Traits type describes our characteristics to the EffectCommon template.
    struct EffectPhysicallyBasedTraits
    {
        using ConstantBufferType = EffectPhysicallyBasedConstants;

        static constexpr int VertexShaderCount = 4;
        static constexpr int PixelShaderCount = 5;
        static constexpr int ShaderPermutationCount = 10;
        static constexpr int RootSignatureCount = 1;
    };

} // namespace

//------------------------------------------------------------------------------
// Internal EffectPhysicallyBased implementation class.
class quetzal::direct3d11::EffectPhysicallyBased::Impl : public EffectCommon<EffectPhysicallyBasedTraits>
{
public:
    Impl(Renderer& renderer);

    com_ptr<ID3D11ShaderResourceView> albedoTexture;
    com_ptr<ID3D11ShaderResourceView> normalTexture;
    com_ptr<ID3D11ShaderResourceView> rmaTexture;
    com_ptr<ID3D11ShaderResourceView> emissiveTexture;

    com_ptr<ID3D11ShaderResourceView> radianceTexture;
    com_ptr<ID3D11ShaderResourceView> irradianceTexture;

    XMVECTOR lightColor[MaxDirectionalLights];

    bool biasedVertexNormals;
    bool velocityEnabled;

    int shader_permutation() const noexcept;

    void apply(ID3D11DeviceContext* pDeviceContext);
};

// Include the precompiled shader code.
namespace
{
#if defined(_XBOX_ONE) && defined(_TITLE)
    #include "Shaders/Compiled/XboxOnePBREffect_VSConstant.inc"
    #include "Shaders/Compiled/XboxOnePBREffect_VSConstantVelocity.inc"
    #include "Shaders/Compiled/XboxOnePBREffect_VSConstantBn.inc"
    #include "Shaders/Compiled/XboxOnePBREffect_VSConstantVelocityBn.inc"

    #include "Shaders/Compiled/XboxOnePBREffect_PSConstant.inc"
    #include "Shaders/Compiled/XboxOnePBREffect_PSTextured.inc"
    #include "Shaders/Compiled/XboxOnePBREffect_PSTexturedEmissive.inc"
    #include "Shaders/Compiled/XboxOnePBREffect_PSTexturedVelocity.inc"
    #include "Shaders/Compiled/XboxOnePBREffect_PSTexturedEmissiveVelocity.inc"
#else
    #include "Shaders/Compiled/PBREffect_VSConstant.inc"
    #include "Shaders/Compiled/PBREffect_VSConstantVelocity.inc"
    #include "Shaders/Compiled/PBREffect_VSConstantBn.inc"
    #include "Shaders/Compiled/PBREffect_VSConstantVelocityBn.inc"

    #include "Shaders/Compiled/PBREffect_PSConstant.inc"
    #include "Shaders/Compiled/PBREffect_PSTextured.inc"
    #include "Shaders/Compiled/PBREffect_PSTexturedEmissive.inc"
    #include "Shaders/Compiled/PBREffect_PSTexturedVelocity.inc"
    #include "Shaders/Compiled/PBREffect_PSTexturedEmissiveVelocity.inc"
#endif
}

template<>
const quetzal::direct3d11::VertexShaderManager<EffectPhysicallyBasedTraits>::bytecode_array_type quetzal::direct3d11::VertexShaderManager<EffectPhysicallyBasedTraits>::m_bytecode =
{{
    { PBREffect_VSConstant, sizeof(PBREffect_VSConstant) },
    { PBREffect_VSConstantVelocity, sizeof(PBREffect_VSConstantVelocity) },
    { PBREffect_VSConstantBn, sizeof(PBREffect_VSConstantBn) },
    { PBREffect_VSConstantVelocityBn, sizeof(PBREffect_VSConstantVelocityBn) },
}};

template<>
const quetzal::direct3d11::VertexShaderManager<EffectPhysicallyBasedTraits>::shader_array_type quetzal::direct3d11::VertexShaderManager<EffectPhysicallyBasedTraits>::m_nShader =
{
    0,      // constant
    0,      // textured
    0,      // textured + emissive
    1,      // textured + velocity
    1,      // textured + emissive + velocity

    2,      // constant (biased vertex normals)
    2,      // textured (biased vertex normals)
    2,      // textured + emissive (biased vertex normals)
    3,      // textured + velocity (biased vertex normals)
    3,      // textured + emissive + velocity (biasoed vertex normals)
};

template<>
const quetzal::direct3d11::PixelShaderManager<EffectPhysicallyBasedTraits>::bytecode_array_type quetzal::direct3d11::PixelShaderManager<EffectPhysicallyBasedTraits>::m_bytecode =
{{
    { PBREffect_PSConstant, sizeof(PBREffect_PSConstant) },
    { PBREffect_PSTextured, sizeof(PBREffect_PSTextured) },
    { PBREffect_PSTexturedEmissive, sizeof(PBREffect_PSTexturedEmissive) },
    { PBREffect_PSTexturedVelocity, sizeof(PBREffect_PSTexturedVelocity) },
    { PBREffect_PSTexturedEmissiveVelocity, sizeof(PBREffect_PSTexturedEmissiveVelocity) }
}};

template<>
const quetzal::direct3d11::PixelShaderManager<EffectPhysicallyBasedTraits>::shader_array_type quetzal::direct3d11::PixelShaderManager<EffectPhysicallyBasedTraits>::m_nShader =
{
    0,      // constant
    1,      // textured
    2,      // textured + emissive
    3,      // textured + velocity
    4,      // textured + emissive + velocity

    0,      // constant (biased vertex normals)
    1,      // textured (biased vertex normals)
    2,      // textured + emissive (biased vertex normals)
    3,      // textured + velocity (biased vertex normals)
    4,      // textured + emissive + velocity (biased vertex normals)
};

quetzal::direct3d11::EffectPhysicallyBased::Impl::Impl(Renderer& renderer) :
    EffectCommon(renderer),
    biasedVertexNormals(false),
    velocityEnabled(false),
    lightColor{}
{
//    static_assert(static_cast<int>(std::size(EffectCommon<EffectPhysicallyBasedTraits>::VertexShaderIndices)) == EffectPhysicallyBasedTraits::ShaderPermutationCount, "array/max mismatch");
//    static_assert(static_cast<int>(std::size(EffectCommon<EffectPhysicallyBasedTraits>::VertexShaderBytecode)) == EffectPhysicallyBasedTraits::VertexShaderCount, "array/max mismatch");
//    static_assert(static_cast<int>(std::size(EffectCommon<EffectPhysicallyBasedTraits>::PixelShaderBytecode)) == EffectPhysicallyBasedTraits::PixelShaderCount, "array/max mismatch");
//    static_assert(static_cast<int>(std::size(EffectCommon<EffectPhysicallyBasedTraits>::PixelShaderIndices)) == EffectPhysicallyBasedTraits::ShaderPermutationCount, "array/max mismatch");

    // Lighting
    static const XMVECTORF32 defaultLightDirection = { { { 0, -1, 0, 0 } } };
    for (int i = 0; i < MaxDirectionalLights; i++)
    {
        lightColor[i] = g_XMOne;
        constants.lightDirection[i] = defaultLightDirection;
        constants.lightDiffuseColor[i] = g_XMZero;
    }

    // Default PBR values
    constants.Albedo = g_XMOne;
    constants.Metallic = 0.5f;
    constants.Roughness = 0.2f;
    constants.numRadianceMipLevels = 1;
}

int quetzal::direct3d11::EffectPhysicallyBased::Impl::shader_permutation() const noexcept
{
    int permutation = 0;

    // Textured RMA vs. constant albedo/roughness/metalness?
    if (velocityEnabled)
    {
        // Optional velocity buffer (implies textured RMA)?
        permutation = 3;
    }
    else if (albedoTexture)
    {
        permutation = 1;
    }

    // Using an emissive texture?
    if (emissiveTexture)
    {
        permutation += 1;
    }

    if (biasedVertexNormals)
    {
        // Compressed normals need to be scaled and biased in the vertex shader.
        permutation += 5;
    }

    return permutation;
}

// Sets our state onto the D3D device.
void quetzal::direct3d11::EffectPhysicallyBased::Impl::apply(ID3D11DeviceContext* pDeviceContext)
{
    // Store old wvp for velocity calculation in shader
    constants.prevWorldViewProj = constants.worldViewProj;

    // Compute derived parameter values.
    matrices.SetConstants(dirtyFlags, constants.worldViewProj);

    // World inverse transpose matrix.
    if (dirtyFlags & EffectDirtyFlags::WorldInverseTranspose)
    {
        constants.world = XMMatrixTranspose(matrices.world);

        XMMATRIX worldInverse = XMMatrixInverse(nullptr, matrices.world);

        constants.worldInverseTranspose[0] = worldInverse.r[0];
        constants.worldInverseTranspose[1] = worldInverse.r[1];
        constants.worldInverseTranspose[2] = worldInverse.r[2];

        dirtyFlags &= ~EffectDirtyFlags::WorldInverseTranspose;
        dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
    }

    // Eye position vector.
    if (dirtyFlags & EffectDirtyFlags::EyePosition)
    {
        XMMATRIX viewInverse = XMMatrixInverse(nullptr, matrices.view);

        constants.eyePosition = viewInverse.r[3];

        dirtyFlags &= ~EffectDirtyFlags::EyePosition;
        dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
    }

    // Set the textures
    if (albedoTexture)
    {
        ID3D11ShaderResourceView* textures[] =
        {
            albedoTexture,
            normalTexture,
            rmaTexture,
            emissiveTexture,
            radianceTexture,
            irradianceTexture
        };
        pDeviceContext->PSSetShaderResources(0, static_cast<UINT>(std::size(textures)), textures);
    }
    else
    {
        ID3D11ShaderResourceView* textures[] =
        {
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            radianceTexture,
            irradianceTexture
        };
        pDeviceContext->PSSetShaderResources(0, static_cast<UINT>(std::size(textures)), textures);
    }

    // Set shaders and constant buffers.
    apply_shaders(pDeviceContext, shader_permutation());
}

quetzal::direct3d11::EffectPhysicallyBased::EffectPhysicallyBased(Renderer& renderer) :
    pImpl(std::make_unique<Impl>(renderer))
{
}

quetzal::direct3d11::EffectPhysicallyBased::EffectPhysicallyBased(EffectPhysicallyBased&& other) noexcept :
    pImpl(std::move(other.pImpl))
{
}

quetzal::direct3d11::EffectPhysicallyBased& quetzal::direct3d11::EffectPhysicallyBased::operator=(EffectPhysicallyBased&& other) noexcept
{
    pImpl = std::move(other.pImpl);
    return *this;
}

// IEffect methods.
void quetzal::direct3d11::EffectPhysicallyBased::apply(ID3D11DeviceContext* pDeviceContext)
{
    pImpl->apply(pDeviceContext);
}

quetzal::direct3d11::ShaderBytecode quetzal::direct3d11::EffectPhysicallyBased::vertex_shader_bytecode() const
{
    return pImpl->vertex_shader_bytecode(pImpl->shader_permutation());
}

// Camera settings.
void XM_CALLCONV quetzal::direct3d11::EffectPhysicallyBased::SetWorld(FXMMATRIX value)
{
    pImpl->matrices.world = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::WorldInverseTranspose;
}

void XM_CALLCONV quetzal::direct3d11::EffectPhysicallyBased::SetView(FXMMATRIX value)
{
    pImpl->matrices.view = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::EyePosition;
}

void XM_CALLCONV quetzal::direct3d11::EffectPhysicallyBased::SetProjection(FXMMATRIX value)
{
    pImpl->matrices.projection = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj;
}

void XM_CALLCONV quetzal::direct3d11::EffectPhysicallyBased::SetMatrices(FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection)
{
    pImpl->matrices.world = world;
    pImpl->matrices.view = view;
    pImpl->matrices.projection = projection;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::WorldInverseTranspose | EffectDirtyFlags::EyePosition;
}

// Light settings
void quetzal::direct3d11::EffectPhysicallyBased::SetLightingEnabled(bool bEnabled)
{
    if (!bEnabled)
    {
        throw std::invalid_argument("EffectPhysicallyBased does not support turning off lighting");
    }
}

void quetzal::direct3d11::EffectPhysicallyBased::SetPerPixelLighting(bool)
{
    // Unsupported interface method.
}

void XM_CALLCONV quetzal::direct3d11::EffectPhysicallyBased::SetAmbientLightColor(FXMVECTOR)
{
    // Unsupported interface.
}

void quetzal::direct3d11::EffectPhysicallyBased::SetLightEnabled(int whichLight, bool value)
{
    EffectLights::ValidateLightIndex(whichLight);

    pImpl->constants.lightDiffuseColor[whichLight] = (value) ? pImpl->lightColor[whichLight] : g_XMZero;

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

void XM_CALLCONV quetzal::direct3d11::EffectPhysicallyBased::SetLightDirection(int whichLight, FXMVECTOR value)
{
    EffectLights::ValidateLightIndex(whichLight);

    pImpl->constants.lightDirection[whichLight] = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

void XM_CALLCONV quetzal::direct3d11::EffectPhysicallyBased::SetLightDiffuseColor(int whichLight, FXMVECTOR value)
{
    EffectLights::ValidateLightIndex(whichLight);

    pImpl->lightColor[whichLight] = value;
    pImpl->constants.lightDiffuseColor[whichLight] = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

void XM_CALLCONV quetzal::direct3d11::EffectPhysicallyBased::SetLightSpecularColor(int, FXMVECTOR)
{
    // Unsupported interface.
}

void quetzal::direct3d11::EffectPhysicallyBased::EnableDefaultLighting()
{
    EffectLights::EnableDefaultLighting(this);
}

// PBR Settings
void quetzal::direct3d11::EffectPhysicallyBased::SetAlpha(float value)
{
    // Set w to new value, but preserve existing xyz (constant albedo).
    pImpl->constants.Albedo = XMVectorSetW(pImpl->constants.Albedo, value);

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

void quetzal::direct3d11::EffectPhysicallyBased::SetConstantAlbedo(FXMVECTOR value)
{
    // Set xyz to new value, but preserve existing w (alpha).
    pImpl->constants.Albedo = XMVectorSelect(pImpl->constants.Albedo, value, g_XMSelect1110);

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

void quetzal::direct3d11::EffectPhysicallyBased::SetConstantMetallic(float value)
{
    pImpl->constants.Metallic = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

void quetzal::direct3d11::EffectPhysicallyBased::SetConstantRoughness(float value)
{
    pImpl->constants.Roughness = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

// Texture settings.
void quetzal::direct3d11::EffectPhysicallyBased::SetAlbedoTexture(ID3D11ShaderResourceView* value)
{
    pImpl->albedoTexture = value;
}

void quetzal::direct3d11::EffectPhysicallyBased::SetNormalTexture(ID3D11ShaderResourceView* value)
{
    pImpl->normalTexture = value;
}

void quetzal::direct3d11::EffectPhysicallyBased::SetRMATexture(ID3D11ShaderResourceView* value)
{
    pImpl->rmaTexture = value;
}

void quetzal::direct3d11::EffectPhysicallyBased::SetEmissiveTexture(ID3D11ShaderResourceView* value)
{
    pImpl->emissiveTexture = value;
}

void quetzal::direct3d11::EffectPhysicallyBased::SetSurfaceTextures(
    ID3D11ShaderResourceView* albedo,
    ID3D11ShaderResourceView* normal,
    ID3D11ShaderResourceView* roughnessMetallicAmbientOcclusion)
{
    pImpl->albedoTexture = albedo;
    pImpl->normalTexture = normal;
    pImpl->rmaTexture = roughnessMetallicAmbientOcclusion;
}

void quetzal::direct3d11::EffectPhysicallyBased::SetIBLTextures(
    ID3D11ShaderResourceView* radiance,
    int numRadianceMips,
    ID3D11ShaderResourceView* irradiance)
{
    pImpl->radianceTexture = radiance;
    pImpl->irradianceTexture = irradiance;

    pImpl->constants.numRadianceMipLevels = numRadianceMips;
    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

// Normal compression settings.
void quetzal::direct3d11::EffectPhysicallyBased::SetBiasedVertexNormals(bool value)
{
    pImpl->biasedVertexNormals = value;
}

// Additional settings.
void quetzal::direct3d11::EffectPhysicallyBased::SetVelocityGeneration(bool value)
{
    pImpl->velocityEnabled = value;
}

void quetzal::direct3d11::EffectPhysicallyBased::SetRenderTargetSizeInPixels(int width, int height)
{
    pImpl->constants.targetWidth = static_cast<float>(width);
    pImpl->constants.targetHeight = static_cast<float>(height);

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}
