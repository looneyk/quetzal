//------------------------------------------------------------------------------
// OpMultiply.cpp
//
//------------------------------------------------------------------------------

#include "OpMultiply.hpp"
#include <cassert>

//------------------------------------------------------------------------------
quetzal::expression::OpMultiply::OpMultiply(Expression::pointer_type pelhs, Expression::pointer_type perhs) :
    OpBinary(pelhs, perhs)
{
}

//------------------------------------------------------------------------------
quetzal::expression::OpMultiply::~OpMultiply()
{
}

//------------------------------------------------------------------------------
float quetzal::expression::OpMultiply::evaluate_float_(const SymbolTable& st) const
{
    return m_pelhs->evaluate_float(st) * m_perhs->evaluate_float(st);
}

//------------------------------------------------------------------------------
void quetzal::expression::OpMultiply::print(std::ostream& os) const
{
    assert(m_pelhs != nullptr);
    assert(m_perhs != nullptr);

    os << "(";
    os << *m_pelhs << " * " << *m_perhs;
    os << ")";

    return;
}
