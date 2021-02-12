//------------------------------------------------------------------------------
// Direct3D 11
// CommonStates.cpp
//------------------------------------------------------------------------------

#include "CommonStates.hpp"
#include "Renderer.hpp"
#include "checked_create.hpp"
#include "debug_util.hpp"

using quetzal::direct3d11::checked_create;

//------------------------------------------------------------------------------
class quetzal::direct3d11::CommonStates::Impl
{
public:

    Impl(quetzal::direct3d11::Renderer& renderer) :
        m_renderer(renderer)
    {
    }

    HRESULT create_blend_state(D3D11_BLEND blendSource, D3D11_BLEND blendDest, ID3D11BlendState** ppBlendState);
    HRESULT create_depth_stencil_state(bool bEnable, bool bWrite, ID3D11DepthStencilState** ppDepthStencilState);
    HRESULT create_rasterizer_state(bool bFrontCounterClockwise, D3D11_CULL_MODE cull_mode, D3D11_FILL_MODE fill_mode, ID3D11RasterizerState** ppRasterizerState);
    HRESULT create_sampler_state(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE texture_address_mode, ID3D11SamplerState** ppSamplerState);

    Renderer& m_renderer;

    quetzal::com_ptr<ID3D11BlendState> m_pBlendStateOpaque;
    quetzal::com_ptr<ID3D11BlendState> m_pBlendStateAlpha;
    quetzal::com_ptr<ID3D11BlendState> m_pBlendStateAdditive;
    quetzal::com_ptr<ID3D11BlendState> m_pBlendStateNonPremultiplied;

    quetzal::com_ptr<ID3D11DepthStencilState> m_pDepthStencilNone;
    quetzal::com_ptr<ID3D11DepthStencilState> m_pDepthStencilDefault;
    quetzal::com_ptr<ID3D11DepthStencilState> m_pDepthStencilRead;

    quetzal::com_ptr<ID3D11RasterizerState> m_pRasterizerCullNone;
    quetzal::com_ptr<ID3D11RasterizerState> m_pRasterizerCullClockwise;
    quetzal::com_ptr<ID3D11RasterizerState> m_pRasterizerCullCounterClockwise;
    quetzal::com_ptr<ID3D11RasterizerState> m_pRasterizerWireframe;

    quetzal::com_ptr<ID3D11SamplerState> m_pSamplerPointWrap;
    quetzal::com_ptr<ID3D11SamplerState> m_pSamplerPointClamp;
    quetzal::com_ptr<ID3D11SamplerState> m_pSamplerLinearWrap;
    quetzal::com_ptr<ID3D11SamplerState> m_pSamplerLinearClamp;
    quetzal::com_ptr<ID3D11SamplerState> m_pSamplerAnisotropicWrap;
    quetzal::com_ptr<ID3D11SamplerState> m_pSamplerAnisotropicClamp;

    std::mutex m_mutex;
};

//------------------------------------------------------------------------------
quetzal::direct3d11::CommonStates::CommonStates(Renderer& renderer) :
    pImpl(new Impl(renderer))
{
}

//------------------------------------------------------------------------------
quetzal::direct3d11::CommonStates::CommonStates(CommonStates&& other) :
    pImpl(std::move(other.pImpl))
{
}

//------------------------------------------------------------------------------
quetzal::direct3d11::CommonStates& quetzal::direct3d11::CommonStates::operator=(CommonStates&& other)
{
    pImpl = std::move(other.pImpl);
    return *this;
}

//------------------------------------------------------------------------------
HRESULT quetzal::direct3d11::CommonStates::Impl::create_blend_state(D3D11_BLEND blendSource, D3D11_BLEND blendDest, ID3D11BlendState** ppBlendState)
{
    D3D11_BLEND_DESC desc = {};

    desc.RenderTarget[0].BlendEnable = (blendSource != D3D11_BLEND_ONE) || (blendDest != D3D11_BLEND_ZERO);
    desc.RenderTarget[0].SrcBlend = blendSource;
    desc.RenderTarget[0].SrcBlendAlpha = blendSource;
    desc.RenderTarget[0].DestBlend = blendDest;
    desc.RenderTarget[0].DestBlendAlpha = blendDest;
    desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    HRESULT hr = m_renderer.device()->CreateBlendState(&desc, ppBlendState);
    if (SUCCEEDED(hr))
    {
        SetDebugObjectName(*ppBlendState, "direct3d11::CommonStates::BlendState");
    }

    return hr;
}

