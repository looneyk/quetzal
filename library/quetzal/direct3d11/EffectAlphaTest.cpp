//------------------------------------------------------------------------------
// direct3d11
// EffectAlphaTest.cpp
//------------------------------------------------------------------------------

#include "EffectAlphaTest.hpp"
#include <stdexcept>

using namespace std;
using namespace DirectX;

namespace
{

    //--------------------------------------------------------------------------
    // Constant buffer layout. Must match the shader!
    struct EffectAlphaTestConstants
    {
        XMVECTOR diffuseColor;
        XMVECTOR alphaTest;
        XMVECTOR fogColor;
        XMVECTOR fogVector;
        XMMATRIX worldViewProj;
    };

    static_assert( ( sizeof(EffectAlphaTestConstants) % 16 ) == 0, "CB size not padded correctly" );

    //--------------------------------------------------------------------------
    // Traits type describes our characteristics to the EffectCommon template.
    struct EffectAlphaTestTraits
    {
        using ConstantBufferType = EffectAlphaTestConstants;

        static constexpr int VertexShaderCount = 4;
        static constexpr int PixelShaderCount = 4;
        static constexpr int ShaderPermutationCount = 8;
    };

} // namespace

//------------------------------------------------------------------------------
// Internal EffectAlphaTest implementation class.
class quetzal::direct3d11::EffectAlphaTest::Impl : public EffectCommon<EffectAlphaTestTraits>
{
public:
    Impl(Renderer& renderer);

    D3D11_COMPARISON_FUNC alphaFunction;
    int referenceAlpha;

    bool vertexColorEnabled;

    EffectColor color;

    int shader_permutation() const noexcept;

    void apply(ID3D11DeviceContext* pDeviceContext);
};

// Include the precompiled shader code.
namespace
{
#if defined(_XBOX_ONE) && defined(_TITLE)
    #include "Shaders/Compiled/XboxOneAlphaTestEffect_VSAlphaTest.inc"
    #include "Shaders/Compiled/XboxOneAlphaTestEffect_VSAlphaTestNoFog.inc"
    #include "Shaders/Compiled/XboxOneAlphaTestEffect_VSAlphaTestVc.inc"
    #include "Shaders/Compiled/XboxOneAlphaTestEffect_VSAlphaTestVcNoFog.inc"

    #include "Shaders/Compiled/XboxOneAlphaTestEffect_PSAlphaTestLtGt.inc"
    #include "Shaders/Compiled/XboxOneAlphaTestEffect_PSAlphaTestLtGtNoFog.inc"
    #include "Shaders/Compiled/XboxOneAlphaTestEffect_PSAlphaTestEqNe.inc"
    #include "Shaders/Compiled/XboxOneAlphaTestEffect_PSAlphaTestEqNeNoFog.inc"
#else
    #include "Shaders/Compiled/AlphaTestEffect_VSAlphaTest.inc"
    #include "Shaders/Compiled/AlphaTestEffect_VSAlphaTestNoFog.inc"
    #include "Shaders/Compiled/AlphaTestEffect_VSAlphaTestVc.inc"
    #include "Shaders/Compiled/AlphaTestEffect_VSAlphaTestVcNoFog.inc"

    #include "Shaders/Compiled/AlphaTestEffect_PSAlphaTestLtGt.inc"
    #include "Shaders/Compiled/AlphaTestEffect_PSAlphaTestLtGtNoFog.inc"
    #include "Shaders/Compiled/AlphaTestEffect_PSAlphaTestEqNe.inc"
    #include "Shaders/Compiled/AlphaTestEffect_PSAlphaTestEqNeNoFog.inc"
#endif
}

template<>
const quetzal::direct3d11::VertexShaderManager<EffectAlphaTestTraits>::bytecode_array_type quetzal::direct3d11::VertexShaderManager<EffectAlphaTestTraits>::m_bytecode =
{{
    { AlphaTestEffect_VSAlphaTest,        sizeof(AlphaTestEffect_VSAlphaTest)        },
    { AlphaTestEffect_VSAlphaTestNoFog,   sizeof(AlphaTestEffect_VSAlphaTestNoFog)   },
    { AlphaTestEffect_VSAlphaTestVc,      sizeof(AlphaTestEffect_VSAlphaTestVc)      },
    { AlphaTestEffect_VSAlphaTestVcNoFog, sizeof(AlphaTestEffect_VSAlphaTestVcNoFog) },
}};

template<>
const quetzal::direct3d11::VertexShaderManager<EffectAlphaTestTraits>::shader_array_type quetzal::direct3d11::VertexShaderManager<EffectAlphaTestTraits>::m_nShader =
{{
    0,      // lt/gt
    1,      // lt/gt, no fog
    2,      // lt/gt, vertex color
    3,      // lt/gt, vertex color, no fog

    0,      // eq/ne
    1,      // eq/ne, no fog
    2,      // eq/ne, vertex color
    3,      // eq/ne, vertex color, no fog
}};

