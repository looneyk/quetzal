#if !defined(QUETZAL_GALLERY_SCENE_HPP)
#define QUETZAL_GALLERY_SCENE_HPP
//------------------------------------------------------------------------------
// Gallery
// Scene.hpp
//------------------------------------------------------------------------------

#include "quetzal/direct3d11/Camera.hpp"
#include "quetzal/direct3d11/Model.hpp"
#include "quetzal/modx/Axes.hpp"

namespace quetzal::gallery
{

    class Application;

    //--------------------------------------------------------------------------
//    class Scene : public quetzal::direct3d11::Scene
    class Scene
    {
    public:

        Scene(Application& application);
        Scene(const Scene&) = delete;
        ~Scene() = default;

        Scene& operator=(const Scene&) = delete;

        void load();
        void update(float dt);
        void draw(direct3d11::Camera& camera);

        size_t vertex_count() const;
        size_t triangle_count() const;

    private:

        Application& m_application;

        modx::Axes m_axes;

        std::shared_ptr<direct3d11::Model> m_pModelFloor;
        std::shared_ptr<direct3d11::Model> m_pModelSpikosahedron;

        // Statistics
        size_t m_nVertices;
        size_t m_nTriangles;
    };

} // namespace quetzal::gallery

#endif // QUETZAL_GALLERY_SCENE_HPP
