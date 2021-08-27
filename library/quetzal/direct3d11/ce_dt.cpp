
//------------------------------------------------------------------------------
quetzal::direct3d11::EffectManager::effect_type quetzal::direct3d11::create_dual_texture_effect(Renderer& renderer, const EffectManager::EffectInfo& effect_info)
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
        com_ptr<ID3D11ShaderResourceView> pShaderResourceView = renderer.texture_manager().open(effect_info.m_diffuseTextureName);
        pEffect->SetTexture(pShaderResourceView);
    }

    if (!effect_info.m_specularTextureName.empty())
    {
        com_ptr<ID3D11ShaderResourceView> pShaderResourceView = renderer.texture_manager().open(effect_info.m_specularTextureName);
        pEffect->SetTexture2(pShaderResourceView);
    }

    return pEffect;
}
