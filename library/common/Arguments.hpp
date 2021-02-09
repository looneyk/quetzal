//------------------------------------------------------------------------------
// Arguments.hpp
//
//------------------------------------------------------------------------------

// Add usage strings for each argument?
// Add list of allowed values for arguments that can be enumerated?
// Argument should be within Arguments class
// Allow multiple options if client specifies to, later takes preference for now
// register_bool and register_string?

#if !defined(QUETZAL_ARGUMENTS_HPP)
#define QUETZAL_ARGUMENTS_HPP

#include <string>
#include <map>
#include <vector>
#include <iosfwd>
#include "string_util.hpp"

namespace quetzal
{

    //----------------------------------------------------------------------
    class Argument
    {
    public:

        typedef size_t size_type;

        Argument();
        Argument(const std::string& name, size_type nParams, bool bValueRequired, const std::string& valueUnspecified = "", const std::string& valueDefault = "");
        Argument(const Argument& a);
        ~Argument();

        Argument& operator=(const Argument& a);

        std::string name() const;
        size_type params() const;
        bool value_required() const;
        std::string unspecified_value() const;
        std::string default_value() const;

    private:

        std::string m_name;
        size_type m_nParams;
        bool m_bValueRequired;
        std::string m_valueUnspecified; // value if argument does not appear in command line
        std::string m_valueDefault; // value applied if params = 0 or if param(s) omitted in command line
    };

    //----------------------------------------------------------------------
    class Arguments
    {
        friend std::ostream& operator<<(std::ostream& os, const Arguments& arguments);

    public:

        typedef Argument::size_type size_type;

        Arguments();
        ~Arguments();

        void register_argument(const std::string& name, const std::string& long_option, const std::string& short_option, size_type nParams, bool bValueRequired, const std::string& valueUnspecified = "", const std::string& valueDefault = "");

        void parse(int argc, char* argv[]);

        // Named arguments

        template<typename T>
        T named_value(const std::string& name) const;

        void set_named_value(const std::string& name, const std::string& value);

        // Positional arguments

        size_type positional() const;
        std::string positional_value(size_type n) const;
        void set_positional_value(const std::string& value);

    private:

        Arguments(const Arguments&); // Not copyable
        Arguments& operator=(const Arguments&); // Not assignable

        typedef std::map<std::string, Argument> Named; // name, Argument
        Named m_named;

        typedef std::map<std::string, std::string> Options; // option, name
        Options m_options;

        typedef std::map<std::string, std::string> Values; // name, value
        Values m_values;

        typedef std::vector<std::string> Positional;
        Positional m_positional;
    };

    std::ostream& operator<<(std::ostream& os, const Argument& argument);
    std::ostream& operator<<(std::ostream& os, const Arguments& arguments);

    //----------------------------------------------------------------------
    template<typename T>
    T Arguments::named_value(const std::string& name) const
    {
        Values::const_iterator i = m_values.find(name);
        if (i != m_values.end())
        {
            return to_type<T>(i->second);
        }

        Named::const_iterator j = m_named.find(name);
        if (j != m_named.end())
        {
            return to_type<T>(j->second.unspecified_value());
        }

        return T();
    }

} // namespace quetzal

#endif // QUETZAL_ARGUMENTS_HPP
