//------------------------------------------------------------------------------
// direct3d11
// EffectDebug.cpp
//------------------------------------------------------------------------------

#include "EffectDebug.hpp"

using namespace std;
using namespace DirectX;

namespace
{

    //--------------------------------------------------------------------------
    // Constant buffer layout. Must match the shader!
    struct EffectDebugConstants
    {
        XMVECTOR ambientDownAndAlpha;
        XMVECTOR ambientRange;

        XMMATRIX world;
        XMVECTOR worldInverseTranspose[3];
        XMMATRIX worldViewProj;
    };

    static_assert((sizeof(EffectDebugConstants) % 16) == 0, "CB size not padded correctly");

    //--------------------------------------------------------------------------
    // Traits type describes our characteristics to the EffectCommon template.
    struct EffectDebugTraits
    {
        using ConstantBufferType = EffectDebugConstants;

        static constexpr int VertexShaderCount = 4;
        static constexpr int PixelShaderCount = 4;
        static constexpr int ShaderPermutationCount = 16;
    };

} // namespace

//------------------------------------------------------------------------------
// Internal EffectDebug implementation class.
class quetzal::direct3d11::EffectDebug::Impl : public EffectCommon<EffectDebugTraits>
{
public:

    Impl(Renderer& renderer);

    bool vertexColorEnabled;
    bool biasedVertexNormals;
    EffectDebug::Mode debugMode;

    int shader_permutation() const noexcept;

    void apply(ID3D11DeviceContext* pDeviceContext);
};

// Include the precompiled shader code.
namespace
{
#if defined(_XBOX_ONE) && defined(_TITLE)
    #include "Shaders/Compiled/XboxOneDebugEffect_VSDebug.inc"
    #include "Shaders/Compiled/XboxOneDebugEffect_VSDebugVc.inc"

    #include "Shaders/Compiled/XboxOneDebugEffect_VSDebugBn.inc"
    #include "Shaders/Compiled/XboxOneDebugEffect_VSDebugVcBn.inc"

    #include "Shaders/Compiled/XboxOneDebugEffect_PSHemiAmbient.inc"
    #include "Shaders/Compiled/XboxOneDebugEffect_PSRGBNormals.inc"
    #include "Shaders/Compiled/XboxOneDebugEffect_PSRGBTangents.inc"
    #include "Shaders/Compiled/XboxOneDebugEffect_PSRGBBiTangents.inc"
#else
    #include "Shaders/Compiled/DebugEffect_VSDebug.inc"
    #include "Shaders/Compiled/DebugEffect_VSDebugVc.inc"

    #include "Shaders/Compiled/DebugEffect_VSDebugBn.inc"
    #include "Shaders/Compiled/DebugEffect_VSDebugVcBn.inc"

    #include "Shaders/Compiled/DebugEffect_PSHemiAmbient.inc"
    #include "Shaders/Compiled/DebugEffect_PSRGBNormals.inc"
    #include "Shaders/Compiled/DebugEffect_PSRGBTangents.inc"
    #include "Shaders/Compiled/DebugEffect_PSRGBBiTangents.inc"
#endif
}

template<>
const quetzal::direct3d11::VertexShaderManager<EffectDebugTraits>::bytecode_array_type quetzal::direct3d11::VertexShaderManager<EffectDebugTraits>::m_bytecode =
{{
    { DebugEffect_VSDebug,      sizeof(DebugEffect_VSDebug)     },
    { DebugEffect_VSDebugVc,    sizeof(DebugEffect_VSDebugVc)   },

    { DebugEffect_VSDebugBn,    sizeof(DebugEffect_VSDebugBn)   },
    { DebugEffect_VSDebugVcBn,  sizeof(DebugEffect_VSDebugVcBn) },
}};

template<>
const quetzal::direct3d11::VertexShaderManager<EffectDebugTraits>::shader_array_type quetzal::direct3d11::VertexShaderManager<EffectDebugTraits>::m_nShader =
{
    0,      // default
    0,      // normals
    0,      // tangents
    0,      // bitangents

    1,      // vertex color + default
    1,      // vertex color + normals
    1,      // vertex color + tangents
    1,      // vertex color + bitangents

    2,      // default (biased vertex normal)
    2,      // normals (biased vertex normal)
    2,      // tangents (biased vertex normal)
    2,      // bitangents (biased vertex normal)

    3,      // vertex color (biased vertex normal)
    3,      // vertex color (biased vertex normal) + normals
    3,      // vertex color (biased vertex normal) + tangents
    3,      // vertex color (biased vertex normal) + bitangents
};

template<>
const quetzal::direct3d11::PixelShaderManager<EffectDebugTraits>::bytecode_array_type quetzal::direct3d11::PixelShaderManager<EffectDebugTraits>::m_bytecode =
{{
    { DebugEffect_PSHemiAmbient,    sizeof(DebugEffect_PSHemiAmbient)          },
    { DebugEffect_PSRGBNormals,     sizeof(DebugEffect_PSRGBNormals)     },
    { DebugEffect_PSRGBTangents,    sizeof(DebugEffect_PSRGBTangents)    },
    { DebugEffect_PSRGBBiTangents,  sizeof(DebugEffect_PSRGBBiTangents) },
}};

template<>
const quetzal::direct3d11::PixelShaderManager<EffectDebugTraits>::shader_array_type quetzal::direct3d11::PixelShaderManager<EffectDebugTraits>::m_nShader =
{
    0,      // default
    1,      // normals
    2,      // tangents
    3,      // bitangents

    0,      // vertex color + default
    1,      // vertex color + normals
    2,      // vertex color + tangents
    3,      // vertex color + bitangents

    0,      // default (biased vertex normal)
    1,      // normals (biased vertex normal)
    2,      // tangents (biased vertex normal)
    3,      // bitangents (biased vertex normal)

    0,      // vertex color (biased vertex normal)
    1,      // vertex color (biased vertex normal) + normals
    2,      // vertex color (biased vertex normal) + tangents
    3,      // vertex color (biased vertex normal) + bitangents
};

