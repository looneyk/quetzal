//------------------------------------------------------------------------------
// XML file io
// Element.cpp
//------------------------------------------------------------------------------

#include "Element.hpp"
#include "Token.hpp"
#include <iostream>
#include <cassert>

using namespace std;

namespace
{

    const string s_whitespace = " \x09\x0a\x0d";

} // namespace

//------------------------------------------------------------------------------
quetzal::xml::Element::Element(const string& name, const string& value) :
    m_name(name),
    m_value(value),
    m_attributes(),
    m_elements(),
    m_pparent(nullptr),
    m_depth(0)
{
}

//------------------------------------------------------------------------------
string quetzal::xml::Element::name() const
{
    return m_name;
}

//------------------------------------------------------------------------------
string quetzal::xml::Element::value() const
{
    return m_value;
}

//------------------------------------------------------------------------------
quetzal::xml::Element* quetzal::xml::Element::parent()
{
    assert(m_pparent != nullptr);
    return m_pparent;
}

//------------------------------------------------------------------------------
size_t quetzal::xml::Element::depth() const
{
    return m_depth;
}

//------------------------------------------------------------------------------
void quetzal::xml::Element::set_name(const string& name)
{
    m_name = name;
    return;
}

//------------------------------------------------------------------------------
void quetzal::xml::Element::set_value(const string& value)
{
    m_value = value;
    return;
}

//------------------------------------------------------------------------------
const quetzal::xml::Element::attributes_type& quetzal::xml::Element::attributes() const
{
    return m_attributes;
}

//------------------------------------------------------------------------------
const quetzal::xml::Element::elements_type& quetzal::xml::Element::elements() const
{
    return m_elements;
}

//------------------------------------------------------------------------------
shared_ptr<quetzal::xml::Element> quetzal::xml::Element::sub_element(const string& name) const
{
    for (auto& pe : m_elements)
    {
        if (pe->name() == name)
        {
            return pe;
        }
    }

    return nullptr;
}

//------------------------------------------------------------------------------
string quetzal::xml::Element::element_value(const string& name) const
{
    const auto& pe = sub_element(name);
    if (pe != nullptr)
    {
        return pe->value();
    }

    return string();
}

//------------------------------------------------------------------------------
const quetzal::xml::Attribute* quetzal::xml::Element::attribute(const string& name) const
{
    for (auto& attribute : m_attributes)
    {
        if (attribute.name() == name)
        {
            return &attribute;
        }
    }

    return nullptr;
}

//------------------------------------------------------------------------------
quetzal::xml::Attribute* quetzal::xml::Element::attribute(const string& name)
{
    for (auto& attribute : m_attributes)
    {
        if (attribute.name() == name)
        {
            return &attribute;
        }
    }

    return nullptr;
}

//------------------------------------------------------------------------------
string quetzal::xml::Element::attribute_value(const string& name) const
{
    const Attribute* pa = attribute(name);
    if (pa != nullptr)
    {
        return pa->value();
    }

    return string();
}

//------------------------------------------------------------------------------
void quetzal::xml::Element::clear()
{
    m_name.clear();
    m_value.clear();
    m_attributes.clear();
    m_elements.clear();
    m_pparent = nullptr;
    m_depth = 0;
    return;
}

//------------------------------------------------------------------------------
void quetzal::xml::Element::parse_attributes(const string& strAttributes)
{
    string::size_type iEnd = strAttributes.length();
    string::size_type iPos = 0;

    // Does not handle escaped quotes (does it need to?)
    // Does not handle name1="value1"name2="value2" (assuming that white space is required?)

    while (iPos < iEnd)
    {
        for (; iPos < iEnd && isspace(strAttributes[iPos]); ++iPos);
        if (iPos >= iEnd) break;

        string::size_type iStart = iPos;
        for (; iPos < iEnd && !isspace(strAttributes[iPos]) && strAttributes[iPos] != '='; ++iPos);
        if (iPos >= iEnd) break;

        string name = strAttributes.substr(iStart, iPos - iStart);

        for (; iPos < iEnd && isspace(strAttributes[iPos]); ++iPos);
        if (iPos >= iEnd) break;

        if (strAttributes[iPos] != '=')
        {
            break;
        }

        ++iPos; // Skip =
        if (iPos >= iEnd) break;

        for (; iPos < iEnd && isspace(strAttributes[iPos]); ++iPos);
        if (iPos >= iEnd) break;

        if (strAttributes[iPos] != '"')
        {
            break;
        }

        ++iPos; // Skip initial "
        if (iPos >= iEnd) break;

        iStart = iPos;

        for (; iPos < iEnd && strAttributes[iPos] != '"'; ++iPos);
        if (iPos >= iEnd) break;

        string value = strAttributes.substr(iStart, iPos - iStart);

        m_attributes.push_back(Attribute(name, value));

        ++iPos; // Skip final "
    }

    return;
}

//------------------------------------------------------------------------------
void quetzal::xml::Element::set_attribute(const string& name, const string& value)
{
    Attribute* pattribute = attribute(name);
    if (pattribute != nullptr)
    {
        pattribute->set_value(value);
    }
    else
    {
        m_attributes.emplace_back(name, value);
    }

    return;
}

