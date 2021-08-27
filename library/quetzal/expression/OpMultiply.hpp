#if !defined(QUETZAL_EXPRESSION_OPMULTIPLY_HPP)
#define QUETZAL_EXPRESSION_OPMULTIPLY_HPP
//------------------------------------------------------------------------------
// OpMultiply.hpp
//
//------------------------------------------------------------------------------

#include "OpBinary.hpp"

namespace quetzal
{

namespace expression
{

    //--------------------------------------------------------------------------
    class OpMultiply : public OpBinary
    {
    public:

        OpMultiply(Expression::pointer_type pelhs, Expression::pointer_type perhs);
        OpMultiply(const OpMultiply&) = delete;
        ~OpMultiply();

        OpMultiply& operator=(const OpMultiply&) = delete;

        virtual void print(std::ostream& os) const;

    protected:

        virtual float evaluate_float_(const SymbolTable& st) const;
    };

} // namespace expression

} // namespace quetzal

#endif // QUETZAL_EXPRESSION_OPMULTIPLY_HPP
