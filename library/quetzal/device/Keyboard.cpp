//------------------------------------------------------------------------------
// Keyboard.cpp
//
//------------------------------------------------------------------------------

#include "Keyboard.hpp"
#include <windows.h>
#include <cassert>

typedef quetzal::device::Key Key;
typedef quetzal::device::KeyModifier KeyModifier;

namespace
{

    //------------------------------------------------------------------------------
    KeyModifier::ModifierFlag s_modifier[Key::VirtualKeys] =
    {
        KeyModifier::Null,          // UNDEFINED_0x00
        KeyModifier::Null,          // LBUTTON
        KeyModifier::Null,          // RBUTTON
        KeyModifier::Null,          // CANCEL
        KeyModifier::Null,          // MBUTTON
        KeyModifier::Null,          // XBUTTON1
        KeyModifier::Null,          // XBUTTON2
        KeyModifier::Null,          // UNDEFINED_0x07
        KeyModifier::Null,          // BACK
        KeyModifier::Null,          // TAB
        KeyModifier::Null,          // RESERVED_0x0A
        KeyModifier::Null,          // RESERVED_0x0B
        KeyModifier::Null,          // CLEAR
        KeyModifier::Null,          // RETURN
        KeyModifier::Null,          // UNDEFINED_0x0E
        KeyModifier::Null,          // UNDEFINED_0x0F
        KeyModifier::Shift,         // SHIFT
        KeyModifier::Control,       // CONTROL
        KeyModifier::Alt,           // MENU
        KeyModifier::Null,          // PAUSE
        KeyModifier::CapsLock,      // CAPITAL
        KeyModifier::Null,          // KANA
        KeyModifier::Null,          // UNDEFINED_0x16
        KeyModifier::Null,          // JUNJA
        KeyModifier::Null,          // FINAL
        KeyModifier::Null,          // HANJA, KANJI
        KeyModifier::Null,          // UNDEFINED_0x1A
        KeyModifier::Null,          // ESCAPE
        KeyModifier::Null,          // CONVERT
        KeyModifier::Null,          // NONCONVERT
        KeyModifier::Null,          // ACCEPT
        KeyModifier::Null,          // MODECHANGE
        KeyModifier::Null,          // SPACE
        KeyModifier::Null,          // PRIOR
        KeyModifier::Null,          // NEXT
        KeyModifier::Null,          // END
        KeyModifier::Null,          // HOME
        KeyModifier::Null,          // LEFT
        KeyModifier::Null,          // UP
        KeyModifier::Null,          // RIGHT
        KeyModifier::Null,          // DOWN
        KeyModifier::Null,          // SELECT
        KeyModifier::Null,          // PRINT
        KeyModifier::Null,          // EXECUTE
        KeyModifier::Null,          // SNAPSHOT
        KeyModifier::Null,          // INSERT
        KeyModifier::Null,          // DELETE
        KeyModifier::Null,          // HELP
        KeyModifier::Null,          // 0
        KeyModifier::Null,          // 1
        KeyModifier::Null,          // 2
        KeyModifier::Null,          // 3
        KeyModifier::Null,          // 4
        KeyModifier::Null,          // 5
        KeyModifier::Null,          // 6
        KeyModifier::Null,          // 7
        KeyModifier::Null,          // 8
        KeyModifier::Null,          // 9
        KeyModifier::Null,          // UNDEFINED_0x3A
        KeyModifier::Null,          // UNDEFINED_0x3B
        KeyModifier::Null,          // UNDEFINED_0x3C
        KeyModifier::Null,          // UNDEFINED_0x3D
        KeyModifier::Null,          // UNDEFINED_0x3E
        KeyModifier::Null,          // UNDEFINED_0x3F
        KeyModifier::Null,          // UNDEFINED_0x40
        KeyModifier::Null,          // A
        KeyModifier::Null,          // B
        KeyModifier::Null,          // C
        KeyModifier::Null,          // D
        KeyModifier::Null,          // E
        KeyModifier::Null,          // F
        KeyModifier::Null,          // G
        KeyModifier::Null,          // H
        KeyModifier::Null,          // I
        KeyModifier::Null,          // J
        KeyModifier::Null,          // K
        KeyModifier::Null,          // L
        KeyModifier::Null,          // M
        KeyModifier::Null,          // N
        KeyModifier::Null,          // O
        KeyModifier::Null,          // P
        KeyModifier::Null,          // Q
        KeyModifier::Null,          // R
        KeyModifier::Null,          // S
        KeyModifier::Null,          // T
        KeyModifier::Null,          // U
        KeyModifier::Null,          // V
        KeyModifier::Null,          // W
        KeyModifier::Null,          // X
        KeyModifier::Null,          // Y
        KeyModifier::Null,          // Z
        KeyModifier::CommandLeft,   // LWIN
        KeyModifier::CommandRight,  // RWIN
        KeyModifier::Null,          // APPS
        KeyModifier::Null,          // RESERVED_0x5E
        KeyModifier::Null,          // SLEEP
        KeyModifier::Null,          // NUMPAD0
        KeyModifier::Null,          // NUMPAD1
        KeyModifier::Null,          // NUMPAD2
        KeyModifier::Null,          // NUMPAD3
        KeyModifier::Null,          // NUMPAD4
        KeyModifier::Null,          // NUMPAD5
        KeyModifier::Null,          // NUMPAD6
        KeyModifier::Null,          // NUMPAD7
        KeyModifier::Null,          // NUMPAD8
        KeyModifier::Null,          // NUMPAD9
        KeyModifier::Null,          // MULTIPLY
        KeyModifier::Null,          // ADD
        KeyModifier::Null,          // SEPARATOR
        KeyModifier::Null,          // SUBTRACT
        KeyModifier::Null,          // DECIMAL
        KeyModifier::Null,          // DIVIDE
        KeyModifier::Null,          // F1
        KeyModifier::Null,          // F2
        KeyModifier::Null,          // F3
        KeyModifier::Null,          // F4
        KeyModifier::Null,          // F5
        KeyModifier::Null,          // F6
        KeyModifier::Null,          // F7
        KeyModifier::Null,          // F8
        KeyModifier::Null,          // F9
        KeyModifier::Null,          // F10
        KeyModifier::Null,          // F11
        KeyModifier::Null,          // F12
        KeyModifier::Null,          // F13
        KeyModifier::Null,          // F14
        KeyModifier::Null,          // F15
        KeyModifier::Null,          // F16
        KeyModifier::Null,          // F17
        KeyModifier::Null,          // F18
        KeyModifier::Null,          // F19
        KeyModifier::Null,          // F20
        KeyModifier::Null,          // F21
        KeyModifier::Null,          // F22
        KeyModifier::Null,          // F23
        KeyModifier::Null,          // F24
        KeyModifier::Null,          // UNASSIGNED_0x88
        KeyModifier::Null,          // UNASSIGNED_0x89
        KeyModifier::Null,          // UNASSIGNED_0x8A
        KeyModifier::Null,          // UNASSIGNED_0x8B
        KeyModifier::Null,          // UNASSIGNED_0x8C
        KeyModifier::Null,          // UNASSIGNED_0x8D
        KeyModifier::Null,          // UNASSIGNED_0x8E
        KeyModifier::Null,          // UNASSIGNED_0x8F
        KeyModifier::NumLock,       // NUMLOCK
        KeyModifier::ScrollLock,    // SCROLL
        KeyModifier::Null,          // OEM_SPECIFIC_0x92
        KeyModifier::Null,          // OEM_SPECIFIC_0x93
        KeyModifier::Null,          // OEM_SPECIFIC_0x94
        KeyModifier::Null,          // OEM_SPECIFIC_0x95
        KeyModifier::Null,          // OEM_SPECIFIC_0x96
        KeyModifier::Null,          // UNASSIGNED_0x97
        KeyModifier::Null,          // UNASSIGNED_0x98
        KeyModifier::Null,          // UNASSIGNED_0x99
        KeyModifier::Null,          // UNASSIGNED_0x9A
        KeyModifier::Null,          // UNASSIGNED_0x9B
        KeyModifier::Null,          // UNASSIGNED_0x9C
        KeyModifier::Null,          // UNASSIGNED_0x9D
        KeyModifier::Null,          // UNASSIGNED_0x9E
        KeyModifier::Null,          // UNASSIGNED_0x9F
        KeyModifier::ShiftLeft,     // LSHIFT
        KeyModifier::ShiftRight,    // RSHIFT
        KeyModifier::ControlLeft,   // LCONTROL
        KeyModifier::ControlRight,  // RCONTROL
        KeyModifier::AltLeft,       // LMENU
        KeyModifier::AltRight,      // RMENU
        KeyModifier::Null,          // BROWSER_BACK
        KeyModifier::Null,          // BROWSER_FORWARD
        KeyModifier::Null,          // BROWSER_REFRESH
        KeyModifier::Null,          // BROWSER_STOP
        KeyModifier::Null,          // BROWSER_SEARCH
        KeyModifier::Null,          // BROWSER_FAVORITES
        KeyModifier::Null,          // BROWSER_HOME
        KeyModifier::Null,          // VOLUME_MUTE
        KeyModifier::Null,          // VOLUME_DOWN
        KeyModifier::Null,          // VOLUME_UP
        KeyModifier::Null,          // MEDIA_NEXT_TRACK
        KeyModifier::Null,          // MEDIA_PREV_TRACK
        KeyModifier::Null,          // MEDIA_STOP
        KeyModifier::Null,          // MEDIA_PLAY_PAUSE
        KeyModifier::Null,          // LAUNCH_MAIL
        KeyModifier::Null,          // LAUNCH_MEDIA_SELECT
        KeyModifier::Null,          // LAUNCH_APP1
        KeyModifier::Null,          // LAUNCH_APP2
        KeyModifier::Null,          // RESERVED_0xB8
        KeyModifier::Null,          // RESERVED_0xB9
        KeyModifier::Null,          // OEM_1
        KeyModifier::Null,          // OEM_PLUS
        KeyModifier::Null,          // OEM_COMMA
        KeyModifier::Null,          // OEM_MINUS
        KeyModifier::Null,          // OEM_PERIOD
        KeyModifier::Null,          // OEM_2
        KeyModifier::Null,          // OEM_3
        KeyModifier::Null,          // RESERVED_0xC1
        KeyModifier::Null,          // RESERVED_0xC2
        KeyModifier::Null,          // RESERVED_0xC3
        KeyModifier::Null,          // RESERVED_0xC4
        KeyModifier::Null,          // RESERVED_0xC5
        KeyModifier::Null,          // RESERVED_0xC6
        KeyModifier::Null,          // RESERVED_0xC7
        KeyModifier::Null,          // RESERVED_0xC8
        KeyModifier::Null,          // RESERVED_0xC9
        KeyModifier::Null,          // RESERVED_0xCA
        KeyModifier::Null,          // RESERVED_0xCB
        KeyModifier::Null,          // RESERVED_0xCC
        KeyModifier::Null,          // RESERVED_0xCD
        KeyModifier::Null,          // RESERVED_0xCE
        KeyModifier::Null,          // RESERVED_0xCF
        KeyModifier::Null,          // RESERVED_0xD1
        KeyModifier::Null,          // RESERVED_0xD2
        KeyModifier::Null,          // RESERVED_0xD3
        KeyModifier::Null,          // RESERVED_0xD4
        KeyModifier::Null,          // RESERVED_0xD5
        KeyModifier::Null,          // RESERVED_0xD6
        KeyModifier::Null,          // RESERVED_0xD7
        KeyModifier::Null,          // UNASSIGNED_0xD8
        KeyModifier::Null,          // UNASSIGNED_0xD9
        KeyModifier::Null,          // UNASSIGNED_0xDA
        KeyModifier::Null,          // OEM_4
        KeyModifier::Null,          // OEM_5
        KeyModifier::Null,          // OEM_6
        KeyModifier::Null,          // OEM_7
        KeyModifier::Null,          // OEM_8
        KeyModifier::Null,          // RESERVED_0xE0
        KeyModifier::Null,          // OEM_SPECIFIC_0xE1
        KeyModifier::Null,          // OEM_102
        KeyModifier::Null,          // OEM_SPECIFIC_0xE3
        KeyModifier::Null,          // OEM_SPECIFIC_0xE4
        KeyModifier::Null,          // PROCESSKEY
        KeyModifier::Null,          // OEM_SPECIFIC_0xE6
        KeyModifier::Null,          // PACKET
        KeyModifier::Null,          // UNASSIGNED_0xE8
        KeyModifier::Null,          // OEM_SPECIFIC_0xE9
        KeyModifier::Null,          // OEM_SPECIFIC_0xEA
        KeyModifier::Null,          // OEM_SPECIFIC_0xEB
        KeyModifier::Null,          // OEM_SPECIFIC_0xEC
        KeyModifier::Null,          // OEM_SPECIFIC_0xED
        KeyModifier::Null,          // OEM_SPECIFIC_0xEE
        KeyModifier::Null,          // OEM_SPECIFIC_0xEF
        KeyModifier::Null,          // OEM_SPECIFIC_0xF0
        KeyModifier::Null,          // OEM_SPECIFIC_0xF1
        KeyModifier::Null,          // OEM_SPECIFIC_0xF2
        KeyModifier::Null,          // OEM_SPECIFIC_0xF3
        KeyModifier::Null,          // OEM_SPECIFIC_0xF4
        KeyModifier::Null,          // OEM_SPECIFIC_0xF5
        KeyModifier::Null,          // ATTN
        KeyModifier::Null,          // CRSEL
        KeyModifier::Null,          // EXSEL
        KeyModifier::Null,          // EREOF
        KeyModifier::Null,          // PLAY
        KeyModifier::Null,          // ZOOM
        KeyModifier::Null,          // NONAME
        KeyModifier::Null,          // PA1
        KeyModifier::Null,          // OEM_CLEAR
        KeyModifier::Null           // RESERVED_0xFF
    };

