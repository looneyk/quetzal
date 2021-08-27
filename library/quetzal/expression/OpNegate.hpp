#if !defined(QUETZAL_EXPRESSION_OPNEGATE_HPP)
#define QUETZAL_EXPRESSION_OPNEGATE_HPP
//------------------------------------------------------------------------------
// OpNegate.hpp
//
//------------------------------------------------------------------------------

#include "OpUnary.hpp"

namespace quetzal
{

namespace expression
{

    //--------------------------------------------------------------------------
    class OpNegate : public OpUnary
    {
    public:

        OpNegate(Expression::pointer_type pe);
        OpNegate(const OpNegate&) = delete;
        ~OpNegate();

        OpNegate& operator=(const OpNegate&) = delete;

        virtual void print(std::ostream& os) const;

    protected:

        virtual float evaluate_float_(const SymbolTable& st) const;
    };

} // namespace expression

} // namespace quetzal

#endif // QUETZAL_EXPRESSION_OPNEGATE_HPP
