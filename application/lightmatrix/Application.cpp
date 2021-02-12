//------------------------------------------------------------------------------
// Light Matrix Simulation
// Copyright (c) 2010 Keith Michael Looney. All Rights Reserved.
// Application.cpp
//------------------------------------------------------------------------------

#include "Application.hpp"
#include "quetzal/brep/Mesh.hpp"
#include "quetzal/brep/MeshTraits.hpp"
#include "quetzal/direct3d11/Vertex.hpp"
#include "quetzal/direct3d11/file_io.hpp"
#include "quetzal/model/mesh_extraction.hpp"
#include "quetzal/model/obj_io.hpp"
#include "quetzal/model/primitives.hpp"
#include "quetzal/common/ComException.hpp"
#include <DirectXColors.h>
#include <cassert>

using namespace std;
using namespace DirectX;

namespace
{

    const std::string s_pattern_path = "resources/patterns/";

    const vector<string> s_patterns =
    {
        "pattern0.xml",
        "pattern1.xml",
        "pattern2.xml",
        "pattern3.xml",
        "pattern4.xml",
        "pattern5.xml",
        "pattern6.xml",
        "pattern7.xml",
        "pattern0.xml",
        "pattern0.xml"
    };

    string s_vertex_shader_filename("vertex_shader.cso");
    string s_pixel_shader_filename("pixel_shader.cso");

    struct ConstantData
    {
        XMMATRIX m_m4World;
        XMMATRIX m_m4View;
        XMMATRIX m_m4Projection;
        XMFLOAT4 m_colorEmissive;
    };

    ConstantData s_constant_data {};


    using mesh_type = quetzal::brep::Mesh<quetzal::brep::MeshTraits<float>>;

    quetzal::direct3d11::Vector3 to_xm(const typename mesh_type::vector_type& v)
    {
        return {v.x(), v.y(), v.z()};
    }

    quetzal::direct3d11::Vector2 to_xm(const typename mesh_type::texcoord_type& v)
    {
        return {v.x(), v.y()};
    }

    // since this is passed, either needs unique name for each or parameter types specified another way ...
    quetzal::direct3d11::VertexPositionNormalTexture to_vertex_type(const mesh_type::vertex_value_type& value)
    {
        return {to_xm(value.position()), to_xm(value.normal()), to_xm(value.texcoord())};
    }

} // namespace

