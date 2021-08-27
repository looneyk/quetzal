//------------------------------------------------------------------------------
// OpSubtract.cpp
//
//------------------------------------------------------------------------------

#include "OpSubtract.hpp"
#include <cassert>

//------------------------------------------------------------------------------
quetzal::expression::OpSubtract::OpSubtract(Expression::pointer_type pelhs, Expression::pointer_type perhs) :
    OpBinary(pelhs, perhs)
{
}

//------------------------------------------------------------------------------
quetzal::expression::OpSubtract::~OpSubtract()
{
}

//------------------------------------------------------------------------------
float quetzal::expression::OpSubtract::evaluate_float_(const SymbolTable& st) const
{
    return m_pelhs->evaluate_float(st) - m_perhs->evaluate_float(st);
}

//------------------------------------------------------------------------------
void quetzal::expression::OpSubtract::print(std::ostream& os) const
{
    assert(m_pelhs != nullptr);
    assert(m_perhs != nullptr);

    os << "(";
    os << *m_pelhs << " - " << *m_perhs;
    os << ")";

    return;
}
