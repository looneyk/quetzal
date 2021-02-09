#if !defined(QUETZAL_DIRECT3D11_IMPORT_MODEL_HPP)
#define QUETZAL_DIRECT3D11_IMPORT_MODEL_HPP
//------------------------------------------------------------------------------
// Direct3D 11
// import_model.hpp
//------------------------------------------------------------------------------

#include "EffectManager.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "Vertex.hpp"
#include "mesh_util.hpp"
#include "brep/Mesh.hpp"
#include "brep/MeshTraits.hpp"
#include "math/VectorTraits.hpp"
#include "model/obj_io.hpp"
#include <filesystem>

namespace quetzal::direct3d11
{

    template<typename V, typename I, typename M>
    std::shared_ptr<Model> import_model(Renderer& renderer, EffectManager& effect_manager, const std::filesystem::path& pathname, std::function<V(const typename M::vertex_type::attributes_type&)> transfer_vertex, std::shared_ptr<IEffect> pEffectDefault = nullptr);

namespace internal
{

    //--------------------------------------------------------------------------
    template<typename V, typename I, typename M>
    std::shared_ptr<IEffect> mesh_effect(EffectManager& effect_manager, const M& mesh, const std::string& name)
    {
		std::shared_ptr<IEffect> pEffect;
        if (name.empty())
        {
            return pEffect;
        }

        pEffect = effect_manager.open(name);
        if (!pEffect)
        {
            mesh;
//			  EffectManager::EffectInfo effectInfo = load_material(mesh, name); // from mtllib file ...
//            pEffect = effect_manager.open(effectInfo);
return pEffect; // ...
        }

        return pEffect;
    }

} // namespace internal

} // namespace quetzal::direct3d11

//------------------------------------------------------------------------------
template<typename V, typename I, typename M>
std::shared_ptr<quetzal::direct3d11::Model> quetzal::direct3d11::import_model(Renderer& renderer, EffectManager& effect_manager, const std::filesystem::path& pathname, std::function<V(const typename M::vertex_type::attributes_type&)> transfer_vertex, std::shared_ptr<IEffect> pEffectDefault)
{
    // select loader from file extension ...
    // this handles quetzal Mesh ...

    using value_type = float;
    using vector_traits = math::VectorTraits<value_type, 3>;
    using mesh_type = brep::Mesh<brep::MeshTraits<vector_traits>>;

    mesh_type mesh;
    model::read_obj(mesh, pathname); // throws on error

    auto pModel = std::make_shared<direct3d11::Model>();
    assert(pModel != nullptr);

    for (const auto& surface : mesh.surfaces())
    {
        auto [vertices, indices] = mesh_geometry<V, I, mesh_type::surface_type>(surface, transfer_vertex);
        std::shared_ptr<IEffect> pEffect = internal::mesh_effect<V, I, mesh_type::surface_type>(effect_manager, surface, surface.property("material")); // "material" ...
        pModel->insert(renderer, vertices, indices, pEffect ? pEffect : pEffectDefault);
    }

    return pModel;
}

#endif // QUETZAL_DIRECT3D11_IMPORT_MODEL_HPP