quetzal::direct3d11::EffectDebug::Impl::Impl(Renderer& renderer) :
    EffectCommon(renderer),
    vertexColorEnabled(false),
    biasedVertexNormals(false),
    debugMode(EffectDebug::Mode_Default)
{
// shader manager arrays are sized with traits anyway ...
//    static_assert(static_cast<int>(std::size(EffectCommon<EffectDebugTraits>::VertexShaderIndices)) == EffectDebugTraits::ShaderPermutationCount, "array/max mismatch");
//    static_assert(static_cast<int>(std::size(EffectCommon<EffectDebugTraits>::VertexShaderBytecode)) == EffectDebugTraits::VertexShaderCount, "array/max mismatch");
//    static_assert(static_cast<int>(std::size(EffectCommon<EffectDebugTraits>::PixelShaderBytecode)) == EffectDebugTraits::PixelShaderCount, "array/max mismatch");
//    static_assert(static_cast<int>(std::size(EffectCommon<EffectDebugTraits>::PixelShaderIndices)) == EffectDebugTraits::ShaderPermutationCount, "array/max mismatch");

    static const XMVECTORF32 s_lower = { { { 0.f, 0.f, 0.f, 1.f } } };

    constants.ambientDownAndAlpha = s_lower;
    constants.ambientRange = g_XMOne;
}

int quetzal::direct3d11::EffectDebug::Impl::shader_permutation() const noexcept
{
    int permutation = static_cast<int>(debugMode);

    // Support vertex coloring?
    if (vertexColorEnabled)
    {
        permutation += 4;
    }

    if (biasedVertexNormals)
    {
        // Compressed normals need to be scaled and biased in the vertex shader.
        permutation += 8;
    }

    return permutation;
}

// Sets our state onto the D3D device.
void quetzal::direct3d11::EffectDebug::Impl::apply(ID3D11DeviceContext* pDeviceContext)
{
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

    // Set shaders and constant buffers.
    apply_shaders(pDeviceContext, shader_permutation());
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectDebug::EffectDebug(Renderer& renderer) :
    pImpl(std::make_unique<Impl>(renderer))
{
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectDebug::EffectDebug(EffectDebug&& other) noexcept :
    pImpl(std::move(other.pImpl))
{
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectDebug& quetzal::direct3d11::EffectDebug::operator=(EffectDebug&& other) noexcept
{
    pImpl = std::move(other.pImpl);
    return *this;
}

// IEffect methods.
void quetzal::direct3d11::EffectDebug::apply(ID3D11DeviceContext* pDeviceContext)
{
    pImpl->apply(pDeviceContext);
}

quetzal::direct3d11::ShaderBytecode quetzal::direct3d11::EffectDebug::vertex_shader_bytecode() const
{
    return pImpl->vertex_shader_bytecode(pImpl->shader_permutation());
}

// Camera settings.
void XM_CALLCONV quetzal::direct3d11::EffectDebug::SetWorld(FXMMATRIX value)
{
    pImpl->matrices.world = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::WorldInverseTranspose;
}

void XM_CALLCONV quetzal::direct3d11::EffectDebug::SetView(FXMMATRIX value)
{
    pImpl->matrices.view = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj;
}

void XM_CALLCONV quetzal::direct3d11::EffectDebug::SetProjection(FXMMATRIX value)
{
    pImpl->matrices.projection = value;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj;
}

void XM_CALLCONV quetzal::direct3d11::EffectDebug::SetMatrices(FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection)
{
    pImpl->matrices.world = world;
    pImpl->matrices.view = view;
    pImpl->matrices.projection = projection;

    pImpl->dirtyFlags |= EffectDirtyFlags::WorldViewProj | EffectDirtyFlags::WorldInverseTranspose;
}

// Material settings.
void quetzal::direct3d11::EffectDebug::SetMode(Mode debugMode)
{
    if (static_cast<int>(debugMode) < 0 || static_cast<int>(debugMode) >= EffectDebugTraits::PixelShaderCount)
    {
        throw std::invalid_argument("Unsupported mode");
    }

    pImpl->debugMode = debugMode;
}

void XM_CALLCONV quetzal::direct3d11::EffectDebug::SetHemisphericalAmbientColor(FXMVECTOR upper, FXMVECTOR lower)
{
    // Set xyz to new value, but preserve existing w (alpha).
    pImpl->constants.ambientDownAndAlpha = XMVectorSelect(pImpl->constants.ambientDownAndAlpha, lower, g_XMSelect1110);

    pImpl->constants.ambientRange = XMVectorSubtract(upper, lower);

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

void quetzal::direct3d11::EffectDebug::SetAlpha(float value)
{
    // Set w to new value, but preserve existing xyz (ambient down).
    pImpl->constants.ambientDownAndAlpha = XMVectorSetW(pImpl->constants.ambientDownAndAlpha, value);

    pImpl->dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
}

// Vertex color setting.
void quetzal::direct3d11::EffectDebug::SetVertexColorEnabled(bool value)
{
    pImpl->vertexColorEnabled = value;
}

// Normal compression settings.
void quetzal::direct3d11::EffectDebug::SetBiasedVertexNormals(bool value)
{
    pImpl->biasedVertexNormals = value;
}
