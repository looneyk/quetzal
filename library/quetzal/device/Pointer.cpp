//------------------------------------------------------------------------------
// Pointer.cpp
//
//------------------------------------------------------------------------------

#include "Pointer.hpp"
#include <cassert>

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

//------------------------------------------------------------------------------
quetzal::device::Pointer::Pointer() :
    m_handle(),
    m_id(0),
    m_nButtons(0),
    m_nSampleRate(0),
    m_bHorizontalWheel(false)
{
}

//------------------------------------------------------------------------------
quetzal::device::Pointer::~Pointer()
{
}

//------------------------------------------------------------------------------
void quetzal::device::Pointer::init(HWND hwnd)
{
    RAWINPUTDEVICE rid[1];
    rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC; 
    rid[0].usUsage = HID_USAGE_GENERIC_MOUSE; 
    rid[0].dwFlags = RIDEV_INPUTSINK; // RIDEV_NOLEGACY, RIDEV_NOHOTKEYS, not RIDEV_INPUTSINK?
    rid[0].hwndTarget = hwnd;
    RegisterRawInputDevices(rid, 1, sizeof rid[0]);

//    update(); // need handle first ...

    return;
}

/*
//------------------------------------------------------------------------------
void quetzal::device::Pointer::update()
{
    // get HANDLE, name ...

    RID_DEVICE_INFO di;
    di.cbSize = sizeof di;
    UINT size = sizeof di;
    BOOL b = GetRawInputDeviceInfo(m_handle, RIDI_DEVICEINFO, &di, &size);
    assert(b);
    assert(di.cbSize == sizeof di);
    assert(di.dwType == RIM_TYPEMOUSE);

    const RID_DEVICE_INFO_MOUSE& dim = di.mouse;
    m_id = dim.dwId;
    m_nButtons = dim.dwNumberOfButtons;
    m_nSampleRate = dim.dwSampleRate;
    m_bHorizontalWheel = dim.fHasHorizontalWheel;

    return;
}
*/
