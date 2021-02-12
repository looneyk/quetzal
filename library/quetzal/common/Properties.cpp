//------------------------------------------------------------------------------
// quetzal
// Properties.cpp
//------------------------------------------------------------------------------

#include "Properties.hpp"

using namespace std;

//------------------------------------------------------------------------------
string quetzal::Properties::property(const string& name) const
{
    if (contains_property(name))
    {
        return m_values.at(name);
    }

    return "";
}

//------------------------------------------------------------------------------
void quetzal::Properties::set_property(const string& name, const string& value)
{
    m_values[name] = value;
    return;
}

//------------------------------------------------------------------------------
bool quetzal::Properties::contains_property(const string& name) const
{
    return m_values.contains(name);
}

//------------------------------------------------------------------------------
void quetzal::Properties::remove_property(const string& name)
{
    if (contains_property(name))
    {
        m_values.erase(name);
    }

    return;
}

//------------------------------------------------------------------------------
void quetzal::Properties::clear_properties()
{
    m_values.clear();
    return;
}
