#if !defined(QUETZAL_PROPERTY_PROPERTY_HPP)
#define QUETZAL_PROPERTY_PROPERTY_HPP
//------------------------------------------------------------------------------
// Property.hpp
//
//------------------------------------------------------------------------------

#include <string>

namespace quetzal
{

namespace property
{

    class PropertyTarget;

    //----------------------------------------------------------------------
    class Property
    {
    public:

        Property(const std::string& name, const std::string& default_value = std::string(), bool bInternal = false, bool bCustom = false);
        Property(const Property& property) = default;
        virtual ~Property();

        Property& operator=(const Property& property) = default;

        std::string name() const;
        void set_name(const std::string& name);

        std::string default_value() const;
        void set_default_value(const std::string& default_value);

        bool internal() const;
        void set_internal(bool b);

        bool custom() const;
        void set_custom(bool b);

        virtual std::string get(const PropertyTarget* p) const = 0;
        virtual void set(PropertyTarget* p, const std::string& value) = 0;

    private:

        std::string m_name;
        std::string m_valueDefault;
        bool m_bInternal;
        bool m_bCustom;
    };

} // namespace property

} // namespace quetzal

#endif // QUETZAL_PROPERTY_PROPERTY_HPP
