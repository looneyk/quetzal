//------------------------------------------------------------------------------
// OpBinary.cpp
//
//------------------------------------------------------------------------------

#include "OpBinary.hpp"
#include <cassert>

//------------------------------------------------------------------------------
quetzal::expression::OpBinary::OpBinary(Expression::pointer_type pelhs, Expression::pointer_type perhs) :
    Expression(),
    m_pelhs(pelhs),
    m_perhs(perhs)
{
    assert(m_pelhs != nullptr);
    assert(m_perhs != nullptr);
}

//------------------------------------------------------------------------------
quetzal::expression::OpBinary::~OpBinary()
{
}
