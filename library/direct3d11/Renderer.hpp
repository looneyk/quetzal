#if !defined(QUETZAL_DIRECT3D11_RENDERER_HPP)
#define QUETZAL_DIRECT3D11_RENDERER_HPP
//------------------------------------------------------------------------------
// Direct3D 11
// Renderer.hpp
//------------------------------------------------------------------------------

#include "common/ComException.hpp"
#include "common/com_ptr.hpp"
#include "framework/Window.hpp"
#include <DirectXColors.h>
#include <d3d11_1.h>

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    class Renderer
    {
    public:

        Renderer(framework::Window& window);
        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        ~Renderer();

        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) = delete;

        void create();

        ID3D11Device* device();
        ID3D11DeviceContext* context();
        ID3D11RenderTargetView* render_target();
        ID3D11DepthStencilView* depth_stencil();

        // default effect? ...

        HRESULT resize(); // merge with/rename set_fullscreen? ...

        HRESULT set_fullscreen(bool b);
        HRESULT set_4xmsaa(bool b);

        HRESULT clear(const FLOAT colorRGBA[4] = DirectX::Colors::Black);
        HRESULT present();

        com_ptr<ID3D11Debug> device_debug();

    protected:

        // device, context
        HRESULT init();
        void release();
        HRESULT reinit();

        // swap chain
        HRESULT init_swap_chain();
        void release_swap_chain();
        HRESULT reinit_swap_chain();

        // render target view, depth stencil view, viewport
        HRESULT init_views();
        void release_views();

        framework::Window& m_window;

        com_ptr<ID3D11Device> m_pDevice;
        com_ptr<ID3D11DeviceContext> m_pContext;
        com_ptr<IDXGISwapChain> m_pSwapChain;

        com_ptr<ID3D11RenderTargetView> m_pRenderTargetView;
        com_ptr<ID3D11DepthStencilView> m_pDepthStencilView;

        D3D11_VIEWPORT m_viewport;
        D3D11_RECT m_scissor_rect;

        bool m_bEnable4xmsaa;
        UINT m_quality4xmsaa;
    };

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_RENDERER_HPP
