#if !defined(QUETZAL_EXPRESSION_OPBINARY_HPP)
#define QUETZAL_EXPRESSION_OPBINARY_HPP
//------------------------------------------------------------------------------
// OpBinary.hpp
//
//------------------------------------------------------------------------------

#include "Expression.hpp"

namespace quetzal
{

namespace expression
{

    //--------------------------------------------------------------------------
    class OpBinary : public Expression
    {
    public:

        OpBinary(Expression::pointer_type pelhs, Expression::pointer_type perhs);
        OpBinary(const OpBinary&) = delete;
        ~OpBinary();

        OpBinary& operator=(const OpBinary&) = delete;

        virtual void print(std::ostream& os) const = 0;

    protected:

        virtual float evaluate_float_(const SymbolTable& st) const = 0;

        Expression::pointer_type m_pelhs;
        Expression::pointer_type m_perhs;
    };

} // namespace expression

} // namespace quetzal

#endif // QUETZAL_EXPRESSION_OPBINARY_HPP
