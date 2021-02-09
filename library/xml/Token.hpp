#if !defined(QUETZAL_XML_TOKEN_HPP)
#define QUETZAL_XML_TOKEN_HPP
//------------------------------------------------------------------------------
// XML file io
// Token.hpp
//------------------------------------------------------------------------------

#include <string>
#include <iosfwd>

namespace quetzal
{

namespace xml
{

    //--------------------------------------------------------------------------
    enum class TokenType
    {
        Text,
        StartTag,
        EndTag,
        EmptyElementTag,
        ProcessingInstruction, // <?
        Other // <!
    };

    //--------------------------------------------------------------------------
    class Token
    {
    public:

        Token();
        Token(TokenType type, const std::string& value);
        Token(const Token& t) = default;
        ~Token() = default;

        Token& operator=(const Token& t) = default;

        TokenType type() const;
        std::string value() const;

        void set_type(TokenType type);
        void set_value(const std::string& value);

    private:

        TokenType m_type;
        std::string m_value;
    };

    Token get_token(std::istream& is);

} // namespace xml

} // namespace quetzal

#endif // QUETZAL_XML_TOKEN_HPP
