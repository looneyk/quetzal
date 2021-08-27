//------------------------------------------------------------------------------
// OpFunctionCall.cpp
//
//------------------------------------------------------------------------------

#include "OpFunctionCall.hpp"
#include "quetzal/Exception.hpp"
#include <algorithm>
#include <sstream>
#include <cassert>

using namespace std;

//------------------------------------------------------------------------------
quetzal::expression::OpFunctionCall::OpFunctionCall(const string& name, vector<Expression::pointer_type> arguments) :
    Expression(),
    m_name(name),
    m_arguments(arguments)
{
}

//------------------------------------------------------------------------------
quetzal::expression::OpFunctionCall::~OpFunctionCall()
{
}

//------------------------------------------------------------------------------
float quetzal::expression::OpFunctionCall::evaluate_float_(const SymbolTable& st) const
{
    // must be a better, more generic way to do this ...

    if (m_name == "abs")
    {
        assert(m_arguments.size() == 1);

        const Expression::pointer_type& pe0 = m_arguments[0];
        assert(pe0 != nullptr);

        return abs(pe0->evaluate_float(st));
    }
    else if (m_name == "min")
    {
        assert(m_arguments.size() == 2);

        const Expression::pointer_type& pe0 = m_arguments[0];
        assert(pe0 != nullptr);

        const Expression::pointer_type& pe1 = m_arguments[1];
        assert(pe1 != nullptr);

        return min(pe0->evaluate_float(st), pe1->evaluate_float(st));
    }
    else if (m_name == "max")
    {
        assert(m_arguments.size() == 2);

        const Expression::pointer_type& pe0 = m_arguments[0];
        assert(pe0 != nullptr);

        const Expression::pointer_type& pe1 = m_arguments[1];
        assert(pe1 != nullptr);

        return max(pe0->evaluate_float(st), pe1->evaluate_float(st));
    }

    ostringstream oss;
    oss << "OpFunctionCall::evaluate_float_ - function not found " << m_name;
    throw Exception(oss.str().c_str(), __FILE__, __LINE__);
}

//------------------------------------------------------------------------------
void quetzal::expression::OpFunctionCall::print(ostream& os) const
{
    os << m_name << "(";

    for (unsigned int i = 0; i < m_arguments.size(); ++i)
    {
        if (i > 0)
        {
            os << ", ";
        }

        Expression::pointer_type pe = m_arguments[i];
        assert(pe != nullptr);

        os << *pe;
    }

    os << ")";

    return;
}
