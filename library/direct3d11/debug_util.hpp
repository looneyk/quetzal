#if !defined(QUETZAL_DIRECT3D11_DEBUG_UTIL_HPP)
#define QUETZAL_DIRECT3D11_DEBUG_UTIL_HPP
//------------------------------------------------------------------------------
// Direct3D 11
// debug_util.hpp
//------------------------------------------------------------------------------

#include <string>
#include <d3d11_1.h>

namespace quetzal::direct3d11
{

//    void SetDebugObjectName(ID3D11DeviceChild* pDeviceChild, const std::string& name);

    // ...
    template<UINT TNameLength>
    inline void SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_z_ const char (&name)[TNameLength])
    {
        resource->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
    }

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_DEBUG_UTIL_HPP
