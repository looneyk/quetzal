#if !defined(QUETZAL_EXPRESSION_OPSUBTRACT_HPP)
#define QUETZAL_EXPRESSION_OPSUBTRACT_HPP
//------------------------------------------------------------------------------
// OpSubtract.hpp
//
//------------------------------------------------------------------------------

#include "OpBinary.hpp"

namespace quetzal
{

namespace expression
{

    //--------------------------------------------------------------------------
    class OpSubtract : public OpBinary
    {
    public:

        OpSubtract(Expression::pointer_type pelhs, Expression::pointer_type perhs);
        OpSubtract(const OpSubtract&) = delete;
        ~OpSubtract();

        OpSubtract& operator=(const OpSubtract&) = delete;

        virtual void print(std::ostream& os) const;

    protected:

        virtual float evaluate_float_(const SymbolTable& st) const;
    };

} // namespace expression

} // namespace quetzal

#endif // QUETZAL_EXPRESSION_OPSUBTRACT_HPP
