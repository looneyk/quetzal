//------------------------------------------------------------------------------
//
// Copyright (c) 2010 Keith Michael Looney. All Rights Reserved.
//
//------------------------------------------------------------------------------
//
// Device
// Keyboard.cpp
//
//------------------------------------------------------------------------------

#include <windows.h>
#include "Keyboard.hpp"

//------------------------------------------------------------------------------
quetzal::device::Keyboard::Keyboard()
{
    clear();
}

//------------------------------------------------------------------------------
quetzal::device::Keyboard::~Keyboard()
{
}

//------------------------------------------------------------------------------
void quetzal::device::Keyboard::clear()
{
    memset(&m_state, 0, sizeof m_state);

    return;
}

//------------------------------------------------------------------------------
void quetzal::device::Keyboard::sync()
{
    GetKeyboardState(reinterpret_cast<PBYTE>(&m_state));

    return;
}

//------------------------------------------------------------------------------
void quetzal::device::Keyboard::async()
{
    for (int i = 0; i < 256; ++i)
    {
        m_state[i] = GetAsyncKeyState(i) >> 8; // OK? ...
    }

    return;
}

//------------------------------------------------------------------------------
void quetzal::device::Keyboard::keyState(int key, bool bDown)
{
    m_state[key] = bDown ? 0x80 : 0x00;

    return;
}

//------------------------------------------------------------------------------
bool quetzal::device::Keyboard::keyDown(int key)
{
    return (m_state[key] & 0x80) != 0;
}
