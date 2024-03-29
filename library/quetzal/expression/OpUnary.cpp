//------------------------------------------------------------------------------
// OpUnary.cpp
//
//------------------------------------------------------------------------------

#include "OpUnary.hpp"
#include <cassert>

//------------------------------------------------------------------------------
quetzal::expression::OpUnary::OpUnary(Expression::pointer_type pe) :
    Expression(),
    m_pe(pe)
{
    assert(m_pe != nullptr);
}

//------------------------------------------------------------------------------
quetzal::expression::OpUnary::~OpUnary()
{
}
