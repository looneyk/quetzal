#if !defined(QUETZAL_DIRECT3D11_TEXTUREMANAGER_HPP)
#define QUETZAL_DIRECT3D11_TEXTUREMANAGER_HPP
//------------------------------------------------------------------------------
// direct3d11
// TextureManager.hpp
//------------------------------------------------------------------------------

#include "Renderer.hpp"
#include "quetzal/common/com_ptr.hpp"
#include <d3d11_1.h>
#include <filesystem>
#include <map>
#include <mutex>
#include <string>

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    class TextureManager
    {
    public:

        using texture_type = com_ptr<ID3D11ShaderResourceView>; // separate typedefs for both SRV and pointer? texture class instead? ...

        TextureManager(Renderer& renderer);
        TextureManager(const TextureManager&) = delete;
        TextureManager(TextureManager&&) = delete;
        ~TextureManager() = default;

        TextureManager& operator=(const TextureManager&) = delete;
        TextureManager& operator=(TextureManager&&) = delete;

        void set_path(const std::filesystem::path& pathname);
        void set_force_srgb(bool b);

        // name must be unique. Should allow for duplicate names as long as path is different ...
        texture_type open(const std::string& name);

        void clear();

    private:

        using textures_type = std::map<std::string, texture_type>;

        Renderer& m_renderer;
        std::filesystem::path m_path;
        bool m_bForceSRGB;
        textures_type m_textures;
        std::mutex m_mutex;
    };

    TextureManager::texture_type create_texture(Renderer& renderer, const std::filesystem::path& pathname, bool bForceSRGB = false);

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_TEXTUREMANAGER_HPP
