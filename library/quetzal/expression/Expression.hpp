#if !defined(QUETZAL_EXPRESSION_EXPRESSION_HPP)
#define QUETZAL_EXPRESSION_EXPRESSION_HPP
//------------------------------------------------------------------------------
// Expression.hpp
//
//------------------------------------------------------------------------------

#include "SymbolTable.hpp"
#include <memory>
#include <iosfwd>

namespace quetzal
{

namespace expression
{

    //--------------------------------------------------------------------------
    class Expression
    {
    public:

        typedef std::shared_ptr<Expression> pointer_type;

        bool parenthesized() const;
        void set_parenthesized(const bool& b);

        float evaluate_float(const SymbolTable& st) const;

        virtual void print(std::ostream& os) const = 0;

    protected:

        Expression();
        Expression(const Expression&) = delete;
        virtual ~Expression();

        Expression& operator=(const Expression&) = delete;

        virtual float evaluate_float_(const SymbolTable& st) const = 0;

        bool m_bParenthesized;
    };

    std::ostream& operator<<(std::ostream& os, const Expression& e);

} // namespace expression

} // namespace quetzal

#endif // QUETZAL_EXPRESSION_EXPRESSION_HPP
