//------------------------------------------------------------------------------
// Device.hpp
//
//------------------------------------------------------------------------------

#if !defined(QUETZAL_DEVICE_DEVICE_HPP)
#define QUETZAL_DEVICE_DEVICE_HPP

#include <vector>
#include <string>
#include <windows.h> // ...

namespace quetzal
{

namespace device
{

    //----------------------------------------------------------------------
    class DeviceIdentifier
    {
    public:

        DeviceIdentifier(DWORD id, std::wstring strName, HANDLE handle);
        ~DeviceIdentifier();

        DWORD m_id;
        std::wstring m_strName;
        HANDLE m_handle;
    };

    typedef std::vector<DeviceIdentifier> DeviceIdentifiers;

    // template, takes collection with push_back? ...
    void enumerate(DeviceIdentifiers& dis);
    void enumerate(DWORD type, DeviceIdentifiers& dis);

} // namespace device

} // namespace quetzal

#endif // QUETZAL_DEVICE_DEVICE_HPP
