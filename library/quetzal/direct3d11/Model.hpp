#if !defined(QUETZAL_DIRECT3D11_MODEL_HPP)
#define QUETZAL_DIRECT3D11_MODEL_HPP
//------------------------------------------------------------------------------
// Direct3D 11
// Model.hpp
//------------------------------------------------------------------------------

#include "CommonStates.hpp"
#include "Effect.hpp"
#include "Renderer.hpp"
#include "ShaderBytecode.hpp"
#include "buffer.hpp"
#include "index_format.hpp"
#include "input_layout.hpp"
#include "quetzal/common/com_ptr.hpp"
#include "quetzal/common/ComException.hpp"
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <memory>
#include <functional>
#include <set>
#include <string>
#include <vector>
#include <cstdint>

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    // Model loading options
    enum ModelLoaderFlags : uint32_t
    {
        ModelLoader_FrontClockwise          = 0x0,
        ModelLoader_FrontCounterClockwise   = 0x1,
        ModelLoader_PremultipledAlpha       = 0x2,
        ModelLoader_MaterialColorsSRGB      = 0x4,
        ModelLoader_AllowLargeModels        = 0x8,
    };

    //--------------------------------------------------------------------------
    // Each mesh part is a submesh with a single effect
    class ModelMeshPart
    {
    public:

        ModelMeshPart() noexcept;
        ModelMeshPart(const ModelMeshPart&) = default;
        ModelMeshPart(ModelMeshPart&&) = default;
        virtual ~ModelMeshPart() = default;

        ModelMeshPart& operator=(const ModelMeshPart&) = default;
        ModelMeshPart& operator=(ModelMeshPart&&) = default;

        // Draw mesh part with custom effect
        void draw(Renderer& renderer, IEffect* pEffect, ID3D11InputLayout* pInputLayout, std::function<void()> set_custom_state = nullptr) const;

        void draw_instanced(Renderer& renderer, IEffect* pEffect, ID3D11InputLayout* pInputLayout, uint32_t m_nInstances, uint32_t nInstanceStart = 0, std::function<void()> set_custom_state = nullptr) const;

        // Change effect used by part and regenerate input layout (be sure to call Model::set_modified as well)
        void set_effect(Renderer& renderer, std::shared_ptr<IEffect>& pEffect, bool bAlpha = false);

        size_t vertex_count() const;
        size_t triangle_count() const;

//    private: // ...

        size_t m_nIndexStart;
        size_t m_nIndices;
        size_t m_vertex_offset;
        size_t m_vertex_stride;
        D3D_PRIMITIVE_TOPOLOGY m_primitive_topoplogy;
        DXGI_FORMAT m_index_format;
        com_ptr<ID3D11InputLayout> m_pInputLayout;
        com_ptr<ID3D11Buffer> m_pBufferVertex;
        com_ptr<ID3D11Buffer> m_pBufferIndex;
        std::shared_ptr<IEffect> m_pEffect;
        std::function<HRESULT (Renderer&, const ShaderBytecode&, com_ptr<ID3D11InputLayout>&)> m_create_input_layout;
        bool m_bAlpha;
        size_t m_nVertices; // For reference only ...
    };

    //--------------------------------------------------------------------------
    // A mesh consists of one or more model mesh parts
    class ModelMesh
    {
    public:

        using parts_type = std::vector<std::unique_ptr<ModelMeshPart>>;

        ModelMesh() noexcept;
        ModelMesh(const ModelMesh&) = default;
        ModelMesh(ModelMesh&&) = default;
        virtual ~ModelMesh() = default;

        ModelMesh& operator=(const ModelMesh&) = default;
        ModelMesh& operator=(ModelMesh&&) = default;

        // Set up states for drawing mesh
        void prepare(Renderer& renderer, CommonStates& common_states, bool bAlpha = false, bool bWireframe = false) const;

        // Draw the mesh
        void XM_CALLCONV draw(Renderer& renderer, DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection, bool bAlpha = false, std::function<void()> set_custom_state = nullptr) const;

        size_t vertex_count() const;
        size_t triangle_count() const;

//    private: // ...

        std::string m_name;
        parts_type m_parts;
        DirectX::BoundingSphere m_bounding_sphere;
        DirectX::BoundingBox m_bounding_box;
        bool m_bFrontCounterClockwise;
        bool m_bAlphaPremultiplied;
    };

    //--------------------------------------------------------------------------
    // A model consists of one or more meshes
    class Model
    {
    public:

        using meshes_type = std::vector<std::shared_ptr<ModelMesh>>;

        Model() noexcept;
        Model(const Model&) = default;
        Model(Model&&) = default;
        virtual ~Model() = default;

        Model& operator=(const Model&) = default;
        Model& operator=(Model&&) = default;

        // Draw all the meshes in the model
        void XM_CALLCONV draw(Renderer& renderer, CommonStates& common_states, DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection, bool bWireframe = false, std::function<void()> set_custom_state = nullptr) const;

        // Notify model that effects, parts list, or mesh list has changed
        void set_modified();

        // Update all effects used by the model
        void update_effects(std::function<void(IEffect*)> set_effect);

        // creates a copy and inserts that, separate function to simply insert an instance? ...
//        void insert(const std::shared_ptr<Model> pModel);

        template<typename V, typename I>
        void insert(Renderer& renderer, const std::vector<V>& vertices, const std::vector<I>& indices, std::shared_ptr<IEffect> pEffect = nullptr, ModelLoaderFlags flags = ModelLoader_FrontCounterClockwise);

        size_t vertex_count() const;
        size_t triangle_count() const;

//    private: // ...

        std::string m_name;
        meshes_type m_meshes;
/*
        // Loads a model from a Visual Studio Starter Kit .CMO file
        static std::shared_ptr<Model> CreateFromCMO(Renderer& renderer, const uint8_t* meshData, size_t dataSize, IEffectFactory& fxFactory, ModelLoaderFlags flags = ModelLoader_FrontClockwise);
        static std::shared_ptr<Model> CreateFromCMO(Renderer& renderer, const std::string& filename, IEffectFactory& fxFactory, ModelLoaderFlags flags = ModelLoader_FrontCounterClockwise);

        // Loads a model from a DirectX SDK .SDKMESH file
        static std::shared_ptr<Model> CreateFromSDKMESH(Renderer& renderer, const uint8_t* meshData, size_t dataSize, IEffectFactory& fxFactory, ModelLoaderFlags flags = ModelLoader_FrontCounterClockwise);
        static std::shared_ptr<Model> CreateFromSDKMESH(Renderer& renderer, const std::string& filename, IEffectFactory& fxFactory, ModelLoaderFlags flags = ModelLoader_FrontClockwise);

        // Loads a model from a .VBO file
        static std::shared_ptr<Model> CreateFromVBO(Renderer& renderer, const uint8_t* meshData, size_t dataSize, std::shared_ptr<IEffect> pEffect = nullptr, ModelLoaderFlags flags = ModelLoader_FrontCounterClockwise);
        static std::shared_ptr<Model> CreateFromVBO(Renderer& renderer, const std::string& filename, std::shared_ptr<IEffect> pEffect = nullptr, ModelLoaderFlags flags = ModelLoader_FrontClockwise);
*/
    private:

        std::set<IEffect*> m_pEffects; // Effect cache
    };

} // namespace quetzal::direct3d11

