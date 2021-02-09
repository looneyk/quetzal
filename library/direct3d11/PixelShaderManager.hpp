#if !defined(QUETZAL_DIRECT3D11_PIXELSHADERMANAGER_HPP)
#define QUETZAL_DIRECT3D11_PIXELSHADERMANAGER_HPP
//------------------------------------------------------------------------------
// direct3d11
// PixelShaderManager.hpp
//------------------------------------------------------------------------------

#include "Renderer.hpp"
#include "ShaderBytecode.hpp"
#include "checked_create.hpp"
#include "common/ComException.hpp"
#include "common/com_ptr.hpp"
#include "DirectXHelpers.h"
#include <d3d11_1.h>
#include <array>
#include <map>
#include <mutex>

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    template<typename Traits>
    class PixelShaderManager
    {
    public:

        using shader_type = com_ptr<ID3D11PixelShader>;
        using shaders_type = std::array<shader_type, Traits::PixelShaderCount>;
        using shaders_pointer_type = std::shared_ptr<shaders_type>;
        using shaders_map_type = std::map<ID3D11Device*, shaders_pointer_type>;

        using bytecode_array_type = std::array<ShaderBytecode, Traits::PixelShaderCount>;
        using shader_array_type = std::array<int, Traits::ShaderPermutationCount>;

        //----------------------------------------------------------------------
        class Shaders
        {
        public:

            //------------------------------------------------------------------
            Shaders(PixelShaderManager& shader_manager, Renderer& renderer) :
                m_shader_manager(shader_manager),
                m_renderer(renderer),
                m_mutex(),
                m_pShaders(shader_manager.get_shaders(renderer))
            {
            }

            //------------------------------------------------------------------
            ID3D11PixelShader* open(int permutation)
            {
                int nShader = m_shader_manager.shader(permutation);

                assert(nShader >= 0 && nShader < static_cast<int>(m_pShaders->size())); // ...
                return checked_create(m_pShaders->data()[nShader], m_mutex,
                    [&](ID3D11PixelShader** ppPixelShader) -> HRESULT // need different checked_create for IXxx*& ...
                    {
                        ShaderBytecode bytecode = m_shader_manager.bytecode(permutation);

                        HRESULT hr;
                        HR_CHECK(m_renderer.device()->CreatePixelShader(bytecode.data(), bytecode.size(), nullptr, ppPixelShader));
                        DirectX::SetDebugObjectName(*ppPixelShader, "PixelShader");
                        return hr;
                    }
                );
            }

        private:

            PixelShaderManager& m_shader_manager;
            Renderer& m_renderer;
            std::mutex m_mutex;
            shaders_pointer_type m_pShaders;
        };

        //----------------------------------------------------------------------
        PixelShaderManager() :
            m_mutex()
        {
        }

        PixelShaderManager(const PixelShaderManager&) = delete;
        ~PixelShaderManager() = default;

        PixelShaderManager& operator=(const PixelShaderManager&) = delete;

        //----------------------------------------------------------------------
        shaders_pointer_type get_shaders(Renderer& renderer)
        {
            std::lock_guard<std::mutex> lock(m_mutex);

            auto i = m_shaders.find(renderer.device());
            if (i != m_shaders.end())
            {
                return i->second;
            }

            auto p = std::make_shared<shaders_type>();
            m_shaders[renderer.device()] = p;
            return p;
        }

        //----------------------------------------------------------------------
        int shader(int permutation)
        {
            assert(permutation >= 0 && permutation < Traits::ShaderPermutationCount);
            return m_nShader[permutation];
        }

        //----------------------------------------------------------------------
        ShaderBytecode bytecode(int permutation)
        {
            assert(permutation >= 0 && permutation < Traits::ShaderPermutationCount);
            int nShader = m_nShader[permutation];

            assert(nShader >= 0 && nShader < Traits::PixelShaderCount);
            return m_bytecode[nShader];
        }

    private:

        shaders_map_type m_shaders;
        std::mutex m_mutex;

        static const bytecode_array_type m_bytecode;
        static const shader_array_type m_nShader;
    };

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_PIXELSHADERMANAGER_HPP
