//------------------------------------------------------------------------------
// direct3d11
// obj_util.cpp
//------------------------------------------------------------------------------

#include "obj_util.hpp"

//------------------------------------------------------------------------------
std::shared_ptr<quetzal::direct3d11::IEffect> quetzal::direct3d11::material_effect(EffectManager& effect_manager, const model::Materials& materials, const std::string& name)
{
	std::shared_ptr<IEffect> pEffect;
    if (name.empty())
    {
        return pEffect;
    }

    pEffect = effect_manager.open(name);
    if (!pEffect)
    {
        EffectManager::EffectInfo effectInfo = to_effect(materials, name);
        pEffect = effect_manager.open(effectInfo);
    }

    return pEffect;
}

//------------------------------------------------------------------------------
quetzal::direct3d11::EffectManager::EffectInfo quetzal::direct3d11::to_effect(const model::Materials& materials, const std::string& name)
{
    EffectManager::EffectInfo effectInfo{};

    if (materials.contains(name))
    {
        wavefront_obj::Material material = materials.at(name);

        effectInfo.m_name = material.m_name;
        effectInfo.m_bNormalMap = !material.m_normalMapName.empty();
        effectInfo.m_specularPower = material.m_specularExponent;
        effectInfo.m_alpha = material.m_alpha;
        effectInfo.m_colorAmbient = DirectX::XMFLOAT3(material.m_ambientColor.data());
        effectInfo.m_colorDiffuse = DirectX::XMFLOAT3(material.m_diffuseColor.data());
        effectInfo.m_colorSpecular = DirectX::XMFLOAT3(material.m_specularColor.data());
        effectInfo.m_colorEmissive = DirectX::XMFLOAT3(material.m_emissiveColor.data());
        effectInfo.m_diffuseTextureName = !material.m_diffuseTextureName.empty() ? material.m_diffuseTextureName : material.m_ambientTextureName;
        effectInfo.m_specularTextureName = material.m_specularTextureName;
        effectInfo.m_normalTextureName = material.m_normalMapName;
    }

    return effectInfo;
}
