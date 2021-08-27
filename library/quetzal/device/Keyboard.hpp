//------------------------------------------------------------------------------
// Keyboard.hpp
//
//------------------------------------------------------------------------------

#if !defined(QUETZAL_DEVICE_KEYBOARD_HPP)
#define QUETZAL_DEVICE_KEYBOARD_HPP

#include "Key.hpp"
#include <vector>

namespace quetzal
{

namespace device
{

    //----------------------------------------------------------------------
    class KeyModifier
    {
    public:

        //--------------------------------------------------------------
        enum ModifierFlag
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

        typedef unsigned int Modifiers; // Key modifier flags
    };

    //----------------------------------------------------------------------
    class Keyboard
    {
    public:

        Keyboard();
        ~Keyboard();

        void poll();

        bool key_down(int keycode) const;
        bool key_toggled(int keycode) const;

        void invalidate();

        KeyModifier::Modifiers modifiers();

    protected:

        std::vector<unsigned char> m_state;
        bool m_bValid;

    private:

        Keyboard(const Keyboard&); // Not copyable
        Keyboard& operator=(const Keyboard&); // Not assignable
    };

} // namespace device

} // namespace quetzal

#endif // QUETZAL_DEVICE_KEYBOARD_HPP
