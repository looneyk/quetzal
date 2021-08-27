//------------------------------------------------------------------------------
// Device.cpp
//
//------------------------------------------------------------------------------

#include "Device.hpp"
#include <cassert>

//#include <fstream>
//#include <iomanip>
//#include <string>

//------------------------------------------------------------------------------
quetzal::device::DeviceIdentifier::DeviceIdentifier(DWORD id, std::wstring strName, HANDLE handle) :
    m_id(id),
    m_strName(strName),
    m_handle(handle)
{
}

//------------------------------------------------------------------------------
quetzal::device::DeviceIdentifier::~DeviceIdentifier()
{
}

//------------------------------------------------------------------------------
void quetzal::device::enumerate(DeviceIdentifiers& dis)
{
    UINT nDevices;
    UINT nReturn;
    std::vector<RAWINPUTDEVICELIST> rids;

    do
    {
        GetRawInputDeviceList(0, &nDevices, sizeof (RAWINPUTDEVICELIST));
        rids.resize(nDevices);
        nReturn = GetRawInputDeviceList(&rids[0], &nDevices, sizeof (RAWINPUTDEVICELIST));
    } while (nReturn < 0 && GetLastError() == ERROR_INSUFFICIENT_BUFFER);

    if (nReturn < 0)
    {
        return;
    }

// test ...
//std::wofstream ofs("C:\\kml\\test\\device.txt", std::ios::app);

    for (int i = 0; i < nDevices; ++i)
    {
        RAWINPUTDEVICELIST& ridl = rids[i];

        UINT sizeRequired;
        GetRawInputDeviceInfo(ridl.hDevice, RIDI_DEVICENAME, 0, &sizeRequired);

        std::vector<wchar_t> c(sizeRequired);
        UINT sizeReturned = GetRawInputDeviceInfo(ridl.hDevice, RIDI_DEVICENAME, &c[0], &sizeRequired);
        if (sizeReturned <= 0) // != sizeRequired
        {
            c[0] = L'\0';
        }

/*
RID_DEVICE_INFO di;
di.cbSize = sizeof di;
if (ridl.dwType != RIM_TYPEHID)
{
sizeReturned = GetRawInputDeviceInfo(ridl.hDevice, RIDI_DEVICEINFO, 0, &sizeRequired);
sizeReturned = GetRawInputDeviceInfo(ridl.hDevice, RIDI_DEVICEINFO, &di, &sizeRequired);
//assert(sizeReturned == sizeof di);
}

ofs << std::endl;
switch (ridl.dwType)
{
    case RIM_TYPEMOUSE:
        ofs << "Mouse" << std::endl;
        ofs << "handle  " << std::setw(8) << std::setfill(L'0') << std::hex << ridl.hDevice << std::dec << std::endl;
        ofs << "name    " << std::wstring(&c[0]) << std::endl;
        ofs << "id      " << di.mouse.dwId << std::endl;
        ofs << "nob     " << di.mouse.dwNumberOfButtons << std::endl;
        ofs << "sr      " << di.mouse.dwSampleRate << std::endl;
        ofs << "horiz   " << di.mouse.fHasHorizontalWheel << std::endl;
        break;

    case RIM_TYPEKEYBOARD:
        ofs << "Keyboard" << std::endl;
        ofs << "handle  " << std::setw(8) << std::setfill(L'0') << std::hex << ridl.hDevice << std::dec << std::endl;
        ofs << "name    " << std::wstring(&c[0]) << std::endl;
        ofs << "type    " << di.keyboard.dwType << std::endl;
        ofs << "subtype " << di.keyboard.dwSubType << std::endl;
        ofs << "mode    " << di.keyboard.dwKeyboardMode << std::endl;
        ofs << "nfk     " << di.keyboard.dwNumberOfFunctionKeys << std::endl;
        ofs << "ni      " << di.keyboard.dwNumberOfIndicators << std::endl;
        ofs << "nkt     " << di.keyboard.dwNumberOfKeysTotal << std::endl;
        break;

    case RIM_TYPEHID:
        ofs << "HID" << std::endl;
        ofs << "handle  " << std::setw(8) << std::setfill(L'0') << std::hex << ridl.hDevice << std::dec << std::endl;
        ofs << "name    " << std::wstring(&c[0]) << std::endl;
        break;
}
*/

        dis.push_back(DeviceIdentifier(ridl.dwType, &c[0], ridl.hDevice));
    }

    return;
}

//------------------------------------------------------------------------------
void quetzal::device::enumerate(DWORD type, DeviceIdentifiers& dis)
{
    type; // ...

    return;
}
