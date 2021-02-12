//------------------------------------------------------------------------------
// direct3d11
// EffectCommon.cpp
//--------------------------------------------------------------------------------------

#include "EffectCommon.hpp"

using namespace std;
using namespace DirectX;

//------------------------------------------------------------------------------
// IEffectMatrices default method
void XM_CALLCONV quetzal::direct3d11::IEffectMatrices::SetMatrices(FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection)
{
    SetWorld(world);
    SetView(view);
    SetProjection(projection);
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectMatrices::EffectMatrices() noexcept
{
    XMMATRIX identity = XMMatrixIdentity();
    world = identity;
    view = identity;
    projection = identity;
    worldView = identity;
}

//------------------------------------------------------------------------------
// Lazily recomputes the combined world+view+projection matrix.
void quetzal::direct3d11::EffectMatrices::SetConstants(int& dirtyFlags, XMMATRIX& worldViewProjConstant)
{
    if (dirtyFlags & EffectDirtyFlags::WorldViewProj)
    {
        worldView = XMMatrixMultiply(world, view);

        worldViewProjConstant = XMMatrixTranspose(XMMatrixMultiply(worldView, projection));

        dirtyFlags &= ~EffectDirtyFlags::WorldViewProj;
        dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
    }
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectFog::EffectFog() noexcept :
    enabled(false),
    start(0),
    end(1.f)
{
}

//------------------------------------------------------------------------------
// Lazily recomputes the derived vector used by shader fog calculations.
void XM_CALLCONV quetzal::direct3d11::EffectFog::SetConstants(int& dirtyFlags, FXMMATRIX worldView, XMVECTOR& fogVectorConstant)
{
    if (enabled)
    {
        if (dirtyFlags & (EffectDirtyFlags::FogVector | EffectDirtyFlags::FogEnable))
        {
            if (start == end)
            {
                // Degenerate case: force everything to 100% fogged if start and end are the same.
                static const XMVECTORF32 fullyFogged = { { { 0, 0, 0, 1 } } };

                fogVectorConstant = fullyFogged;
            }
            else
            {
                // We want to transform vertex positions into view space, take the resulting
                // Z value, then scale and offset according to the fog start/end distances.
                // Because we only care about the Z component, the shader can do all this
                // with a single dot product, using only the Z row of the world+view matrix.

                // _13, _23, _33, _43
                XMVECTOR worldViewZ = XMVectorMergeXY(XMVectorMergeZW(worldView.r[0], worldView.r[2]),
                                                      XMVectorMergeZW(worldView.r[1], worldView.r[3]));

                // 0, 0, 0, fogStart
                XMVECTOR wOffset = XMVectorSwizzle<1, 2, 3, 0>(XMLoadFloat(&start));

                fogVectorConstant = (worldViewZ + wOffset) / (start - end);
            }

            dirtyFlags &= ~(EffectDirtyFlags::FogVector | EffectDirtyFlags::FogEnable);
            dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
        }
    }
    else
    {
        // When fog is disabled, make sure the fog vector is reset to zero.
        if (dirtyFlags & EffectDirtyFlags::FogEnable)
        {
            fogVectorConstant = g_XMZero;

            dirtyFlags &= ~EffectDirtyFlags::FogEnable;
            dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
        }
    }
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectColor::EffectColor() noexcept :
    alpha(1.f)
{
    diffuseColor = g_XMOne;
}

// Lazily recomputes the material color parameter for shaders that do not support realtime lighting.
void quetzal::direct3d11::EffectColor::SetConstants(int& dirtyFlags, XMVECTOR& diffuseColorConstant)
{
    if (dirtyFlags & EffectDirtyFlags::MaterialColor)
    {
        XMVECTOR alphaVector = XMVectorReplicate(alpha);

        // xyz = diffuse * alpha, w = alpha.
        diffuseColorConstant = XMVectorSelect(alphaVector, diffuseColor * alphaVector, g_XMSelect1110);

        dirtyFlags &= ~EffectDirtyFlags::MaterialColor;
        dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
    }
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectLights::EffectLights() noexcept :
    emissiveColor{},
    ambientLightColor{},
    lightEnabled{},
    lightDiffuseColor{},
    lightSpecularColor{}
{
    for (int i = 0; i < MaxDirectionalLights; i++)
    {
        lightEnabled[i] = (i == 0);
        lightDiffuseColor[i] = g_XMOne;
    }
}

//------------------------------------------------------------------------------
// Initializes constant buffer fields to match the current lighting state.
void quetzal::direct3d11::EffectLights::InitializeConstants(XMVECTOR& specularColorAndPowerConstant, XMVECTOR* lightDirectionConstant, XMVECTOR* lightDiffuseConstant, XMVECTOR* lightSpecularConstant) const
{
    static const XMVECTORF32 defaultSpecular = { { { 1, 1, 1, 16 } } };
    static const XMVECTORF32 defaultLightDirection = { { { 0, -1, 0, 0 } } };

    specularColorAndPowerConstant = defaultSpecular;

    for (int i = 0; i < MaxDirectionalLights; i++)
    {
        lightDirectionConstant[i] = defaultLightDirection;

        lightDiffuseConstant[i]  = lightEnabled[i] ? lightDiffuseColor[i]  : g_XMZero;
        lightSpecularConstant[i] = lightEnabled[i] ? lightSpecularColor[i] : g_XMZero;
    }
}

//------------------------------------------------------------------------------
// Lazily recomputes derived parameter values used by shader lighting calculations.
void quetzal::direct3d11::EffectLights::SetConstants(int& dirtyFlags, EffectMatrices const& matrices, XMMATRIX& worldConstant, XMVECTOR worldInverseTransposeConstant[3], XMVECTOR& eyePositionConstant, XMVECTOR& diffuseColorConstant, XMVECTOR& emissiveColorConstant, bool lightingEnabled)
{
    if (lightingEnabled)
    {
        // World inverse transpose matrix.
        if (dirtyFlags & EffectDirtyFlags::WorldInverseTranspose)
        {
            worldConstant = XMMatrixTranspose(matrices.world);

            XMMATRIX worldInverse = XMMatrixInverse(nullptr, matrices.world);

            worldInverseTransposeConstant[0] = worldInverse.r[0];
            worldInverseTransposeConstant[1] = worldInverse.r[1];
            worldInverseTransposeConstant[2] = worldInverse.r[2];

            dirtyFlags &= ~EffectDirtyFlags::WorldInverseTranspose;
            dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
        }

        // Eye position vector.
        if (dirtyFlags & EffectDirtyFlags::EyePosition)
        {
            XMMATRIX viewInverse = XMMatrixInverse(nullptr, matrices.view);

            eyePositionConstant = viewInverse.r[3];

            dirtyFlags &= ~EffectDirtyFlags::EyePosition;
            dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
        }
    }

    // Material color parameters. The desired lighting model is:
    //
    //     ((ambientLightColor + sum(diffuse directional light)) * diffuseColor) + emissiveColor
    //
    // When lighting is disabled, ambient and directional lights are ignored, leaving:
    //
    //     diffuseColor + emissiveColor
    //
    // For the lighting disabled case, we can save one shader instruction by precomputing
    // diffuse+emissive on the CPU, after which the shader can use diffuseColor directly,
    // ignoring its emissive parameter.
    //
    // When lighting is enabled, we can merge the ambient and emissive settings. If we
    // set our emissive parameter to emissive+(ambient*diffuse), the shader no longer
    // needs to bother adding the ambient contribution, simplifying its computation to:
    //
    //     (sum(diffuse directional light) * diffuseColor) + emissiveColor
    //
    // For futher optimization goodness, we merge material alpha with the diffuse
    // color parameter, and premultiply all color values by this alpha.

    if (dirtyFlags & EffectDirtyFlags::MaterialColor)
    {
        XMVECTOR diffuse = diffuseColor;
        XMVECTOR alphaVector = XMVectorReplicate(alpha);

        if (lightingEnabled)
        {
            // Merge emissive and ambient light contributions.
            emissiveColorConstant = (emissiveColor + ambientLightColor * diffuse) * alphaVector;
        }
        else
        {
            // Merge diffuse and emissive light contributions.
            diffuse += emissiveColor;
        }

        // xyz = diffuse * alpha, w = alpha.
        diffuseColorConstant = XMVectorSelect(alphaVector, diffuse * alphaVector, g_XMSelect1110);

        dirtyFlags &= ~EffectDirtyFlags::MaterialColor;
        dirtyFlags |= EffectDirtyFlags::ConstantBuffer;
    }
}

//------------------------------------------------------------------------------
// Helper for turning one of the directional lights on or off.
int quetzal::direct3d11::EffectLights::SetLightEnabled(int nLight, bool bEnabled, XMVECTOR* lightDiffuseConstant, XMVECTOR* lightSpecularConstant)
{
    ValidateLightIndex(nLight);

    if (lightEnabled[nLight] == bEnabled)
        return 0;

    lightEnabled[nLight] = bEnabled;

    if (bEnabled)
    {
        // If this light is now on, store its color in the constant buffer.
        lightDiffuseConstant[nLight] = lightDiffuseColor[nLight];
        lightSpecularConstant[nLight] = lightSpecularColor[nLight];
    }
    else
    {
        // If the light is off, reset constant buffer colors to zero.
        lightDiffuseConstant[nLight] = g_XMZero;
        lightSpecularConstant[nLight] = g_XMZero;
    }

    return EffectDirtyFlags::ConstantBuffer;
}

//------------------------------------------------------------------------------
// Helper for setting diffuse color of one of the directional lights.
int XM_CALLCONV quetzal::direct3d11::EffectLights::SetLightDiffuseColor(int nLight, FXMVECTOR value, XMVECTOR* lightDiffuseConstant)
{
    ValidateLightIndex(nLight);

    // Locally store the new color.
    lightDiffuseColor[nLight] = value;

    // If this light is currently on, also update the constant buffer.
    if (lightEnabled[nLight])
    {
        lightDiffuseConstant[nLight] = value;

        return EffectDirtyFlags::ConstantBuffer;
    }

    return 0;
}

//------------------------------------------------------------------------------
// Helper for setting specular color of one of the directional lights.
int XM_CALLCONV quetzal::direct3d11::EffectLights::SetLightSpecularColor(int nLight, FXMVECTOR value, XMVECTOR* lightSpecularConstant)
{
    ValidateLightIndex(nLight);

    // Locally store the new color.
    lightSpecularColor[nLight] = value;

    // If this light is currently on, also update the constant buffer.
    if (lightEnabled[nLight])
    {
        lightSpecularConstant[nLight] = value;

        return EffectDirtyFlags::ConstantBuffer;
    }

    return 0;
}

//------------------------------------------------------------------------------
// Parameter validation helper.
void quetzal::direct3d11::EffectLights::ValidateLightIndex(int nLight)
{
    if (nLight < 0 || nLight >= MaxDirectionalLights)
    {
        throw std::out_of_range("nLight parameter out of range");
    }
}

//------------------------------------------------------------------------------
// Activates the default lighting rig (key, fill, and back lights).
void quetzal::direct3d11::EffectLights::EnableDefaultLighting(IEffectLights* effect)
{
    static const XMVECTORF32 defaultDirections[MaxDirectionalLights] =
    {
        { { { -0.5265408f, -0.5735765f, -0.6275069f, 0 } } },
        { { {  0.7198464f,  0.3420201f,  0.6040227f, 0 } } },
        { { {  0.4545195f, -0.7660444f,  0.4545195f, 0 } } },
    };

    static const XMVECTORF32 defaultDiffuse[MaxDirectionalLights] =
    {
        { { { 1.0000000f, 0.9607844f, 0.8078432f, 0 } } },
        { { { 0.9647059f, 0.7607844f, 0.4078432f, 0 } } },
        { { { 0.3231373f, 0.3607844f, 0.3937255f, 0 } } },
    };

    static const XMVECTORF32 defaultSpecular[MaxDirectionalLights] =
    {
        { { { 1.0000000f, 0.9607844f, 0.8078432f, 0 } } },
        { { { 0.0000000f, 0.0000000f, 0.0000000f, 0 } } },
        { { { 0.3231373f, 0.3607844f, 0.3937255f, 0 } } },
    };

    static const XMVECTORF32 defaultAmbient = { { { 0.05333332f, 0.09882354f, 0.1819608f, 0 } } };

    effect->SetLightingEnabled(true);
    effect->SetAmbientLightColor(defaultAmbient);

    for (int i = 0; i < MaxDirectionalLights; i++)
    {
        effect->SetLightEnabled(i, true);
        effect->SetLightDirection(i, defaultDirections[i]);
        effect->SetLightDiffuseColor(i, defaultDiffuse[i]);
        effect->SetLightSpecularColor(i, defaultSpecular[i]);
    }
}
