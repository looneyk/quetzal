//------------------------------------------------------------------------------
// XML file io
// Declaration.cpp
//------------------------------------------------------------------------------

#include "Declaration.hpp"
#include <iostream>

using namespace std;

//------------------------------------------------------------------------------
quetzal::xml::Declaration::Declaration() :
    m_version("1.0"),
    m_encoding("UTF-8"),
    m_standalone()
{
}

//------------------------------------------------------------------------------
bool quetzal::xml::Declaration::empty() const
{
    return m_version.empty() && m_encoding.empty() && m_standalone.empty();
}

//------------------------------------------------------------------------------
string quetzal::xml::Declaration::version() const
{
    return m_version;
}

//------------------------------------------------------------------------------
string quetzal::xml::Declaration::encoding() const
{
    return m_encoding;
}

//------------------------------------------------------------------------------
string quetzal::xml::Declaration::standalone() const
{
    return m_standalone;
}

//------------------------------------------------------------------------------
void quetzal::xml::Declaration::set_version(const string& s)
{
    m_version = s;
    return;
}

//------------------------------------------------------------------------------
void quetzal::xml::Declaration::set_encoding(const string& s)
{
    m_encoding = s;
    return;
}

//------------------------------------------------------------------------------
void quetzal::xml::Declaration::set_standalone(const string& s)
{
    m_standalone = s;
    return;
}

//------------------------------------------------------------------------------
//void quetzal::xml::Node::read(istream& is)
void quetzal::xml::Declaration::read(istream& is)
{
    is;
    // ...

    return;
}

//------------------------------------------------------------------------------
//void quetzal::xml::Node::write(ostream& os)
void quetzal::xml::Declaration::write(ostream& os) const
{
    os << "<?xml ";
    if (!m_version.empty())
    {
        os << "version=\"" << m_version << "\" ";
    }

    if (!m_encoding.empty())
    {
        os << "encoding=\"" << m_encoding << "\" ";
    }

    if (!m_standalone.empty())
    {
        os << "standalone=\"" << m_standalone << "\" ";
    }

    os << "?>" << '\x0a';
    return;
}

//------------------------------------------------------------------------------
istream& quetzal::xml::operator>>(istream& is, Declaration& declaration)
{
    declaration.read(is);

    return is;
}

//------------------------------------------------------------------------------
ostream& quetzal::xml::operator<<(ostream& os, const Declaration& declaration)
{
    declaration.write(os);

    return os;
}