//------------------------------------------------------------------------------
template<typename V, typename I>
void quetzal::direct3d11::Model::insert(Renderer& renderer, const std::vector<V>& vertices, const std::vector<I>& indices, std::shared_ptr<IEffect> pEffect, ModelLoaderFlags flags)
{
    assert(pEffect != nullptr);

    HRESULT hr = S_OK;

    // Vertex buffer
    com_ptr<ID3D11Buffer> pBufferVertex;
    HR_CHECK(create_buffer(renderer, vertices, D3D11_BIND_VERTEX_BUFFER, pBufferVertex));

    // Index buffer
    com_ptr<ID3D11Buffer> pBufferIndex;
    HR_CHECK(create_buffer(renderer, indices, D3D11_BIND_INDEX_BUFFER, pBufferIndex));

    // Input layout
    com_ptr<ID3D11InputLayout> pInputLayout;
    HR_CHECK(create_input_layout<V>(renderer, pEffect->vertex_shader_bytecode(), pInputLayout));

    auto pPart = new direct3d11::ModelMeshPart();
    assert(pPart != nullptr);
    pPart->m_nIndices = indices.size();
    pPart->m_nIndexStart = 0;
    pPart->m_vertex_offset = 0;
    pPart->m_vertex_stride = V::stride();
    pPart->m_primitive_topoplogy = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    pPart->m_index_format = index_format<I>();
    pPart->m_pInputLayout = pInputLayout;
    pPart->m_pBufferVertex = pBufferVertex;
    pPart->m_pBufferIndex = pBufferIndex;
    pPart->m_pEffect = pEffect;
    pPart->m_create_input_layout = create_input_layout<V>;
    pPart->m_nVertices = vertices.size(); // indices.size()? ...

    auto pMesh = std::make_shared<direct3d11::ModelMesh>();
    assert(pMesh != nullptr);
    pMesh->m_bFrontCounterClockwise = (flags & ModelLoader_FrontCounterClockwise) != 0;
    pMesh->m_bAlphaPremultiplied = (flags & ModelLoader_PremultipledAlpha) != 0;
//    const DirectX::XMFLOAT3& position0 = vertices.data()->m_position;
//    DirectX::BoundingSphere::CreateFromPoints(pMesh->m_bounding_sphere, vertices.size(), &position0, V::stride());
//    DirectX::BoundingBox::CreateFromPoints(pMesh->m_bounding_box, vertices.size(), &position0, V::stride());
    pMesh->m_parts.emplace_back(pPart);

    m_meshes.emplace_back(pMesh);
    return;
}

#endif // QUETZAL_DIRECT3D11_MODEL_HPP
