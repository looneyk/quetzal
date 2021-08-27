#if !defined(QUETZAL_EXPRESSION_OPUNARY_HPP)
#define QUETZAL_EXPRESSION_OPUNARY_HPP
//------------------------------------------------------------------------------
// OpUnary.hpp
//
//------------------------------------------------------------------------------

#include "Expression.hpp"

namespace quetzal
{

namespace expression
{

    //--------------------------------------------------------------------------
    class OpUnary : public Expression
    {
    public:

        OpUnary(Expression::pointer_type pe);
        OpUnary(const OpUnary&) = delete;
        ~OpUnary();

        OpUnary& operator=(const OpUnary&) = delete;

        virtual void print(std::ostream& os) const = 0;

    protected:

        virtual float evaluate_float_(const SymbolTable& st) const = 0;

        Expression::pointer_type m_pe;
    };

} // namespace expression

} // namespace quetzal

#endif // QUETZAL_EXPRESSION_OPUNARY_HPP