//------------------------------------------------------------------------------
HRESULT quetzal::direct3d11::CommonStates::Impl::create_depth_stencil_state(bool bEnable, bool bWrite, ID3D11DepthStencilState** ppDepthStencilState)
{
    D3D11_DEPTH_STENCIL_DESC desc = {};

    desc.DepthEnable = bEnable ? TRUE : FALSE;
    desc.DepthWriteMask = bWrite ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
    desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    desc.StencilEnable = FALSE;
    desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

    desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

    desc.BackFace = desc.FrontFace;

    HRESULT hr = m_renderer.device()->CreateDepthStencilState(&desc, ppDepthStencilState);
    if (SUCCEEDED(hr))
    {
        SetDebugObjectName(*ppDepthStencilState, "direct3d11::CommonStates::DepthStencilState");
    }

    return hr;
}

//------------------------------------------------------------------------------
HRESULT quetzal::direct3d11::CommonStates::Impl::create_rasterizer_state(bool bFrontCounterClockwise, D3D11_CULL_MODE cull_mode, D3D11_FILL_MODE fill_mode, ID3D11RasterizerState** ppRasterizerState)
{
    D3D11_RASTERIZER_DESC desc = {};

    desc.FrontCounterClockwise = bFrontCounterClockwise;
    desc.CullMode = cull_mode;
    desc.FillMode = fill_mode;
    desc.DepthClipEnable = TRUE;
    desc.MultisampleEnable = TRUE;

    HRESULT hr = m_renderer.device()->CreateRasterizerState(&desc, ppRasterizerState);
    if (SUCCEEDED(hr))
    {
        SetDebugObjectName(*ppRasterizerState, "direct3d11::CommonStates::RasterizerState");
    }

    return hr;
}

//------------------------------------------------------------------------------
HRESULT quetzal::direct3d11::CommonStates::Impl::create_sampler_state(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE texture_address_mode, ID3D11SamplerState** ppSamplerState)
{
    D3D11_SAMPLER_DESC desc = {};

    desc.Filter = filter;

    desc.AddressU = texture_address_mode;
    desc.AddressV = texture_address_mode;
    desc.AddressW = texture_address_mode;

    desc.MaxAnisotropy = (m_renderer.device()->GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1) ? D3D11_MAX_MAXANISOTROPY : 2;

    desc.MaxLOD = FLT_MAX;
    desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

    HRESULT hr = m_renderer.device()->CreateSamplerState(&desc, ppSamplerState);
    if (SUCCEEDED(hr))
    {
        SetDebugObjectName(*ppSamplerState, "direct3d11::CommonStates::SamplerState");
    }

    return hr;
}

//------------------------------------------------------------------------------
ID3D11BlendState* quetzal::direct3d11::CommonStates::BlendOpaque() const
{
    return checked_create(pImpl->m_pBlendStateOpaque, pImpl->m_mutex, [&](ID3D11BlendState** ppBlendState) { return pImpl->create_blend_state(D3D11_BLEND_ONE, D3D11_BLEND_ZERO, ppBlendState); });
}

//------------------------------------------------------------------------------
ID3D11BlendState* quetzal::direct3d11::CommonStates::BlendAlpha() const
{
    return checked_create(pImpl->m_pBlendStateAlpha, pImpl->m_mutex, [&](ID3D11BlendState** ppBlendState) { return pImpl->create_blend_state(D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, ppBlendState); });
}

//------------------------------------------------------------------------------
ID3D11BlendState* quetzal::direct3d11::CommonStates::BlendAdditive() const
{
    return checked_create(pImpl->m_pBlendStateAdditive, pImpl->m_mutex, [&](ID3D11BlendState** ppBlendState) { return pImpl->create_blend_state(D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE, ppBlendState); });
}

//------------------------------------------------------------------------------
ID3D11BlendState* quetzal::direct3d11::CommonStates::BlendNonPremultiplied() const
{
    return checked_create(pImpl->m_pBlendStateNonPremultiplied, pImpl->m_mutex, [&](ID3D11BlendState** ppBlendState) { return pImpl->create_blend_state(D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, ppBlendState); });
}

//------------------------------------------------------------------------------
ID3D11DepthStencilState* quetzal::direct3d11::CommonStates::DepthStencilNone() const
{
    return checked_create(pImpl->m_pDepthStencilNone, pImpl->m_mutex, [&](ID3D11DepthStencilState** ppDepthStencilState) { return pImpl->create_depth_stencil_state(false, false, ppDepthStencilState); });
}

//------------------------------------------------------------------------------
ID3D11DepthStencilState* quetzal::direct3d11::CommonStates::DepthStencilDefault() const
{
    return checked_create(pImpl->m_pDepthStencilDefault, pImpl->m_mutex, [&](ID3D11DepthStencilState** ppDepthStencilState) { return pImpl->create_depth_stencil_state(true, true, ppDepthStencilState); });
}

