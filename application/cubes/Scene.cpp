//------------------------------------------------------------------------------
// Cubes
// Scene.cpp
//------------------------------------------------------------------------------

#include "Scene.hpp"
#include "Application.hpp"
#include "brep/Mesh.hpp"
#include "brep/MeshTraits.hpp"
#include "common/ComException.hpp"
#include "direct3d11/Vertex.hpp"
#include "direct3d11/mesh_util.hpp"
#include "math/VectorTraits.hpp"
#include "math/math_util.hpp"

#include "direct3d11/EffectBasic.hpp"

using namespace std;
using namespace DirectX;

namespace
{

    //--------------------------------------------------------------------------
    float luminance(XMFLOAT4& color)
    {
//        return 0.299f * color.x + 0.587f * color.y + 0.114f * color.z; // Y601
//        return 0.3f * color.x + 0.5f * color.y + 0.2f * color.z;
        return 0.3f * color.x + 0.4f * color.y + 0.3f * color.z;
    }

    vector<quetzal::direct3d11::VertexPositionColor> s_vertices =
    {
        {{-0.5f, -0.5f, -0.5f}, XMFLOAT4{1.0f, 0.0f, 0.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, XMFLOAT4{1.0f, 0.0f, 0.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, XMFLOAT4{1.0f, 0.0f, 0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, XMFLOAT4{1.0f, 0.0f, 0.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, XMFLOAT4{0.0f, 1.0f, 0.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, XMFLOAT4{0.0f, 1.0f, 0.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, XMFLOAT4{0.0f, 1.0f, 0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, XMFLOAT4{0.0f, 1.0f, 0.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, XMFLOAT4{0.0f, 0.0f, 1.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, XMFLOAT4{0.0f, 0.0f, 1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, XMFLOAT4{0.0f, 0.0f, 1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, XMFLOAT4{0.0f, 0.0f, 1.0f, 1.0f}},

        {{-0.5f, -0.5f,  0.5f}, XMFLOAT4{1.0f, 0.0f, 0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, XMFLOAT4{1.0f, 0.0f, 0.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, XMFLOAT4{1.0f, 0.0f, 0.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, XMFLOAT4{1.0f, 0.0f, 0.0f, 1.0f}},

        {{-0.5f,  0.5f, -0.5f}, XMFLOAT4{0.0f, 1.0f, 0.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, XMFLOAT4{0.0f, 1.0f, 0.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, XMFLOAT4{0.0f, 1.0f, 0.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, XMFLOAT4{0.0f, 1.0f, 0.0f, 1.0f}},

        {{ 0.5f, -0.5f, -0.5f}, XMFLOAT4{0.0f, 0.0f, 1.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, XMFLOAT4{0.0f, 0.0f, 1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, XMFLOAT4{0.0f, 0.0f, 1.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, XMFLOAT4{0.0f, 0.0f, 1.0f, 1.0f}}
    };

    vector<uint16_t> s_indices =
    {
         0,  1,  2,
         2,  1,  3,
        12, 13, 14,
        14, 13, 15,
        20, 21, 22,
        22, 21, 23,
         8,  9, 10,
        10,  9, 11,
        16, 17, 18, 
        18, 17, 19,
         4,  5,  6,
         6,  5,  7
    };

    vector<XMFLOAT4> s_colors =
    {
        {1.0f, 0.0f, 0.0f, 1.0f},
        {1.0f, 0.2f, 0.0f, 1.0f},
        {1.0f, 0.4f, 0.0f, 1.0f},
        {1.0f, 0.6f, 0.0f, 1.0f},
        {1.0f, 0.8f, 0.0f, 1.0f},
        {1.0f, 1.0f, 0.0f, 1.0f},
        {0.5f, 1.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 0.5f, 1.0f},
        {0.0f, 1.0f, 1.0f, 1.0f},
        {0.0f, 0.5f, 1.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {0.4f, 0.0f, 1.0f, 1.0f},
        {0.6f, 0.0f, 1.0f, 1.0f},
        {1.0f, 0.0f, 1.0f, 1.0f},
        {1.0f, 0.0f, 0.5f, 1.0f}
    };

    //--------------------------------------------------------------------------
    void set_vertex_colors(quetzal::direct3d11::Renderer& renderer, quetzal::direct3d11::Model* pModel, XMFLOAT4 color)
    {
        const XMFLOAT4 color1(color);
        const XMFLOAT4 color2(color.x / 2, color.y / 2, color.z / 2, color.w);
        const XMFLOAT4 color4(color.x / 4, color.y / 4, color.z / 4, color.w);
        const XMFLOAT4 colors[] {color1, color4, color2, color1, color4, color2};

        // Apply colors to vertices ...

        for (int i = 0; i < 6; ++i)
        {
            int j = i * 4;
            s_vertices[j + 0].m_color = colors[i];
            s_vertices[j + 1].m_color = colors[i];
            s_vertices[j + 2].m_color = colors[i];
            s_vertices[j + 3].m_color = colors[i];
        }

        // Rebuild vertex buffer (not the best way to do this ...
        quetzal::com_ptr<ID3D11Buffer> pBufferVertex;
        {
            HRESULT hr;

            D3D11_BUFFER_DESC buffer_desc = {};
            buffer_desc.Usage = D3D11_USAGE_DEFAULT;
            buffer_desc.ByteWidth = static_cast<UINT>(sizeof(quetzal::direct3d11::VertexPositionColor) * s_vertices.size());
            buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

            D3D11_SUBRESOURCE_DATA subresource_data = {};
            subresource_data.pSysMem = s_vertices.data();

            HR_CHECK(renderer.device()->CreateBuffer(&buffer_desc, &subresource_data, &pBufferVertex.p()));
        }

        pModel->m_meshes.front()->m_parts.front()->m_pBufferVertex = pBufferVertex;
        return;
    }

    using V = quetzal::direct3d11::VertexPositionColor;
    using I = uint16_t;
    using mesh_type = quetzal::brep::Mesh<quetzal::brep::MeshTraits<quetzal::math::VectorTraits<float, 3>>>;

} // namespace

//------------------------------------------------------------------------------
quetzal::cubes::Scene::Scene(Application& application) :
    m_application(application),
    m_axes(application),
    m_pModel(),
    m_nColors(s_colors.size()),
    m_nColor(11),
    m_color(s_colors[m_nColor]),
    m_nSide(7),
    m_bCycle(false),
    m_bSmooth(false),
    m_tTransition(0.15f),
    m_dt(0.0f),
    m_nVertices(0),
    m_nTriangles(0)
{
}

//------------------------------------------------------------------------------
void quetzal::cubes::Scene::load()
{
    m_axes.load();

    float yMin = 1.0f;
    for (int i = 0; i < m_nColors; ++i)
    {
        XMFLOAT4& color = s_colors[i];
        float y = luminance(color);
        if (y < yMin)
        {
            yMin = y;
        }
    }

    if (yMin > 0.0f)
    {
        for (int i = 0; i < m_nColors; ++i)
        {
            XMFLOAT4& color = s_colors[i];
            float y = luminance(color);
            float yScale = yMin / y;
            color.x *= yScale;
            color.y *= yScale;
            color.z *= yScale;
        }
    }

    // Effect
//    auto pEffect = make_shared<direct3d11::EffectBasic>(m_application.renderer());
//    pEffect->SetVertexColorEnabled(true);

    direct3d11::EffectManager::EffectInfo effectInfo = {};
    effectInfo.m_name = "vertex_color";
    effectInfo.m_bVertexColor = true;
    effectInfo.m_alpha = 1.0f; // ...
    effectInfo.m_colorDiffuse = {1.0f, 1.0f, 1.0f}; // ...
//    auto pEffect = create_basic_effect(m_application.renderer(), effectInfo);
    auto pEffect = m_application.effect_manager().open(effectInfo);
    assert(pEffect != nullptr);

    using direct3d11::to_xm;
    auto transfer_vertex = [](const mesh_type::vertex_attributes_type& av) -> direct3d11::VertexPositionColor { return {to_xm<mesh_type>(av.position()), {1.0f, 1.0f, 1.0f, 1.0f}}; };

    m_pModel = std::make_shared<direct3d11::Model>();
    assert(m_pModel != nullptr);
    m_pModel->insert(m_application.renderer(), s_vertices, s_indices, pEffect);
    return;
}

//------------------------------------------------------------------------------
bool quetzal::cubes::Scene::cycle() const
{
    return m_bCycle;
}

//------------------------------------------------------------------------------
bool quetzal::cubes::Scene::smooth() const
{
    return m_bSmooth;
}

//------------------------------------------------------------------------------
void quetzal::cubes::Scene::set_cycle(bool b)
{
    m_bCycle = b;
    return;
}

//------------------------------------------------------------------------------
void quetzal::cubes::Scene::set_smooth(bool b)
{
    m_bSmooth = b;
    return;
}

//------------------------------------------------------------------------------
void quetzal::cubes::Scene::update(float dt)
{
    m_dt += dt;

    while (m_dt >= m_tTransition)
    {
        if (m_bCycle)
        {
            m_nColor = ++m_nColor % m_nColors;
        }

        m_dt -= m_tTransition;
    }

    if (m_bCycle) // and only if color actually changed ...
    {
        if (m_bSmooth)
        {
            update_smooth(m_dt / m_tTransition);
        }
        else
        {
            update_discrete();
        }
    }

    return;
}

//------------------------------------------------------------------------------
void quetzal::cubes::Scene::draw(direct3d11::Camera& camera)
{
    m_axes.draw(camera, 1.0f);

    set_vertex_colors(m_application.renderer(), m_pModel.get(), m_color);

    m_nVertices = 0;
    m_nTriangles = 0;

//    const CubeArray& cubearray = m_display.cubearray();
    for (int i = 0; i < m_nSide; ++i)
    {
        float z = (m_nSide - i - 0.5f); // 0.5 = cube dimension / 2

        for (int j = 0; j <= i; ++j)
        {
            float x = i - j + 0.5f;
            float y = j + 0.5f;
if (i == m_nSide - 1 && j == 0)
set_vertex_colors(m_application.renderer(), m_pModel.get(), s_colors[0]);
else if (i == m_nSide - 1 && j == i)
set_vertex_colors(m_application.renderer(), m_pModel.get(), s_colors[7]);
else
set_vertex_colors(m_application.renderer(), m_pModel.get(), m_color);
//            const Cube& cube = cubearray.cube(nCube++);
//            draw_cube(cube);
            draw_cube(camera, x, y, z);
        }
    }

    return;
}

//------------------------------------------------------------------------------
size_t quetzal::cubes::Scene::vertex_count() const
{
    return m_nVertices;
}

//------------------------------------------------------------------------------
size_t quetzal::cubes::Scene::triangle_count() const
{
    return m_nTriangles;
}

//------------------------------------------------------------------------------
void quetzal::cubes::Scene::update_discrete()
{
    assert(m_nColor < m_nColors);
    m_color = s_colors[m_nColor];
    return;
}

//------------------------------------------------------------------------------
void quetzal::cubes::Scene::update_smooth(float fraction)
{
    size_t nColor0 = m_nColor;
    assert(nColor0 < m_nColors);

    size_t nColor1 = (nColor0 + 1) % m_nColors;
    assert(nColor1 < m_nColors);

    float fraction1 = fraction;
    assert(fraction1 >= 0.0f && fraction1 <= 1.0f);

    float fraction0 = 1.0f - fraction1;
    assert(fraction0 >= 0.0f && fraction0 <= 1.0f);

    m_color.x = s_colors[nColor0].x * fraction0 + s_colors[nColor1].x * fraction1;
    m_color.y = s_colors[nColor0].y * fraction0 + s_colors[nColor1].y * fraction1;
    m_color.z = s_colors[nColor0].z * fraction0 + s_colors[nColor1].z * fraction1;
    return;
}

//------------------------------------------------------------------------------
void quetzal::cubes::Scene::draw_cube(direct3d11::Camera& camera, /*const Cube& cube, */float x, float y, float z)
{
    DirectX::XMMATRIX mWorld = XMMatrixTranslation(x, y, z);

    m_pModel->draw(m_application.renderer(), m_application.common_states(), mWorld, camera.view(), camera.projection());

    m_nVertices += s_vertices.size();
    m_nTriangles += s_indices.size() / 3;
    return;
}