    //----------------------------------------------------------------------
    void set_modifier_flag(KeyModifier::Modifiers& m, KeyModifier::ModifierFlag f, BYTE state)
    {
        if (state) // no bit test necessary? ...
        {
            m |= f;
        }
        else
        {
            m &= ~f;
        }

        return;
    }

} // namespace

//------------------------------------------------------------------------------
quetzal::device::Keyboard::Keyboard() :
    m_state(Key::VirtualKeys),
    m_bValid(false)
{
}

//------------------------------------------------------------------------------
quetzal::device::Keyboard::~Keyboard()
{
}

//------------------------------------------------------------------------------
void quetzal::device::Keyboard::poll()
{
// ...
for (int i = 0; i < Key::VirtualKeys; ++i)
{
    m_state[i] = GetAsyncKeyState(i) ? 0x80 : 0;
}

// GetAsyncKeyState down 0x8000
// GetKeyState down 0x8000 toggled 0x01

// find out why this isn't working ...
//    BOOL b = GetKeyboardState(&m_state[0]);
//    assert(b);

    m_bValid = true;

    return;
}

//------------------------------------------------------------------------------
bool quetzal::device::Keyboard::key_down(int keycode) const
{
    assert(keycode > 0 && keycode < Key::VirtualKeys);
    assert(m_bValid);

    return (m_state[keycode] & 0x80) != 0;
}

