//------------------------------------------------------------------------------
// OpNegate.cpp
//
//------------------------------------------------------------------------------

#include "OpNegate.hpp"
#include <cassert>

//------------------------------------------------------------------------------
quetzal::expression::OpNegate::OpNegate(Expression::pointer_type pe) :
    OpUnary(pe)
{
}

//------------------------------------------------------------------------------
quetzal::expression::OpNegate::~OpNegate()
{
}

//------------------------------------------------------------------------------
float quetzal::expression::OpNegate::evaluate_float_(const SymbolTable& st) const
{
    assert(m_pe != nullptr);

    return -m_pe->evaluate_float(st);
}

//------------------------------------------------------------------------------
void quetzal::expression::OpNegate::print(std::ostream& os) const
{
    assert(m_pe != nullptr);

    os << "-" << *m_pe;

    return;
}