template<>
const quetzal::direct3d11::PixelShaderManager<EffectAlphaTestTraits>::bytecode_array_type quetzal::direct3d11::PixelShaderManager<EffectAlphaTestTraits>::m_bytecode =
{{
    { AlphaTestEffect_PSAlphaTestLtGt,      sizeof(AlphaTestEffect_PSAlphaTestLtGt)      },
    { AlphaTestEffect_PSAlphaTestLtGtNoFog, sizeof(AlphaTestEffect_PSAlphaTestLtGtNoFog) },
    { AlphaTestEffect_PSAlphaTestEqNe,      sizeof(AlphaTestEffect_PSAlphaTestEqNe)      },
    { AlphaTestEffect_PSAlphaTestEqNeNoFog, sizeof(AlphaTestEffect_PSAlphaTestEqNeNoFog) },
}};

template<>
const quetzal::direct3d11::PixelShaderManager<EffectAlphaTestTraits>::shader_array_type quetzal::direct3d11::PixelShaderManager<EffectAlphaTestTraits>::m_nShader =
{{
    0,      // lt/gt
    1,      // lt/gt, no fog
    0,      // lt/gt, vertex color
    1,      // lt/gt, vertex color, no fog

    2,      // eq/ne
    3,      // eq/ne, no fog
    2,      // eq/ne, vertex color
    3,      // eq/ne, vertex color, no fog
}};

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectAlphaTest::Impl::Impl(Renderer& renderer) :
    EffectCommon(renderer),
    alphaFunction(D3D11_COMPARISON_GREATER),
    referenceAlpha(0),
    vertexColorEnabled(false)
{
// shader manager arrays are sized with traits anyway ...
//    static_assert( _countof(EffectCommon<EffectAlphaTestTraits>::VertexShaderIndices) == EffectAlphaTestTraits::ShaderPermutationCount, "array/max mismatch" );
//    static_assert( _countof(EffectCommon<EffectAlphaTestTraits>::VertexShaderBytecode) == EffectAlphaTestTraits::VertexShaderCount, "array/max mismatch" );
//    static_assert( _countof(EffectCommon<EffectAlphaTestTraits>::PixelShaderBytecode) == EffectAlphaTestTraits::PixelShaderCount, "array/max mismatch" );
//    static_assert( _countof(EffectCommon<EffectAlphaTestTraits>::PixelShaderIndices) == EffectAlphaTestTraits::ShaderPermutationCount, "array/max mismatch" );
}

int quetzal::direct3d11::EffectAlphaTest::Impl::shader_permutation() const noexcept
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

    // Which alpha compare mode?
    if (alphaFunction == D3D11_COMPARISON_EQUAL ||
        alphaFunction == D3D11_COMPARISON_NOT_EQUAL)
    {
        permutation += 4;
    }

    return permutation;
}

