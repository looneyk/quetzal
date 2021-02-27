//------------------------------------------------------------------------------
// quetzal
// Properties.cpp
//------------------------------------------------------------------------------

#include "Properties.hpp"
#include <iostream>

using namespace std;

//------------------------------------------------------------------------------
string quetzal::Properties::get(const string& name) const
{
    if (m_values.contains(name))
    {
        return m_values.at(name);
    }

    return "";
}

//------------------------------------------------------------------------------
void quetzal::Properties::set(const string& name, const string& value)
{
    m_values[name] = value;
    return;
}

//------------------------------------------------------------------------------
void quetzal::Properties::set(const Properties& properties)
{
    for (const auto& value : properties.m_values)
    {
        m_values[value.first] = value.second;
    }
    return;
}

//------------------------------------------------------------------------------
bool quetzal::Properties::contains(const string& name) const
{
    return m_values.contains(name);
}

//------------------------------------------------------------------------------
void quetzal::Properties::erase(const string& name)
{
    m_values.erase(name);
    return;
}

//------------------------------------------------------------------------------
void quetzal::Properties::clear()
{
    m_values.clear();
    return;
}

//------------------------------------------------------------------------------
void quetzal::Properties::print(ostream& os) const
{
    bool bFirst = true;

    os << "{";
    for (const auto& x : m_values)
    {
        if (!bFirst)
        {
            os << ",";
        }

        os << x.first << ": " << x.second;
        bFirst = false;
    }

    os << "}";
    return;
}

//------------------------------------------------------------------------------
ostream& quetzal::operator<<(ostream& os, const Properties& properties)
{
    properties.print(os);
    return os;
}