//------------------------------------------------------------------------------
ID3D11DepthStencilState* quetzal::direct3d11::CommonStates::DepthStencilRead() const
{
    return checked_create(pImpl->m_pDepthStencilRead, pImpl->m_mutex, [&](ID3D11DepthStencilState** ppDepthStencilState) { return pImpl->create_depth_stencil_state(true, false, ppDepthStencilState); });
}

//------------------------------------------------------------------------------
ID3D11RasterizerState* quetzal::direct3d11::CommonStates::RasterizerCullNone() const
{
    return checked_create(pImpl->m_pRasterizerCullNone, pImpl->m_mutex, [&](ID3D11RasterizerState** ppRasterizerState) { return pImpl->create_rasterizer_state(true, D3D11_CULL_NONE, D3D11_FILL_SOLID, ppRasterizerState); });
}

//------------------------------------------------------------------------------
ID3D11RasterizerState* quetzal::direct3d11::CommonStates::RasterizerCullClockwise() const
{
    return checked_create(pImpl->m_pRasterizerCullClockwise, pImpl->m_mutex, [&](ID3D11RasterizerState** ppRasterizerState) { return pImpl->create_rasterizer_state(true, D3D11_CULL_BACK, D3D11_FILL_SOLID, ppRasterizerState); });
}

//------------------------------------------------------------------------------
ID3D11RasterizerState* quetzal::direct3d11::CommonStates::RasterizerCullCounterClockwise() const
{
    return checked_create(pImpl->m_pRasterizerCullCounterClockwise, pImpl->m_mutex, [&](ID3D11RasterizerState** ppRasterizerState) { return pImpl->create_rasterizer_state(false, D3D11_CULL_BACK, D3D11_FILL_SOLID, ppRasterizerState); });
}

//------------------------------------------------------------------------------
ID3D11RasterizerState* quetzal::direct3d11::CommonStates::RasterizerWireframe() const
{
    return checked_create(pImpl->m_pRasterizerWireframe, pImpl->m_mutex, [&](ID3D11RasterizerState** ppRasterizerState) { return pImpl->create_rasterizer_state(true, D3D11_CULL_NONE, D3D11_FILL_WIREFRAME, ppRasterizerState); });
}

//------------------------------------------------------------------------------
ID3D11SamplerState* quetzal::direct3d11::CommonStates::SamplerPointWrap() const
{
    return checked_create(pImpl->m_pSamplerPointWrap, pImpl->m_mutex, [&](ID3D11SamplerState** ppSamplerState) { return pImpl->create_sampler_state(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP, ppSamplerState); });
}

//------------------------------------------------------------------------------
ID3D11SamplerState* quetzal::direct3d11::CommonStates::SamplerPointClamp() const
{
    return checked_create(pImpl->m_pSamplerPointClamp, pImpl->m_mutex, [&](ID3D11SamplerState** ppSamplerState) { return pImpl->create_sampler_state(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP, ppSamplerState); });
}

//------------------------------------------------------------------------------
ID3D11SamplerState* quetzal::direct3d11::CommonStates::SamplerLinearWrap() const
{
    return checked_create(pImpl->m_pSamplerLinearWrap, pImpl->m_mutex, [&](ID3D11SamplerState** ppSamplerState) { return pImpl->create_sampler_state(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, ppSamplerState); });
}

//------------------------------------------------------------------------------
ID3D11SamplerState* quetzal::direct3d11::CommonStates::SamplerLinearClamp() const
{
    return checked_create(pImpl->m_pSamplerLinearClamp, pImpl->m_mutex, [&](ID3D11SamplerState** ppSamplerState) { return pImpl->create_sampler_state(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, ppSamplerState); });
}

//------------------------------------------------------------------------------
ID3D11SamplerState* quetzal::direct3d11::CommonStates::SamplerAnisotropicWrap() const
{
    return checked_create(pImpl->m_pSamplerAnisotropicWrap, pImpl->m_mutex, [&](ID3D11SamplerState** ppSamplerState) { return pImpl->create_sampler_state(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, ppSamplerState); });
}

//------------------------------------------------------------------------------
ID3D11SamplerState* quetzal::direct3d11::CommonStates::SamplerAnisotropicClamp() const
{
    return checked_create(pImpl->m_pSamplerAnisotropicClamp, pImpl->m_mutex, [&](ID3D11SamplerState** ppSamplerState) { return pImpl->create_sampler_state(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_CLAMP, ppSamplerState); });
}