// Sets our state onto the D3D device.
void quetzal::direct3d11::EffectAlphaTest::Impl::apply(ID3D11DeviceContext* pDeviceContext)
{
    // Compute derived parameter values.
    matrices.SetConstants(dirtyFlags, constants.worldViewProj);

    fog.SetConstants(dirtyFlags, matrices.worldView, constants.fogVector);

    color.SetConstants(dirtyFlags, constants.diffuseColor);

    // Recompute the alpha test settings?
    if (dirtyFlags & EffectDirtyFlags::AlphaTest)
    {
        // Convert reference alpha from 8 bit integer to 0-1 float format.
        float reference = (float)referenceAlpha / 255.0f;

        // Comparison tolerance of half the 8 bit integer precision.
        const float threshold = 0.5f / 255.0f;

        // What to do if the alpha comparison passes or fails. Positive accepts the pixel, negative clips it.
        static const XMVECTORF32 selectIfTrue  = { { {  1, -1 } } };
        static const XMVECTORF32 selectIfFalse = { { { -1,  1 } } };
        static const XMVECTORF32 selectNever   = { { { -1, -1 } } };
        static const XMVECTORF32 selectAlways  = { { {  1,  1 } } };

        float compareTo;
        XMVECTOR resultSelector;

        switch (alphaFunction)
        {
            case D3D11_COMPARISON_LESS:
                // Shader will evaluate: clip((a < x) ? z : w)
                compareTo = reference - threshold;
                resultSelector = selectIfTrue;
                break;

            case D3D11_COMPARISON_LESS_EQUAL:
                // Shader will evaluate: clip((a < x) ? z : w)
                compareTo = reference + threshold;
                resultSelector = selectIfTrue;
                break;

            case D3D11_COMPARISON_GREATER_EQUAL:
                // Shader will evaluate: clip((a < x) ? z : w)
                compareTo = reference - threshold;
                resultSelector = selectIfFalse;
                break;

            case D3D11_COMPARISON_GREATER:
                // Shader will evaluate: clip((a < x) ? z : w)
                compareTo = reference + threshold;
                resultSelector = selectIfFalse;
                break;

            case D3D11_COMPARISON_EQUAL:
                // Shader will evaluate: clip((abs(a - x) < y) ? z : w)
                compareTo = reference;
                resultSelector = selectIfTrue;
                break;

            case D3D11_COMPARISON_NOT_EQUAL:
                // Shader will evaluate: clip((abs(a - x) < y) ? z : w)
                compareTo = reference;
                resultSelector = selectIfFalse;
                break;

            case D3D11_COMPARISON_NEVER:
                // Shader will evaluate: clip((a < x) ? z : w)
                compareTo = 0;
                resultSelector = selectNever;
                break;

            case D3D11_COMPARISON_ALWAYS:
                // Shader will evaluate: clip((a < x) ? z : w)
                compareTo = 0;
                resultSelector = selectAlways;
                break;

            default:
                throw std::runtime_error("Unknown alpha test function");
        }

        // x = compareTo, y = threshold, zw = resultSelector.
        constants.alphaTest = XMVectorPermute<0, 1, 4, 5>(XMVectorSet(compareTo, threshold, 0, 0), resultSelector);

        dirtyFlags &= ~EffectDirtyFlags::AlphaTest;
        dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
    }

    // Set the texture.
    ID3D11ShaderResourceView* ppShaderResourceView[] = {m_pTexture};

    pDeviceContext->PSSetShaderResources(0, 1, ppShaderResourceView);

    // Set shaders and constant buffers.
    apply_shaders(pDeviceContext, shader_permutation());
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectAlphaTest::EffectAlphaTest(Renderer& renderer) :
    pImpl(new Impl(renderer))
{
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectAlphaTest::EffectAlphaTest(EffectAlphaTest&& other) :
    pImpl(std::move(other.pImpl))
{
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectAlphaTest& quetzal::direct3d11::EffectAlphaTest::operator=(EffectAlphaTest&& other)
{
    pImpl = std::move(other.pImpl);
    return *this;
}

// IEffect methods.
void quetzal::direct3d11::EffectAlphaTest::apply(ID3D11DeviceContext* pDeviceContext)
{
    pImpl->apply(pDeviceContext);
}

quetzal::direct3d11::ShaderBytecode quetzal::direct3d11::EffectAlphaTest::vertex_shader_bytecode() const
{
    return pImpl->vertex_shader_bytecode(pImpl->shader_permutation());
}

// Camera settings.
void XM_CALLCONV quetzal::direct3d11::EffectAlphaTest::SetWorld(FXMMATRIX value)
{
    pImpl->matrices.world = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::WorldInverseTranspose | EffectDirtyFlags::FogVector;
}

void XM_CALLCONV quetzal::direct3d11::EffectAlphaTest::SetView(FXMMATRIX value)
{
    pImpl->matrices.view = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::EyePosition | EffectDirtyFlags::FogVector;
}

void XM_CALLCONV quetzal::direct3d11::EffectAlphaTest::SetProjection(FXMMATRIX value)
{
    pImpl->matrices.projection = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj;
}

void XM_CALLCONV quetzal::direct3d11::EffectAlphaTest::SetMatrices(FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection)
{
    pImpl->matrices.world = world;
    pImpl->matrices.view = view;
    pImpl->matrices.projection = projection;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::WorldInverseTranspose | EffectDirtyFlags::EyePosition | EffectDirtyFlags::FogVector;
}

// Material settings
void XM_CALLCONV quetzal::direct3d11::EffectAlphaTest::SetDiffuseColor(FXMVECTOR value)
{
    pImpl->color.diffuseColor = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}

void quetzal::direct3d11::EffectAlphaTest::SetAlpha(float value)
{
    pImpl->color.alpha = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}

void XM_CALLCONV quetzal::direct3d11::EffectAlphaTest::SetColorAndAlpha(FXMVECTOR value)
{
    pImpl->color.diffuseColor = value;
    pImpl->color.alpha = XMVectorGetW(value);

    pImpl->dirtyFlags |= EffectDirtyFlags::MaterialColor;
}

// Fog settings.
void quetzal::direct3d11::EffectAlphaTest::SetFogEnabled(bool bEnabled)
{
    pImpl->fog.enabled = bEnabled;

    pImpl->dirtyFlags |= EffectDirtyFlags::FogEnable;
}

void quetzal::direct3d11::EffectAlphaTest::SetFogStart(float value)
{
    pImpl->fog.start = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::FogVector;
}

void quetzal::direct3d11::EffectAlphaTest::SetFogEnd(float value)
{
    pImpl->fog.end = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::FogVector;
}

void XM_CALLCONV quetzal::direct3d11::EffectAlphaTest::SetFogColor(FXMVECTOR value)
{
    pImpl->constants.fogColor = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

// Vertex color setting.
void quetzal::direct3d11::EffectAlphaTest::SetVertexColorEnabled(bool bEnabled)
{
    pImpl->vertexColorEnabled = bEnabled;
}

// Texture settings.
void quetzal::direct3d11::EffectAlphaTest::SetTexture(ID3D11ShaderResourceView* pShaderResourceView)
{
    pImpl->m_pTexture = pShaderResourceView;
}

void quetzal::direct3d11::EffectAlphaTest::SetAlphaFunction(D3D11_COMPARISON_FUNC value)
{
    pImpl->alphaFunction = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::AlphaTest;
}

void quetzal::direct3d11::EffectAlphaTest::SetReferenceAlpha(int value)
{
    pImpl->referenceAlpha = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::AlphaTest;
}
