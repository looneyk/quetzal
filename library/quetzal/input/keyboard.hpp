#if !defined(QUETZAL_INPUT_KEYBOARD_HPP)
#define QUETZAL_INPUT_KEYBOARD_HPP
//------------------------------------------------------------------------------
// keyboard.hpp
//------------------------------------------------------------------------------

#include <iosfwd>

namespace quetzal::input
{

    //--------------------------------------------------------------------------
    namespace key
    {

        //----------------------------------------------------------------------
        enum class Code
        {
            V_0x00_UNDEFINED        = 0x00, // Undefined
            V_LBUTTON               = 0x01, // Left mouse button
            V_RBUTTON               = 0x02, // Right mouse button
            V_CANCEL                = 0x03, // Control-break processing
            V_MBUTTON               = 0x04, // Middle mouse button (three-button mouse)
            V_XBUTTON1              = 0x05, // X1 mouse button
            V_XBUTTON2              = 0x06, // X2 mouse button
            V_0x07_UNDEFINED        = 0x07, // Undefined
            V_BACK                  = 0x08, // BACKSPACE key
            V_TAB                   = 0x09, // TAB key
            V_0x0A_RESERVED         = 0x0a, // Reserved
            V_0x0B_RESERVED         = 0x0b, // Reserved
            V_CLEAR                 = 0x0c, // CLEAR key
            V_RETURN                = 0x0d, // ENTER key
            V_0x0E_UNDEFINED        = 0x0e, // Undefined
            V_0x0F_UNDEFINED        = 0x0f, // Undefined
            V_SHIFT                 = 0x10, // SHIFT key
            V_CONTROL               = 0x11, // CTRL key
            V_MENU                  = 0x12, // ALT key
            V_PAUSE                 = 0x13, // PAUSE key
            V_CAPITAL               = 0x14, // CAPS LOCK key
            V_KANA                  = 0x15, // IME Kana mode, IME Hangul mode
            V_0x16_UNDEFINED        = 0x16, // Undefined
            V_JUNJA                 = 0x17, // IME Junja mode
            V_FINAL                 = 0x18, // IME final mode
            V_HANJA                 = 0x19, // IME Hanja mode
            V_KANJI                 = 0x19, // IME Kanji mode
            V_0x1A_UNDEFINED        = 0x1a, // Undefined
            V_ESCAPE                = 0x1b, // ESC key
            V_CONVERT               = 0x1c, // IME convert
            V_NONCONVERT            = 0x1d, // IME nonconvert
            V_ACCEPT                = 0x1e, // IME accept
            V_MODECHANGE            = 0x1f, // IME mode change request
            V_SPACE                 = 0x20, // SPACEBAR
            V_PRIOR                 = 0x21, // PAGE UP key
            V_NEXT                  = 0x22, // PAGE DOWN key
            V_END                   = 0x23, // END key
            V_HOME                  = 0x24, // HOME key
            V_LEFT                  = 0x25, // LEFT ARROW key
            V_UP                    = 0x26, // UP ARROW key
            V_RIGHT                 = 0x27, // RIGHT ARROW key
            V_DOWN                  = 0x28, // DOWN ARROW key
            V_SELECT                = 0x29, // SELECT key
            V_PRINT                 = 0x2a, // PRINT key
            V_EXECUTE               = 0x2b, // EXECUTE key
            V_SNAPSHOT              = 0x2c, // PRINT SCREEN key
            V_INSERT                = 0x2d, // INS key
            V_DELETE                = 0x2e, // DEL key
            V_HELP                  = 0x2f, // HELP key
            V_0                     = 0x30,
            V_1                     = 0x31,
            V_2                     = 0x32,
            V_3                     = 0x33,
            V_4                     = 0x34,
            V_5                     = 0x35,
            V_6                     = 0x36,
            V_7                     = 0x37,
            V_8                     = 0x38,
            V_9                     = 0x39,
            V_0x3A_UNDEFINED        = 0x3a, // Undefined
            V_0x3B_UNDEFINED        = 0x3b, // Undefined
            V_0x3C_UNDEFINED        = 0x3c, // Undefined
            V_0x3D_UNDEFINED        = 0x3d, // Undefined
            V_0x3E_UNDEFINED        = 0x3e, // Undefined
            V_0x3F_UNDEFINED        = 0x3f, // Undefined
            V_0x40_UNDEFINED        = 0x40, // Undefined
            V_A                     = 0x41,
            V_B                     = 0x42,
            V_C                     = 0x43,
            V_D                     = 0x44,
            V_E                     = 0x45,
            V_F                     = 0x46,
            V_G                     = 0x47,
            V_H                     = 0x48,
            V_I                     = 0x49,
            V_J                     = 0x4a,
            V_K                     = 0x4b,
            V_L                     = 0x4c,
            V_M                     = 0x4d,
            V_N                     = 0x4e,
            V_O                     = 0x4f,
            V_P                     = 0x50,
            V_Q                     = 0x51,
            V_R                     = 0x52,
            V_S                     = 0x53,
            V_T                     = 0x54,
            V_U                     = 0x55,
            V_V                     = 0x56,
            V_W                     = 0x57,
            V_X                     = 0x58,
            V_Y                     = 0x59,
            V_Z                     = 0x5a,
            V_LWIN                  = 0x5b, // Left Windows key (Natural keyboard)
            V_RWIN                  = 0x5c, // Right Windows key (Natural keyboard)
            V_APPS                  = 0x5d, // Applications key (Natural keyboard)
            V_0x5E_RESERVED         = 0x5e, // Reserved
            V_SLEEP                 = 0x5f, // Computer Sleep key
            V_NUMPAD0               = 0x60, // Numeric keypad 0 key
            V_NUMPAD1               = 0x61, // Numeric keypad 1 key
            V_NUMPAD2               = 0x62, // Numeric keypad 2 key
            V_NUMPAD3               = 0x63, // Numeric keypad 3 key
            V_NUMPAD4               = 0x64, // Numeric keypad 4 key
            V_NUMPAD5               = 0x65, // Numeric keypad 5 key
            V_NUMPAD6               = 0x66, // Numeric keypad 6 key
            V_NUMPAD7               = 0x67, // Numeric keypad 7 key
            V_NUMPAD8               = 0x68, // Numeric keypad 8 key
            V_NUMPAD9               = 0x69, // Numeric keypad 9 key
            V_MULTIPLY              = 0x6a, // Multiply key
            V_ADD                   = 0x6b, // Add key
            V_SEPARATOR             = 0x6c, // Separator key
            V_SUBTRACT              = 0x6d, // Subtract key
            V_DECIMAL               = 0x6e, // Decimal key
            V_DIVIDE                = 0x6f, // Divide key
            V_F1                    = 0x70, // F1 key
            V_F2                    = 0x71, // F2 key
            V_F3                    = 0x72, // F3 key
            V_F4                    = 0x73, // F4 key
            V_F5                    = 0x74, // F5 key
            V_F6                    = 0x75, // F6 key
            V_F7                    = 0x76, // F7 key
            V_F8                    = 0x77, // F8 key
            V_F9                    = 0x78, // F9 key
            V_F10                   = 0x79, // F10 key
            V_F11                   = 0x7a, // F11 key
            V_F12                   = 0x7b, // F12 key
            V_F13                   = 0x7c, // F13 key
            V_F14                   = 0x7d, // F14 key
            V_F15                   = 0x7e, // F15 key
            V_F16                   = 0x7f, // F16 key
            V_F17                   = 0x80, // F17 key
            V_F18                   = 0x81, // F18 key
            V_F19                   = 0x82, // F19 key
            V_F20                   = 0x83, // F20 key
            V_F21                   = 0x84, // F21 key
            V_F22                   = 0x85, // F22 key
            V_F23                   = 0x86, // F23 key
            V_F24                   = 0x87, // F24 key
            V_0x88_UNASSIGNED       = 0x88, // Unassigned
            V_0x89_UNASSIGNED       = 0x89, // Unassigned
            V_0x8A_UNASSIGNED       = 0x8a, // Unassigned
            V_0x8B_UNASSIGNED       = 0x8b, // Unassigned
            V_0x8C_UNASSIGNED       = 0x8c, // Unassigned
            V_0x8D_UNASSIGNED       = 0x8d, // Unassigned
            V_0x8E_UNASSIGNED       = 0x8e, // Unassigned
            V_0x8F_UNASSIGNED       = 0x8f, // Unassigned
            V_NUMLOCK               = 0x90, // NUM LOCK key
            V_SCROLL                = 0x91, // SCROLL LOCK key
            V_0x92_OEM_SPECIFIC     = 0x92, // OEM specific
            V_0x93_OEM_SPECIFIC     = 0x93, // OEM specific
            V_0x94_OEM_SPECIFIC     = 0x94, // OEM specific
            V_0x95_OEM_SPECIFIC     = 0x95, // OEM specific
            V_0x96_OEM_SPECIFIC     = 0x96, // OEM specific
            V_0x97_UNASSIGNED       = 0x97, // Unassigned
            V_0x98_UNASSIGNED       = 0x98, // Unassigned
            V_0x99_UNASSIGNED       = 0x99, // Unassigned
            V_0x9A_UNASSIGNED       = 0x9a, // Unassigned
            V_0x9B_UNASSIGNED       = 0x9b, // Unassigned
            V_0x9C_UNASSIGNED       = 0x9c, // Unassigned
            V_0x9D_UNASSIGNED       = 0x9d, // Unassigned
            V_0x9E_UNASSIGNED       = 0x9e, // Unassigned
            V_0x9F_UNASSIGNED       = 0x9f, // Unassigned
            V_LSHIFT                = 0xa0, // Left SHIFT key
            V_RSHIFT                = 0xa1, // Right SHIFT key
            V_LCONTROL              = 0xa2, // Left CONTROL key
            V_RCONTROL              = 0xa3, // Right CONTROL key
            V_LMENU                 = 0xa4, // Left MENU key
            V_RMENU                 = 0xa5, // Right MENU key
            V_BROWSER_BACK          = 0xa6, // Browser Back key
            V_BROWSER_FORWARD       = 0xa7, // Browser Forward key
            V_BROWSER_REFRESH       = 0xa8, // Browser Refresh key
            V_BROWSER_STOP          = 0xa9, // Browser Stop key
            V_BROWSER_SEARCH        = 0xaa, // Browser Search key
            V_BROWSER_FAVORITES     = 0xab, // Browser Favorites key
            V_BROWSER_HOME          = 0xac, // Browser Start and Home key
            V_VOLUME_MUTE           = 0xad, // Volume Mute key
            V_VOLUME_DOWN           = 0xae, // Volume Down key
            V_VOLUME_UP             = 0xaf, // Volume Up key
            V_MEDIA_NEXT_TRACK      = 0xb0, // Next Track key
            V_MEDIA_PREV_TRACK      = 0xb1, // Previous Track key
            V_MEDIA_STOP            = 0xb2, // Stop Media key
            V_MEDIA_PLAY_PAUSE      = 0xb3, // Play/Pause Media key
            V_LAUNCH_MAIL           = 0xb4, // Start Mail key
            V_LAUNCH_MEDIA_SELECT   = 0xb5, // Select Media key
            V_LAUNCH_APP1           = 0xb6, // Start Application 1 key
            V_LAUNCH_APP2           = 0xb7, // Start Application 2 key
            V_0xB8_RESERVED         = 0xb8, // Reserved
            V_0xB9_RESERVED         = 0xb9, // Reserved
            V_OEM_1                 = 0xba, // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key
            V_OEM_PLUS              = 0xbb, // For any country/region, the '+' key
            V_OEM_COMMA             = 0xbc, // For any country/region, the ',' key
            V_OEM_MINUS             = 0xbd, // For any country/region, the '-' key
            V_OEM_PERIOD            = 0xbe, // For any country/region, the '.' key
            V_OEM_2                 = 0xbf, // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key
            V_OEM_3                 = 0xc0, // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key
            V_0xC1_RESERVED         = 0xc1, // Reserved
            V_0xC2_RESERVED         = 0xc2, // Reserved
            V_0xC3_RESERVED         = 0xc3, // Reserved
            V_0xC4_RESERVED         = 0xc4, // Reserved
            V_0xC5_RESERVED         = 0xc5, // Reserved
            V_0xC6_RESERVED         = 0xc6, // Reserved
            V_0xC7_RESERVED         = 0xc7, // Reserved
            V_0xC8_RESERVED         = 0xc8, // Reserved
            V_0xC9_RESERVED         = 0xc9, // Reserved
            V_0xCA_RESERVED         = 0xca, // Reserved
            V_0xCB_RESERVED         = 0xcb, // Reserved
            V_0xCC_RESERVED         = 0xcc, // Reserved
            V_0xCD_RESERVED         = 0xcd, // Reserved
            V_0xCE_RESERVED         = 0xce, // Reserved
            V_0xCF_RESERVED         = 0xcf, // Reserved
            V_0xD1_RESERVED         = 0xd1, // Reserved
            V_0xD2_RESERVED         = 0xd2, // Reserved
            V_0xD3_RESERVED         = 0xd3, // Reserved
            V_0xD4_RESERVED         = 0xd4, // Reserved
            V_0xD5_RESERVED         = 0xd5, // Reserved
            V_0xD6_RESERVED         = 0xd6, // Reserved
            V_0xD7_RESERVED         = 0xd7, // Reserved
            V_0xD8_UNASSIGNED       = 0xd8, // Unassigned
            V_0xD9_UNASSIGNED       = 0xd9, // Unassigned
            V_0xDA_UNASSIGNED       = 0xda, // Unassigned
            V_OEM_4                 = 0xdb, // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key
            V_OEM_5                 = 0xdc, // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\|' key
            V_OEM_6                 = 0xdd, // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key
            V_OEM_7                 = 0xde, // Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ''"' key
            V_OEM_8                 = 0xdf, // Used for miscellaneous characters; it can vary by keyboard.
            V_0xE0_RESERVED         = 0xe0, // Reserved
            V_0xE1_OEM_SPECIFIC     = 0xe1, // OEM specific
            V_OEM_102               = 0xe2, // Either the angle bracket key or the backslash key on the RT 102-key keyboard
            V_0xE3_OEM_SPECIFIC     = 0xe3, // OEM specific
            V_0xE4_OEM_SPECIFIC     = 0xe4, // OEM specific
            V_PROCESSKEY            = 0xe5, // IME PROCESS key
            V_0xE6_OEM_SPECIFIC     = 0xe6, // OEM specific
            V_PACKET                = 0xe7, // Used to pass Unicode characters as if they were keystrokes. The V_PACKET key is th e low word of a 32-bit Virtual key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP
            V_0xE8_UNASSIGNED       = 0xe8, // Unassigned
            V_0xE9_OEM_SPECIFIC     = 0xe9, // OEM specific
            V_0xEA_OEM_SPECIFIC     = 0xea, // OEM specific
            V_0xEB_OEM_SPECIFIC     = 0xeb, // OEM specific
            V_0xEC_OEM_SPECIFIC     = 0xec, // OEM specific
            V_0xED_OEM_SPECIFIC     = 0xed, // OEM specific
            V_0xEE_OEM_SPECIFIC     = 0xee, // OEM specific
            V_0xEF_OEM_SPECIFIC     = 0xef, // OEM specific
            V_0xF0_OEM_SPECIFIC     = 0xf0, // OEM specific
            V_0xF1_OEM_SPECIFIC     = 0xf1, // OEM specific
            V_0xF2_OEM_SPECIFIC     = 0xf2, // OEM specific
            V_0xF3_OEM_SPECIFIC     = 0xf3, // OEM specific
            V_0xF4_OEM_SPECIFIC     = 0xf4, // OEM specific
            V_0xF5_OEM_SPECIFIC     = 0xf5, // OEM specific
            V_ATTN                  = 0xf6, // Attn key
            V_CRSEL                 = 0xf7, // CrSel key
            V_EXSEL                 = 0xf8, // ExSel key
            V_EREOF                 = 0xf9, // Erase EOF key
            V_PLAY                  = 0xfa, // Play key
            V_ZOOM                  = 0xfb, // Zoom key
            V_NONAME                = 0xfc, // Reserved
            V_PA1                   = 0xfd, // PA1 key
            V_OEM_CLEAR             = 0xfe, // Clear key
            V_RESERVED_FF           = 0xff
        };

