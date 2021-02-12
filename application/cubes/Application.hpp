#if !defined(QUETZAL_CUBES_APPLICATION_HPP)
#define QUETZAL_CUBES_APPLICATION_HPP
//------------------------------------------------------------------------------
// Cubes
// Copyright (c) 2013 Keith Michael Looney. All Rights Reserved.
// Application.hpp
//------------------------------------------------------------------------------

#include "quetzal/direct3d11/Application.hpp"
#include "quetzal/direct3d11/Camera.hpp"
#include "quetzal/direct3d11/Statistics.hpp"
#include "Scene.hpp"

namespace quetzal::cubes
{

    //--------------------------------------------------------------------------
    class Application : public quetzal::direct3d11::Application
    {
    public:

        Application();
        Application(const Application&) = delete;
        ~Application() = default;

        Application& operator=(const Application&) = delete;

        com_ptr<ID3D11Debug> device_debug();

    protected:

        virtual void update(float dt);
        virtual void render();

        virtual void on_size(size_type size);
        virtual void on_key_down(Timestamp ts, quetzal::input::key::Modifiers modifiers, quetzal::input::key::Code code);
        virtual void on_mouse_down(Timestamp ts, quetzal::input::key::Modifiers modifiers, position_type position, quetzal::input::key::Code code);
        virtual void on_mouse_up(Timestamp ts, quetzal::input::key::Modifiers modifiers, position_type position, quetzal::input::key::Code code);
        virtual void on_mouse_move(Timestamp ts, quetzal::input::key::Modifiers modifiers, position_type position);

    private:

        void check_input(float dt);

        void init_projection();

        Scene m_scene;

        direct3d11::Camera m_camera; // main camera, up vector, part of direct3d11::Application ...
        DirectX::XMVECTOR m_up; // World up vector
        bool m_bOrthographic;
        float m_scale; // for orthographic projection ...
        float m_speed; // In camera forward direction
        float m_mouseSensitivity; // radians per mouse tick, separate x and y ...

        Timestamp m_tFrame;
        Timestamp m_t;

        quetzal::direct3d11::Statistics m_statistics;
    };

} // namespace quetzal::cubes

#endif // QUETZAL_CUBES_APPLICATION_HPP