//------------------------------------------------------------------------------
quetzal::lightmatrix::Application::Application() :
    direct3d11::Application(),
    m_pVertexShader(),
    m_pPixelShader(),
    m_pInputLayout(),
    m_pBufferVertex(),
    m_pBufferIndex(),
    m_pBufferConstant(),
    m_frame(),
    m_scene(m_renderer, m_frame),
    m_controller(m_frame),
    m_nIndices(0),
    m_camera(),
    m_up(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)),
    m_speed(100.0f),
    m_tDissolve(3.0f),
    m_statistics()
{
    m_window.create_floating("Light Matrix", {100, 100}, {800, 800});
    m_renderer.create();

    init_geometry();

    set_pattern(0);

    m_camera.look(XMVectorSet(0.0f, 0.0f, -500.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), m_up),
    update_projection();

    m_window.show();
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Application::update(float dt)
{
    check_input(dt);

    m_camera.update_view();

    m_controller.update(dt);

// based on scene ...
    m_statistics.set_vertices(m_scene.vertex_count());
    m_statistics.set_triangles(m_scene.face_count());
    m_statistics.update(dt);

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Application::render()
{
//    m_scene.draw(m_camera);
    draw();
    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Application::on_size(framework::Window::size_type size)
{
    size;
    update_projection();
    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Application::on_key_down(Timestamp ts, input::key::Modifiers modifiers, input::key::Code code)
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

        // Layout

        case input::key::Code::V_Q:
            m_controller.set_radial(false);
            break;

        case input::key::Code::V_R:
            m_controller.set_radial(true);
            break;

        // Initial pattern

        case input::key::Code::V_1:
        case input::key::Code::V_2:
        case input::key::Code::V_3:
        case input::key::Code::V_4:
        case input::key::Code::V_5:
        case input::key::Code::V_6:
        case input::key::Code::V_7:
        case input::key::Code::V_8:
        case input::key::Code::V_9:
        case input::key::Code::V_0:
            set_pattern(static_cast<int>(code) - static_cast<int>(input::key::Code::V_1));
            break;

        // Brightness/color

        case input::key::Code::V_C:
            m_controller.toggle_shift_color();
            break;

        case input::key::Code::V_B:
            m_controller.toggle_shift_brightness();
            break;

        case input::key::Code::V_M:
            m_controller.cycle_smoothing();
            break;

        // Amination

        case input::key::Code::V_NUMPAD1:
            m_controller.set_direction(-1., -1.);
            break;

        case input::key::Code::V_NUMPAD2:
            m_controller.set_direction(0., -1.);
            break;

        case input::key::Code::V_NUMPAD3:
            m_controller.set_direction(1., -1.);
            break;

        case input::key::Code::V_NUMPAD4:
            m_controller.set_direction(-1., 0.);
            break;

        case input::key::Code::V_NUMPAD5:
            m_controller.set_direction(0., 0.);
            break;

        case input::key::Code::V_NUMPAD6:
            m_controller.set_direction(1., 0.);
            break;

        case input::key::Code::V_NUMPAD7:
            m_controller.set_direction(-1., 1.);
            break;

        case input::key::Code::V_NUMPAD8:
            m_controller.set_direction(0., 1.);
            break;

        case input::key::Code::V_NUMPAD9:
            m_controller.set_direction(1., 1.);
            break;

        // Rate

        case input::key::Code::V_LEFT:
            m_controller.increment_velocity(-0.1f, 0.0f);
            break;

        case input::key::Code::V_RIGHT:
            m_controller.increment_velocity(0.1f, 0.0f);
            break;

        case input::key::Code::V_DOWN:
            m_controller.increment_velocity(0.0f, -0.1f);
            break;

        case input::key::Code::V_UP:
            m_controller.increment_velocity(0.0f, 0.1f);
            break;
    }

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Application::on_mouse_move(Timestamp ts, input::key::Modifiers modifiers, framework::Window::position_type position)
{
    ts;
    modifiers;

    // Camera movement

    float roty = position.x / 1000.0f;
    float rotx = position.y / 1000.0f;
    if (roty == 0.0f && rotx == 0.0f)
    {
        return;
    }

// mouse_move disabled ...
//m_ofs << "ri mouse " << roty << ", " << rotx << endl;
//    XMMATRIX rotation = XMMatrixRotationY(roty) * XMMatrixRotationX(rotx);
//    m_forward = XMVector3Transform(m_forward, rotation);
//    m_forward = XMVector3Normalize(m_forward); // ? ...

    float rx = XMConvertToRadians(0.25f * static_cast<float>(position.x));
    float ry = XMConvertToRadians(0.25f * static_cast<float>(position.y));

    // Camera movement
    m_camera.pan(rx);
//    m_camera.pan(rx, m_up);
    m_camera.tilt(ry);
    m_camera.update_view();

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Application::init_geometry()
{
//    m_scene.load();

    HRESULT hr = S_OK;

    mesh_type mesh;
    model::create_icosahedron(mesh, "icosahedron", 1.0f);

    using vertex_type = direct3d11::VertexPositionNormalTexture;
    using index_type = uint16_t;

//    auto [vertices, indices] = model::extract_input<vertex_type, index_type>(mesh, to_vertex_type);
    auto vertices = model::extract_vertices<vertex_type>(mesh, to_vertex_type);
    auto indices = model::extract_indices<index_type>(mesh);

    m_nIndices = indices.size();

    // Load and create the vertex shader
    vector<char> shader_data = direct3d11::read(s_vertex_shader_filename);
    HR_CHECK(m_renderer.device()->CreateVertexShader(shader_data.data(), shader_data.size(), nullptr, &m_pVertexShader.p()));

    // Create the vertex input layout
    direct3d11::ShaderBytecode shader_bytecode(shader_data.data(), shader_data.size());
    HR_CHECK(direct3d11::create_input_layout<vertex_type>(m_renderer, shader_bytecode, m_pInputLayout));

    // Load and create the pixel shader
    shader_data = direct3d11::read(s_pixel_shader_filename);
    HR_CHECK(m_renderer.device()->CreatePixelShader(shader_data.data(), shader_data.size(), nullptr, &m_pPixelShader.p()));

    // Create vertex buffer
    D3D11_BUFFER_DESC vertex_buffer_desc {static_cast<UINT>(sizeof (vertex_type) * vertices.size()), D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0};
    D3D11_SUBRESOURCE_DATA vertex_data {vertices.data()};
    HR_CHECK(m_renderer.device()->CreateBuffer(&vertex_buffer_desc, &vertex_data, &m_pBufferVertex.p()));

    // Create index buffer
    D3D11_BUFFER_DESC index_buffer_desc {static_cast<UINT>(sizeof (index_type) * indices.size()), D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0, 0, 0};
    D3D11_SUBRESOURCE_DATA index_data {indices.data()};
    HR_CHECK(m_renderer.device()->CreateBuffer(&index_buffer_desc, &index_data, &m_pBufferIndex.p()));

    // Create constant buffer
    D3D11_BUFFER_DESC constant_buffer_desc {sizeof s_constant_data, D3D11_USAGE_DEFAULT, D3D11_BIND_CONSTANT_BUFFER, 0, 0, 0};
    HR_CHECK(m_renderer.device()->CreateBuffer(&constant_buffer_desc, nullptr, &m_pBufferConstant.p()));

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Application::update_projection()
{
    m_camera.set_lens(.25f * XM_PI, static_cast<float>(m_window.client_size().aspect_ratio()), 1.0f, 5000.0f);
    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Application::check_input(float dt)
{
    // Rate

    if (input::query_key_down(input::key::Code::V_OEM_PLUS))
    {
        m_controller.increment_velocity(0.1f, 0.1f);
    }

    if (input::query_key_down(input::key::Code::V_OEM_MINUS))
    {
        m_controller.increment_velocity(-0.1f, -0.1f);
    }

    // Movement

    if (input::query_key_down(input::key::Code::V_W))
    {
        m_camera.set_position(XMVectorAdd(m_camera.position(), XMVectorScale(m_camera.forward(), m_speed * dt)));
    }

    if (input::query_key_down(input::key::Code::V_S))
    {
        m_camera.set_position(XMVectorAdd(m_camera.position(), XMVectorScale(m_camera.forward(), -m_speed * dt)));
    }

/*
    if (input::query_key_down(input::key::Code::V_A))
    {
        m_camera.set_position(XMVectorAdd(m_camera.position(), XMVectorScale(m_camera.right(), -dt * m_speed)));
    }

    if (input::query_key_down(input::key::Code::V_D))
    {
        m_camera.set_position(XMVectorAdd(m_camera.position(), XMVectorScale(m_camera.right(), dt * m_speed)));
    }
*/

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Application::set_pattern(int nPattern)
{
    if (nPattern < 0 || nPattern >= static_cast<int>(s_patterns.size()))
    {
        return;
    }

    string path = s_patterns[nPattern];

    if (!s_pattern_path.empty())
    {
        path = s_pattern_path + path;
    }

    m_controller.load_pattern(path, m_tDissolve);

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Application::draw()
{
    ID3D11DeviceContext* pContext = m_renderer.context();
    assert(pContext);

    m_renderer.clear();

    pContext->IASetInputLayout(m_pInputLayout);
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    UINT stride = static_cast<UINT>(direct3d11::VertexPositionNormalTexture::stride());
    UINT offset = 0;
    ID3D11Buffer* pBuffers[] = {m_pBufferVertex.get()};
    pContext->IASetVertexBuffers(0, 1, pBuffers, &stride, &offset);
    pContext->IASetIndexBuffer(m_pBufferIndex, DXGI_FORMAT_R16_UINT, 0);

    const Frame& frame = m_controller.frame();
    const int w = frame.width();
    const int h = frame.height();

    for (int j = 0; j < h; ++j)
    {
        for (int i = 0; i < w; ++i)
        {
            draw_pixel(frame.pixel(i, j));
        }
    }

//    m_statistics.text(); ...

    m_renderer.present();

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Application::draw_pixel(const Pixel& pixel)
{
    ID3D11DeviceContext* pContext = m_renderer.context();

    // Set per object constants

    float r = pixel.radius();
    float x = pixel.x();
    float y = pixel.y();
    Color color = pixel.color();

    XMMATRIX m4World = XMMatrixScaling(r, r, r) * XMMatrixTranslation(x, y, 0.0f);
    s_constant_data.m_m4World = XMMatrixTranspose(m4World);
    s_constant_data.m_m4View = XMMatrixTranspose(m_camera.view());
    s_constant_data.m_m4Projection = XMMatrixTranspose(m_camera.projection());
    s_constant_data.m_colorEmissive = XMFLOAT4(color.r(), color.g(), color.b(), color.a());
    pContext->UpdateSubresource(m_pBufferConstant, 0, nullptr, &s_constant_data, 0, 0);

    // Vertex shader
    pContext->VSSetShader(m_pVertexShader, nullptr, 0);
    ID3D11Buffer* pBuffers[] = {m_pBufferConstant.get()};
    pContext->VSSetConstantBuffers(0, 1, pBuffers);

    // Pixel shader
    pContext->PSSetShader(m_pPixelShader, nullptr, 0);
    pContext->PSSetConstantBuffers(0, 1, pBuffers);

    pContext->DrawIndexed(static_cast<UINT>(m_nIndices), 0, 0);
    return;
}
