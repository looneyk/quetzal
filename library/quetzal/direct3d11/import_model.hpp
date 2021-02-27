#if !defined(QUETZAL_DIRECT3D11_IMPORT_MODEL_HPP)
#define QUETZAL_DIRECT3D11_IMPORT_MODEL_HPP
//------------------------------------------------------------------------------
// Direct3D 11
// import_model.hpp
//------------------------------------------------------------------------------

#include "EffectManager.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "mesh_util.hpp"
#include "obj_util.hpp"
#include "quetzal/brep/Mesh.hpp"
#include "quetzal/brep/MeshTraits.hpp"
#include "quetzal/math/VectorTraits.hpp"
#include "quetzal/model/obj_io.hpp"
#include <filesystem>

namespace quetzal::direct3d11
{

    template<typename V, typename I, typename M>
    std::shared_ptr<Model> import_model(Renderer& renderer, EffectManager& effect_manager, const std::filesystem::path& pathname, std::function<V(const typename M::vertex_type::attributes_type&)> transfer_vertex, std::shared_ptr<IEffect> pEffectDefault = nullptr);

} // namespace quetzal::direct3d11

//------------------------------------------------------------------------------
template<typename V, typename I, typename M>
std::shared_ptr<quetzal::direct3d11::Model> quetzal::direct3d11::import_model(Renderer& renderer, EffectManager& effect_manager, const std::filesystem::path& pathname, std::function<V(const typename M::vertex_type::attributes_type&)> transfer_vertex, std::shared_ptr<IEffect> pEffectDefault)
{
    // select loader from file extension, this file handles quetzal brep Mesh ...

    using value_type = float;
    using vector_traits = math::VectorTraits<value_type, 3>;
    using mesh_type = brep::Mesh<brep::MeshTraits<vector_traits>>;

    mesh_type mesh;
    model::read_obj(mesh, pathname);
    model::Materials materials = model::read_materials(mesh, pathname.parent_path());

    auto pModel = std::make_shared<direct3d11::Model>();
    assert(pModel != nullptr);

    for (const auto& surface : mesh.surfaces())
    {
        auto [vertices, indices] = mesh_geometry<V, I, mesh_type::surface_type>(surface, transfer_vertex);
        std::shared_ptr<IEffect> pEffect = material_effect(effect_manager, materials, surface.properties().get(model::MaterialPropertyName));
        pModel->insert(renderer, vertices, indices, pEffect ? pEffect : pEffectDefault);
    }

    return pModel;
}

#endif // QUETZAL_DIRECT3D11_IMPORT_MODEL_HPP
