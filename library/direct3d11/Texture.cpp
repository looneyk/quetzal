//------------------------------------------------------------------------------
// Direct3D 11
// Texture.hpp
//------------------------------------------------------------------------------

#include "Texture.hpp"
#include "file_io.hpp"
#include "common/ComException.hpp"
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"

using namespace std;
using namespace DirectX;

//------------------------------------------------------------------------------
quetzal::direct3d11::Texture::Texture(Renderer& renderer) :
    m_renderer(renderer),
    m_pResource(),
    m_pShaderResourceView()
{
}

//------------------------------------------------------------------------------
quetzal::com_ptr<ID3D11Resource> quetzal::direct3d11::Texture::resource() const
{
    return m_pResource;
}

//------------------------------------------------------------------------------
quetzal::com_ptr<ID3D11ShaderResourceView> quetzal::direct3d11::Texture::shader_resource_view() const
{
    return m_pShaderResourceView;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Texture::open(const filesystem::path& pathname)
{
    HRESULT hr = S_OK;

    HR_CHECK(CreateDDSTextureFromFile(m_renderer.device(), pathname.wstring().c_str(), &m_pResource.p(), &m_pShaderResourceView.p()));
    SetDebugObjectName(m_pResource.get(), "Resource");
    SetDebugObjectName(m_pShaderResourceView.get(), "ShaderResourceView");

    assert(m_pResource != nullptr);
    assert(m_pShaderResourceView != nullptr);

    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::Texture::open(const filesystem::path& pathname, int w, int h, int stride, DXGI_FORMAT format)
{
    HRESULT hr = S_OK;

    // Load the raw texture data from disk and construct a subresource description that references it.
    auto texture_data = direct3d11::read(pathname);
    D3D11_SUBRESOURCE_DATA texture_subresource_data = {};
    texture_subresource_data.pSysMem = texture_data.data();
    texture_subresource_data.SysMemPitch = stride; // Specify the size of a row in bytes, known a priori about the texture data.
    texture_subresource_data.SysMemSlicePitch = 0; // As this is not a texture array or 3D texture, this parameter is ignored.

    // Create a texture description from information known a priori about the data.
    // Generalized texture loading code can be found in the Resource Loading sample.
    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = w;
    textureDesc.Height = h;
    textureDesc.MipLevels = 1; // Most textures contain more than one MIP level. For simplicity, this sample uses only one.
    textureDesc.ArraySize = 1; // Not a texture array
    textureDesc.Format = format;
    textureDesc.SampleDesc.Count = 1; // Don't use multi-sampling.
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // Allow the texture to be bound as a shader resource.
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    com_ptr<ID3D11Texture2D> pTexture2D;
    HR_CHECK(m_renderer.device()->CreateTexture2D(&textureDesc, &texture_subresource_data, &pTexture2D.p()));
    SetDebugObjectName(pTexture2D.get(), "Texture2D");

    // Once the texture is created, we must create a shader resource view of it so that shaders may use it.
    // In general, the view description will match the texture description.
    D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc = {};
    textureViewDesc.Format = textureDesc.Format;
    textureViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    textureViewDesc.Texture2D.MostDetailedMip = 0;
    textureViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;

    HR_CHECK(m_renderer.device()->CreateShaderResourceView(pTexture2D, &textureViewDesc, &m_pShaderResourceView.p()));
    SetDebugObjectName(m_pShaderResourceView.get(), "ShaderResourceView");

    return;
}
