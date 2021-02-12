//------------------------------------------------------------------------------
// Direct3D 11
// Device.cpp
//------------------------------------------------------------------------------

#include "Device.hpp"
#include "Exception.hpp"
#include <vector>
#include <DirectXColors.h>

using namespace std;

//------------------------------------------------------------------------------
quetzal::direct3d11::Device::Device(Window& window, bool bEnable4xmsaa) :
    m_window(window),
    m_pDevice(),
    m_pContext(),
    m_pSwapChain(),
    m_pRenderTargetView(),
    m_pDepthStencilView(),
    m_viewport{},
    m_bEnable4xmsaa(bEnable4xmsaa),
    m_quality4xmsaa(0)
{
    COM_CHECK(init());
}

//------------------------------------------------------------------------------
quetzal::direct3d11::Device::~Device()
{
    if (m_pContext)
    {
        m_pContext->ClearState();
    }
}

//------------------------------------------------------------------------------
ID3D11Device* quetzal::direct3d11::Device::device()
{
    return m_pDevice;
}

//------------------------------------------------------------------------------
ID3D11DeviceContext* quetzal::direct3d11::Device::context()
{
    return m_pContext;
}

//------------------------------------------------------------------------------
ID3D11RenderTargetView* quetzal::direct3d11::Device::render_target()
{
    return m_pRenderTargetView;
}

//------------------------------------------------------------------------------
ID3D11DepthStencilView* quetzal::direct3d11::Device::depth_stencil()
{
    return m_pDepthStencilView;
}

//------------------------------------------------------------------------------
HRESULT quetzal::direct3d11::Device::resize()
{
    HRESULT hr = S_OK;

    release_views();

    assert(m_pSwapChain);
    hr = m_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0); // Retain settings and get size from window client area
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        HR_CHECK(reinit());
        return hr;
    }

    HR_CHECK(init_views());

    return hr;
}

//------------------------------------------------------------------------------
HRESULT quetzal::direct3d11::Device::set_fullscreen(bool b)
{
    HRESULT hr = S_OK;

    assert(m_pSwapChain);
    HR_CHECK(m_pSwapChain->SetFullscreenState(b, nullptr));
    HR_CHECK(resize());

    return hr;
}

//------------------------------------------------------------------------------
HRESULT quetzal::direct3d11::Device::set_4xmsaa(bool b)
{
    HRESULT hr = S_OK;

    m_bEnable4xmsaa = b;
    HR_CHECK(resize()); // is resize sufficient? ...

    return hr;
}

//------------------------------------------------------------------------------
HRESULT quetzal::direct3d11::Device::clear()
{
    assert(m_pContext);
    assert(m_pRenderTargetView);
    assert(m_pDepthStencilView);

    HRESULT hr = S_OK;

    m_pContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::Black);
    m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

// why does this neeed to be done every frame? ...
ID3D11RenderTargetView* pRenderTargetViews[] = {m_pRenderTargetView};
m_pContext->OMSetRenderTargets(1, pRenderTargetViews, m_pDepthStencilView);

    return hr;
}

//------------------------------------------------------------------------------
HRESULT quetzal::direct3d11::Device::present()
{
    HRESULT hr = S_OK;

    assert(m_pSwapChain);
    hr = m_pSwapChain->Present(1, 0);
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        HR_CHECK(reinit());
    }

    return hr;
}

//------------------------------------------------------------------------------
HRESULT quetzal::direct3d11::Device::init()
{
    assert(!m_pDevice);
    assert(!m_pContext);
    assert(!m_pSwapChain);
    assert(!m_pRenderTargetView);
    assert(!m_pDepthStencilView);

    HRESULT hr = S_OK;

    UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT; // Support surfaces with color-channel ordering different from API default; required for compatibility with Direct2D
#if defined(DEBUG) || defined(_DEBUG)
    // Check for correct debug layer
    if (SUCCEEDED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_NULL, 0, D3D11_CREATE_DEVICE_DEBUG, nullptr, 0, D3D11_SDK_VERSION, nullptr, nullptr, nullptr)))
    {
        flags |= D3D11_CREATE_DEVICE_DEBUG;
    }
#endif

    // except for 11.1, could just use default level set ...
    vector<D3D_FEATURE_LEVEL> levelsRequested {D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1};

    D3D_FEATURE_LEVEL levelAcquired;
    hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, levelsRequested.data(), static_cast<UINT>(levelsRequested.size()), D3D11_SDK_VERSION, &m_pDevice.p(), &levelAcquired, &m_pContext.p());
    if (hr == E_INVALIDARG) // Retry without 11.1
    {
        HR_CHECK(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, flags, &levelsRequested.data()[1], static_cast<UINT>(levelsRequested.size() - 1), D3D11_SDK_VERSION, &m_pDevice.p(), &levelAcquired, &m_pContext.p()));
    }

    if (levelAcquired != D3D_FEATURE_LEVEL_11_1 && levelAcquired != D3D_FEATURE_LEVEL_11_0)
    {
        return E_FAIL;
    }

    assert(m_pDevice);
    assert(m_pContext);

    HR_CHECK(m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_quality4xmsaa));
    HR_CHECK(init_swap_chain());

    return hr;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Device::release()
{
    release_views();
    release_swap_chain();

    m_pContext = nullptr;
    m_pDevice = nullptr;
    return;
}

