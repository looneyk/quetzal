#if !defined(QUETZAL_DIRECT3D11_OBJ_UTIL_HPP)
#define QUETZAL_DIRECT3D11_OBJ_UTIL_HPP
//------------------------------------------------------------------------------
// direct3d11
// obj_util.hpp
//------------------------------------------------------------------------------

#include "EffectManager.hpp"
#include "quetzal/model/obj_io.hpp"
#include "quetzal/wavefront_obj/Material.hpp"
#include <memory>
#include <string>

namespace quetzal::direct3d11
{

    std::shared_ptr<IEffect> material_effect(EffectManager& effect_manager, const model::Materials& materials, const std::string& name);

    EffectManager::EffectInfo to_effect(const model::Materials& materials, const std::string& name);

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_OBJ_UTIL_HPP
