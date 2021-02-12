//--------------------------------------------------------------------------------------
// direct3d11
// EffectDualTexture.cpp
//--------------------------------------------------------------------------------------

#include "EffectDualTexture.hpp"

using namespace std;
using namespace DirectX;

namespace
{

    //--------------------------------------------------------------------------
    // Constant buffer layout. Must match the shader!
    struct EffectDualTextureConstants
    {
        XMVECTOR diffuseColor;
        XMVECTOR fogColor;
        XMVECTOR fogVector;
        XMMATRIX worldViewProj;
    };

    static_assert( ( sizeof(EffectDualTextureConstants) % 16 ) == 0, "CB size not padded correctly" );

    //--------------------------------------------------------------------------
    // Traits type describes our characteristics to the EffectCommon template.
    struct EffectDualTextureTraits
    {
        using ConstantBufferType = EffectDualTextureConstants;

        static constexpr int VertexShaderCount = 4;
        static constexpr int PixelShaderCount = 2;
        static constexpr int ShaderPermutationCount = 4;
    };

} // namespace

//------------------------------------------------------------------------------
// Internal EffectDualTexture implementation class.
class quetzal::direct3d11::EffectDualTexture::Impl : public EffectCommon<EffectDualTextureTraits>
{
public:

    Impl(Renderer& renderer);

    bool vertexColorEnabled;

    EffectColor color;

    com_ptr<ID3D11ShaderResourceView> m_pTexture2;

    int shader_permutation() const noexcept;

    void apply(ID3D11DeviceContext* pDeviceContext);
};

// Include the precompiled shader code.
namespace
{
#if defined(_XBOX_ONE) && defined(_TITLE)
    #include "Shaders/Compiled/XboxOneDualTextureEffect_VSDualTexture.inc"
    #include "Shaders/Compiled/XboxOneDualTextureEffect_VSDualTextureNoFog.inc"
    #include "Shaders/Compiled/XboxOneDualTextureEffect_VSDualTextureVc.inc"
    #include "Shaders/Compiled/XboxOneDualTextureEffect_VSDualTextureVcNoFog.inc"

    #include "Shaders/Compiled/XboxOneDualTextureEffect_PSDualTexture.inc"
    #include "Shaders/Compiled/XboxOneDualTextureEffect_PSDualTextureNoFog.inc"
#else
    #include "Shaders/Compiled/DualTextureEffect_VSDualTexture.inc"
    #include "Shaders/Compiled/DualTextureEffect_VSDualTextureNoFog.inc"
    #include "Shaders/Compiled/DualTextureEffect_VSDualTextureVc.inc"
    #include "Shaders/Compiled/DualTextureEffect_VSDualTextureVcNoFog.inc"

    #include "Shaders/Compiled/DualTextureEffect_PSDualTexture.inc"
    #include "Shaders/Compiled/DualTextureEffect_PSDualTextureNoFog.inc"
#endif
}

template<>
const quetzal::direct3d11::VertexShaderManager<EffectDualTextureTraits>::bytecode_array_type quetzal::direct3d11::VertexShaderManager<EffectDualTextureTraits>::m_bytecode =
{{
    { DualTextureEffect_VSDualTexture,        sizeof(DualTextureEffect_VSDualTexture)        },
    { DualTextureEffect_VSDualTextureNoFog,   sizeof(DualTextureEffect_VSDualTextureNoFog)   },
    { DualTextureEffect_VSDualTextureVc,      sizeof(DualTextureEffect_VSDualTextureVc)      },
    { DualTextureEffect_VSDualTextureVcNoFog, sizeof(DualTextureEffect_VSDualTextureVcNoFog) },
}};

template<>
const quetzal::direct3d11::VertexShaderManager<EffectDualTextureTraits>::shader_array_type quetzal::direct3d11::VertexShaderManager<EffectDualTextureTraits>::m_nShader =
{{
    0,      // basic
    1,      // no fog
    2,      // vertex color
    3,      // vertex color, no fog
}};

template<>
const quetzal::direct3d11::PixelShaderManager<EffectDualTextureTraits>::bytecode_array_type quetzal::direct3d11::PixelShaderManager<EffectDualTextureTraits>::m_bytecode =
{{
    { DualTextureEffect_PSDualTexture,        sizeof(DualTextureEffect_PSDualTexture)        },
    { DualTextureEffect_PSDualTextureNoFog,   sizeof(DualTextureEffect_PSDualTextureNoFog)   },
}};

template<>
const quetzal::direct3d11::PixelShaderManager<EffectDualTextureTraits>::shader_array_type quetzal::direct3d11::PixelShaderManager<EffectDualTextureTraits>::m_nShader =
{{
    0,      // basic
    1,      // no fog
    0,      // vertex color
    1,      // vertex color, no fog
}};

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectDualTexture::Impl::Impl(Renderer& renderer) :
    EffectCommon(renderer),
    vertexColorEnabled(false)
{
// shader manager arrays are sized with traits anyway ...
//    static_assert( _countof(EffectCommon<EffectDualTextureTraits>::VertexShaderIndices) == EffectDualTextureTraits::ShaderPermutationCount, "array/max mismatch" );
//    static_assert( _countof(EffectCommon<EffectDualTextureTraits>::VertexShaderBytecode) == EffectDualTextureTraits::VertexShaderCount, "array/max mismatch" );
//    static_assert( _countof(EffectCommon<EffectDualTextureTraits>::PixelShaderBytecode) == EffectDualTextureTraits::PixelShaderCount, "array/max mismatch" );
//    static_assert( _countof(EffectCommon<EffectDualTextureTraits>::PixelShaderIndices) == EffectDualTextureTraits::ShaderPermutationCount, "array/max mismatch" );
}

