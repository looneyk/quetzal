//------------------------------------------------------------------------------
// Cubes
// Application.cpp
//------------------------------------------------------------------------------

#include "Application.hpp"
#include <cassert>

using namespace std;
using namespace DirectX;

//------------------------------------------------------------------------------
quetzal::cubes::Application::Application() :
    direct3d11::Application(),
    m_scene(*this),
    m_camera(),
    m_up(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)),
    m_bOrthographic(false),
    m_scale(0.03f),
    m_speed(6.0f),
    m_mouseSensitivity(0.001f),
    m_tFrame(),
    m_t(),
    m_statistics()
{
    m_window.create_floating("Cubes", {100, 100}, {800, 600});
    m_renderer.create();
    m_model_manager.set_path("models");

    m_scene.load();

    // camera should be set from scene ...
    m_camera.set_position(XMVectorSet(12.0f, 12.0f, 10.0f, 0.0f)),
    m_camera.look(XMVectorSet(0.0f, 0.0f, 3.5f, 0.0f), m_up),
    init_projection();

    m_window.show();
}

//------------------------------------------------------------------------------
quetzal::com_ptr<ID3D11Debug> quetzal::cubes::Application::device_debug()
{
    return direct3d11::Application::device_debug();
}

//------------------------------------------------------------------------------
void quetzal::cubes::Application::update(float dt)
{
    check_input(dt);
    m_camera.update_view();
    m_scene.update(dt);
    m_statistics.update(dt);
    return;
}

//------------------------------------------------------------------------------
void quetzal::cubes::Application::render()
{
    m_renderer.clear();
    m_scene.draw(m_camera);

    m_statistics.set_vertices(m_scene.vertex_count());
    m_statistics.set_triangles(m_scene.triangle_count());
//    m_statistics.text(); ...

    m_renderer.present();
    return;
}

//------------------------------------------------------------------------------
void quetzal::cubes::Application::on_size(size_type size)
{
    size;
    init_projection();
    return;
}

//------------------------------------------------------------------------------
void quetzal::cubes::Application::on_key_down(Timestamp ts, input::key::Modifiers modifiers, input::key::Code code)
{
    ts;
    modifiers;

    switch (code)
    {
        case input::key::Code::V_ESCAPE:
            m_window.destroy();
            break;

/*
        case input::key::Code::V_F:
            if (fullscreen())
            {
                set_windowed();
            }
            else
            {
                set_fullscreen();
            }
            break;

        case input::key::Code::V_I:
            if (solid())
            {
                set_wireframe();
            }
            else
            {
                set_solid();
            }
            break;
*/

        case input::key::Code::V_C:
            m_scene.set_cycle(!m_scene.cycle());
            break;

        case input::key::Code::V_M:
            m_scene.set_smooth(!m_scene.smooth());
            break;

        case input::key::Code::V_G:
            m_bOrthographic = !m_bOrthographic;
            // separate zoom speed for ortho, limit to scale range ...
            m_scale = 0.03f;
            init_projection();
            break;
    }

    return;
}

//------------------------------------------------------------------------------
void quetzal::cubes::Application::on_mouse_down(Timestamp ts, quetzal::input::key::Modifiers modifiers, position_type position, input::key::Code code)
{
    ts;
    modifiers;
    position;
    code;

//    SetCapture(m_window.hwnd());
    return;
}

//------------------------------------------------------------------------------
void quetzal::cubes::Application::on_mouse_up(Timestamp ts, quetzal::input::key::Modifiers modifiers, position_type position, input::key::Code code)
{
    ts;
    modifiers;
    position;
    code;

//    ReleaseCapture();
    return;
}

//------------------------------------------------------------------------------
void quetzal::cubes::Application::on_mouse_move(Timestamp ts, input::key::Modifiers modifiers, position_type position)
{
    ts;
    modifiers;

    if (position.x == 0.0f && position.y == 0.0f)
    {
        return;
    }

    float rx = m_mouseSensitivity * static_cast<float>(-position.x);
    float ry = m_mouseSensitivity * static_cast<float>(-position.y);

    m_camera.pan(rx);
//    m_camera.pan(rx, m_up);
    m_camera.tilt(ry);
    m_camera.update_view();
    return;
}

//------------------------------------------------------------------------------
void quetzal::cubes::Application::check_input(float dt)
{
    if (input::query_key_down(input::key::Code::V_W))
    {
        if (m_bOrthographic)
        {
            m_camera.set_position(XMVectorAdd(m_camera.position(), XMVectorScale(m_camera.up(), dt * m_speed)));
        }
        else
        {
            m_camera.set_position(XMVectorAdd(m_camera.position(), XMVectorScale(m_camera.forward(), dt * m_speed)));
        }
    }

    if (input::query_key_down(input::key::Code::V_S))
    {
        if (m_bOrthographic)
        {
            m_camera.set_position(XMVectorAdd(m_camera.position(), XMVectorScale(m_camera.up(), -dt * m_speed)));
        }
        else
        {
            m_camera.set_position(XMVectorAdd(m_camera.position(), XMVectorScale(m_camera.forward(), -dt * m_speed)));
        }
    }

    if (input::query_key_down(input::key::Code::V_A))
    {
        if (m_bOrthographic)
        {
            m_camera.set_position(XMVectorAdd(m_camera.position(), XMVectorScale(m_camera.right(), -dt * m_speed)));
        }
        else
        {
            m_camera.set_position(XMVectorAdd(m_camera.position(), XMVectorScale(m_camera.right(), -dt * m_speed)));
        }
    }

    if (input::query_key_down(input::key::Code::V_D))
    {
        if (m_bOrthographic)
        {
            m_camera.set_position(XMVectorAdd(m_camera.position(), XMVectorScale(m_camera.right(), dt * m_speed)));
        }
        else
        {
            m_camera.set_position(XMVectorAdd(m_camera.position(), XMVectorScale(m_camera.right(), dt * m_speed)));
        }
    }

    if (input::query_key_down(input::key::Code::V_Q))
    {
        if (m_bOrthographic)
        {
            // separate zoom speed for ortho, limit to scale range ...
            m_scale -= m_speed / 500.0f * dt;
            init_projection();
        }
    }

    if (input::query_key_down(input::key::Code::V_Z))
    {
        if (m_bOrthographic)
        {
            // separate zoom speed for ortho, limit to scale range ...
            m_scale += m_speed / 500.0f * dt;
            init_projection();
        }
    }

    return;
}

//------------------------------------------------------------------------------
void quetzal::cubes::Application::init_projection()
{
    if (m_bOrthographic)
    {
        m_camera.set_lens_ortho(m_window.client_size().x * m_scale, m_window.client_size().y * m_scale, 1.0f, 1000.0f);
    }
    else
    {
        m_camera.set_lens(static_cast<float>(m_window.client_size().aspect_ratio()), 1.0f, 1000.0f);
    }

    return;
}
