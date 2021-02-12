#if !defined(QUETZAL_LIGHTMATRIX_SCENE_HPP)
#define QUETZAL_LIGHTMATRIX_SCENE_HPP
//------------------------------------------------------------------------------
// Light Matrix Simulation
// Scene.hpp
//------------------------------------------------------------------------------

#include "Frame.hpp"
//#include "quetzal/direct3d11/Scene.hpp"
#include "quetzal/direct3d11/Renderer.hpp"
#include "quetzal/direct3d11/Camera.hpp"
#include "quetzal/direct3d11/Model.hpp"

namespace quetzal
{

namespace lightmatrix
{

    //--------------------------------------------------------------------------
//    class Scene : public quetzal::direct3d11::Scene
    class Scene
    {
    public:

        Scene(direct3d11::Renderer& renderer, Frame& frame);
        Scene(const Scene&) = delete;
        ~Scene() = default;

        Scene& operator=(const Scene&) = delete;

        void load();
        void update(float dt);
        void draw(direct3d11::Camera& camera);

        size_t vertex_count() const;
        size_t face_count() const;

    private:

        direct3d11::Renderer& m_renderer;

        Frame& m_frame;

        // pixel spacing here too ...
        float m_rPixel;

        std::unique_ptr<direct3d11::Model> m_pModelPixel;

        // Statistics
        size_t m_nVertices;
        size_t m_nFaces;
    };

} // namespace lightmatrix

} // namespace quetzal

#endif // QUETZAL_LIGHTMATRIX_SCENE_HPP
