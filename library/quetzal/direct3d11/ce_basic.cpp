
//------------------------------------------------------------------------------
quetzal::direct3d11::EffectManager::effect_type quetzal::direct3d11::create_basic_effect(Renderer& renderer, const EffectManager::EffectInfo& effect_info)
{
    auto pEffect = make_shared<EffectBasic>(renderer);

    pEffect->EnableDefaultLighting();
    pEffect->SetLightingEnabled(true);

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
        com_ptr<ID3D11ShaderResourceView> pShaderResourceView = renderer.texture_manager().open(effect_info.m_diffuseTextureName);
        pEffect->SetTexture(pShaderResourceView);
        pEffect->SetTextureEnabled(true);
    }

    if (effect_info.m_bBiasedVertexNormals)
    {
        pEffect->SetBiasedVertexNormals(true);
    }

    return pEffect;
}
