#if !defined(QUETZAL_XML_DOCUMENT_HPP)
#define QUETZAL_XML_DOCUMENT_HPP
//------------------------------------------------------------------------------
// XML file io
// Document.hpp
//------------------------------------------------------------------------------

#include "Declaration.hpp"
#include "Element.hpp"
#include <iosfwd>

namespace quetzal
{

namespace xml
{

    //--------------------------------------------------------------------------
    class Document
    {
    public:

        Document();
        Document(const Document&) = default;
        ~Document() = default;

        Document& operator=(const Document&) = default;

        void clear();

        const Declaration& declaration() const;
        Declaration& declaration();

        const Element& element() const;
        Element& element();

    private:

        Declaration m_declaration;
        Element m_element;
    };

    std::ostream& operator<<(std::ostream& os, const xml::Document& document);
    std::istream& operator>>(std::istream& is, xml::Document& document);

} // namespace xml

} // namespace quetzal

#endif // QUETZAL_XML_DOCUMENT_HPP
