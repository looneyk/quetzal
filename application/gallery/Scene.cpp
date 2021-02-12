//------------------------------------------------------------------------------
// Gallery
// Scene.cpp
//------------------------------------------------------------------------------

#include "Scene.hpp"
#include "Application.hpp"
#include "quetzal/brep/Mesh.hpp"
#include "quetzal/brep/MeshTraits.hpp"
#include "quetzal/direct3d11/Model.hpp"
#include "quetzal/direct3d11/Vertex.hpp"
#include "quetzal/direct3d11/import_model.hpp"
#include "quetzal/direct3d11/mesh_util.hpp"
#include "quetzal/common/ComException.hpp"
#include "quetzal/model/primitives.hpp"

#include "quetzal/direct3d11/DDSTextureLoader.h"

using namespace std;
using namespace DirectX;
using namespace quetzal;

namespace
{

    using value_type = float;
    using vector_traits = math::VectorTraits<value_type, 3>;
    using mesh_type = brep::Mesh<brep::MeshTraits<vector_traits>>;

    using V = direct3d11::VertexPositionNormalTextureTangent;
    using I = uint16_t; // uint32_t should work too ...

} // namespace

//------------------------------------------------------------------------------
quetzal::gallery::Scene::Scene(Application& application) :
    m_application(application),
    m_axes(application),
    m_pModelFloor(),
    m_pModelSpikosahedron(),
    m_nVertices(0),
    m_nTriangles(0)
{
}

//------------------------------------------------------------------------------
void quetzal::gallery::Scene::load()
{
    m_axes.load();

    using direct3d11::to_xm;
    auto transfer_vertex = [](const mesh_type::vertex_attributes_type& av) -> direct3d11::VertexPositionNormalTextureTangent { return {to_xm<mesh_type>(av.position()), to_xm<mesh_type>(av.normal()), to_xm<mesh_type>(av.texcoord()), {1.0f, 0.0f, 0.0f}}; };

    direct3d11::EffectManager::EffectInfo effect_info = {};
    effect_info.m_name = "floor";
    effect_info.m_bNormalMap = true;
    effect_info.m_colorDiffuse = XMFLOAT3(1.0f, 1.0f, 1.0f);
    effect_info.m_alpha = 1.0f;
    effect_info.m_diffuseTextureName = "stones.dds";
    effect_info.m_normalTextureName = "stones_nmap.dds";

    //effect_info.m_bNormalMap = false;
    //effect_info.m_diffuseTextureName = "WoodCrate02.dds";
    
    auto pEffectFloor = m_application.effect_manager().open(effect_info);

	mesh_type mesh;
    model::create_grid(mesh, "floor", 2, 2, 1000.0f, 1000.0f, true);
    triangulate(mesh);
    auto [vertices, indices] = direct3d11::mesh_geometry<V, I, mesh_type>(mesh, transfer_vertex);

    m_pModelFloor = std::make_shared<direct3d11::Model>();
    assert(m_pModelFloor != nullptr);
    m_pModelFloor->insert(m_application.renderer(), vertices, indices, pEffectFloor);

//    mesh.clear(); 
//    create spikosahedron(mesh, ...
//    triangulate(mesh);

    effect_info = {};
    effect_info.m_name = "spikosahedron";
    effect_info.m_colorDiffuse = XMFLOAT3(1.0f, 1.0f, 1.0f);
    effect_info.m_colorEmissive = XMFLOAT3(0.5f, 0.5f, 0.5f);
    effect_info.m_alpha = 1.0f;
    effect_info.m_diffuseTextureName = "142LL.dds";

    auto pEffectSpikosahedron = m_application.effect_manager().open(effect_info);

    m_pModelSpikosahedron = m_application.model_manager().open<V, I, mesh_type>("spikosahedron.obj", transfer_vertex, pEffectSpikosahedron);

    return;
}

//------------------------------------------------------------------------------
void quetzal::gallery::Scene::update(float dt)
{
    dt;
    return;
}

//------------------------------------------------------------------------------
void quetzal::gallery::Scene::draw(direct3d11::Camera& camera)
{
    m_axes.draw(camera, 100.0f);

    m_nVertices = 0;
    m_nTriangles = 0;
    bool bWireframe = false;

    DirectX::XMMATRIX mWorld = XMMatrixIdentity();
    m_pModelFloor->draw(m_application.renderer(), m_application.common_states(), mWorld, camera.view(), camera.projection());

    m_nVertices += m_pModelFloor->vertex_count();
    m_nTriangles += m_pModelFloor->triangle_count();

    mWorld = XMMatrixTranslation(0.0f, 0.0f, 115.225f);
    m_pModelSpikosahedron->draw(m_application.renderer(), m_application.common_states(), mWorld, camera.view(), camera.projection(), bWireframe);

    m_nVertices += m_pModelSpikosahedron->vertex_count();
    m_nTriangles += m_pModelSpikosahedron->triangle_count();

    return;
}

//------------------------------------------------------------------------------
size_t quetzal::gallery::Scene::vertex_count() const
{
    return m_nVertices;
}

//------------------------------------------------------------------------------
size_t quetzal::gallery::Scene::triangle_count() const
{
    return m_nTriangles;
}
