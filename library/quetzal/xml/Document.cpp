//------------------------------------------------------------------------------
// XML file io
// Document.cpp
//------------------------------------------------------------------------------

#include "Document.hpp"
#include "Token.hpp"
#include <iostream>

using namespace std;

//------------------------------------------------------------------------------
quetzal::xml::Document::Document() :
    m_declaration(),
    m_element()
{
}

//------------------------------------------------------------------------------
void quetzal::xml::Document::clear()
{
    m_declaration.set_version("1.0");
    m_declaration.set_encoding("UTF-8");
    m_declaration.set_standalone("");
    m_element.clear();
    return;
}

//------------------------------------------------------------------------------
const quetzal::xml::Declaration& quetzal::xml::Document::declaration() const
{
    return m_declaration;
}

//------------------------------------------------------------------------------
quetzal::xml::Declaration& quetzal::xml::Document::declaration()
{
    return m_declaration;
}

//------------------------------------------------------------------------------
const quetzal::xml::Element& quetzal::xml::Document::element() const
{
    return m_element;
}

//------------------------------------------------------------------------------
quetzal::xml::Element& quetzal::xml::Document::element()
{
    return m_element;
}

//------------------------------------------------------------------------------
ostream& quetzal::xml::operator<<(ostream& os, const quetzal::xml::Document& document)
{
    os << document.declaration();
    os << document.element();
    return os;
}

//------------------------------------------------------------------------------
istream& quetzal::xml::operator>>(istream& is, quetzal::xml::Document& document)
{
    Token tStart;
    do
    {
        tStart = get_token(is);
    } while (!is.eof() && tStart.type() != TokenType::StartTag);

    xml::Element& e = document.element();

    e.set_name(tStart.value());

    is >> e;

    return is;
}
