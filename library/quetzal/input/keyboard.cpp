//------------------------------------------------------------------------------
// keyboard.cpp
//------------------------------------------------------------------------------

#include "keyboard.hpp"
#include "windows.h"
#include <ostream>
#include <string>

using namespace std;
namespace key = quetzal::input::key;

namespace
{

    //--------------------------------------------------------------------------
    const key::Modifier s_modifier_mapping[key::Codes] =
	{
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, // key::Modifier::Shift,
        0, // key::Modifier::Control,
        0, // key::Modifier::Alt,
        0,
        key::Modifier::CapsLock,
                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        key::Modifier::CommandLeft,
        key::Modifier::CommandRight,
                                               0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        key::Modifier::NumLock,
        key::Modifier::ScrollLock,
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        key::Modifier::ShiftLeft,
        key::Modifier::ShiftRight,
        key::Modifier::ControlLeft,
        key::Modifier::ControlRight,
        key::Modifier::AltLeft,
        key::Modifier::AltRight,
                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    //--------------------------------------------------------------------------
    const string s_code_string[key::Codes] =
    {
        "UNDEFINED_0x00",
        "LBUTTON",
        "RBUTTON",
        "CANCEL",
        "MBUTTON",
        "XBUTTON1",
        "XBUTTON2",
        "UNDEFINED_0x07",
        "BACK",
        "TAB",
        "RESERVED_0x0A",
        "RESERVED_0x0B",
        "CLEAR",
        "RETURN",
        "UNDEFINED_0x0E",
        "UNDEFINED_0x0F",
        "SHIFT",
        "CONTROL",
        "MENU",
        "PAUSE",
        "CAPITAL",
        "KANA",
        "UNDEFINED_0x16",
        "JUNJA",
        "FINAL",
        "HANJA", // "KANJI",
        "UNDEFINED_0x1A",
        "ESCAPE",
        "CONVERT",
        "NONCONVERT",
        "ACCEPT",
        "MODECHANGE",
        "SPACE",
        "PRIOR",
        "NEXT",
        "END",
        "HOME",
        "LEFT",
        "UP",
        "RIGHT",
        "DOWN",
        "SELECT",
        "PRINT",
        "EXECUTE",
        "SNAPSHOT",
        "INSERT",
        "DELETE",
        "HELP",
        "0",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "UNDEFINED_0x3A",
        "UNDEFINED_0x3B",
        "UNDEFINED_0x3C",
        "UNDEFINED_0x3D",
        "UNDEFINED_0x3E",
        "UNDEFINED_0x3F",
        "UNDEFINED_0x40",
        "A",
        "B",
        "C",
        "D",
        "E",
        "F",
        "G",
        "H",
        "I",
        "J",
        "K",
        "L",
        "M",
        "N",
        "O",
        "P",
        "Q",
        "R",
        "S",
        "T",
        "U",
        "V",
        "W",
        "X",
        "Y",
        "Z",
        "LWIN",
        "RWIN",
        "APPS",
        "RESERVED_0x5E",
        "SLEEP",
        "NUMPAD0",
        "NUMPAD1",
        "NUMPAD2",
        "NUMPAD3",
        "NUMPAD4",
        "NUMPAD5",
        "NUMPAD6",
        "NUMPAD7",
        "NUMPAD8",
        "NUMPAD9",
        "MULTIPLY",
        "ADD",
        "SEPARATOR",
        "SUBTRACT",
        "DECIMAL",
        "DIVIDE",
        "F1",
        "F2",
        "F3",
        "F4",
        "F5",
        "F6",
        "F7",
        "F8",
        "F9",
        "F10",
        "F11",
        "F12",
        "F13",
        "F14",
        "F15",
        "F16",
        "F17",
        "F18",
        "F19",
        "F20",
        "F21",
        "F22",
        "F23",
        "F24",
        "UNASSIGNED_0x88",
        "UNASSIGNED_0x89",
        "UNASSIGNED_0x8A",
        "UNASSIGNED_0x8B",
        "UNASSIGNED_0x8C",
        "UNASSIGNED_0x8D",
        "UNASSIGNED_0x8E",
        "UNASSIGNED_0x8F",
        "NUMLOCK",
        "SCROLL",
        "OEM_SPECIFIC_0x92",
        "OEM_SPECIFIC_0x93",
        "OEM_SPECIFIC_0x94",
        "OEM_SPECIFIC_0x95",
        "OEM_SPECIFIC_0x96",
        "UNASSIGNED_0x97",
        "UNASSIGNED_0x98",
        "UNASSIGNED_0x99",
        "UNASSIGNED_0x9A",
        "UNASSIGNED_0x9B",
        "UNASSIGNED_0x9C",
        "UNASSIGNED_0x9D",
        "UNASSIGNED_0x9E",
        "UNASSIGNED_0x9F",
        "LSHIFT",
        "RSHIFT",
        "LCONTROL",
        "RCONTROL",
        "LMENU",
        "RMENU",
        "BROWSER_BACK",
        "BROWSER_FORWARD",
        "BROWSER_REFRESH",
        "BROWSER_STOP",
        "BROWSER_SEARCH",
        "BROWSER_FAVORITES",
        "BROWSER_HOME",
        "VOLUME_MUTE",
        "VOLUME_DOWN",
        "VOLUME_UP",
        "MEDIA_NEXT_TRACK",
        "MEDIA_PREV_TRACK",
        "MEDIA_STOP",
        "MEDIA_PLAY_PAUSE",
        "LAUNCH_MAIL",
        "LAUNCH_MEDIA_SELECT",
        "LAUNCH_APP1",
        "LAUNCH_APP2",
        "RESERVED_0xB8",
        "RESERVED_0xB9",
        "OEM_1",
        "OEM_PLUS",
        "OEM_COMMA",
        "OEM_MINUS",
        "OEM_PERIOD",
        "OEM_2",
        "OEM_3",
        "RESERVED_0xC1",
        "RESERVED_0xC2",
        "RESERVED_0xC3",
        "RESERVED_0xC4",
        "RESERVED_0xC5",
        "RESERVED_0xC6",
        "RESERVED_0xC7",
        "RESERVED_0xC8",
        "RESERVED_0xC9",
        "RESERVED_0xCA",
        "RESERVED_0xCB",
        "RESERVED_0xCC",
        "RESERVED_0xCD",
        "RESERVED_0xCE",
        "RESERVED_0xCF",
        "RESERVED_0xD1",
        "RESERVED_0xD2",
        "RESERVED_0xD3",
        "RESERVED_0xD4",
        "RESERVED_0xD5",
        "RESERVED_0xD6",
        "RESERVED_0xD7",
        "UNASSIGNED_0xD8",
        "UNASSIGNED_0xD9",
        "UNASSIGNED_0xDA",
        "OEM_4",
        "OEM_5",
        "OEM_6",
        "OEM_7",
        "OEM_8",
        "RESERVED_0xE0",
        "OEM_SPECIFIC_0xE1",
        "OEM_102",
        "OEM_SPECIFIC_0xE3",
        "OEM_SPECIFIC_0xE4",
        "PROCESSKEY",
        "OEM_SPECIFIC_0xE6",
        "PACKET",
        "UNASSIGNED_0xE8",
        "OEM_SPECIFIC_0xE9",
        "OEM_SPECIFIC_0xEA",
        "OEM_SPECIFIC_0xEB",
        "OEM_SPECIFIC_0xEC",
        "OEM_SPECIFIC_0xED",
        "OEM_SPECIFIC_0xEE",
        "OEM_SPECIFIC_0xEF",
        "OEM_SPECIFIC_0xF0",
        "OEM_SPECIFIC_0xF1",
        "OEM_SPECIFIC_0xF2",
        "OEM_SPECIFIC_0xF3",
        "OEM_SPECIFIC_0xF4",
        "OEM_SPECIFIC_0xF5",
        "ATTN",
        "CRSEL",
        "EXSEL",
        "EREOF",
        "PLAY",
        "ZOOM",
        "NONAME",
        "PA1",
        "OEM_CLEAR",
        "RESERVED_0xFF"
    };

    const unsigned int KeyStateDown = 0x8000;
    const unsigned int KeyStateToggled = 0x0001;

} // namespace

//------------------------------------------------------------------------------
key::Code quetzal::input::translate_key(key::Code code, unsigned int flags)
{
    bool bExtended = (flags & (1 << 24)) != 0;

//m_ofs << to_string(code) << " " << hex << setw(8) << setfill('0') << flags << endl;
    switch (code)    
    {
        case key::Code::V_SHIFT:
            if (GetAsyncKeyState(static_cast<int>(key::Code::V_LSHIFT))) // Not dependable if RSHIFT pressed while LSHIFT is held down in any case ...
            {
                code = key::Code::V_LSHIFT;
            }
            else
            {
                code = key::Code::V_RSHIFT;
            }
            break;

        case key::Code::V_CONTROL:
            code = (bExtended ? key::Code::V_RCONTROL : key::Code::V_LCONTROL);
            break;

        case key::Code::V_MENU:
            code = (bExtended ? key::Code::V_RMENU : key::Code::V_LMENU);
            break;

// check on these ...
//        case key::Code::V_RETURN:
//            if (bExtended)
//            {
//                code = Keys::NUMPAD_ENTER;
//            }
//            break;

//        case key::Code::V_DELETE:
//            if (bExtended)
//            {
//                code = key::Code::V_DECIMAL;
//            }
//            break;

        case key::Code::V_END:
            if (!bExtended)
            {
                code = key::Code::V_NUMPAD1;
            }
            break;

        case key::Code::V_DOWN:
            if (!bExtended)
            {
                code = key::Code::V_NUMPAD2;
            }
            break;

        case key::Code::V_NEXT:
            if (!bExtended)
            {
                code = key::Code::V_NUMPAD3;
            }
            break;

        case key::Code::V_LEFT:
            if (!bExtended)
            {
                code = key::Code::V_NUMPAD4;
            }
            break;

        case key::Code::V_CLEAR:
            if (!bExtended)
            {
                code = key::Code::V_NUMPAD5;
            }
            break;

        case key::Code::V_RIGHT:
            if (!bExtended)
            {
                code = key::Code::V_NUMPAD6;
            }
            break;

        case key::Code::V_HOME:
            if (!bExtended)
            {
                code = key::Code::V_NUMPAD7;
            }
            break;

        case key::Code::V_UP:
            if (!bExtended)
            {
                code = key::Code::V_NUMPAD8;
            }
            break;

        case key::Code::V_PRIOR:
            if (!bExtended)
            {
                code = key::Code::V_NUMPAD9;
            }
            break;
    }
//m_ofs << "after " << to_string(code) << " " << hex << setw(8) << setfill('0') << flags << endl;

    return code;
}

//------------------------------------------------------------------------------
bool quetzal::input::query_key_down(key::Code code)
{
    SHORT state = GetAsyncKeyState(static_cast<int>(code));
    return (state & KeyStateDown) != 0;
}

//------------------------------------------------------------------------------
key::Modifiers quetzal::input::query_key_modifiers()
{
    key::Modifiers modifiers = 0;

    SHORT state = GetAsyncKeyState(static_cast<int>(key::Code::V_LSHIFT));
    if (state & KeyStateDown) modifiers |= key::Modifier::ShiftLeft;

    state = GetAsyncKeyState(static_cast<int>(key::Code::V_RSHIFT));
    if (state & KeyStateDown) modifiers |= key::Modifier::ShiftRight;

    state = GetAsyncKeyState(static_cast<int>(key::Code::V_LCONTROL));
    if (state & KeyStateDown) modifiers |= key::Modifier::ControlLeft;

    state = GetAsyncKeyState(static_cast<int>(key::Code::V_RCONTROL));
    if (state & KeyStateDown) modifiers |= key::Modifier::ControlRight;

    state = GetAsyncKeyState(static_cast<int>(key::Code::V_LMENU));
    if (state & KeyStateDown) modifiers |= key::Modifier::AltLeft;

    state = GetAsyncKeyState(static_cast<int>(key::Code::V_RMENU));
    if (state & KeyStateDown) modifiers |= key::Modifier::AltRight;

    state = GetAsyncKeyState(static_cast<int>(key::Code::V_LWIN));
    if (state & KeyStateDown) modifiers |= key::Modifier::CommandLeft;

    state = GetAsyncKeyState(static_cast<int>(key::Code::V_RWIN));
    if (state & KeyStateDown) modifiers |= key::Modifier::CommandRight;

    state = GetAsyncKeyState(static_cast<int>(key::Code::V_CAPITAL));
    if (state) modifiers |= key::Modifier::CapsLock;

    state = GetAsyncKeyState(static_cast<int>(key::Code::V_NUMLOCK));
    if (state) modifiers |= key::Modifier::NumLock;

    state = GetAsyncKeyState(static_cast<int>(key::Code::V_SCROLL));
    if (state) modifiers |= key::Modifier::ScrollLock;

    return modifiers;
}

//------------------------------------------------------------------------------
ostream& quetzal::input::operator<<(ostream& os, key::Code code)
{
    os << s_code_string[static_cast<unsigned int>(code)];
    return os;
}

/*
RAWINPUTDEVICE device;
device.usUsagePage = 0x01;
device.usUsage = 0x06;
device.dwFlags = RIDEV_NOLEGACY;        // do not generate legacy messages such as WM_KEYDOWN
device.hwndTarget = hWnd;
RegisterRawInputDevices(&device, 1, sizeof(device));


case WM_INPUT:
{
  char buffer[sizeof(RAWINPUT)] = {};
  UINT size = sizeof(RAWINPUT);
  GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, buffer, &size, sizeof(RAWINPUTHEADER));

  // extract keyboard raw input data
  RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(buffer);
  if (raw->header.dwType == RIM_TYPEKEYBOARD)
  {
    const RAWKEYBOARD& rawKB = raw->data.keyboard;
    // do something with the data here
  }
}


UINT virtualKey = rawKB.VKey;
UINT scanCode = rawKB.MakeCode;
UINT flags = rawKB.Flags;

if (virtualKey == 255)
{
  // discard "fake keys" which are part of an escaped sequence
  return 0;
}
else if (virtualKey == VK_SHIFT)
{
  // correct left-hand / right-hand SHIFT
  virtualKey = MapVirtualKey(scanCode, MAPVK_VSC_TO_VK_EX);
}
else if (virtualKey == VK_NUMLOCK)
{
  // correct PAUSE/BREAK and NUM LOCK silliness, and set the extended bit
  scanCode = (MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC) | 0x100);
}


// e0 and e1 are escape sequences used for certain special keys, such as PRINT and PAUSE/BREAK.
// see http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
const bool isE0 = ((flags & RI_KEY_E0) != 0);
const bool isE1 = ((flags & RI_KEY_E1) != 0);

if (isE1)
{
  // for escaped sequences, turn the virtual key into the correct scan code using MapVirtualKey.
  // however, MapVirtualKey is unable to map VK_PAUSE (this is a known bug), hence we map that by hand.
  if (virtualKey == VK_PAUSE)
    scanCode = 0x45;
  else
    scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);
}


switch (virtualKey)
{
  // right-hand CONTROL and ALT have their e0 bit set
  case VK_CONTROL:
    if (isE0)
      virtualKey = Keys::RIGHT_CONTROL;
    else
      virtualKey = Keys::LEFT_CONTROL;
    break;

  case VK_MENU:
    if (isE0)
      virtualKey = Keys::RIGHT_ALT;
    else
      virtualKey = Keys::LEFT_ALT;
    break;

  // NUMPAD ENTER has its e0 bit set
  case VK_RETURN:
    if (isE0)
      virtualKey = Keys::NUMPAD_ENTER;
    break;

  // the standard INSERT, DELETE, HOME, END, PRIOR and NEXT keys will always have their e0 bit set, but the
  // corresponding keys on the NUMPAD will not.
  case VK_INSERT:
    if (!isE0)
    virtualKey = Keys::NUMPAD_0;
    break;

  case VK_DELETE:
    if (!isE0)
      virtualKey = Keys::NUMPAD_DECIMAL;
    break;

  case VK_HOME:
    if (!isE0)
      virtualKey = Keys::NUMPAD_7;
    break;

  case VK_END:
    if (!isE0)
      virtualKey = Keys::NUMPAD_1;
    break;

  case VK_PRIOR:
    if (!isE0)
      virtualKey = Keys::NUMPAD_9;
    break;

  case VK_NEXT:
    if (!isE0)
      virtualKey = Keys::NUMPAD_3;
    break;

  // the standard arrow keys will always have their e0 bit set, but the
  // corresponding keys on the NUMPAD will not.
  case VK_LEFT:
    if (!isE0)
      virtualKey = Keys::NUMPAD_4;
    break;

  case VK_RIGHT:
    if (!isE0)
      virtualKey = Keys::NUMPAD_6;
    break;

  case VK_UP:
    if (!isE0)
      virtualKey = Keys::NUMPAD_8;
    break;

  case VK_DOWN:
    if (!isE0)
      virtualKey = Keys::NUMPAD_2;
    break;

  // NUMPAD 5 doesn't have its e0 bit set
  case VK_CLEAR:
    if (!isE0)
      virtualKey = Keys::NUMPAD_5;
    break;
}


// a key can either produce a "make" or "break" scancode. this is used to differentiate between down-presses and releases
// see http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
const bool wasUp = ((flags & RI_KEY_BREAK) != 0);

// getting a human-readable string
UINT key = (scanCode << 16) | (isE0 << 24);
char buffer[512] = {};
GetKeyNameText((LONG)key, buffer, 512);



//------------------------------------------------------------------------------
void quetzal::direct3d11::Application::init_raw_input()
{
    RAWINPUTDEVICE rids[2];

    // HID mouse, ignore legacy mouse messages
    rids[0].usUsagePage = 0x01;
    rids[0].usUsage = 0x02;
    rids[0].dwFlags = RIDEV_NOLEGACY;
    rids[0].hwndTarget = 0;

    // HID keyboard, ignore legacy keyboard messages
    rids[1].usUsagePage = 0x01;
    rids[1].usUsage = 0x06;
    rids[1].dwFlags = RIDEV_NOLEGACY;
    rids[1].hwndTarget = 0;

    if (RegisterRawInputDevices(rids, size(rids), sizeof rids[0]) == FALSE)
    {
        // GetLastError() ...
    }

    return;
}


                if (rih.dwType == RIM_TYPEKEYBOARD)
                {
                    const RAWKEYBOARD& rk = pri->data.keyboard;
                    unsigned int flags = rk.Flags;
                    unsigned int scanCode = rk.MakeCode;
                    input::key::Code code = translate_key(rk.VKey, scanCode, flags);

                    if ((flags & RI_KEY_BREAK) == 0)
                    {
                        on_key_down(m_tsMessage, modifiers, code);
                    }
                    else
                    {
                        on_key_up(m_tsMessage, modifiers, code);
                    }
                }

*/
