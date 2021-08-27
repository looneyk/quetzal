//------------------------------------------------------------------------------
// OpLiteral.cpp
//
//------------------------------------------------------------------------------

#include "OpLiteral.hpp"
#include "quetzal/string_util.hpp"
#include "quetzal/Exception.hpp"

//------------------------------------------------------------------------------
quetzal::expression::OpLiteral::OpLiteral(const std::string& c) :
    Expression(),
    m_c(c)
{
}

//------------------------------------------------------------------------------
quetzal::expression::OpLiteral::~OpLiteral()
{
}

//------------------------------------------------------------------------------
float quetzal::expression::OpLiteral::evaluate_float_(const SymbolTable& st) const
{
    st;

    return to_type<float>(m_c);
}

//------------------------------------------------------------------------------
void quetzal::expression::OpLiteral::print(std::ostream& os) const
{
    os << m_c;

    return;
}
