#if !defined(QUETZAL_EXPRESSION_OPDIVIDE_HPP)
#define QUETZAL_EXPRESSION_OPDIVIDE_HPP
//------------------------------------------------------------------------------
// OpDivide.hpp
//
//------------------------------------------------------------------------------

#include "OpBinary.hpp"

namespace quetzal
{

namespace expression
{

    //--------------------------------------------------------------------------
    class OpDivide : public OpBinary
    {
    public:

        OpDivide(Expression::pointer_type pelhs, Expression::pointer_type perhs);
        OpDivide(const OpDivide&) = delete;
        ~OpDivide();

        OpDivide& operator=(const OpDivide&) = delete;

        virtual void print(std::ostream& os) const;

    protected:

        virtual float evaluate_float_(const SymbolTable& st) const;
    };

} // namespace expression

} // namespace quetzal

#endif // QUETZAL_EXPRESSION_OPDIVIDE_HPP
