//------------------------------------------------------------------------------
// Arguments.cpp
//
//------------------------------------------------------------------------------

#include "Arguments.hpp"
#include <cassert>

//------------------------------------------------------------------------------
quetzal::Argument::Argument() :
    m_name(),
    m_nParams(0),
    m_bValueRequired(false),
    m_valueUnspecified(),
    m_valueDefault()
{
}

//------------------------------------------------------------------------------
quetzal::Argument::Argument(const std::string& name, size_type nParams, bool bValueRequired, const std::string& valueUnspecified, const std::string& valueDefault) :
    m_name(name),
    m_nParams(nParams),
    m_bValueRequired(bValueRequired),
    m_valueUnspecified(valueUnspecified),
    m_valueDefault(valueDefault)
{
}

//------------------------------------------------------------------------------
quetzal::Argument::Argument(const Argument& a) :
    m_name(a.m_name),
    m_nParams(a.m_nParams),
    m_bValueRequired(a.m_bValueRequired),
    m_valueUnspecified(a.m_valueUnspecified),
    m_valueDefault(a.m_valueDefault)
{
}

//------------------------------------------------------------------------------
quetzal::Argument::~Argument()
{
}

//------------------------------------------------------------------------------
quetzal::Argument& quetzal::Argument::operator=(const Argument& a)
{
    if (this != &a)
    {
        m_name = a.m_name;
        m_nParams = a.m_nParams;
        m_bValueRequired = a.m_bValueRequired;
        m_valueUnspecified = a.m_valueUnspecified;
        m_valueDefault = a.m_valueDefault;
    }

    return *this;
}

//------------------------------------------------------------------------------
std::string quetzal::Argument::name() const
{
    return m_name;
}

//------------------------------------------------------------------------------
quetzal::Argument::size_type quetzal::Argument::params() const
{
    return m_nParams;
}

//------------------------------------------------------------------------------
bool quetzal::Argument::value_required() const
{
    return m_bValueRequired;
}

//------------------------------------------------------------------------------
std::string quetzal::Argument::unspecified_value() const
{
    return m_valueUnspecified;
}

//------------------------------------------------------------------------------
std::string quetzal::Argument::default_value() const
{
    return m_valueDefault;
}

//------------------------------------------------------------------------------
quetzal::Arguments::Arguments() :
    m_named(),
    m_options(),
    m_values(),
    m_positional()
{
}

//------------------------------------------------------------------------------
quetzal::Arguments::~Arguments()
{
}

//------------------------------------------------------------------------------
void quetzal::Arguments::register_argument(const std::string& name, const std::string& long_option, const std::string& short_option, size_type nParams, bool bValueRequired, const std::string& valueUnspecified, const std::string& valueDefault)
{
    m_named[name] = Argument(name, nParams, bValueRequired, valueUnspecified, valueDefault);

    if (!long_option.empty())
    {
        m_options[long_option] = name;
    }

    if (!short_option.empty())
    {
        m_options[short_option] = name;
    }

    return;
}

//------------------------------------------------------------------------------
void quetzal::Arguments::parse(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        Options::const_iterator j = m_options.find(argv[i]);
        if (j != m_options.end())
        {
            Named::const_iterator k = m_named.find(j->second);
            assert(k != m_named.end());
            const Argument& argument = k->second;

            std::string name = argument.name();
            std::string value;

            bool bOptionNext = false;
            if (!argument.value_required())
            {
                // End of args qualifies as a virtual option
                bOptionNext = (i + 1 >= argc) || (m_options.contains(argv[i + 1]));
            }

            if (argument.value_required() || (argument.params() > 0 && !bOptionNext))
            {
                // This may not concatenate as many params as specified if there are missing params at end of args
                // This does not detect missing params ...

                for (size_type l = 0; l < argument.params() && (i + 1 < argc); ++l)
                {
                    if (!value.empty())
                    {
                        value += " ";
                    }

                    value += argv[++i];
                }
            }
            else
            {
                value = argument.default_value();
            }

            set_named_value(name, value);
        }
        else
        {
            set_positional_value(argv[i]);
        }
    }

    return;
}

//------------------------------------------------------------------------------
void quetzal::Arguments::set_named_value(const std::string& name, const std::string& value)
{
    assert(m_named.contains(name));

    m_values[name] = value;

    return;
}

//------------------------------------------------------------------------------
quetzal::Argument::size_type quetzal::Arguments::positional() const
{
    return m_positional.size();
}

//------------------------------------------------------------------------------
std::string quetzal::Arguments::positional_value(size_type n) const
{
    assert(n < m_positional.size());

    return m_positional[n];
}

//------------------------------------------------------------------------------
void quetzal::Arguments::set_positional_value(const std::string& value)
{
    m_positional.push_back(value);

    return;
}

//------------------------------------------------------------------------------
std::ostream& quetzal::operator<<(std::ostream& os, const Argument& argument)
{
    os << argument.name() << std::endl;
    os << "Params:          " << argument.params() << std::endl;
    os << "Params Required: " << argument.value_required() << std::endl;
    os << "Unspecified:     " << argument.unspecified_value() << std::endl;
    os << "Default:         " << argument.default_value() << std::endl;

    return os;
}

//------------------------------------------------------------------------------
std::ostream& quetzal::operator<<(std::ostream& os, const Arguments& arguments)
{
    Arguments::Named::const_iterator iEnd = arguments.m_named.end();
    for (Arguments::Named::const_iterator i = arguments.m_named.begin(); i != iEnd; ++i)
    {
        os << i->first << std::endl;
        os << i->second << std::endl;
    }

    os << std::endl;
    os << "Options" << std::endl;
    Arguments::Options::const_iterator jEnd = arguments.m_options.end();
    for (Arguments::Options::const_iterator i = arguments.m_options.begin(); i != jEnd; ++i)
    {
        os << i->first << " -> " << i->second << std::endl;
    }

    os << std::endl;
    os << "Named Values" << std::endl;
    Arguments::Values::const_iterator kEnd = arguments.m_values.end();
    for (Arguments::Values::const_iterator i = arguments.m_values.begin(); i != kEnd; ++i)
    {
        os << i->first << " -> " << i->second << std::endl;
    }

    os << std::endl;
    os << "Positional Values" << std::endl;
    const Arguments::size_type n = arguments.positional();
    for (Arguments::size_type i = 0; i < n; ++i)
    {
        os << "[" << i << "] -> " << arguments.positional_value(i) << std::endl;
    }

    return os;
}
