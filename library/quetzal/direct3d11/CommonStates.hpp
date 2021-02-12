#if !defined(QUETZAL_DIRECT3D11_COMMONSTATES_HPP)
#define QUETZAL_DIRECT3D11_COMMONSTATES_HPP
//------------------------------------------------------------------------------
// Direct3D 11
// CommonStates.hpp
//------------------------------------------------------------------------------

#include "Renderer.hpp"
#include <d3d11_1.h>
#include <memory>

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    class CommonStates
    {
    public:

        explicit CommonStates(Renderer& renderer);
        CommonStates(const CommonStates&) = delete;
        CommonStates(CommonStates&& other);
        virtual ~CommonStates() = default;

        CommonStates& operator=(const CommonStates&) = delete;
        CommonStates& operator=(CommonStates&& other);

        // Blend states.
        ID3D11BlendState* BlendOpaque() const;
        ID3D11BlendState* BlendAlpha() const;
        ID3D11BlendState* BlendAdditive() const;
        ID3D11BlendState* BlendNonPremultiplied() const;

        // Depth stencil states.
        ID3D11DepthStencilState* DepthStencilNone() const;
        ID3D11DepthStencilState* DepthStencilDefault() const;
        ID3D11DepthStencilState* DepthStencilRead() const;

        // Rasterizer states.
        ID3D11RasterizerState* RasterizerCullNone() const;
        ID3D11RasterizerState* RasterizerCullClockwise() const;
        ID3D11RasterizerState* RasterizerCullCounterClockwise() const;
        ID3D11RasterizerState* RasterizerWireframe() const;

        // Sampler states.
        ID3D11SamplerState* SamplerPointWrap() const;
        ID3D11SamplerState* SamplerPointClamp() const;
        ID3D11SamplerState* SamplerLinearWrap() const;
        ID3D11SamplerState* SamplerLinearClamp() const;
        ID3D11SamplerState* SamplerAnisotropicWrap() const;
        ID3D11SamplerState* SamplerAnisotropicClamp() const;

    private:

        // Private implementation.
        class Impl;

        std::shared_ptr<Impl> pImpl;
    };

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_COMMONSTATES_HPP
