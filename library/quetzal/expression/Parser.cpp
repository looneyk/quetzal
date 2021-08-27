//------------------------------------------------------------------------------
// Parser.cpp
//
//------------------------------------------------------------------------------

#include "quetzal/expression/Parser.hpp"
#include "quetzal/expression/OpLiteral.hpp"
#include "quetzal/expression/OpIdentifier.hpp"
#include "quetzal/expression/OpNegate.hpp"
#include "quetzal/expression/OpAdd.hpp"
#include "quetzal/expression/OpSubtract.hpp"
#include "quetzal/expression/OpMultiply.hpp"
#include "quetzal/expression/OpDivide.hpp"
#include "quetzal/expression/OpFunctionCall.hpp"
#include "quetzal/Exception.hpp"
#include <sstream>
#include <cassert>

using namespace std;

//------------------------------------------------------------------------------
quetzal::expression::Parser::Parser() :
    m_functions(),
    m_expression(),
    m_pos(0),
    m_tokenType(TokenType::Null),
    m_token()
{
    init_functions();
}

//------------------------------------------------------------------------------
quetzal::expression::Parser::~Parser()
{
}

//------------------------------------------------------------------------------
void quetzal::expression::Parser::init_functions()
{
    m_functions["abs"] = Function("abs", 1);
    m_functions["min"] = Function("min", 2);
    m_functions["max"] = Function("max", 2);

    return;
}

//------------------------------------------------------------------------------
quetzal::expression::Expression::pointer_type quetzal::expression::Parser::parse(const string& str)
{
    try
    {
        m_expression = str;
        m_pos = 0;

        next();
        return expression();
    }
//        catch (const Exception& e)
//        {
//            cout << "parse error: " << e.what() << endl;
//        }
    catch (...)
    {
    }

    return parse("0");
}

//------------------------------------------------------------------------------
int quetzal::expression::Parser::precedence(TokenType op)
{
    switch (op)
    {
        case TokenType::FunctionCall:
        case TokenType::LeftParen:
        case TokenType::RightParen:
        case TokenType::Comma:
            return 0;

        case TokenType::OperatorAdd:
        case TokenType::OperatorSubtract:
            return 1;

        case TokenType::OperatorMultiply:
        case TokenType::OperatorDivide:
            return 2;

        default:
            assert(false);
    }

    return 0;
}

//------------------------------------------------------------------------------
quetzal::expression::Expression::pointer_type quetzal::expression::Parser::expression()
{
    Expression::pointer_type pterm = term();
    assert(pterm);

    while (m_tokenType != TokenType::End && precedence(m_tokenType) >= precedence(TokenType::OperatorAdd))
    {
        if (accept(TokenType::OperatorAdd))
        {
            pterm = Expression::pointer_type(new OpAdd(pterm, term()));
        }
        else if (accept(TokenType::OperatorSubtract))
        {
            pterm = Expression::pointer_type(new OpSubtract(pterm, term()));
        }
        else
        {
            ostringstream oss;
            oss << "Parser::expression - invalid token " << m_tokenType << " at position " << m_pos;
            throw Exception(oss.str().c_str(), __FILE__, __LINE__);
        }
    }

    return pterm;
}

//------------------------------------------------------------------------------
quetzal::expression::Expression::pointer_type quetzal::expression::Parser::term()
{
    Expression::pointer_type pfactor = factor();
    assert(pfactor);

    while (m_tokenType != TokenType::End && precedence(m_tokenType) >= precedence(TokenType::OperatorMultiply))
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
            oss << "Parser::term - invalid token " << m_tokenType << " at position " << m_pos;
            throw Exception(oss.str().c_str(), __FILE__, __LINE__);
        }
    }

    return pfactor;
}

