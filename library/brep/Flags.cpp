//------------------------------------------------------------------------------
// brep
// Flags.hpp
//------------------------------------------------------------------------------

#include "Flags.hpp"
#include <type_traits>
#include <iostream>

using namespace std;

namespace
{

    //--------------------------------------------------------------------------
    enum class Bits : uint32_t
    {
        Deleted = 0x00000001,
        Checked = 0x00000002,
        Marked  = 0x00000004
    };

} // namespace

//------------------------------------------------------------------------------
bool quetzal::brep::Flags::deleted() const
{
    return (m_bits & static_cast<underlying_type<Bits>::type>(Bits::Deleted)) != 0;
}

//------------------------------------------------------------------------------
void quetzal::brep::Flags::set_deleted(bool b) const
{
    if (b)
    {
        m_bits |= static_cast<underlying_type<Bits>::type>(Bits::Deleted);
    }
    else
    {
        m_bits &= ~static_cast<underlying_type<Bits>::type>(Bits::Deleted);
    }

    return;
}

//------------------------------------------------------------------------------
bool quetzal::brep::Flags::checked() const
{
    return (m_bits & static_cast<underlying_type<Bits>::type>(Bits::Checked)) != 0;
}

//------------------------------------------------------------------------------
void quetzal::brep::Flags::set_checked(bool b) const
{
    if (b)
    {
        m_bits |= static_cast<underlying_type<Bits>::type>(Bits::Checked);
    }
    else
    {
        m_bits &= ~static_cast<underlying_type<Bits>::type>(Bits::Checked);
    }

    return;
}

//------------------------------------------------------------------------------
bool quetzal::brep::Flags::marked() const
{
    return (m_bits & static_cast<underlying_type<Bits>::type>(Bits::Marked)) != 0;
}

//------------------------------------------------------------------------------
void quetzal::brep::Flags::set_marked(bool b) const
{
    if (b)
    {
        m_bits |= static_cast<underlying_type<Bits>::type>(Bits::Marked);
    }
    else
    {
        m_bits &= ~static_cast<underlying_type<Bits>::type>(Bits::Marked);
    }

    return;
}

//------------------------------------------------------------------------------
bool quetzal::brep::Flags::unset() const
{
    return deleted() ? true : empty();
}

//------------------------------------------------------------------------------
void quetzal::brep::Flags::reset() const
{
    m_bits &= static_cast<underlying_type<Bits>::type>(Bits::Deleted);
    return;
}

//------------------------------------------------------------------------------
bool quetzal::brep::Flags::empty() const
{
    return m_bits == 0;
}

//------------------------------------------------------------------------------
void quetzal::brep::Flags::clear() const
{
    m_bits = 0;
    return;
}

//------------------------------------------------------------------------------
ostream& quetzal::brep::operator<<(ostream& os, const Flags& flags)
{
    os << (flags.deleted() ? "D" : "-");
    os << (flags.checked() ? "C" : "-");
    os << (flags.marked() ? "M" : "-");
    return os;
}
