#if !defined(QUETZAL_DIRECT3D11_INPUT_LAYOUT_HPP)
#define QUETZAL_DIRECT3D11_INPUT_LAYOUT_HPP
//------------------------------------------------------------------------------
// direct3d11
// input_layout.hpp
//------------------------------------------------------------------------------

#include "Renderer.hpp"
#include "common/ComException.hpp"
#include "common/com_ptr.hpp"
#include <d3d11_1.h>

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    template<typename T>
    HRESULT create_input_layout(Renderer& renderer, const ShaderBytecode& shader_bytecode, com_ptr<ID3D11InputLayout>& pInputLayout)
    {
        const auto& input_elements = T::input_elements();
        return renderer.device()->CreateInputLayout(input_elements.data(), static_cast<UINT>(input_elements.size()), shader_bytecode.data(), static_cast<UINT>(shader_bytecode.size()), &pInputLayout.p());
    }

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_INPUT_LAYOUT_HPP
