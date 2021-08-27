#if !defined(QUETZAL_EXPRESSION_OPLITERAL_HPP)
#define QUETZAL_EXPRESSION_OPLITERAL_HPP
//------------------------------------------------------------------------------
// OpLiteral.hpp
//
//------------------------------------------------------------------------------

#include "Expression.hpp"

namespace quetzal
{

namespace expression
{

    //--------------------------------------------------------------------------
    class OpLiteral : public Expression
    {
    public:

        OpLiteral(const std::string& c);
        ~OpLiteral();

        virtual void print(std::ostream& os) const;

    protected:

        virtual float evaluate_float_(const SymbolTable& st) const;

    private:

        std::string m_c;
    };

} // namespace expression

} // namespace quetzal

#endif // QUETZAL_EXPRESSION_OPLITERAL_HPP