//------------------------------------------------------------------------------
void quetzal::xml::Element::set_attribute(const quetzal::xml::Attribute& attribute)
{
    m_attributes.push_back(attribute);
    return;
}

//------------------------------------------------------------------------------
void quetzal::xml::Element::set_attributes(const attributes_type& attributes)
{
    m_attributes.insert(m_attributes.end(), attributes.begin(), attributes.end());
    return;
}

//------------------------------------------------------------------------------
shared_ptr<quetzal::xml::Element> quetzal::xml::Element::add_element(const string& name, const string& value)
{
    return add_element(make_shared<Element>(name, value));
}

//------------------------------------------------------------------------------
shared_ptr<quetzal::xml::Element> quetzal::xml::Element::add_element(shared_ptr<Element> pelement)
{
    assert(pelement != nullptr);
    pelement->m_pparent = this;
    pelement->m_depth = m_depth + 1;
    m_elements.push_back(pelement);
    return pelement;
}

//------------------------------------------------------------------------------
void quetzal::xml::Element::add_elements(const elements_type& elements)
{
    for (auto& pe : elements)
    {
        add_element(pe);
    }

    return;
}

//------------------------------------------------------------------------------
std::shared_ptr<quetzal::xml::Element> quetzal::xml::Element::insert_element(std::shared_ptr<Element> pelementPosition, const std::string& name, const std::string& value)
{
    return insert_element(pelementPosition, make_shared<Element>(name, value));
}

//------------------------------------------------------------------------------
std::shared_ptr<quetzal::xml::Element> quetzal::xml::Element::insert_element(std::shared_ptr<Element> pelementPosition, std::shared_ptr<Element> pelement)
{
    auto position = find(m_elements.begin(), m_elements.end(), pelementPosition);
    return *insert_element(position, pelement);
}

//------------------------------------------------------------------------------
void quetzal::xml::Element::insert_elements(std::shared_ptr<Element> pelementPosition, const elements_type& elements)
{
    auto position = find(m_elements.begin(), m_elements.end(), pelementPosition);
    for (auto& pe : elements)
    {
        position = insert_element(position, pe);
        ++position;
    }

    return;
}

//------------------------------------------------------------------------------
quetzal::xml::Element::elements_type::iterator quetzal::xml::Element::insert_element(elements_type::iterator position, std::shared_ptr<Element> pelement)
{
    assert(pelement != nullptr);
    pelement->m_pparent = this;
    pelement->m_depth = m_depth + 1;
    return m_elements.insert(position, pelement);
}

//------------------------------------------------------------------------------
ostream& quetzal::xml::operator<<(ostream& os, const quetzal::xml::Element& element)
{
    os << string(element.depth() * 4, ' '); // Indentation
    os << '<' << element.name();
    for (auto& attribute : element.attributes())
    {
        os << ' ' << attribute.name() << '=' << '"' << attribute.value() << '"';
    }

    if (element.value().empty() && element.elements().empty())
    {
        os << "/>" << '\x0a';
        return os;
    }

    os << '>';
    os << element.value();

    if (!element.elements().empty())
    {
        os << '\x0a';

        for (auto& pe : element.elements())
        {
            os << *pe;
        }

        os << string(element.depth() * 4, ' '); // Indentation
    }

    os << "</" << element.name() << '>' << '\x0a';
    return os;
}

//------------------------------------------------------------------------------
istream& quetzal::xml::operator>>(istream& is, quetzal::xml::Element& element)
{
    bool bEmpty = false;

    string strStart = element.name();

    string::size_type posNameEnd = strStart.find_first_of(s_whitespace);
    element.set_name(strStart.substr(0, posNameEnd));
    if (posNameEnd != string::npos)
    {
        string::size_type posAttributeStart = strStart.find_first_not_of(s_whitespace, posNameEnd);
        if (posAttributeStart != string::npos)
        {
            string::size_type posAttributeEnd = strStart.find_last_not_of(s_whitespace);
            if (strStart[posAttributeEnd] == '/')
            {
                --posAttributeEnd;
                bEmpty = true;
            }

            element.parse_attributes(strStart.substr(posAttributeStart, posAttributeEnd - posAttributeStart + 1));
        }
    }

    if (bEmpty)
    {
        return is;
    }

    string value;
    Token t;
    do
    {
        t = get_token(is);
        switch (t.type())
        {
            case TokenType::Text:
                value += t.value();
                break;

            case TokenType::StartTag:
            {
                auto pe = make_shared<Element>(t.value());
                is >> *pe;
                element.add_element(pe);
                break;
            }

            case TokenType::EndTag:
                break;

            case TokenType::EmptyElementTag:
            {
                auto pe = make_shared<Element>(t.value());
                element.add_element(pe);
                t.set_type(TokenType::EndTag);
                break;
            }

            case TokenType::ProcessingInstruction:
                // ...
                break;

            case TokenType::Other:
                // unhandled ...
                break;
        }
    } while (!is.eof() && (t.type() != TokenType::EndTag || t.value() != element.name()));

    element.set_value(value);

    if (t.type() != TokenType::EndTag || t.value() != element.name())
    {
        // error ...
    }

    return is;
}
