//------------------------------------------------------------------------------
// Light Matrix Simulation
// Scene.hpp
//------------------------------------------------------------------------------

#include "Scene.hpp"
#include "brep/Mesh.hpp"
#include "brep/MeshTraits.hpp"
#include "common/Exception.hpp"
#include "direct3d11/Effects.hpp"
#include "direct3d11/Vertex.hpp"
#include "model/mesh_extraction.hpp"
#include "model/primitives.hpp"

using namespace std;
using namespace DirectX;

//------------------------------------------------------------------------------
quetzal::lightmatrix::Scene::Scene(direct3d11::Renderer& renderer, Frame& frame) :
    m_renderer(renderer),
    m_frame(frame),
    m_rPixel(0.5f),
    m_pModelPixel(),
    m_nVertices(0),
    m_nFaces()
{
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Scene::load()
{
    direct3d11::EffectManager::EffectInfo effect_info = {};
    effect_info.m_name = "pixel";
    effect_info.m_colorDiffuse = XMFLOAT3(1.0f, 1.0f, 1.0f);
    effect_info.m_colorEmissive = XMFLOAT3(0.5f, 0.5f, 0.5f);
    effect_info.m_alpha = 1.0f;

    auto pEffectPixel = m_renderer.effect_manager().open(effect_info);

    using mesh_type = brep::Mesh<brep::MeshTraits<float>>;
    mesh_type mesh;
    model::create_icosahedron(mesh, "icosahedron", m_rPixel);

//    auto [vertices, indices] = model::extract_input_layout<geometry::PositionNormalTexture<float>, int16_t>(mesh);
// ...
    std::vector<direct3d11::VertexPositionNormalTexture> vertices;
    std::vector<uint32_t> indices;

    // instead, just pass vertices in as is and change emissive color in effect ...
    // either all vertices need color changes or each pixel needs separate tracked effect ...
//    vector<direct3d11::VertexPositionColor> vpcs(12);
//    transform(vertices.begin(), vertices.end(), vpcs.begin(), [](const direct3d11::geometry_vertex_type& v) -> direct3d11::VertexPositionColor { return {v.m_position, {0.0f, 0.0f, 0.0f, 1.0f}}; });
//
//    m_pModelPixel = direct3d11::Model::load(m_renderer, vpcs, indices, pEffectPixel);
    m_pModelPixel = direct3d11::Model::load(m_renderer, vertices, indices, pEffectPixel);

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Scene::update(float dt)
{
    dt;
    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Scene::draw(direct3d11::Camera& camera)
{
    m_nVertices = 0;
    m_nFaces = 0;

    int w = m_frame.width();
    int h = m_frame.height();

    for (int j = 0; j < h; ++j)
    {
        for (int i = 0; i < w; ++i)
        {
            Pixel pixel = m_frame.pixel(i, j);

            // set effect color from pixel.color() ...

            DirectX::XMMATRIX mWorld = XMMatrixTranslation(pixel.x(), 0.0f, -pixel.y());
            m_pModelPixel->draw(m_renderer, mWorld, camera.view(), camera.projection());

            m_nVertices += m_pModelPixel->vertex_count();
            m_nFaces += m_pModelPixel->face_count();
        }
    }

    return;
}

//------------------------------------------------------------------------------
size_t quetzal::lightmatrix::Scene::vertex_count() const
{
    return m_nVertices;
}

//------------------------------------------------------------------------------
size_t quetzal::lightmatrix::Scene::face_count() const
{
    return m_nFaces;
}
