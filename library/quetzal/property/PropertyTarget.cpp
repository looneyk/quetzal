//------------------------------------------------------------------------------
// PropertyTarget.cpp
//
//------------------------------------------------------------------------------

#include "PropertyTarget.hpp"
#include "PropertyCustom.hpp"
#include "quetzal/Exception.hpp"
#include <sstream>
#include <cassert>

using namespace std;

//------------------------------------------------------------------------------
quetzal::property::PropertyTarget::PropertyTarget() :
    m_properties()
{
}

//------------------------------------------------------------------------------
quetzal::property::PropertyTarget::~PropertyTarget()
{
}

//------------------------------------------------------------------------------
string quetzal::property::PropertyTarget::property(const string& name) const
{
    Properties::const_iterator i = m_properties.find(name);
    if (i != m_properties.end())
    {
        return i->second->get(this);
    }

    return string();
}

//------------------------------------------------------------------------------
void quetzal::property::PropertyTarget::set_property(const string& name, const string& value)
{
    Properties::const_iterator i = m_properties.find(name);
    if (i != m_properties.end())
    {
        i->second->set(this, value);
        return;
    }

    register_property(name);

    set_property(name, value);

    return;
}

//------------------------------------------------------------------------------
string quetzal::property::PropertyTarget::property_default(const string& name) const
{
    Properties::const_iterator i = m_properties.find(name);
    if (i != m_properties.end())
    {
        return i->second->default_value();
    }

    return string();
}

//------------------------------------------------------------------------------
void quetzal::property::PropertyTarget::set_property_default(const string& name, const string& default_value)
{
    Properties::const_iterator i = m_properties.find(name);
    if (i != m_properties.end())
    {
        i->second->set_default_value(default_value);
        return;
    }

    register_property(name, default_value);

    return;
}

//------------------------------------------------------------------------------
void quetzal::property::PropertyTarget::register_property(const string& name, const string& default_value)
{
    assert(m_properties.find(name) == m_properties.end());

    m_properties[name] = std::shared_ptr<Property>(new PropertyCustom(name, default_value));

    return;
}

//------------------------------------------------------------------------------
void quetzal::property::PropertyTarget::register_property(std::shared_ptr<Property> pproperty)
{
    assert(pproperty != nullptr);

    assert(m_properties.find(pproperty->name()) == m_properties.end());

    m_properties[pproperty->name()] = pproperty;

    return;
}

//------------------------------------------------------------------------------
void quetzal::property::PropertyTarget::remove_property(const string& name)
{
    Properties::const_iterator i = m_properties.find(name);
    if (i != m_properties.end())
    {
        if (i->second->custom())
        {
            m_properties.erase(i);
        }
    }

    return;
}

//------------------------------------------------------------------------------
void quetzal::property::PropertyTarget::reset_properties()
{
    Properties::iterator iEnd = m_properties.end();
    for (Properties::iterator i = m_properties.begin(); i != iEnd; )
    {
        Properties::iterator j = i;
        ++i;

        if (j->second->custom())
        {
            m_properties.erase(j);
        }
    }

    return;
}

//------------------------------------------------------------------------------
const quetzal::property::PropertyTarget::Properties& quetzal::property::PropertyTarget::properties() const
{
    return m_properties;
}

//------------------------------------------------------------------------------
quetzal::property::PropertyTarget::Properties& quetzal::property::PropertyTarget::properties()
{
    return m_properties;
}