//------------------------------------------------------------------------------
quetzal::expression::Expression::pointer_type quetzal::expression::Parser::factor()
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
    else if (accept(TokenType::FunctionCall))
    {
        return function_call(tokenPrev);
    }
    else if (accept(TokenType::LeftParen))
    {
        Expression::pointer_type pe = expression();
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
quetzal::expression::Expression::pointer_type quetzal::expression::Parser::function_call(const string& name)
{
    Functions::const_iterator i = m_functions.find(name);
    assert(i != m_functions.end());

    expect(TokenType::LeftParen);

    vector<Expression::pointer_type> pes;
    bool bFirst = true;

    for (; ; )
    {
        if (accept(TokenType::RightParen))
        {
            if (pes.size() != i->second.m_nArguments)
            {
                ostringstream oss;
                oss << "Parser::function_call - " << name << ", "<< pes.size() << " arguments found, " << i->second.m_nArguments << " arguments expected at position " << m_pos;
                throw Exception(oss.str().c_str(), __FILE__, __LINE__);
            }

            return Expression::pointer_type(new OpFunctionCall(name, pes));
        }
        else if (bFirst)
        {
            Expression::pointer_type pe = expression();
            assert(pe);

            pes.push_back(pe);

            bFirst = false;
        }
        else if (accept(TokenType::Comma))
        {
            Expression::pointer_type pe = expression();
            assert(pe);

            pes.push_back(pe);
        }
        else
        {
            break;
        }
    }

    ostringstream oss;
    oss << "Parser::function_call - invalid token " << m_tokenType << " at position " << m_pos;
    throw Exception(oss.str().c_str(), __FILE__, __LINE__);
}

//------------------------------------------------------------------------------
void quetzal::expression::Parser::next()
{
    if (m_pos == string::npos)
    {
        assert(m_tokenType == TokenType::End);
        return;
    }

    m_pos = m_expression.find_first_not_of(' ', m_pos); // Skip spaces

    if (m_pos == string::npos)
    {
        m_tokenType = TokenType::End;
        m_token.erase();
        return;
    }

    if (isdigit(m_expression[m_pos]))
    {
        m_tokenType = TokenType::Literal;
        char* pend = nullptr;
        strtod(&m_expression.c_str()[m_pos], &pend);
        size_t length = pend - &m_expression.c_str()[m_pos];
        m_token = m_expression.substr(m_pos, length);
        m_pos += length;
        return;
    }

    if (isalpha(m_expression[m_pos]) || m_expression[m_pos] == '_')
    {
        size_t length = 0;
        while (m_pos + length < m_expression.size() && (isalnum(m_expression[m_pos + length]) || m_expression[m_pos + length] == '_'))
        {
            ++length;
        }
        m_token = m_expression.substr(m_pos, length);
        m_pos += length;

        if (m_functions.find(m_token) != m_functions.end())
        {
            m_tokenType = TokenType::FunctionCall;
            return;
        }

        m_tokenType = TokenType::Identifier;
        return;
    }

    switch (m_expression[m_pos])
    {
        case '+':
            m_tokenType = TokenType::OperatorAdd;
            m_token = m_expression[m_pos];
            ++m_pos;
            return;

        case '-':
            m_tokenType = TokenType::OperatorSubtract;
            m_token = m_expression[m_pos];
            ++m_pos;
            return;

        case '*':
            m_tokenType = TokenType::OperatorMultiply;
            m_token = m_expression[m_pos];
            ++m_pos;
            return;

        case '/':
            m_tokenType = TokenType::OperatorDivide;
            m_token = m_expression[m_pos];
            ++m_pos;
            return;

        case '(':
            m_tokenType = TokenType::LeftParen;
            m_token = m_expression[m_pos];
            ++m_pos;
            return;

        case ')':
            m_tokenType = TokenType::RightParen;
            m_token = m_expression[m_pos];
            ++m_pos;
            return;

        case ',':
            m_tokenType = TokenType::Comma;
            m_token = m_expression[m_pos];
            ++m_pos;
            return;
    }

    ostringstream oss;
    oss << "Parser::next - invalid character encountered " << m_expression[m_pos] << " at position " << m_pos;
    throw Exception(oss.str().c_str(), __FILE__, __LINE__);
}

//------------------------------------------------------------------------------
bool quetzal::expression::Parser::accept(TokenType tokenType)
{
    if (m_tokenType == tokenType)
    {
        next();
        return true;
    }

    return false;
}

//------------------------------------------------------------------------------
bool quetzal::expression::Parser::expect(TokenType tokenType)
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
ostream& quetzal::expression::operator<<(ostream& os, const Parser::TokenType& tokenType)
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

        case Parser::TokenType::FunctionCall:
            os << "call()";
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
