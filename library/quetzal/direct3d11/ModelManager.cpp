//------------------------------------------------------------------------------
// direct3d11
// ModelManager.hpp
//------------------------------------------------------------------------------

#include "ModelManager.hpp"
#include "Model.hpp"

using namespace std;
using namespace DirectX;

//------------------------------------------------------------------------------
quetzal::direct3d11::ModelManager::ModelManager(Renderer& renderer, EffectManager& effect_manager) :
    m_renderer(renderer),
    m_effect_manager(effect_manager),
    m_path(),
    m_models(),
    m_mutex()
{
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::ModelManager::set_path(const std::filesystem::path& pathname)
{
    m_path = pathname;
    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::ModelManager::clear()
{
    lock_guard<mutex> lock(m_mutex);
    m_models.clear();
    return;
}
