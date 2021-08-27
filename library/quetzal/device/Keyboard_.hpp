//------------------------------------------------------------------------------
//
// Copyright (c) 2010 Keith Michael Looney. All Rights Reserved.
//
//------------------------------------------------------------------------------
//
// Device
// Keyboard.hpp
//
// Enfore singleness? ...
//
//------------------------------------------------------------------------------

#if !defined(QUETZAL_DEVICE_KEYBOARD_HPP)
#define QUETZAL_DEVICE_KEYBOARD_HPP

namespace quetzal
{

namespace device
{

    //----------------------------------------------------------------------
    class Keyboard
    {
    public:

        Keyboard();
        ~Keyboard();

        void clear();

        void sync(); // ...
        void async(); // ...

        void keyState(int key, bool bDown);

        bool keyDown(int key); 

    private:

        Keyboard(const Keyboard&); // Not copyable
        Keyboard& operator=(const Keyboard&); // Not copyable

        unsigned char m_state[256];
    };

} // namespace device

} // namespace quetzal

#endif // QUETZAL_DEVICE_KEYBOARD_HPP
