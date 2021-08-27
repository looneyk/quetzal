#if !defined(QUETZAL_PROPERTY_PROPERTYTARGET_HPP)
#define QUETZAL_PROPERTY_PROPERTYTARGET_HPP
//------------------------------------------------------------------------------
// PropertyTarget.hpp
//
//------------------------------------------------------------------------------

#include <map>
#include <memory>
#include "Property.hpp"
#include "PropertyTemplate.hpp"
#include "PropertyObjectTemplate.hpp"

namespace quetzal
{

namespace property
{

    //----------------------------------------------------------------------
    class PropertyTarget
    {
    public:

        typedef std::map<std::string, std::shared_ptr<Property>> Properties; // std::string is Property name

        PropertyTarget();
        PropertyTarget(const PropertyTarget&) = delete;
        virtual ~PropertyTarget();

        PropertyTarget& operator=(const PropertyTarget&) = delete;

        std::string property(const std::string& name) const;
        void set_property(const std::string& name, const std::string& value);

        std::string property_default(const std::string& name) const;
        void set_property_default(const std::string& name, const std::string& default_value);

        template<class C, typename T>
        void register_property(const std::string& name, const T& default_value, T (C::*get)() const, void (C::*set)(const T&), bool bInternal = false);

        template<class C, typename T>
        void register_property(const std::string& name, const T& default_value, C& object, T (C::*get)() const, void (C::*set)(const T&), bool bInternal = false);

        void register_property(const std::string& name, const std::string& default_value = std::string());
        void register_property(std::shared_ptr<Property> pproperty);
        void remove_property(const std::string& name); // Only succeeds for custom properties
        void reset_properties(); // Removes all custom properties

        const Properties& properties() const;
        Properties& properties();

    private:

        Properties m_properties;
    };

    //----------------------------------------------------------------------
    template<class C, typename T>
    void PropertyTarget::register_property(const std::string& name, const T& default_value, T (C::*get)() const, void (C::*set)(const T&), bool bInternal)
    {
        register_property(std::shared_ptr<Property>(new PropertyTemplate<C, T>(name, default_value, get, set, bInternal)));

        return;
    }

    //----------------------------------------------------------------------
    template<class C, typename T>
    void PropertyTarget::register_property(const std::string& name, const T& default_value, C& object, T (C::*get)() const, void (C::*set)(const T&), bool bInternal)
    {
        register_property(std::shared_ptr<Property>(new PropertyObjectTemplate<C, T>(name, default_value, object, get, set, bInternal)));

        return;
    }

} // namespace property

} // namespace quetzal

#endif // QUETZAL_PROPERTY_PROPERTYTARGET_HPP
