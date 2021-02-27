//------------------------------------------------------------------------------
// modx
// Axes.cpp
//------------------------------------------------------------------------------

#include "Axes.hpp"
#include "quetzal/brep/Mesh.hpp"
#include "quetzal/brep/MeshTraits.hpp"
#include "quetzal/direct3d11/Vertex.hpp"
#include "quetzal/math/VectorTraits.hpp"
#include <d3d11_1.h>

using namespace std;
using namespace DirectX;

namespace
{

    using V = quetzal::direct3d11::VertexPositionColor;
    using I = uint16_t;
    using mesh_type = quetzal::brep::Mesh<quetzal::brep::MeshTraits<quetzal::math::VectorTraits<float, 3>>>;

} // namespace

//------------------------------------------------------------------------------
quetzal::modx::Axes::Axes(direct3d11::Application& application) :
    m_application(application),
    m_pModelOrigin(),
    m_pModelShaft(),
    m_pModelHead()
{
}

//------------------------------------------------------------------------------
void quetzal::modx::Axes::load()
{
    using direct3d11::to_xm;
    auto transfer_vertex = [](const mesh_type::vertex_attributes_type& av) -> direct3d11::VertexPositionColor { return {to_xm<mesh_type>(av.position()), {1.0f, 1.0f, 1.0f, 1.0f}}; };

    direct3d11::EffectManager::EffectInfo effectInfo = {};
    effectInfo.m_alpha = 1.0f; // ...

    effectInfo.m_name = "emissive_red";
    effectInfo.m_colorEmissive = {1.0f, 0.0f, 0.0f};
    auto pEffect = m_application.effect_manager().open(effectInfo);
    assert(pEffect != nullptr);

    effectInfo.m_name = "emissive_green";
    effectInfo.m_colorEmissive = {0.0f, 1.0f, 0.0f};
    pEffect = m_application.effect_manager().open(effectInfo);
    assert(pEffect != nullptr);

    effectInfo.m_name = "emissive_blue";
    effectInfo.m_colorEmissive = {0.0f, 0.0f, 1.0f};
    pEffect = m_application.effect_manager().open(effectInfo);
    assert(pEffect != nullptr);

    effectInfo.m_name = "emissive_white";
    effectInfo.m_colorEmissive = {1.0f, 1.0f, 1.0f};
    pEffect = m_application.effect_manager().open(effectInfo);
    assert(pEffect != nullptr);

    m_pModelOrigin = m_application.model_manager().open<V, I, mesh_type>("unit_geosphere2.obj", transfer_vertex, pEffect);
    m_pModelShaft = m_application.model_manager().open<V, I, mesh_type>("unit_cylinder.obj", transfer_vertex, pEffect);
    m_pModelHead = m_application.model_manager().open<V, I, mesh_type>("unit_cone.obj", transfer_vertex, pEffect);
    return;
}

//------------------------------------------------------------------------------
void quetzal::modx::Axes::draw(direct3d11::Camera& camera, float scale, direct3d11::Vector3 position)
{
    auto pEffect = m_application.effect_manager().open("emissive_white");
    assert(pEffect != nullptr);
    draw_origin(camera, position, scale, pEffect);

    pEffect = m_application.effect_manager().open("emissive_red");
    assert(pEffect != nullptr);
    draw_axis(camera, position, scale, 1.0f, {1.0f, 0.0f, 0.0f}, {0.0f, math::PiHalf<float>, 0.0f}, pEffect);

    pEffect = m_application.effect_manager().open("emissive_green");
    assert(pEffect != nullptr);
    draw_axis(camera, position, scale, 1.0f, {0.0f, 1.0f, 0.0f}, {-math::PiHalf<float>, 0.0f, 0.0f}, pEffect);

    pEffect = m_application.effect_manager().open("emissive_blue");
    assert(pEffect != nullptr);
    draw_axis(camera, position, scale, 1.0f, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, pEffect);
    return;
}

//------------------------------------------------------------------------------
size_t quetzal::modx::Axes::vertex_count() const
{
    return m_pModelOrigin->vertex_count() + 3 * (m_pModelShaft->vertex_count() + m_pModelShaft->vertex_count());
}

//------------------------------------------------------------------------------
size_t quetzal::modx::Axes::triangle_count() const
{
    return m_pModelOrigin->triangle_count() + 3 * (m_pModelShaft->triangle_count() + m_pModelShaft->triangle_count());
}

//------------------------------------------------------------------------------
void quetzal::modx::Axes::draw_origin(direct3d11::Camera& camera, direct3d11::Vector3 position, float scale, std::shared_ptr<direct3d11::IEffect> pEffect)
{
    if (pEffect)
    {
        for (auto& pMesh : m_pModelOrigin->m_meshes)
        {
            for (auto& pPart : pMesh->m_parts)
            {
                pPart->set_effect(m_application.renderer(), pEffect);
            }
        }
    }

    scale *= 0.05f;
    DirectX::XMMATRIX mWorld = XMMatrixScaling(scale, scale, scale) * XMMatrixTranslationFromVector(XMLoadFloat3(&position.rep()));
 
    m_pModelOrigin->draw(m_application.renderer(), m_application.common_states(), mWorld, camera.view(), camera.projection());
    return;
}

//------------------------------------------------------------------------------
void quetzal::modx::Axes::draw_axis(direct3d11::Camera& camera, direct3d11::Vector3 position, float scale, float length, direct3d11::Vector3 head, direct3d11::Vector3 rotation, std::shared_ptr<direct3d11::IEffect> pEffect)
{
    if (pEffect)
    {
        for (auto& pMesh : m_pModelShaft->m_meshes)
        {
            for (auto& pPart : pMesh->m_parts)
            {
                pPart->set_effect(m_application.renderer(), pEffect);
            }
        }
        for (auto& pMesh : m_pModelHead->m_meshes)
        {
            for (auto& pPart : pMesh->m_parts)
            {
                pPart->set_effect(m_application.renderer(), pEffect);
            }
        }
    }

    DirectX::XMMATRIX mWorldShaft = XMMatrixScaling(0.02f * scale, 0.02f * scale, length * scale);
    if (rotation.x() != 0.0f)
    {
        mWorldShaft *= XMMatrixRotationX(rotation.x());
    }
    if (rotation.y() != 0.0f)
    {
        mWorldShaft *= XMMatrixRotationY(rotation.y());
    }
    if (rotation.z() != 0.0f)
    {
        mWorldShaft *= XMMatrixRotationZ(rotation.z());
    }
    mWorldShaft *= XMMatrixTranslationFromVector(XMLoadFloat3(&position.rep()));
    
    DirectX::XMMATRIX mWorldHead = XMMatrixScaling(0.05f * scale, 0.05f * scale, 0.2f * scale);
    if (rotation.x() != 0.0f)
    {
        mWorldHead *= XMMatrixRotationX(rotation.x());
    }
    if (rotation.y() != 0.0f)
    {
        mWorldHead *= XMMatrixRotationY(rotation.y());
    }
    if (rotation.z() != 0.0f)
    {
        mWorldHead *= XMMatrixRotationZ(rotation.z());
    }
    mWorldHead *= XMMatrixTranslationFromVector(XMLoadFloat3(&(head * scale).rep()));
    mWorldHead *= XMMatrixTranslationFromVector(XMLoadFloat3(&position.rep()));

    m_pModelShaft->draw(m_application.renderer(), m_application.common_states(), mWorldShaft, camera.view(), camera.projection());
    m_pModelHead->draw(m_application.renderer(), m_application.common_states(), mWorldHead, camera.view(), camera.projection());
    return;
}
