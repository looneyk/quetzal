//------------------------------------------------------------------------------
// Expression.cpp
//
//------------------------------------------------------------------------------

#include "Expression.hpp"
#include <memory>
#include <iosfwd>

//------------------------------------------------------------------------------
quetzal::expression::Expression::Expression() :
    m_bParenthesized(false)
{
}

//------------------------------------------------------------------------------
quetzal::expression::Expression::~Expression()
{
}

//------------------------------------------------------------------------------
bool quetzal::expression::Expression::parenthesized() const
{
    return m_bParenthesized;
}

//------------------------------------------------------------------------------
void quetzal::expression::Expression::set_parenthesized(const bool& b)
{
    m_bParenthesized = b;

    return;
}

//------------------------------------------------------------------------------
float quetzal::expression::Expression::evaluate_float(const SymbolTable& st) const
{
    try
    {
        return evaluate_float_(st);
    }
//        catch (const Exception& e)
//        {
//            std::cout << "evaluate error: " << e.what() << endl;
//        }
    catch (...)
    {
    }

    return 0.f;
}

//------------------------------------------------------------------------------
std::ostream& quetzal::expression::operator<<(std::ostream& os, const Expression& e)
{
    e.print(os);

    return os;
}
