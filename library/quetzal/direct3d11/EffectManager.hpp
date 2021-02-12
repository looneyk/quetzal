#if !defined(QUETZAL_DIRECT3D11_EFFECTMANAGER_HPP)
#define QUETZAL_DIRECT3D11_EFFECTMANAGER_HPP
//------------------------------------------------------------------------------
// direct3d11
// EffectManager.hpp
//------------------------------------------------------------------------------

#include "Effect.hpp"
#include "Renderer.hpp"
#include "TextureManager.hpp"
#include <d3d11_1.h>
#include <filesystem>
#include <mutex>
#include <string>
#include <unordered_map>

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    class EffectManager
    {
    public:

        //----------------------------------------------------------------------
        struct EffectInfo
        {
            std::string m_name;
            bool m_bVertexColor;
            bool m_bSkinning;
            bool m_bDualTexture;
            bool m_bNormalMap;
            bool m_bBiasedVertexNormals;
            bool m_bLightingDefault; // Used only in EffectBasic so that lighting can be turned off when using only vertex color
            float m_specularPower;
            float m_alpha;
            DirectX::XMFLOAT3 m_colorAmbient;
            DirectX::XMFLOAT3 m_colorDiffuse;
            DirectX::XMFLOAT3 m_colorSpecular;
            DirectX::XMFLOAT3 m_colorEmissive;
            std::string m_diffuseTextureName;
            std::string m_specularTextureName;
            std::string m_normalTextureName;
        };

        using effect_type = std::shared_ptr<IEffect>;

        EffectManager(Renderer& renderer, TextureManager& texture_manager);
        EffectManager(const EffectManager&) = delete;
        EffectManager(EffectManager&&) = delete;
        ~EffectManager() = default;

        EffectManager& operator=(const EffectManager&) = delete;
        EffectManager& operator=(EffectManager&&) = delete;

        void set_path(const std::filesystem::path& path);

        effect_type open(const std::string& name);
        effect_type open(const EffectInfo& effect_info);

        void clear();

        // default effect? public or private? ...

    private:

        using effects_type = std::unordered_map<std::string, effect_type>;

        Renderer& m_renderer;
        TextureManager& m_texture_manager;
        std::filesystem::path m_path;
        effects_type m_effects; // cached effects stored and accessible by name

        std::mutex m_mutex;
    };

    // move to the matching EffectXxx file ...
    EffectManager::effect_type create_basic_effect(Renderer& renderer, TextureManager& texture_manager, const EffectManager::EffectInfo& effect_info);
    EffectManager::effect_type create_skinning_effect(Renderer& renderer, TextureManager& texture_manager, const EffectManager::EffectInfo& effect_info);
    EffectManager::effect_type create_dual_texture_effect(Renderer& renderer, TextureManager& texture_manager, const EffectManager::EffectInfo& effect_info);
    EffectManager::effect_type create_normal_map_effect(Renderer& renderer, TextureManager& texture_manager, const EffectManager::EffectInfo& effect_info);

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_EFFECTMANAGER_HPP
