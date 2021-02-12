#if !defined(QUETZAL_GALLERY_APPLICATION_HPP)
#define QUETZAL_GALLERY_APPLICATION_HPP
//------------------------------------------------------------------------------
// Gallery
// Application.hpp
//------------------------------------------------------------------------------

#include "quetzal/direct3d11/Application.hpp"
#include "quetzal/direct3d11/Camera.hpp"
#include "quetzal/direct3d11/Statistics.hpp"
#include "Scene.hpp"

namespace quetzal::gallery
{

    //--------------------------------------------------------------------------
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
        float m_speed; // In camera forward direction
        float m_mouseSensitivity; // radians per mouse tick, separate x and y ...

        Timestamp m_tFrame;
        Timestamp m_t;

        quetzal::direct3d11::Statistics m_statistics;
    };

} // namespace quetzal::gallery

#endif // QUETZAL_GALLERY_APPLICATION_HPP

/*
    ID3D11Buffer* mShapesVB;
    ID3D11Buffer* mShapesIB;

    ID3D11Buffer* mSkullVB;
    ID3D11Buffer* mSkullIB;

    ID3D11ShaderResourceView* mFloorTexSRV;
    ID3D11ShaderResourceView* mStoneTexSRV;
    ID3D11ShaderResourceView* mBrickTexSRV;

    DirectionalLight mDirLights[3];
    Material mGridMat;
    Material mBoxMat;
    Material mCylinderMat;
    Material mSphereMat;
    Material mSkullMat;

    // Define transformations from local spaces to world space.
    XMFLOAT4X4 mSphereWorld[10];
    XMFLOAT4X4 mCylWorld[10];
    XMFLOAT4X4 mBoxWorld;
    XMFLOAT4X4 mGridWorld;
    XMFLOAT4X4 mSkullWorld;

    int mBoxVertexOffset;
    int mGridVertexOffset;
    int mSphereVertexOffset;
    int mCylinderVertexOffset;

    UINT mBoxIndexOffset;
    UINT mGridIndexOffset;
    UINT mSphereIndexOffset;
    UINT mCylinderIndexOffset;

    UINT mBoxIndexCount;
    UINT mGridIndexCount;
    UINT mSphereIndexCount;
    UINT mCylinderIndexCount;

    UINT mSkullIndexCount;

    UINT mLightCount;
*/
