#if !defined(QUETZAL_XML_ATTRIBUTE_HPP)
#define QUETZAL_XML_ATTRIBUTE_HPP
//------------------------------------------------------------------------------
// XML file io
// Attribute.hpp
//------------------------------------------------------------------------------

#include <string>
#include <iosfwd>

namespace quetzal::xml
{

    //--------------------------------------------------------------------------
    class Attribute
    {
    public:

        Attribute() = default;
        Attribute(const std::string& name, const std::string& value);
        Attribute(const Attribute&) = default;
        ~Attribute() = default;

        Attribute& operator=(const Attribute&) = default;

        std::string name() const;
        std::string value() const;

        void set_name(const std::string& name);
        void set_value(const std::string& value);

    private:

        std::string m_name;
        std::string m_value;
    };

    std::ostream& operator<<(std::ostream& os, const Attribute& attribute);

} // namespace quetzal::xml

#endif // QUETZAL_XML_ATTRIBUTE_HPP
