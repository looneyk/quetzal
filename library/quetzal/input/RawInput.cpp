//------------------------------------------------------------------------------
// RawInput.cpp
//------------------------------------------------------------------------------

#include "RawInput.hpp"
#include <cassert>

//------------------------------------------------------------------------------
quetzal::input::RawInput::RawInput() :
    m_buffer()
{
    RAWINPUTDEVICE rid {};

    // HID mouse, ignore legacy mouse messages
    rid.usUsagePage = 0x01;
    rid.usUsage = 0x02;
//    rid.dwFlags = RIDEV_NOLEGACY; // legacy needed for windowed mode ...
    rid.hwndTarget = 0;

    if (RegisterRawInputDevices(&rid, 1, sizeof rid) == FALSE)
    {
        // GetLastError() ...
    }
}

//------------------------------------------------------------------------------
const RAWINPUTHEADER& quetzal::input::RawInput::header(LPARAM lParam)
{
    UINT size;
    GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof (RAWINPUTHEADER));
    m_buffer.resize(size);

    size = GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, m_buffer.data(), &size, sizeof (RAWINPUTHEADER));
    assert(size == m_buffer.size());

    return reinterpret_cast<RAWINPUT*>(m_buffer.data())->header;
}

//------------------------------------------------------------------------------
const RAWMOUSE& quetzal::input::RawInput::mouse() const
{
    assert(m_buffer.size() >= sizeof (RAWINPUT));
    return reinterpret_cast<const RAWINPUT*>(m_buffer.data())->data.mouse;
}
