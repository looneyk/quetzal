//------------------------------------------------------------------------------
// Property.cpp
//
//------------------------------------------------------------------------------

#include "Property.hpp"

using namespace std;

//------------------------------------------------------------------------------
quetzal::property::Property::Property(const string& name, const string& default_value, bool bInternal, bool bCustom) :
    m_name(name),
    m_valueDefault(default_value),
    m_bInternal(bInternal),
    m_bCustom(bCustom)
{
}

//------------------------------------------------------------------------------
quetzal::property::Property::~Property()
{
}

//------------------------------------------------------------------------------
string quetzal::property::Property::name() const
{
    return m_name;
}

//------------------------------------------------------------------------------
void quetzal::property::Property::set_name(const string& name)
{
    m_name = name;

    return;
}

//------------------------------------------------------------------------------
string quetzal::property::Property::default_value() const
{
    return m_valueDefault;
}

//------------------------------------------------------------------------------
void quetzal::property::Property::set_default_value(const string& default_value)
{
    m_valueDefault = default_value;

    return;
}

//------------------------------------------------------------------------------
bool quetzal::property::Property::internal() const
{
    return m_bInternal;
}

//------------------------------------------------------------------------------
void quetzal::property::Property::set_internal(bool b)
{
    m_bInternal = b;

    return;
}

//------------------------------------------------------------------------------
bool quetzal::property::Property::custom() const
{
    return m_bCustom;
}

//------------------------------------------------------------------------------
void quetzal::property::Property::set_custom(bool b)
{
    m_bCustom = b;

    return;
}
