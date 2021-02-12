#if !defined(QUETZAL_LIGHTMATRIX_APPLICATION_HPP)
#define QUETZAL_LIGHTMATRIX_APPLICATION_HPP
//------------------------------------------------------------------------------
// Light Matrix Simulation
// Copyright (c) 2010 Keith Michael Looney. All Rights Reserved.
// Application.hpp
//------------------------------------------------------------------------------

#include "quetzal/direct3d11/Application.hpp"
#include "quetzal/direct3d11/Camera.hpp"
#include "quetzal/direct3d11/Statistics.hpp"
#include "Controller.hpp"
#include "Scene.hpp"
#include <DirectXMath.h>

namespace quetzal
{

namespace lightmatrix
{

    //----------------------------------------------------------------------
    class Application : public quetzal::direct3d11::Application
    {
    public:

        Application();
        Application(const Application&) = delete;
        ~Application() = default;

        Application& operator=(const Application&) = delete;

    protected:

        virtual void update(float dt);
        virtual void render();

        virtual void on_size(quetzal::framework::Window::size_type size);
        virtual void on_key_down(Timestamp ts, quetzal::input::key::Modifiers modifiers, quetzal::input::key::Code code);
        virtual void on_mouse_move(Timestamp ts, quetzal::input::key::Modifiers modifiers, quetzal::framework::Window::position_type position);

    private:

        void init_geometry();
        void update_projection();

        void check_input(float dt);

        void set_pattern(int nPattern);

        void draw();
        void draw_pixel(const Pixel& pixel);

        Frame m_frame;
        Scene m_scene;
        Controller m_controller;

        direct3d11::Camera m_camera;
        DirectX::XMVECTOR m_up; // World up vector
        float m_speed;
        float m_tDissolve;
		
        quetzal::direct3d11::Statistics m_statistics;


        com_ptr<ID3D11VertexShader> m_pVertexShader;
        com_ptr<ID3D11PixelShader> m_pPixelShader;

        com_ptr<ID3D11InputLayout> m_pInputLayout;

        com_ptr<ID3D11Buffer> m_pBufferVertex;
        com_ptr<ID3D11Buffer> m_pBufferIndex;
        com_ptr<ID3D11Buffer> m_pBufferConstant;

        size_t m_nIndices; // from scene model, will be hidden ...
    };

} // namespace lightmatrix

} // namespace quetzal

#endif // QUETZAL_LIGHTMATRIX_APPLICATION_HPP
