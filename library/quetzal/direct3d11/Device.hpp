#if !defined(QUETZAL_DIRECT3D11_DEVICE_HPP)
#define QUETZAL_DIRECT3D11_DEVICE_HPP
//------------------------------------------------------------------------------
// Direct3D 11
// Device.hpp
//------------------------------------------------------------------------------

#include <d3d11_1.h>
#include "Window.hpp"
#include "quetzal/com_ptr.hpp"

namespace quetzal
{

namespace direct3d11
{

    //--------------------------------------------------------------------------
    class Device
    {
    public:

        Device(Window& window, bool bEnable4xmsaa = true);
        Device(const Device&) = delete;
        ~Device();

        Device& operator=(const Device&) = delete;

        ID3D11Device* device();
        ID3D11DeviceContext* context();
        ID3D11RenderTargetView* render_target();
        ID3D11DepthStencilView* depth_stencil();

        HRESULT resize(); // merge with/rename set_fullscreen? ...

        HRESULT set_fullscreen(bool b);
        HRESULT set_4xmsaa(bool b);

        HRESULT clear(); // should take color default parameter ...
        HRESULT present();

    protected:

        HRESULT init(); // device, context
        void release();
        HRESULT reinit();

        HRESULT init_swap_chain(); // swap chain
        void release_swap_chain();
        HRESULT reinit_swap_chain();

        HRESULT init_views(); // render target view, depth stencil view, viewport
        void release_views();

        Window& m_window;

        com_ptr<ID3D11Device> m_pDevice;
        com_ptr<ID3D11DeviceContext> m_pContext;
        com_ptr<IDXGISwapChain> m_pSwapChain;

        com_ptr<ID3D11RenderTargetView> m_pRenderTargetView;
        com_ptr<ID3D11DepthStencilView> m_pDepthStencilView;

        D3D11_VIEWPORT m_viewport;

        bool m_bEnable4xmsaa;
        UINT m_quality4xmsaa;
    };

} // namespace direct3d11

} // namespace quetzal

#endif // QUETZAL_DIRECT3D11_DEVICE_HPP
