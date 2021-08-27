#if !defined(QUETZAL_EXPRESSION_OPADD_HPP)
#define QUETZAL_EXPRESSION_OPADD_HPP
//------------------------------------------------------------------------------
// OpAdd.hpp
//
//------------------------------------------------------------------------------

#include "OpBinary.hpp"

namespace quetzal
{

namespace expression
{

    //--------------------------------------------------------------------------
    class OpAdd : public OpBinary
    {
    public:

        OpAdd(Expression::pointer_type pelhs, Expression::pointer_type perhs);
        OpAdd(const OpAdd&) = delete;
        ~OpAdd();

        OpAdd& operator=(const OpAdd&) = delete;

        virtual void print(std::ostream& os) const;

    protected:

        virtual float evaluate_float_(const SymbolTable& st) const;
    };

} // namespace expression

} // namespace quetzal

#endif // QUETZAL_EXPRESSION_OPADD_HPP
