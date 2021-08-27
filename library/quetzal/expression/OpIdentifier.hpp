#if !defined(QUETZAL_EXPRESSION_OPIDENTIFIER_HPP)
#define QUETZAL_EXPRESSION_OPIDENTIFIER_HPP
//------------------------------------------------------------------------------
// OpIdentifier.hpp
//
//------------------------------------------------------------------------------

#include "Expression.hpp"

namespace quetzal
{

namespace expression
{

    //--------------------------------------------------------------------------
    class OpIdentifier : public Expression
    {
    public:

        OpIdentifier(const std::string& str);
        ~OpIdentifier();

        virtual void print(std::ostream& os) const;

    protected:

        virtual float evaluate_float_(const SymbolTable& st) const;

    private:

        std::string m_id;
    };

} // namespace expression

} // namespace quetzal

#endif // QUETZAL_EXPRESSION_OPIDENTIFIER_HPP
