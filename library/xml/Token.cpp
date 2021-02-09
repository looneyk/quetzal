//------------------------------------------------------------------------------
// XML file io
// Token.cpp
//------------------------------------------------------------------------------

#include "Token.hpp"
#include <iostream>
#include <sstream>

using namespace std;

//--------------------------------------------------------------------------
quetzal::xml::Token::Token() :
    m_type(TokenType::Text),
    m_value()
{
}

//--------------------------------------------------------------------------
quetzal::xml::Token::Token(TokenType type, const string& value) :
    m_type(type),
    m_value(value)
{
}

//--------------------------------------------------------------------------
quetzal::xml::TokenType quetzal::xml::Token::type() const
{
    return m_type;
}

//--------------------------------------------------------------------------
string quetzal::xml::Token::value() const
{
    return m_value;
}

//--------------------------------------------------------------------------
void quetzal::xml::Token::set_type(TokenType type)
{
    m_type = type;
    return;
}

//--------------------------------------------------------------------------
void quetzal::xml::Token::set_value(const string& value)
{
    m_value = value;
    return;
}

//--------------------------------------------------------------------------
quetzal::xml::Token quetzal::xml::get_token(istream& is)
{
    using Token = quetzal::xml::Token;
    using TokenType = quetzal::xml::TokenType;

    Token t;
    char c;
    ostringstream oss;

    do
    {
        is.get(c);
    } while (!is.eof() && isspace(c));

    if (c == '<')
    {
        is.get(c);
        if (c == '/')
        {
            t.set_type(TokenType::EndTag);
            is.get(c);
        }
        else if (c == '?')
        {
            t.set_type(TokenType::ProcessingInstruction);
            is.get(c);
        }
        else if (c == '!')
        {
            t.set_type(TokenType::Other);
            is.get(c);
        }
        else
        {
            t.set_type(TokenType::StartTag);
        }
    }
    else
    {
        t.set_type(TokenType::Text);
    }

    oss << c;

    if (t.type() == TokenType::Text)
    {
        do
        {
            is.get(c);
            if (c != '<')
            {
                oss << c;
            }
            else
            {
                is.putback(c);
            }
        } while (!is.eof() && c != '<');
    }
    else
    {
        do
        {
            is.get(c);
            if (c == '>')
            {
                break;
            }
            else if (c == '/')
            {
                is.get(c);
                if (c == '>')
                {
                    t.set_type(TokenType::EmptyElementTag);
                    break;
                }
                else
                {
                    oss << '/';
                }
            }

            oss << c;
        } while (!is.eof());
    }

    t.set_value(oss.str());

    return t;
}
