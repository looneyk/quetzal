//------------------------------------------------------------------------------
// Keycache.cpp
//
//------------------------------------------------------------------------------

#include "Keycache.hpp"
#include <cassert>

typedef quetzal::device::Key Key;

namespace
{


} // namespace

//------------------------------------------------------------------------------
quetzal::device::Keycache::Keycache() :
    m_state(Key::VirtualKeys)
{
}

//------------------------------------------------------------------------------
quetzal::device::Keycache::~Keycache()
{
}

//------------------------------------------------------------------------------
void quetzal::device::Keycache::clear()
{
    memset(&m_state[0], 0, m_state.size() * sizeof m_state[0]);

    return;
}

//------------------------------------------------------------------------------
bool quetzal::device::Keycache::key_down(int keycode) const
{
    assert(keycode > 0 && keycode < Key::VirtualKeys);

    return (m_state[keycode] & 0x80) != 0;
}

//------------------------------------------------------------------------------
bool quetzal::device::Keycache::key_toggled(int keycode) const
{
    assert(keycode > 0 && keycode < Key::VirtualKeys);

    return (m_state[keycode] & 0x01) != 0;
}

//------------------------------------------------------------------------------
void quetzal::device::Keycache::set_key_down(int keycode, bool b)
{
    if (b)
    {
        m_state[keycode] |= 0x80;
    }
    else
    {
        m_state[keycode] &= ~0x80;
    }

    return;
}

//------------------------------------------------------------------------------
void quetzal::device::Keycache::set_key_toggled(int keycode, bool b)
{
    if (b)
    {
        m_state[keycode] |= 0x01;
    }
    else
    {
        m_state[keycode] &= ~0x01;
    }

    return;
}
