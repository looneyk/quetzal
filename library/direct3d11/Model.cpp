//------------------------------------------------------------------------------
// Direct3D 11
// Model.cpp
//------------------------------------------------------------------------------

#include "Model.hpp"
#include "common/ComException.hpp"
#include <algorithm>

using namespace std;
using namespace DirectX;

//------------------------------------------------------------------------------
quetzal::direct3d11::ModelMeshPart::ModelMeshPart() noexcept :
    m_nIndexStart(0),
    m_nIndices(0),
    m_vertex_offset(0),
    m_vertex_stride(0),
    m_primitive_topoplogy(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
    m_index_format(DXGI_FORMAT_R16_UINT),
    m_pInputLayout(),
    m_pBufferVertex(),
    m_pBufferIndex(),
    m_pEffect(),
    m_create_input_layout(),
    m_bAlpha(false),
    m_nVertices(0)
{
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::ModelMeshPart::draw(Renderer& renderer, IEffect* pEffect, ID3D11InputLayout* pInputLayout, function<void()> set_custom_state) const
{
    ID3D11DeviceContext* pContext = renderer.context();
    assert(pContext != nullptr);

    pContext->IASetInputLayout(pInputLayout);

    ID3D11Buffer* ppBuffers[] = {m_pBufferVertex};
    UINT stride = static_cast<UINT>(m_vertex_stride);
    UINT offset = 0;
    pContext->IASetVertexBuffers(0, 1, ppBuffers, &stride, &offset);

    pContext->IASetIndexBuffer(m_pBufferIndex, m_index_format, 0);

    assert(pEffect != nullptr);
    pEffect->apply(pContext);

    // Hook lets the caller replace our shaders or state settings with whatever else they see fit.
    if (set_custom_state != nullptr)
    {
        set_custom_state();
    }

    // Draw the primitive.
    pContext->IASetPrimitiveTopology(m_primitive_topoplogy);
    pContext->DrawIndexed(static_cast<UINT>(m_nIndices), static_cast<UINT>(m_nIndexStart), static_cast<INT>(m_vertex_offset));
    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::ModelMeshPart::draw_instanced(Renderer& renderer, IEffect* pEffect, ID3D11InputLayout* pInputLayout, uint32_t nInstances, uint32_t nInstanceStart, std::function<void()> set_custom_state) const
{
    ID3D11DeviceContext* pContext = renderer.context();
    assert(pContext != nullptr);

    pContext->IASetInputLayout(pInputLayout);

    ID3D11Buffer* ppBuffers[] = {m_pBufferVertex};
    UINT stride = static_cast<UINT>(m_vertex_stride);
    UINT offset = 0;
    pContext->IASetVertexBuffers(0, 1, ppBuffers, &stride, &offset);

    pContext->IASetIndexBuffer(m_pBufferIndex, m_index_format, 0);

    assert(pEffect != nullptr);
    pEffect->apply(pContext);

    // Hook lets the caller replace our shaders or state settings with whatever else they see fit.
    if (set_custom_state != nullptr)
    {
        set_custom_state();
    }

    // Draw the primitive.
    pContext->IASetPrimitiveTopology(m_primitive_topoplogy);
    pContext->DrawIndexedInstanced(static_cast<UINT>(m_nIndices), nInstances, static_cast<UINT>(m_nIndexStart), static_cast<INT>(m_vertex_offset), nInstanceStart);
    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::ModelMeshPart::set_effect(Renderer& renderer, shared_ptr<IEffect>& pEffect, bool bAlpha)
{
    HRESULT hr = S_OK;
  
    assert(pEffect != nullptr);
    m_pEffect = pEffect;
    m_bAlpha = bAlpha;

    assert(m_create_input_layout);
    ShaderBytecode bytecode = m_pEffect->vertex_shader_bytecode();
    HR_CHECK(m_create_input_layout(renderer, bytecode, m_pInputLayout));

    return;
}

//------------------------------------------------------------------------------
size_t quetzal::direct3d11::ModelMeshPart::vertex_count() const
{
    return m_nVertices;
}

//------------------------------------------------------------------------------
size_t quetzal::direct3d11::ModelMeshPart::triangle_count() const
{
    return m_nIndices / 3;
}

//------------------------------------------------------------------------------
quetzal::direct3d11::ModelMesh::ModelMesh() noexcept :
    m_parts(),
    m_bounding_sphere(),
    m_bounding_box(),
    m_bFrontCounterClockwise(true),
    m_bAlphaPremultiplied(true)
{
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::ModelMesh::prepare(Renderer& renderer, CommonStates& common_states, bool bAlpha, bool bWireframe) const
{
    ID3D11DeviceContext* pContext = renderer.context();
    assert(pContext != nullptr);

    // Set the blend and depth stencil state.
    ID3D11BlendState* pBlendState = nullptr;
    ID3D11DepthStencilState* pDepthStencilState = nullptr;

    if (bAlpha)
    {
        if (m_bAlphaPremultiplied)
        {
            pBlendState = common_states.BlendAlpha();
            pDepthStencilState = common_states.DepthStencilRead();
        }
        else
        {
            pBlendState = common_states.BlendNonPremultiplied();
            pDepthStencilState = common_states.DepthStencilRead();
        }
    }
    else
    {
        pBlendState = common_states.BlendOpaque();
        pDepthStencilState = common_states.DepthStencilDefault();
    }

    pContext->OMSetBlendState(pBlendState, nullptr, 0xFFFFFFFF);
    pContext->OMSetDepthStencilState(pDepthStencilState, 0);

    // Set the rasterizer state.
    if (bWireframe)
    {
        pContext->RSSetState(common_states.RasterizerWireframe());
    }
    else
    {
        pContext->RSSetState(m_bFrontCounterClockwise ? common_states.RasterizerCullClockwise() : common_states.RasterizerCullCounterClockwise());
    }

    // Set the sampler state.
    ID3D11SamplerState* samplers[] =
    {
        common_states.SamplerLinearWrap(),
        common_states.SamplerLinearWrap(),
    };

    pContext->PSSetSamplers(0, 2, samplers);

    return;
}

//------------------------------------------------------------------------------
void XM_CALLCONV quetzal::direct3d11::ModelMesh::draw(Renderer& renderer, FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection, bool bAlpha, function<void()> set_custom_state) const
{
    ID3D11DeviceContext* pContext = renderer.context();
    assert(pContext != nullptr);

    for (const auto& pPart : m_parts)
    {
        assert(pPart != nullptr);

        // Skip alpha parts when drawing opaque or skip opaque parts if drawing alpha
        if (pPart->m_bAlpha != bAlpha)
        {
            continue;
        }

        auto imatrices = dynamic_cast<IEffectMatrices*>(pPart->m_pEffect.get());
        if (imatrices != nullptr)
        {
            imatrices->SetMatrices(world, view, projection);
        }

        pPart->draw(renderer, pPart->m_pEffect.get(), pPart->m_pInputLayout, set_custom_state); // context ...
    }

    return;
}

//------------------------------------------------------------------------------
size_t quetzal::direct3d11::ModelMesh::vertex_count() const
{
    size_t nVertices = 0;
    for_each(m_parts.begin(), m_parts.end(), [&](const std::unique_ptr<ModelMeshPart>& part) {nVertices += part->vertex_count();});
    return nVertices;
}

//------------------------------------------------------------------------------
size_t quetzal::direct3d11::ModelMesh::triangle_count() const
{
    size_t nTriangles = 0;
    for_each(m_parts.begin(), m_parts.end(), [&](const std::unique_ptr<ModelMeshPart>& part) {nTriangles += part->triangle_count();});
    return nTriangles;
}

//------------------------------------------------------------------------------
quetzal::direct3d11::Model::Model() noexcept :
    m_meshes(),
    m_pEffects()
{
}

//------------------------------------------------------------------------------
void XM_CALLCONV quetzal::direct3d11::Model::draw(Renderer& renderer, CommonStates& common_states, FXMMATRIX world, CXMMATRIX view, CXMMATRIX projection, bool bWireframe, function<void()> set_custom_state) const
{
    ID3D11DeviceContext* pContext = renderer.context();
    assert(pContext != nullptr);

    // Draw opaque parts
    for (auto i = m_meshes.cbegin(); i != m_meshes.cend(); ++i)
    {
        auto pMesh = i->get();
        assert(pMesh != nullptr);

        pMesh->prepare(renderer, common_states, false, bWireframe); // context ...
        pMesh->draw(renderer, world, view, projection, false, set_custom_state); // context ...
    }

    // Draw transparent parts
    for (auto i = m_meshes.cbegin(); i != m_meshes.cend(); ++i)
    {
        auto pMesh = i->get();
        assert(pMesh != nullptr);

        pMesh->prepare(renderer, common_states, true, bWireframe); // context ...
        pMesh->draw(renderer, world, view, projection, true, set_custom_state); // context ...
    }

    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Model::set_modified()
{
    m_pEffects.clear();
    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Model::update_effects(function<void(IEffect*)> set_effect)
{
    if (m_pEffects.empty())
    {
        // This cache ensures we only set each effect once (could be shared)
        for (auto& pMesh : m_meshes)
        {
            assert(pMesh != nullptr);
            for (auto& pPart : pMesh->m_parts)
            {
                assert(pPart != nullptr);
                if (pPart->m_pEffect != nullptr)
                {
                    m_pEffects.insert(pPart->m_pEffect.get());
                }
            }
        }
    }

    assert(set_effect != nullptr);

    for (auto pEffect: m_pEffects)
    {
        set_effect(pEffect);
    }

    return;
}

/*
//------------------------------------------------------------------------------
void quetzal::direct3d11::Model::insert(const std::shared_ptr<Model> pModel)
{
    assert(pModel);

    for (auto& pMesh : pModel->m_meshes)
    {
        assert(pMesh != nullptr);
        ModelMesh* pMeshClone = std::make_shared<direct3d11::ModelMesh>();
        pMeshClone->m_name = pMesh->m_name;
        pMeshClone->m_bounding_sphere = pMesh->m_bounding_sphere;
        pMeshClone->m_bounding_box = pMesh->m_bounding_box;
        pMeshClone->m_bFrontCounterClockwise = pMesh->m_bFrontCounterClockwise;
        pMeshClone->m_bAlphaPremultiplied = pMesh->m_bAlphaPremultiplied;

        for (auto& pPart : pMesh->m_parts)
        {
            assert(pPart != nullptr);
            ModelMeshPart* pPartClone = new direct3d11::ModelMeshPart();
            assert(pPartClone != nullptr);
            *pPartClone = *pPart;
            pMeshClone->m_parts.emplace_back(pPartClone);
        }

        m_meshes.emplace_back(pMeshClone);
    }

    return;
}
*/

//------------------------------------------------------------------------------
size_t quetzal::direct3d11::Model::vertex_count() const
{
    size_t nVertices = 0;
    for_each(m_meshes.begin(), m_meshes.end(), [&](const std::shared_ptr<ModelMesh>& mesh) {nVertices += mesh->vertex_count();});
    return nVertices;
}

//------------------------------------------------------------------------------
size_t quetzal::direct3d11::Model::triangle_count() const
{
    size_t nTriangles = 0;
    for_each(m_meshes.begin(), m_meshes.end(), [&](const std::shared_ptr<ModelMesh>& mesh) {nTriangles += mesh->triangle_count();});
    return nTriangles;
}