int quetzal::direct3d11::EffectDualTexture::Impl::shader_permutation() const noexcept
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

    return permutation;
}

// Sets our state onto the D3D device.
void quetzal::direct3d11::EffectDualTexture::Impl::apply(ID3D11DeviceContext* pDeviceContext)
{
    // Compute derived parameter values.
    matrices.SetConstants(dirtyFlags, constants.worldViewProj);

    fog.SetConstants(dirtyFlags, matrices.worldView, constants.fogVector);

    color.SetConstants(dirtyFlags, constants.diffuseColor);

    // Set the textures.
    ID3D11ShaderResourceView* textures[2] =
    {
        m_pTexture,
        m_pTexture2
    };

    pDeviceContext->PSSetShaderResources(0, 2, textures);

    // Set shaders and constant buffers.
    apply_shaders(pDeviceContext, shader_permutation());
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectDualTexture::EffectDualTexture(Renderer& renderer) :
    pImpl(new Impl(renderer))
{
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectDualTexture::EffectDualTexture(EffectDualTexture&& other) :
    pImpl(std::move(other.pImpl))
{
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectDualTexture& quetzal::direct3d11::EffectDualTexture::operator=(EffectDualTexture&& other)
{
    pImpl = std::move(other.pImpl);
    return *this;
}

// IEffect methods.
void quetzal::direct3d11::EffectDualTexture::apply(ID3D11DeviceContext* pDeviceContext)
{
    pImpl->apply(pDeviceContext);
}

quetzal::direct3d11::ShaderBytecode quetzal::direct3d11::EffectDualTexture::vertex_shader_bytecode() const
{
    return pImpl->vertex_shader_bytecode(pImpl->shader_permutation());
}

// Camera settings.
void XM_CALLCONV quetzal::direct3d11::EffectDualTexture::SetWorld(FXMMATRIX value)
{
    pImpl->matrices.world = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::WorldInverseTranspose | EffectDirtyFlags::FogVector;
}

void XM_CALLCONV quetzal::direct3d11::EffectDualTexture::SetView(FXMMATRIX value)
{
    pImpl->matrices.view = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::EyePosition | EffectDirtyFlags::FogVector;
}

void XM_CALLCONV quetzal::direct3d11::EffectDualTexture::SetProjection(FXMMATRIX value)
{
    pImpl->matrices.projection = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj;
}

void XM_CALLCONV quetzal::direct3d11::EffectDualTexture::SetMatrices(FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection)
{
    pImpl->matrices.world = world;
    pImpl->matrices.view = view;
    pImpl->matrices.projection = projection;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::WorldInverseTranspose | EffectDirtyFlags::EyePosition | EffectDirtyFlags::FogVector;
}

// Material settings.
void XM_CALLCONV quetzal::direct3d11::EffectDualTexture::SetDiffuseColor(FXMVECTOR value)
{
    pImpl->color.diffuseColor = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}

void quetzal::direct3d11::EffectDualTexture::SetAlpha(float value)
{
    pImpl->color.alpha = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}

void XM_CALLCONV quetzal::direct3d11::EffectDualTexture::SetColorAndAlpha(FXMVECTOR value)
{
    pImpl->color.diffuseColor = value;
    pImpl->color.alpha = XMVectorGetW(value);

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}

// Fog settings.
void quetzal::direct3d11::EffectDualTexture::SetFogEnabled(bool bEnabled)
{
    pImpl->fog.enabled = bEnabled;

    pImpl->dirtyFlags |= EffectDirtyFlags::FogEnable;
}

void quetzal::direct3d11::EffectDualTexture::SetFogStart(float value)
{
    pImpl->fog.start = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::FogVector;
}

void quetzal::direct3d11::EffectDualTexture::SetFogEnd(float value)
{
    pImpl->fog.end = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::FogVector;
}

void XM_CALLCONV quetzal::direct3d11::EffectDualTexture::SetFogColor(FXMVECTOR value)
{
    pImpl->constants.fogColor = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

// Vertex color setting.
void quetzal::direct3d11::EffectDualTexture::SetVertexColorEnabled(bool bEnabled)
{
    pImpl->vertexColorEnabled = bEnabled;
}

// Texture settings.
void quetzal::direct3d11::EffectDualTexture::SetTexture(ID3D11ShaderResourceView* pShaderResourceView)
{
    pImpl->m_pTexture = pShaderResourceView;
}

void quetzal::direct3d11::EffectDualTexture::SetTexture2(ID3D11ShaderResourceView* pShaderResourceView)
{
    pImpl->m_pTexture2 = pShaderResourceView;
}
