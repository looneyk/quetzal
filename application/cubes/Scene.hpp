#if !defined(QUETZAL_CUBES_SCENE_HPP)
#define QUETZAL_CUBES_SCENE_HPP
//------------------------------------------------------------------------------
// Cubes
// Scene.hpp
//------------------------------------------------------------------------------

#include "quetzal/direct3d11/Camera.hpp"
#include "quetzal/direct3d11/Model.hpp"
#include "quetzal/modx/Axes.hpp"

namespace quetzal::cubes
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

        bool cycle() const;
        bool smooth() const;

        void set_cycle(bool b);
        void set_smooth(bool b);

        void load();
        void update(float dt);
        void draw(direct3d11::Camera& camera);

        size_t vertex_count() const;
        size_t triangle_count() const;

    private:

        void update_discrete();
        void update_smooth(float fraction); // fraction: 0..1

        void draw_cube(direct3d11::Camera& camera, /*const Cube& cube, */float x, float y, float z);

        Application& m_application;

        modx::Axes m_axes;
        std::shared_ptr<direct3d11::Model> m_pModel;

        size_t m_nColors;
        size_t m_nColor;
        DirectX::XMFLOAT4 m_color;

        size_t m_nSide;
        bool m_bCycle;
        bool m_bSmooth;
        float m_tTransition; // Period
        float m_dt; // Partial period

        // Statistics
        size_t m_nVertices;
        size_t m_nTriangles;
    };

} // namespace quetzal::cubes

#endif // QUETZAL_CUBES_SCENE_HPP
