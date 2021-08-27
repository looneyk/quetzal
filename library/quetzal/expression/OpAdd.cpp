//------------------------------------------------------------------------------
// OpAdd.cpp
//
//------------------------------------------------------------------------------

#include "OpAdd.hpp"
#include <cassert>

//------------------------------------------------------------------------------
quetzal::expression::OpAdd::OpAdd(Expression::pointer_type pelhs, Expression::pointer_type perhs) :
    OpBinary(pelhs, perhs)
{
}

//------------------------------------------------------------------------------
quetzal::expression::OpAdd::~OpAdd()
{
}

//------------------------------------------------------------------------------
float quetzal::expression::OpAdd::evaluate_float_(const SymbolTable& st) const
{
    return m_pelhs->evaluate_float(st) + m_perhs->evaluate_float(st);
}

//------------------------------------------------------------------------------
void quetzal::expression::OpAdd::print(std::ostream& os) const
{
    assert(m_pelhs != nullptr);
    assert(m_perhs != nullptr);

    os << "(";
    os << *m_pelhs << " + " << *m_perhs;
    os << ")";

    return;
}
