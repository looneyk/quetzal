//------------------------------------------------------------------------------
// Parser.cpp
//
//------------------------------------------------------------------------------

#include "quetzal/xml/Parser.hpp"
#include "quetzal/xml/Element.hpp"
#include "quetzal/xml/Attribute.hpp"
#include "quetzal/common/Exception.hpp"
#include <sstream>
#include <cassert>

using namespace std;

namespace
{

    //--------------------------------------------------------------------------
    size_t skip_ws(const string& s, size_t pos)
    {
        assert(pos < s.size());

        size_t pos_orig = pos;
        while (s[pos] == ' ' || s[pos] == '\x09' || s[pos] == '\x0a' || s[pos] == '\x0d')
        {
            ++pos;
            if (pos == s.size())
            {
                pos = string::npos;
                break;
            }
        }

        return pos - pos_orig;
    }

    //--------------------------------------------------------------------------
    bool is_id_first_char(int c)
    {
        if (isalpha(c) || c == '_')
        {
            return true;
        }

        return false;
    }

    //--------------------------------------------------------------------------
    bool is_id_char(int c)
    {
        if (isalnum(c) || c == '_' || c == '-' || c == '.' || c == ':')
        {
            return true;
        }

        return false;
    }

} // namespace

//------------------------------------------------------------------------------
quetzal::xml::Parser::Parser() :
    m_xml(),
    m_pos(0),
    m_tokenType(TokenType::Null),
    m_token(),
    m_context(Context::Value)
{
}

//------------------------------------------------------------------------------
quetzal::xml::Parser::~Parser()
{
}

//------------------------------------------------------------------------------
void quetzal::xml::Parser::parse(const string& str)
{
    try
    {
        m_xml = str;
        m_pos = 0;

        next();
        while (m_tokenType != TokenType::End)
        {
            m_nodes.push_back(node());
            next();
        }
    }
    catch (const Exception& e)
    {
        cout << "parse error: " << e.what() << endl;
    }

    return;
}

//------------------------------------------------------------------------------
quetzal::xml::node::pointer_type quetzal::xml::Parser::node()
{
    if (accept(TokenType::ElementOpenStart))
    {
        new element from tag ...
    }

    Expression::pointer_type ptag = tag();
    assert(ptag);

    while (m_tokenType != TokenType::End)
    {
        if (accept(TokenType::OperatorAdd))
        {
            ptag = Expression::pointer_type(new OpAdd(ptag, tag()));
        }
        else if (accept(TokenType::OperatorSubtract))
        {
            ptag = Expression::pointer_type(new OpSubtract(ptag, tag()));
        }
        else
        {
            ostringstream oss;
            oss << "Parser::node - invalid token " << m_tokenType << " at position " << m_pos;
            throw Exception(oss.str().c_str(), __FILE__, __LINE__);
        }
    }

    return ptag;
}

//------------------------------------------------------------------------------
quetzal::xml::Expression::pointer_type quetzal::xml::Parser::tag()
{
    Expression::pointer_type pfactor = factor();
    assert(pfactor);

    while (m_tokenType != TokenType::End)
    {
        if (accept(TokenType::OperatorMultiply))
        {
            pfactor = Expression::pointer_type(new OpMultiply(pfactor, factor()));
        }
        else if (accept(TokenType::OperatorDivide))
        {
            pfactor = Expression::pointer_type(new OpDivide(pfactor, factor()));
        }
        else
        {
            ostringstream oss;
            oss << "Parser::tag - invalid token " << m_tokenType << " at position " << m_pos;
            throw Exception(oss.str().c_str(), __FILE__, __LINE__);
        }
    }

    return pfactor;
}

//------------------------------------------------------------------------------
quetzal::xml::Expression::pointer_type quetzal::xml::Parser::factor()
{
    string tokenPrev = m_token;

    if (accept(TokenType::Literal))
    {
        return Expression::pointer_type(new OpLiteral(tokenPrev));
    }
    else if (accept(TokenType::Identifier))
    {
        return Expression::pointer_type(new OpIdentifier(tokenPrev));
    }
    else if (accept(TokenType::LeftParen))
    {
        Expression::pointer_type pe = node();
        assert(pe);
        pe->set_parenthesized(true);
        expect(TokenType::RightParen);
        return pe;
    }
    else if (accept(TokenType::OperatorSubtract))
    {
        return Expression::pointer_type(new OpNegate(factor()));
    }

    ostringstream oss;
    oss << "Parser::factor - invalid token " << m_tokenType << " at position " << m_pos;
    throw Exception(oss.str().c_str(), __FILE__, __LINE__);
}

