#if !defined(QUETZAL_DIRECT3D11_MODELMANAGER_HPP)
#define QUETZAL_DIRECT3D11_MODELMANAGER_HPP
//------------------------------------------------------------------------------
// direct3d11
// ModelManager.hpp
//------------------------------------------------------------------------------

#include "Effect.hpp"
#include "EffectManager.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "import_model.hpp"
#include <filesystem>
#include <mutex>
#include <string>
#include <unordered_map>

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    class ModelManager
    {
    public:

        using model_type = std::shared_ptr<Model>;

        ModelManager(Renderer& renderer, EffectManager& effect_manager);
        ModelManager(const ModelManager&) = delete;
        ModelManager(ModelManager&&) = delete;
        ~ModelManager() = default;

        ModelManager& operator=(const ModelManager&) = delete;
        ModelManager& operator=(ModelManager&&) = delete;

        void set_path(const std::filesystem::path& pathname);

        template<typename V, typename I, typename M>
        model_type open(const std::string& name, std::function<V(const typename M::vertex_type::attributes_type&)> transfer_vertex, std::shared_ptr<IEffect> pEffectDefault = nullptr);

        void clear();

    private:

        using models_type = std::unordered_map<std::string, model_type>;

        Renderer& m_renderer;
        EffectManager& m_effect_manager;
        std::filesystem::path m_path;
        models_type m_models; // cached models stored and accessible by name
        std::mutex m_mutex;
    };

} // namespace quetzal::direct3d11

//------------------------------------------------------------------------------
template<typename V, typename I, typename M>
quetzal::direct3d11::ModelManager::model_type quetzal::direct3d11::ModelManager::open(const std::string& name, std::function<V(const typename M::vertex_type::attributes_type&)> transfer_vertex, std::shared_ptr<IEffect> pEffectDefault)
{
    if (name.empty())
    {
        return nullptr;
    }

    if (m_models.contains(name))
    {
        return m_models[name];
    }

    std::filesystem::path pathname = m_path / name;
    model_type pModel = import_model<V, I, M>(m_renderer, m_effect_manager, pathname, transfer_vertex, pEffectDefault);

    std::lock_guard<std::mutex> lock(m_mutex);
    m_models[name] = pModel;

    return pModel;
}

#endif // QUETZAL_DIRECT3D11_MODELMANAGER_HPP