//------------------------------------------------------------------------------
HRESULT quetzal::direct3d11::Device::reinit()
{
    HRESULT hr = S_OK;

    release();
    HR_CHECK(init());

    return hr;
}

//------------------------------------------------------------------------------
HRESULT quetzal::direct3d11::Device::init_swap_chain()
{
    assert(m_pDevice);
    assert(m_pContext);
    assert(!m_pSwapChain);
    assert(!m_pRenderTargetView);
    assert(!m_pDepthStencilView);

    HRESULT hr = S_OK;

    DXGI_SWAP_CHAIN_DESC scd {};
//    auto size = m_window.client_size();
//    scd.BufferDesc.Width = size.x;
//    scd.BufferDesc.Height = size.y;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // DXGI_FORMAT_B8G8R8A8_UNORM most common? ...
    if (m_bEnable4xmsaa && m_quality4xmsaa > 0)
    {
        scd.SampleDesc.Count = 4;
        scd.SampleDesc.Quality = m_quality4xmsaa - 1;
    }
    else
    {
        scd.SampleDesc.Count = 1;
        scd.SampleDesc.Quality = 0;
    }
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.BufferCount = 2;
    scd.OutputWindow = m_window.hwnd();
    scd.Windowed = m_window.mode() != Window::Mode::Fullscreen;
    scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    scd.Flags = 0;

    // Create the DXGI device object to use in other factories, such as Direct2D
    com_ptr<IDXGIDevice> pDXGIDevice;
    HR_CHECK(m_pDevice.QueryInterface(pDXGIDevice));

    // Create swap chain
    com_ptr<IDXGIAdapter> pAdapter;
    HR_CHECK(pDXGIDevice->GetAdapter(&pAdapter.p()));

    com_ptr<IDXGIFactory> pFactory;
    HR_CHECK(pAdapter->GetParent(IID_PPV_ARGS(&pFactory.p())));

    HR_CHECK(pFactory->CreateSwapChain(m_pDevice, &scd, &m_pSwapChain.p()));

    // Configure buffers, views, and viewport
    HR_CHECK(init_views());

    return hr;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Device::release_swap_chain()
{
    release_views();
    m_pSwapChain = nullptr;
    return;
}

//------------------------------------------------------------------------------
HRESULT quetzal::direct3d11::Device::reinit_swap_chain()
{
    HRESULT hr = S_OK;

    release_swap_chain();
    HR_CHECK(init_swap_chain());

    return hr;
}

//------------------------------------------------------------------------------
HRESULT quetzal::direct3d11::Device::init_views()
{
    assert(m_pDevice);
    assert(m_pContext);
    assert(m_pSwapChain);
    assert(!m_pRenderTargetView);
    assert(!m_pDepthStencilView);

    HRESULT hr = S_OK;

    // Back buffer
    com_ptr<ID3D11Texture2D> pBackBuffer;
    HR_CHECK(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), pBackBuffer.ppv()));

    // Render target view
    HR_CHECK(m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView.p()));

    assert(m_pRenderTargetView);

    D3D11_TEXTURE2D_DESC backBufferDesc {};
    pBackBuffer->GetDesc(&backBufferDesc);

    // Depth stencil buffer
    CD3D11_TEXTURE2D_DESC depthStencilDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, backBufferDesc.Width, backBufferDesc.Height, 1, 1, D3D11_BIND_DEPTH_STENCIL);
    if (m_bEnable4xmsaa) // Match swap chain
    {
        depthStencilDesc.SampleDesc.Count = 4;
        depthStencilDesc.SampleDesc.Quality = m_quality4xmsaa - 1;
    }
    com_ptr<ID3D11Texture2D> pDepthStencil;
    HR_CHECK(m_pDevice->CreateTexture2D(&depthStencilDesc, nullptr, &pDepthStencil.p()));

    // Depth stencil view
    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D); // may be null? ...
    HR_CHECK(m_pDevice->CreateDepthStencilView(pDepthStencil, &depthStencilViewDesc, &m_pDepthStencilView.p()));

    assert(m_pDepthStencilView);

// rasterizer state ...
//    m_pContext->RSSetState(...);

    // Viewport to cover the entire area
    m_viewport = {};
    m_viewport.TopLeftX = 0.f;
    m_viewport.TopLeftY = 0.f;
    m_viewport.Width = static_cast<float>(backBufferDesc.Width);
    m_viewport.Height = static_cast<float>(backBufferDesc.Height);
    m_viewport.MinDepth = D3D11_MIN_DEPTH;
    m_viewport.MaxDepth = D3D11_MAX_DEPTH;
    m_pContext->RSSetViewports(1, &m_viewport);

// why is this not sufficient here? ...
    // Bind the render target view and depth/stencil view to the output merger pipeline stage
    ID3D11RenderTargetView* pRenderTargetViews[] = {m_pRenderTargetView};
    m_pContext->OMSetRenderTargets(1, pRenderTargetViews, m_pDepthStencilView);
//m_pContext->OMSetDepthStencilState(...) // ...

    return hr;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Device::release_views()
{
    assert(m_pContext);
    vector<ID3D11RenderTargetView*> null_views {nullptr};
    m_pContext->OMSetRenderTargets(static_cast<UINT>(null_views.size()), null_views.data(), nullptr);

    m_pRenderTargetView = nullptr;
    m_pDepthStencilView = nullptr;

    m_pContext->Flush();
    return;
}
