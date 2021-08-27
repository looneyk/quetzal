#if !defined(QUETZAL_XML_ELEMENT_HPP)
#define QUETZAL_XML_ELEMENT_HPP
//------------------------------------------------------------------------------
// XML file io
// Element.hpp
//------------------------------------------------------------------------------

// vector instead of list?
// Attribute *?

#include "Attribute.hpp"
#include <list>
#include <memory>
#include <string>
#include <iosfwd>

namespace quetzal::xml
{

    //--------------------------------------------------------------------------
    class Element
    {
    public:

        typedef std::list<std::shared_ptr<Element>> elements_type;
        typedef std::list<Attribute> attributes_type;

        Element() = default;
        explicit Element(const std::string& name, const std::string& value = std::string());
        Element(const Element&) = default;
        ~Element() = default;

        Element& operator=(const Element&) = default;

        std::string name() const;
        std::string value() const;

        Element* parent();
        size_t depth() const;

        void set_name(const std::string& name);
        void set_value(const std::string& value);

        const attributes_type& attributes() const;
        const elements_type& elements() const;

        std::shared_ptr<Element> sub_element(const std::string& name) const;
        std::string element_value(const std::string& name) const;

        const Attribute* attribute(const std::string& name) const;
        Attribute* attribute(const std::string& name);
        std::string attribute_value(const std::string& name) const;

        void clear();
        void parse_attributes(const std::string& strAttributes);

        void set_attribute(const std::string& name, const std::string& value);
        void set_attribute(const Attribute& attribute);
        void set_attributes(const attributes_type& attributes);

        std::shared_ptr<Element> add_element(const std::string& name, const std::string& value = std::string());
        std::shared_ptr<Element> add_element(std::shared_ptr<Element> pelement);
        void add_elements(const elements_type& elements);

        std::shared_ptr<Element> insert_element(std::shared_ptr<Element> pelementPosition, const std::string& name, const std::string& value = std::string());
        std::shared_ptr<Element> insert_element(std::shared_ptr<Element> pelementPosition, std::shared_ptr<Element> pelement);
        void insert_elements(std::shared_ptr<Element> pelementPosition, const elements_type& elements);

    private:

        elements_type::iterator insert_element(elements_type::iterator position, std::shared_ptr<Element> pelement);

        std::string m_name;
        std::string m_value;
        attributes_type m_attributes;
        elements_type m_elements;

        Element* m_pparent = nullptr;
        mutable int m_depth = 0; // Used by output operator
    };

    std::ostream& operator<<(std::ostream& os, const xml::Element& element);
    std::istream& operator>>(std::istream& is, xml::Element& element);

} // namespace quetzal::xml

#endif // QUETZAL_XML_ELEMENT_HPP