//------------------------------------------------------------------------------
void quetzal::xml::Parser::next()
{
    assert(m_pos == string::npos || m_pos <= m_xml.size())

    if (m_pos == string::npos || m_pos == m_xml.size())
    {
        assert(m_tokenType == TokenType::End);
        m_token.erase();
        return;
    }

    if (string(&m_xml[m_pos], 5) == "<?xml ")
    {
        m_context = Context::Tag;
        m_tokenType = TokenType::DeclarationStart;
        m_token = "<?xml ";
        m_pos += m_token.size();
        skip_ws();
    }
    else if (string(&m_xml[m_pos], 5) == "?>")
    {
        m_context = Context::Value;
        m_tokenType = TokenType::DeclarationEnd;
        m_token = "?>";
        m_pos += m_token.size();
        skip_ws();
    }
    else if (string(&m_xml[m_pos], 2) == "<?")
    {
        m_pos += 2;
        size_t posPrev = m_pos;
        while (string(&m_xml[m_pos], 2) != "?>")
        {
            ++m_pos;
        }

        m_tokenType = TokenType::ProcessingInstruction;
        m_token = string(&m_xml[posPrev], m_pos - posPrev);
        m_pos += m_token.size();
        m_pos += 2;
        skip_ws();
    }
    else if (string(&m_xml[m_pos], 4) == "<!--")
    {
        m_pos += 4;
        size_t posPrev = m_pos;
        while (string(&m_xml[m_pos], 3) != "-->")
        {
            if (string(&m_xml[m_pos], 2) == "--")
            {
                ostringstream oss;
                oss << "Parser::next - Stray -- embedded in comment at position " << m_pos;
                throw Exception(oss.str().c_str(), __FILE__, __LINE__);
            }

            ++m_pos;
        }

        m_tokenType = TokenType::Comment;
        m_token = string(&m_xml[posPrev], m_pos - posPrev);
        m_pos += m_token.size();
        m_pos += 3;
        skip_ws();
    }
    else if (string(&m_xml[m_pos], 9) == "<!DOCTYPE")
    {
        m_pos += 9;
        size_t posPrev = m_pos;
        while (m_xml[m_pos] != '>')
        {
            ++m_pos;
        }

        m_tokenType = TokenType::Doctype;
        m_token = string(&m_xml[posPrev], m_pos - posPrev);
        m_pos += m_token.size();
        m_pos += 1;
        skip_ws();
    }
    else if (string(&m_xml[m_pos], 9) == "<!CDATA[[")
    {
        m_pos += 9;
        size_t posPrev = m_pos;
        while (string(&m_xml[m_pos], 3) != "]]>")
        {
            ++m_pos;
        }

        m_tokenType = TokenType::Cdata;
        m_token = string(&m_xml[posPrev], m_pos - posPrev);
        m_pos += m_token.size();
        m_pos += 3;
        skip_ws();
    }
    else if (string(&m_xml[m_pos], 2) == "</")
    {
        m_pos += 2;
        size_t posPrev = m_pos;
        while (m_xml[m_pos] != '>')
        {
            ++m_pos;
        }

        m_tokenType = TokenType::ElementCloseTag;
        m_token = string(&m_xml[posPrev], m_pos - posPrev);
        m_pos += m_token.size();
        ++m_pos;
    }
    else if (m_xml[m_pos] == '<')
    {
        m_context = Context::Tag;
        m_tokenType = TokenType::ElementOpenTagStart;
        m_token = "<";
        m_pos += m_token.size();
    }
    else if (string(&m_xml[m_pos], 2) == "/>")
    {
        m_tokenType = TokenType::EmptyElementTagEnd;
        m_token = "/>";
        m_pos += m_token.size();
        m_context = Context::Value;
    }
    else if (m_context == Context::Tag)
    {
        if (m_xml[m_pos] == '>')
        {
            m_tokenType = TokenType::ElementTagEnd
            m_token = ">";
            m_pos += m_token.size();
            m_context = Context::Value;
        }
        else if (m_xml[m_pos] == '=')
        {
            m_tokenType = TokenType::Equals;
            m_token = "=";
            m_pos += m_token.size();
            skip_ws();
        }
        else if (m_xml[m_pos] == '"' || m_xml[m_pos] == '\'')
        {
            string delimiter(1, m_xml[m_pos]);
            ++m_pos; // Skip initial delimiter
            size_t posStart = m_pos;
            size_t posEnd = m_xml.find_first_of(delimiter, m_pos);
            if (posEnd == string::npos)
            {
                ostringstream oss;
                oss << "Parser::next - unterminated quoted string at position " << m_pos;
                throw Exception(oss.str().c_str(), __FILE__, __LINE__);
            }

            m_tokenType = TokenType::QuotedString;
            m_token = m_xml.substr(posStart, posEnd - posStart);
            m_pos += m_token.size();
            ++m_pos; // Skip final delimiter
            skip_ws();
        }
        else if (is_id_first_char(m_xml[m_pos]))
        {
            size_t posPrev = m_pos;
            ++m_pos;

            while (is_id_char(m_xml[m_pos]))
            {
                ++m_pos;
            }

            m_tokenType = TokenType::Id;
            m_token = string(&m_xml[posPrev], m_pos - posPrev);
            skip_ws();
        }
        else
        {
            ostringstream oss;
            oss << "Parser::next - invalid character encountered " << m_xml[m_pos] << " at position " << m_pos;
            throw Exception(oss.str().c_str(), __FILE__, __LINE__);
        }
    }
    else // Element value
    {
        assert(m_context == Context::Value)

        // this only extracts element value that precedes any child elements ...
        // switch back to Context::Value after each child element?
        size_t posPrev = m_pos;
        while (m_xml[m_pos] != '<')
        {
            ++m_pos;
        }

        m_tokenType = TokenType::ElementValue;
        m_token = string(&m_xml[posPrev], m_pos - posPrev);
    }

    return;
}

