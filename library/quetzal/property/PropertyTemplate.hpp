#if !defined(QUETZAL_PROPERTY_PROPERTYTEMPLATE_HPP)
#define QUETZAL_PROPERTY_PROPERTYTEMPLATE_HPP
//------------------------------------------------------------------------------
// PropertyTemplate.hpp
//
//------------------------------------------------------------------------------

#include "Property.hpp"
#include "quetzal/string_util.hpp"

namespace quetzal
{

namespace property
{

    //----------------------------------------------------------------------
    template<class C, typename T>
    class PropertyTemplate : public Property
    {
    public:

        typedef T (C::*get_property_type)() const;
        typedef void (C::*set_property_type)(const T&);

        PropertyTemplate(const std::string& name, const T& default_value, get_property_type get, set_property_type set, bool bInternal = false);
        PropertyTemplate(const PropertyTemplate& property);
        virtual ~PropertyTemplate();

        PropertyTemplate& operator=(const PropertyTemplate& property);

        std::string get(const PropertyTarget* p) const;
        void set(PropertyTarget* p, const std::string& value);

    private:

        get_property_type m_get;
        set_property_type m_set;
    };

    //----------------------------------------------------------------------
    template<class C, typename T>
    PropertyTemplate<C, T>::PropertyTemplate(const std::string& name, const T& default_value, get_property_type get, set_property_type set, bool bInternal) :
        Property(name, to_string(default_value), bInternal, false),
        m_get(get),
        m_set(set)
    {
    }

    //----------------------------------------------------------------------
    template<class C, typename T>
    PropertyTemplate<C, T>::PropertyTemplate(const PropertyTemplate& property) :
        Property(property),
        m_get(property.m_get),
        m_set(property.m_set)
    {
    }

    //----------------------------------------------------------------------
    template<class C, typename T>
    PropertyTemplate<C, T>::~PropertyTemplate()
    {
    }

    //----------------------------------------------------------------------
    template<class C, typename T>
    PropertyTemplate<C, T>& PropertyTemplate<C, T>::operator=(const PropertyTemplate& property)
    {
        if (this != &property)
        {
            Property::operator=(property);
            m_get = property.m_get;
            m_set = property.m_set;
        }

        return *this;
    }

    //----------------------------------------------------------------------
    template<class C, typename T>
    std::string PropertyTemplate<C, T>::get(const PropertyTarget* p) const
    {
        assert(p != nullptr);
        assert(dynamic_cast<const C*>(p) != nullptr);

        const C* pc = static_cast<const C*>(p);

        return to_string((pc->*m_get)());
    }

    //----------------------------------------------------------------------
    template<class C, typename T>
    void PropertyTemplate<C, T>::set(PropertyTarget* p, const std::string& value)
    {
        assert(p != nullptr);
        assert(dynamic_cast<C*>(p) != nullptr);

        C* pc = static_cast<C*>(p);

        (pc->*m_set)(to_type<T>(value));

        return;
    }

} // namespace property

} // namespace quetzal

#endif // QUETZAL_PROPERTY_PROPERTYTEMPLATE_HPP
