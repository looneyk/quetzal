#if !defined(QUETZAL_MODX_AXES_HPP)
#define QUETZAL_MODX_AXES_HPP
//------------------------------------------------------------------------------
// modx
// Axes.hpp
//------------------------------------------------------------------------------

#include "quetzal/direct3d11/Application.hpp"
#include "quetzal/direct3d11/Camera.hpp"
#include "quetzal/direct3d11/Model.hpp"
#include "quetzal/direct3d11/Vector3.hpp"

namespace quetzal::modx
{

    //--------------------------------------------------------------------------
    class Axes
    {
    public:

        Axes(direct3d11::Application& application);
        Axes(const Axes&) = delete;
        Axes(Axes&&) = delete;
        ~Axes() = default;

        Axes& operator=(const Axes&) = delete;
        Axes& operator=(Axes&&) = delete;

        void load();
        void draw(direct3d11::Camera& camera, float scale = 1.0f, direct3d11::Vector3 position = {0.0f, 0.0f, 0.0f});

        size_t vertex_count() const;
        size_t triangle_count() const;

    private:

        void draw_origin(direct3d11::Camera& camera, direct3d11::Vector3 position, float scale, std::shared_ptr<direct3d11::IEffect> pEffect);
        void draw_axis(direct3d11::Camera& camera, direct3d11::Vector3 position, direct3d11::Vector3 head, float length, direct3d11::Vector3 rotation, float scale, std::shared_ptr<direct3d11::IEffect> pEffect);

        direct3d11::Application& m_application;

        std::shared_ptr<direct3d11::Model> m_pModelOrigin;
        std::shared_ptr<direct3d11::Model> m_pModelShaft;
        std::shared_ptr<direct3d11::Model> m_pModelHead;
    };

} // namespace quetzal::modx

#endif // QUETZAL_MODX_AXES_HPP
