#if !defined(QUETZAL_EXPRESSION_OPFUNCTIONCALL_HPP)
#define QUETZAL_EXPRESSION_OPFUNCTIONCALL_HPP
//------------------------------------------------------------------------------
// OpFunctionCall.hpp
//
//------------------------------------------------------------------------------

#include "Expression.hpp"
#include <string>
#include <vector>

namespace quetzal
{

namespace expression
{

    //--------------------------------------------------------------------------
    class OpFunctionCall : public Expression
    {
    public:

        OpFunctionCall(const std::string& name, std::vector<Expression::pointer_type> arguments);
        OpFunctionCall(const OpFunctionCall&) = delete;
        ~OpFunctionCall();

        OpFunctionCall& operator=(const OpFunctionCall&) = delete;

        virtual void print(std::ostream& os) const;

    protected:

        virtual float evaluate_float_(const SymbolTable& st) const;

        std::string m_name;
        std::vector<Expression::pointer_type> m_arguments;
        // function object (variadic?) ...
    };

} // namespace expression

} // namespace quetzal

#endif // QUETZAL_EXPRESSION_OPFUNCTIONCALL_HPP
