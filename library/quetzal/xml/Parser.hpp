#if !defined(QUETZAL_XML_PARSER_HPP)
#define QUETZAL_XML_PARSER_HPP
//------------------------------------------------------------------------------
// XML file io
// Parser.hpp
//------------------------------------------------------------------------------

#include "quetzal/xml/Document.hpp"
#include <string>
#include <map>
#include <iosfwd>

namespace quetzal::xml
{

    //--------------------------------------------------------------------------
    class Parser
    {
    public:

        Parser();
        Parser(const Parser&) = delete;
        ~Parser();

        Parser& operator=(const Parser&) = delete;

        Expression::pointer_type parse(const std::string& str);

//    private:

        //----------------------------------------------------------------------
        enum class TokenType
        {
            Null,
            End,
            DeclarationStart,
            DeclarationEnd,
            ProcessingInstruction,
            Comment,
            Doctype,
            Cdata,
            ElementOpenTagStart,
            ElementTagEnd,
            EmptyElementTagEnd,
            ElementCloseTag,
            Id,
            Equals,
            QuotedString,
            ElementValue
        };

        enum class Context
        {
            Null,
            Tag,
            Value
        }

    private:

        Expression::pointer_type node();
        Expression::pointer_type term();
        Expression::pointer_type factor();

        void next();
        bool accept(TokenType tokenType);
        bool expect(TokenType tokenType);

        void skip_ws();

        std::string m_xml;
        size_t m_pos;

        TokenType m_tokenType;
        std::string m_token;

        Context m_context;
        bool m_bFirstNode; // error if declaration appears after first node ...
        std::stack<string> m_element_stack;

        friend
        std::ostream& operator<<(std::ostream& os, const Parser::TokenType& tokenType);
    };

    std::ostream& operator<<(std::ostream& os, const Parser::TokenType& tokenType);

} // namespace quetzal::xml

#endif // QUETZAL_XML_PARSER_HPP