        //----------------------------------------------------------------------
        class Modifier
        {
        public:

            Modifier(unsigned int f) :
                value(f)
            {}

            static const unsigned int ShiftLeft    = 0x0001;
            static const unsigned int ShiftRight   = 0x0002;
            static const unsigned int Shift        = 0x0003;
            static const unsigned int ControlLeft  = 0x0004;
            static const unsigned int ControlRight = 0x0008;
            static const unsigned int Control      = 0x000c;
            static const unsigned int AltLeft      = 0x0010;
            static const unsigned int AltRight     = 0x0020;
            static const unsigned int Alt          = 0x0030;
            static const unsigned int CommandLeft  = 0x0040;
            static const unsigned int CommandRight = 0x0080;
            static const unsigned int Command      = 0x00c0;
            static const unsigned int CapsLock     = 0x0100;
            static const unsigned int NumLock      = 0x0200;
            static const unsigned int ScrollLock   = 0x0400;
            static const unsigned int Toggle       = 0x0700;
            static const unsigned int Absolute     = 0x0800;

            unsigned int value;
        };

        typedef unsigned int Modifiers; // key modifier flags

        const size_t Codes = 256; // CodeCount? ...

    } // namespace key

    key::Code translate_key(key::Code code, unsigned int flags);

    bool query_key_down(key::Code code);
    key::Modifiers query_key_modifiers();

    std::ostream& operator<<(std::ostream& os, key::Code code);

} // namespace quetzal::input

#endif // QUETZAL_INPUT_KEYBOARD_HPP
