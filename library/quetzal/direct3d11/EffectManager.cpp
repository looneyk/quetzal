//------------------------------------------------------------------------------
// direct3d11
// EffectManager.cpp
//------------------------------------------------------------------------------

#include "EffectManager.hpp"
#include "EffectBasic.hpp"
#include "EffectDualTexture.hpp"
#include "EffectNormalMap.hpp"
#include "EffectSkinned.hpp"

using namespace std;
using namespace DirectX;

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectManager::EffectManager(Renderer& renderer, TextureManager& texture_manager) :
    m_renderer(renderer),
    m_texture_manager(texture_manager),
    m_path(),
    m_effects(),
    m_mutex()
{
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::EffectManager::set_path(const filesystem::path& path)
{
    m_path = path;
    return;
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectManager::effect_type quetzal::direct3d11::EffectManager::open(const string& name)
{
    effect_type pEffect;
    if (m_effects.contains(name))
    {
        pEffect = m_effects[name];
    }

    return pEffect;
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectManager::effect_type quetzal::direct3d11::EffectManager::open(const EffectInfo& effect_info)
{
    effect_type pEffect;

    string name = effect_info.m_name;
    if (!name.empty() && m_effects.contains(name))
    {
        return m_effects[name];
    }

    // shared resources ...
    if (effect_info.m_bSkinning)
    {
        pEffect = create_skinning_effect(m_renderer, m_texture_manager, effect_info);
    }
    else if (effect_info.m_bDualTexture)
    {
        pEffect = create_dual_texture_effect(m_renderer, m_texture_manager, effect_info);
    }
    else if (effect_info.m_bNormalMap)
    {
        pEffect = create_normal_map_effect(m_renderer, m_texture_manager, effect_info);
    }
    else
    {
        pEffect = create_basic_effect(m_renderer, m_texture_manager, effect_info);
    }

    if (pEffect && !name.empty())
    {
        lock_guard<mutex> lock(m_mutex);
        m_effects[name] = pEffect;
    }

    return pEffect;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::EffectManager::clear()
{
    lock_guard<mutex> lock(m_mutex);
    m_effects.clear();
    return;
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectManager::effect_type quetzal::direct3d11::create_basic_effect(Renderer& renderer, TextureManager& texture_manager, const EffectManager::EffectInfo& effect_info)
{
    auto pEffect = make_shared<EffectBasic>(renderer);

    if (effect_info.m_bLightingDefault)
    {
        pEffect->EnableDefaultLighting();
        pEffect->SetLightingEnabled(true);
    }

    pEffect->SetAlpha(effect_info.m_alpha);

    if (effect_info.m_bVertexColor)
    {
        pEffect->SetVertexColorEnabled(true);
    }

    // Basic Effect does not have an ambient material color

    XMVECTOR color = XMLoadFloat3(&effect_info.m_colorDiffuse);
    pEffect->SetDiffuseColor(color);

    if (effect_info.m_colorSpecular.x != 0 || effect_info.m_colorSpecular.y != 0 || effect_info.m_colorSpecular.z != 0)
    {
        color = XMLoadFloat3(&effect_info.m_colorSpecular);
        pEffect->SetSpecularColor(color);
        pEffect->SetSpecularPower(effect_info.m_specularPower);
    }
    else
    {
        pEffect->DisableSpecular();
    }

    if (effect_info.m_colorEmissive.x != 0 || effect_info.m_colorEmissive.y != 0 || effect_info.m_colorEmissive.z != 0)
    {
        color = XMLoadFloat3(&effect_info.m_colorEmissive);
        pEffect->SetEmissiveColor(color);
    }

    if (!effect_info.m_diffuseTextureName.empty())
    {
        com_ptr<ID3D11ShaderResourceView> pShaderResourceView = texture_manager.open(effect_info.m_diffuseTextureName);
        pEffect->SetTexture(pShaderResourceView);
        pEffect->SetTextureEnabled(true);
    }

    if (effect_info.m_bBiasedVertexNormals)
    {
        pEffect->SetBiasedVertexNormals(true);
    }

    return pEffect;
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectManager::effect_type quetzal::direct3d11::create_skinning_effect(Renderer& renderer, TextureManager& texture_manager, const EffectManager::EffectInfo& effect_info)
{
    auto pEffect = make_shared<EffectSkinned>(renderer);

    pEffect->EnableDefaultLighting();
    pEffect->SetAlpha(effect_info.m_alpha);

    // Skinned Effect does not have an ambient material color, or per-vertex color support

    XMVECTOR color = XMLoadFloat3(&effect_info.m_colorDiffuse);
    pEffect->SetDiffuseColor(color);

    if (effect_info.m_colorSpecular.x != 0 || effect_info.m_colorSpecular.y != 0 || effect_info.m_colorSpecular.z != 0)
    {
        color = XMLoadFloat3(&effect_info.m_colorSpecular);
        pEffect->SetSpecularColor(color);
        pEffect->SetSpecularPower(effect_info.m_specularPower);
    }
    else
    {
        pEffect->DisableSpecular();
    }

    if (effect_info.m_colorEmissive.x != 0 || effect_info.m_colorEmissive.y != 0 || effect_info.m_colorEmissive.z != 0)
    {
        color = XMLoadFloat3(&effect_info.m_colorEmissive);
        pEffect->SetEmissiveColor(color);
    }

    if (!effect_info.m_diffuseTextureName.empty())
    {
        com_ptr<ID3D11ShaderResourceView> pShaderResourceView = texture_manager.open(effect_info.m_diffuseTextureName);
        pEffect->SetTexture(pShaderResourceView);
    }

    if (effect_info.m_bBiasedVertexNormals)
    {
        pEffect->SetBiasedVertexNormals(true);
    }

    return pEffect;
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectManager::effect_type quetzal::direct3d11::create_dual_texture_effect(Renderer& renderer, TextureManager& texture_manager, const EffectManager::EffectInfo& effect_info)
{
    auto pEffect = make_shared<EffectDualTexture>(renderer);

    // Dual texture effect doesn't support lighting (usually it's lightmaps)

    pEffect->SetAlpha(effect_info.m_alpha);

    if (effect_info.m_bVertexColor)
    {
        pEffect->SetVertexColorEnabled(true);
    }

    XMVECTOR color = XMLoadFloat3(&effect_info.m_colorDiffuse);
    pEffect->SetDiffuseColor(color);

    if (!effect_info.m_diffuseTextureName.empty())
    {
        com_ptr<ID3D11ShaderResourceView> pShaderResourceView = texture_manager.open(effect_info.m_diffuseTextureName);
        pEffect->SetTexture(pShaderResourceView);
    }

    if (!effect_info.m_specularTextureName.empty())
    {
        com_ptr<ID3D11ShaderResourceView> pShaderResourceView = texture_manager.open(effect_info.m_specularTextureName);
        pEffect->SetTexture2(pShaderResourceView);
    }

    return pEffect;
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectManager::effect_type quetzal::direct3d11::create_normal_map_effect(Renderer& renderer, TextureManager& texture_manager, const EffectManager::EffectInfo& effect_info)
{
    auto pEffect = make_shared<EffectNormalMap>(renderer);

    pEffect->EnableDefaultLighting();
    pEffect->SetAlpha(effect_info.m_alpha);

    if (effect_info.m_bVertexColor)
    {
        pEffect->SetVertexColorEnabled(true);
    }

    // NormalMap Effect does not have an ambient material color

    XMVECTOR color = XMLoadFloat3(&effect_info.m_colorDiffuse);
    pEffect->SetDiffuseColor(color);

    if (effect_info.m_colorSpecular.x != 0 || effect_info.m_colorSpecular.y != 0 || effect_info.m_colorSpecular.z != 0)
    {
        color = XMLoadFloat3(&effect_info.m_colorSpecular);
        pEffect->SetSpecularColor(color);
        pEffect->SetSpecularPower(effect_info.m_specularPower);
    }
    else
    {
        pEffect->DisableSpecular();
    }

    if (effect_info.m_colorEmissive.x != 0 || effect_info.m_colorEmissive.y != 0 || effect_info.m_colorEmissive.z != 0)
    {
        color = XMLoadFloat3(&effect_info.m_colorEmissive);
        pEffect->SetEmissiveColor(color);
    }

    if (!effect_info.m_diffuseTextureName.empty())
    {
        com_ptr<ID3D11ShaderResourceView> pShaderResourceView = texture_manager.open(effect_info.m_diffuseTextureName);
        pEffect->SetTexture(pShaderResourceView);
    }

    if (!effect_info.m_specularTextureName.empty())
    {
        com_ptr<ID3D11ShaderResourceView> pShaderResourceView = texture_manager.open(effect_info.m_specularTextureName);
        pEffect->SetSpecularTexture(pShaderResourceView);
    }

    if (!effect_info.m_normalTextureName.empty())
    {
        com_ptr<ID3D11ShaderResourceView> pShaderResourceView = texture_manager.open(effect_info.m_normalTextureName);
        pEffect->SetNormalTexture(pShaderResourceView);
    }

    if (effect_info.m_bBiasedVertexNormals)
    {
        pEffect->SetBiasedVertexNormalsAndTangents(true);
    }

    return pEffect;
}
