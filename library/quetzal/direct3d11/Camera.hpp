#if !defined(QUETZAL_DIRECT3D11_CAMERA_HPP)
#define QUETZAL_DIRECT3D11_CAMERA_HPP
//------------------------------------------------------------------------------
// Direct3D 11
// Camera.hpp
//------------------------------------------------------------------------------

#include <DirectXMath.h>

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    class Camera
    {
    public:

        Camera(float fov_nominal = DirectX::XM_PI / 3.0f); // fov_nominal is the fov where zoom == 1
        Camera(const Camera&) = delete;
        Camera(Camera&&) = delete;
        ~Camera() = default;

        Camera& operator=(const Camera&) = delete;
        Camera& operator=(Camera&&) = delete;

        DirectX::XMVECTOR position() const;
        DirectX::XMVECTOR right() const;
        DirectX::XMVECTOR up() const;
        DirectX::XMVECTOR forward() const;

        // azimuth and elevation of forward view ...
        float theta(DirectX::XMVECTOR up) const; // elevation angle, p1/2 .. -pi/2 given the world up vector
        float phi() const; // azimuth angle, range? ...

        // focal length? proportional to zoom? ...
        float fov() const;
        void set_fov(float fov);

        float zoom() const; // inverse fov ...
        void set_zoom(float f);

        DirectX::XMMATRIX view() const;
        DirectX::XMMATRIX projection() const;
        DirectX::XMMATRIX view_projection() const;

        // Call update_view when finished calling these
        void set_position(DirectX::XMVECTOR position);
        void set_target(DirectX::XMVECTOR target);
        void set_direction(DirectX::XMVECTOR direction); // Forward direction
        // Add set_ versions of the following for absolute positions relative to camera vectors? ...
        void pan(float radians); // Relative rotation around internal up vector
        void pan(float radians, DirectX::XMVECTOR up); // Relative rotation around explicit up vector
        void tilt(float radians); // Relative rotation around right vector
        void roll(float radians); // Relative rotation around the forward vector

        // Add quaternion camera rotation? ...

        // Update view matrix
        void look(DirectX::XMVECTOR target, DirectX::XMVECTOR up);

        // Update projection matrix
        void set_lens(float fov, float aspect, float fnear, float ffar);
        void set_lens(float aspect, float fnear, float ffar); // If fov is set explicitly or determined by zoom
        void set_lens_ortho(float w, float h, float fnear, float ffar);

        void update_view();

    protected:

        DirectX::XMVECTOR m_position;
        DirectX::XMVECTOR m_up;
        DirectX::XMVECTOR m_right;
        DirectX::XMVECTOR m_forward;

        float m_fov_nominal;
        float m_fov;

        DirectX::XMMATRIX m_view;
        DirectX::XMMATRIX m_projection;
    };

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_CAMERA_HPP
