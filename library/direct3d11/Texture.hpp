#if !defined(QUETZAL_DIRECT3D11_TEXTURE_HPP)
#define QUETZAL_DIRECT3D11_TEXTURE_HPP
//------------------------------------------------------------------------------
// Direct3D 11
// Texture.hpp
//------------------------------------------------------------------------------

#include "Renderer.hpp"
#include "common/com_ptr.hpp"
#include <d3d11_1.h>
#include <filesystem>

namespace quetzal::direct3d11
{

    //----------------------------------------------------------------------
    class Texture
    {
    public:

        Texture(Renderer& renderer);
        Texture(const Texture&) = delete;
        Texture(Texture&&) = delete;
        ~Texture() = default;

        Texture& operator=(const Texture&) = delete;
        Texture& operator=(Texture&&) = delete;

        com_ptr<ID3D11Resource> resource() const;
        com_ptr<ID3D11ShaderResourceView> shader_resource_view() const;

        void open(const std::filesystem::path& pathname);
        void open(const std::filesystem::path& pathname, int w, int h, int stride, DXGI_FORMAT format); // raw ...

    private:

        Renderer& m_renderer;
        com_ptr<ID3D11Resource> m_pResource;
        com_ptr<ID3D11ShaderResourceView>m_pShaderResourceView;
    };

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_TEXTURE_HPP
