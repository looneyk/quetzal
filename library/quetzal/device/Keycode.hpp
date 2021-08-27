//------------------------------------------------------------------------------
// KeyCode.hpp
//------------------------------------------------------------------------------

#if !defined(QUETZAL_DEVICE_KEYCODE_HPP)
#define QUETZAL_DEVICE_KEYCODE_HPP

#include <string>

namespace quetzal
{

namespace device
{

    //--------------------------------------------------------------------------
    enum class KeyCode
    {
    public:

        UNDEFINED_0x00      = 0x00,
        LBUTTON             = 0x01, // Left mouse button
        RBUTTON             = 0x02, // Right mouse button
        CANCEL              = 0x03, // Control-break processing
        MBUTTON             = 0x04, // Middle mouse button (three-button mouse)
        XBUTTON1            = 0x05, // X1 mouse button
        XBUTTON2            = 0x06, // X2 mouse button
        UNDEFINED_0x07      = 0x07,
        BACK                = 0x08, // BACKSPACE key
        TAB                 = 0x09, // TAB key
        RESERVED_0x0A       = 0x0a,
        RESERVED_0x0B       = 0x0b,
        CLEAR               = 0x0c, // CLEAR key
        RETURN              = 0x0d, // ENTER key
        UNDEFINED_0x0E      = 0x0e,
        UNDEFINED_0x0F      = 0x0f,
        SHIFT               = 0x10, // SHIFT key
        CONTROL             = 0x11, // CTRL key
        MENU                = 0x12, // ALT key
        PAUSE               = 0x13, // PAUSE key
        CAPITAL             = 0x14, // CAPS LOCK key
        KANA                = 0x15, // IME Kana mode, IME Hangul mode
        UNDEFINED_0x16      = 0x16,
        JUNJA               = 0x17, // IME Junja mode
        FINAL               = 0x18, // IME final mode
        HANJA               = 0x19, // IME Hanja mode
        KANJI               = 0x19, // IME Kanji mode
        UNDEFINED_0x1A      = 0x1a,
        ESCAPE              = 0x1b, // ESC key
        CONVERT             = 0x1c, // IME convert
        NONCONVERT          = 0x1d, // IME nonconvert
        ACCEPT              = 0x1e, // IME accept
        MODECHANGE          = 0x1f, // IME mode change request
        SPACE               = 0x20, // SPACEBAR
        PRIOR               = 0x21, // PAGE UP key
        NEXT                = 0x22, // PAGE DOWN key
        END                 = 0x23, // END key
        HOME                = 0x24, // HOME key
        LEFT                = 0x25, // LEFT ARROW key
        UP                  = 0x26, // UP ARROW key
        RIGHT               = 0x27, // RIGHT ARROW key
        DOWN                = 0x28, // DOWN ARROW key
        SELECT              = 0x29, // SELECT key
        PRINT               = 0x2a, // PRINT key
        EXECUTE             = 0x2b, // EXECUTE key
        SNAPSHOT            = 0x2c, // PRINT SCREEN key
        INSERT              = 0x2d, // INS key
        DELETE              = 0x2e, // DEL key
        HELP                = 0x2f, // HELP key
        DIGIT0              = 0x30,
        DIGIT1              = 0x31,
        DIGIT2              = 0x32,
        DIGIT3              = 0x33,
        DIGIT4              = 0x34,
        DIGIT5              = 0x35,
        DIGIT6              = 0x36,
        DIGIT7              = 0x37,
        DIGIT8              = 0x38,
        DIGIT9              = 0x39,
        UNDEFINED_0x3A      = 0x3a,
        UNDEFINED_0x3B      = 0x3b,
        UNDEFINED_0x3C      = 0x3c,
        UNDEFINED_0x3D      = 0x3d,
        UNDEFINED_0x3E      = 0x3e,
        UNDEFINED_0x3F      = 0x3f,
        UNDEFINED_0x40      = 0x40,
        A                   = 0x41,
        B                   = 0x42,
        C                   = 0x43,
        D                   = 0x44,
        E                   = 0x45,
        F                   = 0x46,
        G                   = 0x47,
        H                   = 0x48,
        I                   = 0x49,
        J                   = 0x4a,
        K                   = 0x4b,
        L                   = 0x4c,
        M                   = 0x4d,
        N                   = 0x4e,
        O                   = 0x4f,
        P                   = 0x50,
        Q                   = 0x51,
        R                   = 0x52,
        S                   = 0x53,
        T                   = 0x54,
        U                   = 0x55,
        V                   = 0x56,
        W                   = 0x57,
        X                   = 0x58,
        Y                   = 0x59,
        Z                   = 0x5a,
        LWIN                = 0x5b, // Left Windows key (Natural keyboard)
        RWIN                = 0x5c, // Right Windows key (Natural keyboard)
        APPS                = 0x5d, // Applications key (Natural keyboard)
        RESERVED_0x5E       = 0x5e,
        SLEEP               = 0x5f, // Computer Sleep key
        NUMPAD0             = 0x60, // Numeric keypad 0 key
        NUMPAD1             = 0x61, // Numeric keypad 1 key
        NUMPAD2             = 0x62, // Numeric keypad 2 key
        NUMPAD3             = 0x63, // Numeric keypad 3 key
        NUMPAD4             = 0x64, // Numeric keypad 4 key
        NUMPAD5             = 0x65, // Numeric keypad 5 key
        NUMPAD6             = 0x66, // Numeric keypad 6 key
        NUMPAD7             = 0x67, // Numeric keypad 7 key
        NUMPAD8             = 0x68, // Numeric keypad 8 key
        NUMPAD9             = 0x69, // Numeric keypad 9 key
        MULTIPLY            = 0x6a, // Multiply key
        ADD                 = 0x6b, // Add key
        SEPARATOR           = 0x6c, // Separator key
        SUBTRACT            = 0x6d, // Subtract key
        DECIMAL             = 0x6e, // Decimal key
        DIVIDE              = 0x6f, // Divide key
        F1                  = 0x70, // F1 key
        F2                  = 0x71, // F2 key
        F3                  = 0x72, // F3 key
        F4                  = 0x73, // F4 key
        F5                  = 0x74, // F5 key
        F6                  = 0x75, // F6 key
        F7                  = 0x76, // F7 key
        F8                  = 0x77, // F8 key
        F9                  = 0x78, // F9 key
        F10                 = 0x79, // F10 key
        F11                 = 0x7a, // F11 key
        F12                 = 0x7b, // F12 key
        F13                 = 0x7c, // F13 key
        F14                 = 0x7d, // F14 key
        F15                 = 0x7e, // F15 key
        F16                 = 0x7f, // F16 key
        F17                 = 0x80, // F17 key
        F18                 = 0x81, // F18 key
        F19                 = 0x82, // F19 key
        F20                 = 0x83, // F20 key
        F21                 = 0x84, // F21 key
        F22                 = 0x85, // F22 key
        F23                 = 0x86, // F23 key
        F24                 = 0x87, // F24 key
        UNASSIGNED_0x88     = 0x88,
        UNASSIGNED_0x89     = 0x89,
        UNASSIGNED_0x8A     = 0x8a,
        UNASSIGNED_0x8B     = 0x8b,
        UNASSIGNED_0x8C     = 0x8c,
        UNASSIGNED_0x8D     = 0x8d,
        UNASSIGNED_0x8E     = 0x8e,
        UNASSIGNED_0x8F     = 0x8f,
        NUMLOCK             = 0x90, // NUM LOCK key
        SCROLL              = 0x91, // SCROLL LOCK key
        OEM_SPECIFIC_0x92   = 0x92,
        OEM_SPECIFIC_0x93   = 0x93,
        OEM_SPECIFIC_0x94   = 0x94,
        OEM_SPECIFIC_0x95   = 0x95,
        OEM_SPECIFIC_0x96   = 0x96,
        UNASSIGNED_0x97     = 0x97,
        UNASSIGNED_0x98     = 0x98,
        UNASSIGNED_0x99     = 0x99,
        UNASSIGNED_0x9A     = 0x9a,
        UNASSIGNED_0x9B     = 0x9b,
        UNASSIGNED_0x9C     = 0x9c,
        UNASSIGNED_0x9D     = 0x9d,
        UNASSIGNED_0x9E     = 0x9e,
        UNASSIGNED_0x9F     = 0x9f,
        LSHIFT              = 0xa0, // Left SHIFT key
        RSHIFT              = 0xa1, // Right SHIFT key
        LCONTROL            = 0xa2, // Left CONTROL key
        RCONTROL            = 0xa3, // Right CONTROL key
        LMENU               = 0xa4, // Left MENU key
        RMENU               = 0xa5, // Right MENU key
        BROWSER_BACK        = 0xa6, // Browser Back key
        BROWSER_FORWARD     = 0xa7, // Browser Forward key
        BROWSER_REFRESH     = 0xa8, // Browser Refresh key
        BROWSER_STOP        = 0xa9, // Browser Stop key
        BROWSER_SEARCH      = 0xaa, // Browser Search key
        BROWSER_FAVORITES   = 0xab, // Browser Favorites key
        BROWSER_HOME        = 0xac, // Browser Start and Home key
        VOLUME_MUTE         = 0xad, // Volume Mute key
        VOLUME_DOWN         = 0xae, // Volume Down key
        VOLUME_UP           = 0xaf, // Volume Up key
        MEDIA_NEXT_TRACK    = 0xb0, // Next Track key
        MEDIA_PREV_TRACK    = 0xb1, // Previous Track key
        MEDIA_STOP          = 0xb2, // Stop Media key
        MEDIA_PLAY_PAUSE    = 0xb3, // Play/Pause Media key
        LAUNCH_MAIL         = 0xb4, // Start Mail key
        LAUNCH_MEDIA_SELECT = 0xb5, // Select Media key
        LAUNCH_APP1         = 0xb6, // Start Application 1 key
        LAUNCH_APP2         = 0xb7, // Start Application 2 key
        RESERVED_0xB8       = 0xb8,
        RESERVED_0xB9       = 0xb9,
        OEM_1               = 0xba, // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key
        OEM_PLUS            = 0xbb, // For any country/region, the '+' key
        OEM_COMMA           = 0xbc, // For any country/region, the ',' key
        OEM_MINUS           = 0xbd, // For any country/region, the '-' key
        OEM_PERIOD          = 0xbe, // For any country/region, the '.' key
        OEM_2               = 0xbf, // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key
        OEM_3               = 0xc0, // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key
        RESERVED_0xC1       = 0xc1,
        RESERVED_0xC2       = 0xc2,
        RESERVED_0xC3       = 0xc3,
        RESERVED_0xC4       = 0xc4,
        RESERVED_0xC5       = 0xc5,
        RESERVED_0xC6       = 0xc6,
        RESERVED_0xC7       = 0xc7,
        RESERVED_0xC8       = 0xc8,
        RESERVED_0xC9       = 0xc9,
        RESERVED_0xCA       = 0xca,
        RESERVED_0xCB       = 0xcb,
        RESERVED_0xCC       = 0xcc,
        RESERVED_0xCD       = 0xcd,
        RESERVED_0xCE       = 0xce,
        RESERVED_0xCF       = 0xcf,
        RESERVED_0xD1       = 0xd1,
        RESERVED_0xD2       = 0xd2,
        RESERVED_0xD3       = 0xd3,
        RESERVED_0xD4       = 0xd4,
        RESERVED_0xD5       = 0xd5,
        RESERVED_0xD6       = 0xd6,
        RESERVED_0xD7       = 0xd7,
        UNASSIGNED_0xD8     = 0xd8,
        UNASSIGNED_0xD9     = 0xd9,
        UNASSIGNED_0xDA     = 0xda,
        OEM_4               = 0xdb, // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key
        OEM_5               = 0xdc, // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\|' key
        OEM_6               = 0xdd, // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key
        OEM_7               = 0xde, // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ''"' key
        OEM_8               = 0xdf, // Used for miscellaneous characters; it can vary by keyboard.
        RESERVED_0xE0       = 0xe0,
        OEM_SPECIFIC_0xE1   = 0xe1,
        OEM_102             = 0xe2, // Either the angle bracket key or the backslash key on the RT 102-key keyboard
        OEM_SPECIFIC_0xE3   = 0xe3,
        OEM_SPECIFIC_0xE4   = 0xe4,
        PROCESSKEY          = 0xe5, // IME PROCESS key
        OEM_SPECIFIC_0xE6   = 0xe6,
        PACKET              = 0xe7, // Used to pass Unicode characters as if they were keystrokes. The PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP
        UNASSIGNED_0xE8     = 0xe8,
        OEM_SPECIFIC_0xE9   = 0xe9,
        OEM_SPECIFIC_0xEA   = 0xea,
        OEM_SPECIFIC_0xEB   = 0xeb,
        OEM_SPECIFIC_0xEC   = 0xec,
        OEM_SPECIFIC_0xED   = 0xed,
        OEM_SPECIFIC_0xEE   = 0xee,
        OEM_SPECIFIC_0xEF   = 0xef,
        OEM_SPECIFIC_0xF0   = 0xf0,
        OEM_SPECIFIC_0xF1   = 0xf1,
        OEM_SPECIFIC_0xF2   = 0xf2,
        OEM_SPECIFIC_0xF3   = 0xf3,
        OEM_SPECIFIC_0xF4   = 0xf4,
        OEM_SPECIFIC_0xF5   = 0xf5,
        ATTN                = 0xf6, // Attn key
        CRSEL               = 0xf7, // CrSel key
        EXSEL               = 0xf8, // ExSel key
        EREOF               = 0xf9, // Erase EOF key
        PLAY                = 0xfa, // Play key
        ZOOM                = 0xfb, // Zoom key
        NONAME              = 0xfc,
        PA1                 = 0xfd, // PA1 key
        OEM_CLEAR           = 0xfe, // Clear key
        RESERVED_0xFF       = 0xff
    };

    const int KeyCodes = 256; // KeyCodeCount? ...

    std::string to_string(KeyCode keycode);

    //--------------------------------------------------------------------------
    enum class KeyModifier
    {
        Null            = 0x0000,
        ShiftLeft       = 0x0001,
        ShiftRight      = 0x0002,
        Shift           = 0x0003, // Read only
        ControlLeft     = 0x0004,
        ControlRight    = 0x0008,
        Control         = 0x000c, // Read only
        AltLeft         = 0x0010,
        AltRight        = 0x0020,
        Alt             = 0x0030, // Read only
        CommandLeft     = 0x0040,
        CommandRight    = 0x0080,
        Command         = 0x00c0, // Read only
        CapsLock        = 0x0100,
        NumLock         = 0x0200,
        ScrollLock      = 0x0400,
        Toggle          = 0x0700 // Read only
    };

    // consts for combinations? ...

    typedef unsigned int Modifiers; // Key modifier flags

} // namespace device

} // namespace quetzal

#endif // QUETZAL_DEVICE_KEYCODE_HPP
