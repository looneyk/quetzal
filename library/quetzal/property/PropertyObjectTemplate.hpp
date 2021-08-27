#if !defined(QUETZAL_PROPERTY_PROPERTYOBJECTTEMPLATE_HPP)
#define QUETZAL_PROPERTY_PROPERTYOBJECTTEMPLATE_HPP
//------------------------------------------------------------------------------
// PropertyObjectTemplate.hpp
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
    class PropertyObjectTemplate : public Property
    {
    public:

        typedef T (C::*get_property_type)() const;
        typedef void (C::*set_property_type)(const T&);

        PropertyObjectTemplate(const std::string& name, const T& default_value, C& object, get_property_type get, set_property_type set, bool bInternal = false);
        PropertyObjectTemplate(const PropertyObjectTemplate& property);
        virtual ~PropertyObjectTemplate();

        PropertyObjectTemplate& operator=(const PropertyObjectTemplate& property);

        std::string get(const PropertyTarget* p) const;
        void set(PropertyTarget* p, const std::string& value);

    private:

        C& m_object;
        get_property_type m_get;
        set_property_type m_set;
    };

    //----------------------------------------------------------------------
    template<class C, typename T>
    PropertyObjectTemplate<C, T>::PropertyObjectTemplate(const std::string& name, const T& default_value, C& object, get_property_type get, set_property_type set, bool bInternal) :
        Property(name, to_string(default_value), bInternal, false),
        m_object(object),
        m_get(get),
        m_set(set)
    {
    }

    //----------------------------------------------------------------------
    template<class C, typename T>
    PropertyObjectTemplate<C, T>::PropertyObjectTemplate(const PropertyObjectTemplate& property) :
        Property(property),
        m_object(property.m_object),
        m_get(property.m_get),
        m_set(property.m_set)
    {
    }

    //----------------------------------------------------------------------
    template<class C, typename T>
    PropertyObjectTemplate<C, T>::~PropertyObjectTemplate()
    {
    }

    //----------------------------------------------------------------------
    template<class C, typename T>
    PropertyObjectTemplate<C, T>& PropertyObjectTemplate<C, T>::operator=(const PropertyObjectTemplate& property)
    {
        if (this != &property)
        {
            Property::operator=(property);
            m_object = property.m_object;
            m_get = property.m_get;
            m_set = property.m_set;
        }

        return *this;
    }

    //----------------------------------------------------------------------
    template<class C, typename T>
    std::string PropertyObjectTemplate<C, T>::get(const PropertyTarget* p) const
    {
        assert(p != nullptr);
        p; // Unused here

        return to_string((m_object.*m_get)());
    }

    //----------------------------------------------------------------------
    template<class C, typename T>
    void PropertyObjectTemplate<C, T>::set(PropertyTarget* p, const std::string& value)
    {
        assert(p != nullptr);
        p; // Unused here

        (m_object.*m_set)(to_type<T>(value));

        return;
    }

} // namespace property

} // namespace quetzal

#endif // QUETZAL_PROPERTY_PROPERTYOBJECTTEMPLATE_HPP
