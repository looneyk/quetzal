#if !defined(QUETZAL_EXPRESSION_PARSER_HPP)
#define QUETZAL_EXPRESSION_PARSER_HPP
//------------------------------------------------------------------------------
// Parser.hpp
//
//------------------------------------------------------------------------------

#include "quetzal/expression/Expression.hpp"
#include <string>
#include <map>
#include <iosfwd>

namespace quetzal
{

namespace expression
{

    //--------------------------------------------------------------------------
    class Parser
    {
    public:

        Parser();
        Parser(const Parser&) = delete;
        ~Parser();

        Parser& operator=(const Parser&) = delete;

        // Syntax errors silently result in an expression that evaluates to 0
        Expression::pointer_type parse(const std::string& str);

//    private:

        //----------------------------------------------------------------------
        enum class TokenType
        {
            Null,
            End,
            Literal,
            Identifier,
            FunctionCall,
            LeftParen,
            RightParen,
            Comma,
            OperatorAdd,
            OperatorSubtract,
            OperatorMultiply,
            OperatorDivide
        };

    private:

        //----------------------------------------------------------------------
        class Function
        {
        public:

            Function() :
                m_name(),
                m_nArguments(0)
            {
            }

            Function(const std::string& name, int nArguments) :
                m_name(name),
                m_nArguments(nArguments)
            {
            }

            std::string m_name;
            unsigned int m_nArguments;
            // signature (argument and return data types)
        };

        void init_functions();

        int precedence(TokenType op);

        Expression::pointer_type expression();
        Expression::pointer_type term();
        Expression::pointer_type factor();
        Expression::pointer_type function_call(const std::string& name);

        void next();
        bool accept(TokenType tokenType);
        bool expect(TokenType tokenType);

        typedef std::map<std::string, Function> Functions;
        Functions m_functions;

        std::string m_expression;
        size_t m_pos;

        TokenType m_tokenType;
        std::string m_token;

        friend
        std::ostream& operator<<(std::ostream& os, const Parser::TokenType& tokenType);
    };

    std::ostream& operator<<(std::ostream& os, const Parser::TokenType& tokenType);

} // namespace expression

} // namespace quetzal

#endif // QUETZAL_EXPRESSION_PARSER_HPP