//------------------------------------------------------------------------------
bool quetzal::device::Keyboard::key_toggled(int keycode) const
{
    assert(keycode > 0 && keycode < Key::VirtualKeys);
    assert(m_bValid);

    return (m_state[keycode] & 0x01) != 0;
}

//------------------------------------------------------------------------------
void quetzal::device::Keyboard::invalidate()
{
    m_bValid = false;

    return;
}

//------------------------------------------------------------------------------
KeyModifier::Modifiers quetzal::device::Keyboard::modifiers()
{
    if (!m_bValid)
    {
        poll();
    }

    assert(m_bValid);

    KeyModifier::Modifiers m = 0;

    set_modifier_flag(m, KeyModifier::ShiftLeft, m_state[Key::V_LSHIFT]);
    set_modifier_flag(m, KeyModifier::ShiftRight, m_state[Key::V_RSHIFT]);
    set_modifier_flag(m, KeyModifier::ControlLeft, m_state[Key::V_LCONTROL]);
    set_modifier_flag(m, KeyModifier::ControlRight, m_state[Key::V_RCONTROL]);
    set_modifier_flag(m, KeyModifier::AltLeft, m_state[Key::V_LMENU]);
    set_modifier_flag(m, KeyModifier::AltRight, m_state[Key::V_RMENU]);
    set_modifier_flag(m, KeyModifier::CommandLeft, m_state[Key::V_LWIN]);
    set_modifier_flag(m, KeyModifier::CommandRight, m_state[Key::V_RWIN]);
    set_modifier_flag(m, KeyModifier::CapsLock, m_state[Key::V_CAPITAL]);
    set_modifier_flag(m, KeyModifier::NumLock, m_state[Key::V_NUMLOCK]);
    set_modifier_flag(m, KeyModifier::ScrollLock, m_state[Key::V_SCROLL]);

    return m;
}
