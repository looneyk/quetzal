#if !defined(QUETZAL_DIRECT3D11_BUFFER_HPP)
#define QUETZAL_DIRECT3D11_BUFFER_HPP
//------------------------------------------------------------------------------
// direct3d11
// buffer.hpp
//------------------------------------------------------------------------------

#include "Renderer.hpp"
#include "common/ComException.hpp"
#include "common/com_ptr.hpp"
#include <d3d11_1.h>

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    template<typename T>
    HRESULT create_buffer(Renderer& renderer, T items, UINT flags, com_ptr<ID3D11Buffer>& pBuffer)
    {
        D3D11_BUFFER_DESC buffer_desc = {};
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        buffer_desc.ByteWidth = static_cast<UINT>(sizeof(T::value_type) * items.size());
        buffer_desc.BindFlags = flags;

        D3D11_SUBRESOURCE_DATA subresource_data = {};
        subresource_data.pSysMem = items.data();

        return renderer.device()->CreateBuffer(&buffer_desc, &subresource_data, &pBuffer.p());
    }

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_BUFFER_HPP
