//------------------------------------------------------------------------------
// PropertyCustom.cpp
//
//------------------------------------------------------------------------------

#include "PropertyCustom.hpp"
#include <cassert>

using namespace std;

//----------------------------------------------------------------------
quetzal::property::PropertyCustom::PropertyCustom(const string& name, const string& default_value) :
    Property(name, default_value, false, true),
    m_value(default_value)
{
}

//----------------------------------------------------------------------
quetzal::property::PropertyCustom::PropertyCustom(const PropertyCustom& property) :
    Property(property),
    m_value(property.m_value)
{
}

//----------------------------------------------------------------------
quetzal::property::PropertyCustom::~PropertyCustom()
{
}

//----------------------------------------------------------------------
quetzal::property::PropertyCustom& quetzal::property::PropertyCustom::operator=(const PropertyCustom& property)
{
    if (this != &property)
    {
        Property::operator=(property);
        m_value = property.m_value;
    }

    return *this;
}

//----------------------------------------------------------------------
string quetzal::property::PropertyCustom::get(const PropertyTarget* p) const
{
    assert(p != nullptr);
    p;

    return m_value;
}

//----------------------------------------------------------------------
void quetzal::property::PropertyCustom::set(PropertyTarget* p, const string& value)
{
    assert(p != nullptr);
    p;

    m_value = value;

    return;
}
