#if !defined(QUETZAL_DIRECT3D11_INDEX_FORMAT_HPP)
#define QUETZAL_DIRECT3D11_INDEX_FORMAT_HPP
//------------------------------------------------------------------------------
// direct3d11
// index_format.hpp
//------------------------------------------------------------------------------

#include <d3d11_1.h>

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    template<typename I>
    auto index_format = []() {
        if constexpr (sizeof (I) == 4)
        {
            return DXGI_FORMAT_R32_UINT;
        }
        else if constexpr (sizeof (I) == 2)
        {
            return DXGI_FORMAT_R16_UINT;
        }
        else
        {
            return DXGI_FORMAT_UNKNOWN;
        }
    };

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_INDEX_FORMAT_HPP
