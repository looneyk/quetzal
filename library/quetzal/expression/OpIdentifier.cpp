//------------------------------------------------------------------------------
// OpIdentifier.cpp
//
//------------------------------------------------------------------------------

#include "OpIdentifier.hpp"
#include "quetzal/string_util.hpp"
#include "quetzal/Exception.hpp"

//------------------------------------------------------------------------------
quetzal::expression::OpIdentifier::OpIdentifier(const std::string& str) :
    Expression(),
    m_id(str)
{
}

//------------------------------------------------------------------------------
quetzal::expression::OpIdentifier::~OpIdentifier()
{
}

//------------------------------------------------------------------------------
float quetzal::expression::OpIdentifier::evaluate_float_(const SymbolTable& st) const
{
    SymbolTable::const_iterator i  = st.find(m_id);
    if (i != st.end())
    {
        return to_type<float>(i->second);
    }

    std::ostringstream oss;
    oss << "OpIdentifier::evaluate_float_ - identifier not found " << m_id;
    throw Exception(oss.str().c_str(), __FILE__, __LINE__);
}

//------------------------------------------------------------------------------
void quetzal::expression::OpIdentifier::print(std::ostream& os) const
{
    os << m_id;

    return;
}
