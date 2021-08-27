#if !defined(QUETZAL_PROPERTY_PROPERTYCUSTOM_HPP)
#define QUETZAL_PROPERTY_PROPERTYCUSTOM_HPP
//------------------------------------------------------------------------------
// PropertyCustom.hpp
//
//------------------------------------------------------------------------------

#include "Property.hpp"

namespace quetzal
{

namespace property
{

    //----------------------------------------------------------------------
    class PropertyCustom : public Property
    {
    public:

        PropertyCustom(const std::string& name, const std::string& default_value = std::string());
        PropertyCustom(const PropertyCustom& property);
        virtual ~PropertyCustom();

        PropertyCustom& operator=(const PropertyCustom& property);

        std::string get(const PropertyTarget* p) const;
        void set(PropertyTarget* p, const std::string& value);

    private:

        std::string m_value;
    };

} // namespace property

} // namespace quetzal

#endif // QUETZAL_PROPERTY_PROPERTYCUSTOM_HPP
