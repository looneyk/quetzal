#if !defined(QUETZAL_SVG_ATTRIBUTE_HPP)
#define QUETZAL_SVG_ATTRIBUTE_HPP
//------------------------------------------------------------------------------
// svg
// Attribute.hpp
//------------------------------------------------------------------------------

#include "quetzal/common/string_util.hpp"
#include <string>
#include <iosfwd>

namespace quetzal
{

namespace svg
{

    //--------------------------------------------------------------------------
    template<typename T>
    class Attribute
    {
    public:

        Attribute(const std::string& name, const T& value);
        Attribute(const Attribute&) = default;
        ~Attribute() = default;

        Attribute& operator=(const Attribute&) = default;

        std::string name() const;
        std::string value() const;

        void set_name(const std::string& name);
        void set_value(const T& value);

    private:

        std::string m_name;
        T m_value;
    };

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const Attribute<T>& attribute);    

} // namespace svg

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename T>
quetzal::svg::Attribute<T>::Attribute(const std::string& name, const T& value) :
    m_name(name),
    m_value(value)
{
}

//------------------------------------------------------------------------------
template<typename T>
std::string quetzal::svg::Attribute<T>::name() const
{
    return m_name;
}

//------------------------------------------------------------------------------
template<typename T>
std::string quetzal::svg::Attribute<T>::value() const
{
    return quetzal::to_string(m_value);
}

//------------------------------------------------------------------------------
template<typename T>
void quetzal::svg::Attribute<T>::set_name(const std::string& name)
{
    m_name = name;
    return;
}

//------------------------------------------------------------------------------
template<typename T>
void quetzal::svg::Attribute<T>::set_value(const T& value)
{
    m_value = value;
    return;
}

//------------------------------------------------------------------------------
template<typename T>
std::ostream& quetzal::svg::operator<<(std::ostream& os, const quetzal::svg::Attribute<T>& attribute)
{
    os << attribute.name() << "=\"" << attribute.value() << "\"";
    return os;
}    

#endif // QUETZAL_SVG_ATTRIBUTE_HPP