//------------------------------------------------------------------------------
/*
void quetzal::xml::Parser::next()
{
    if (m_pos == string::npos)
    {
        assert(m_tokenType == TokenType::End);
        return;
    }

    m_pos = m_xml.find_first_not_of(' ', m_pos); // Skip spaces

    if (m_pos == string::npos)
    {
        m_tokenType = TokenType::End;
        m_token.erase();
        return;
    }

    if (isdigit(m_xml[m_pos]))
    {
        m_tokenType = TokenType::Literal;
        char* pend = nullptr;
        strtod(&m_xml.c_str()[m_pos], &pend);
        size_t length = pend - &m_xml.c_str()[m_pos];
        m_token = m_xml.substr(m_pos, length);
        m_pos += length;
        return;
    }

    if (isalpha(m_xml[m_pos]) || m_xml[m_pos] == '_')
    {
        size_t length = 0;
        while (m_pos + length < m_xml.size() && (isalnum(m_xml[m_pos + length]) || m_xml[m_pos + length] == '_'))
        {
            ++length;
        }
        m_token = m_xml.substr(m_pos, length);
        m_pos += length;

        m_tokenType = TokenType::Identifier;
        return;
    }

    switch (m_xml[m_pos])
    {
        case '+':
            m_tokenType = TokenType::OperatorAdd;
            m_token = m_xml[m_pos];
            ++m_pos;
            return;

        case '-':
            m_tokenType = TokenType::OperatorSubtract;
            m_token = m_xml[m_pos];
            ++m_pos;
            return;

        case '*':
            m_tokenType = TokenType::OperatorMultiply;
            m_token = m_xml[m_pos];
            ++m_pos;
            return;

        case '/':
            m_tokenType = TokenType::OperatorDivide;
            m_token = m_xml[m_pos];
            ++m_pos;
            return;

        case '(':
            m_tokenType = TokenType::LeftParen;
            m_token = m_xml[m_pos];
            ++m_pos;
            return;

        case ')':
            m_tokenType = TokenType::RightParen;
            m_token = m_xml[m_pos];
            ++m_pos;
            return;

        case ',':
            m_tokenType = TokenType::Comma;
            m_token = m_xml[m_pos];
            ++m_pos;
            return;
    }

    ostringstream oss;
    oss << "Parser::next - invalid character encountered " << m_xml[m_pos] << " at position " << m_pos;
    throw Exception(oss.str().c_str(), __FILE__, __LINE__);
}
*/

//------------------------------------------------------------------------------
bool quetzal::xml::Parser::accept(TokenType tokenType)
{
    if (m_tokenType == tokenType)
    {
        next();
        return true;
    }

    return false;
}

//------------------------------------------------------------------------------
bool quetzal::xml::Parser::expect(TokenType tokenType)
{
    if (accept(tokenType))
    {
        return true;
    }

    ostringstream oss;
    oss << "Parser::expect - invalid token " << m_tokenType << ", " << tokenType << " expected at position " << m_pos;
    throw Exception(oss.str().c_str(), __FILE__, __LINE__);
}

//------------------------------------------------------------------------------
void quetzal::xml::Parser::skip_ws()
{
    m_pos = skip_ws(m_xml, m_pos);
    return;
}

//------------------------------------------------------------------------------
ostream& quetzal::xml::operator<<(ostream& os, const Parser::TokenType& tokenType)
{
    switch (tokenType)
    {
        case Parser::TokenType::Null:
            os << "null";
            break;

        case Parser::TokenType::End:
            os << "end";
            break;

        case Parser::TokenType::Literal:
            os << "literal";
            break;

        case Parser::TokenType::Identifier:
            os << "identifier";
            break;

        case Parser::TokenType::LeftParen:
            os << "(";
            break;

        case Parser::TokenType::RightParen:
            os << ")";
            break;

        case Parser::TokenType::Comma:
            os << ",";
            break;

        case Parser::TokenType::OperatorAdd:
            os << "+";
            break;

        case Parser::TokenType::OperatorSubtract:
            os << "-";
            break;

        case Parser::TokenType::OperatorMultiply:
            os << "*";
            break;

        case Parser::TokenType::OperatorDivide:
            os << "/";
            break;

        default:
            assert(false);
    }

    return os;
}
