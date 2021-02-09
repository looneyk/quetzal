//------------------------------------------------------------------------------
// XML file io
// Attribute.cpp
//------------------------------------------------------------------------------

#include "Attribute.hpp"
#include <iostream>

using namespace std;

//------------------------------------------------------------------------------
quetzal::xml::Attribute::Attribute(const string& name, const string& value) :
    m_name(name),
    m_value(value)
{
}

//------------------------------------------------------------------------------
string quetzal::xml::Attribute::name() const
{
    return m_name;
}

//------------------------------------------------------------------------------
string quetzal::xml::Attribute::value() const
{
    return m_value;
}

//------------------------------------------------------------------------------
void quetzal::xml::Attribute::set_name(const string& name)
{
    m_name = name;
    return;
}

//------------------------------------------------------------------------------
void quetzal::xml::Attribute::set_value(const string& value)
{
    m_value = value;
    return;
}

//------------------------------------------------------------------------------
ostream& quetzal::xml::operator<<(ostream& os, const quetzal::xml::Attribute& attribute)
{
    os << attribute.name() << "=\"" << attribute.value() << "\""; // quote style, entity substitution (denormalize?)...

    return os;
}
