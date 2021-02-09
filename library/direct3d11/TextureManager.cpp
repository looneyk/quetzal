//------------------------------------------------------------------------------
// direct3d11
// TextureManager.hpp
//------------------------------------------------------------------------------

#include "TextureManager.hpp"
#include "DDSTextureLoader.h"
#include "common/ComException.hpp"
#include <filesystem>

using namespace std;
using namespace DirectX;

//------------------------------------------------------------------------------
quetzal::direct3d11::TextureManager::TextureManager(Renderer& renderer) :
    m_renderer(renderer),
    m_path(),
    m_bForceSRGB(false),
    m_textures(),
    m_mutex()
{
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::TextureManager::set_path(const filesystem::path& pathname)
{
    m_path = pathname;
    return;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::TextureManager::set_force_srgb(bool b)
{
    m_bForceSRGB = b;
    return;
}

//------------------------------------------------------------------------------
quetzal::direct3d11::TextureManager::texture_type quetzal::direct3d11::TextureManager::open(const string& name)
{
    if (name.empty())
    {
        return nullptr;
    }

    if (m_textures.contains(name))
    {
        return m_textures[name];
    }

    // shared resource ...
    filesystem::path pathname = m_path / name;
    texture_type pShaderResourceView = create_texture(m_renderer, pathname);

    lock_guard<mutex> lock(m_mutex);
    m_textures[name] = pShaderResourceView;

    return pShaderResourceView;
}

//------------------------------------------------------------------------------
void quetzal::direct3d11::TextureManager::clear()
{
    lock_guard<mutex> lock(m_mutex);
    m_textures.clear();
    return;
}

//------------------------------------------------------------------------------
quetzal::direct3d11::TextureManager::texture_type quetzal::direct3d11::create_texture(Renderer& renderer, const filesystem::path& pathname, bool bForceSRGB)
{
    TextureManager::texture_type pShaderResourceView;

    filesystem::path ext = pathname.extension();
    if (ext == ".dds")
    {
        HRESULT hr = CreateDDSTextureFromFileEx(renderer.device(), pathname.wstring().c_str(), 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, bForceSRGB, nullptr, &pShaderResourceView.p());
        if (FAILED(hr))
        {
//            throw exception("CreateDDSTextureFromFile");
            return nullptr;
        }
    }
/*
// pContext passed as nullptr from DirectXTK ...
//    else if (pContext)
//    {
//        lock_guard<mutex> lock(mutex);
//        HRESULT hr = CreateWICTextureFromFileEx(renderer.device(), pContext, pathname, 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, bForceSRGB ? WIC_LOADER_FORCE_SRGB : WIC_LOADER_DEFAULT, nullptr, &pShaderResourceView.p());
//        if (FAILED(hr))
//        {
//            throw exception("CreateWICTextureFromFile");
//        }
//    }
    else
    {
        HRESULT hr = CreateWICTextureFromFileEx(renderer.device(), pathname, 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0, bForceSRGB ? WIC_LOADER_FORCE_SRGB : WIC_LOADER_DEFAULT, nullptr, &pShaderResourceView.p());
        if (FAILED(hr))
        {
            throw exception("CreateWICTextureFromFile");
        }
    }
*/

    return pShaderResourceView;
}
