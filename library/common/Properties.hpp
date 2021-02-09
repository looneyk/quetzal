#if !defined(QUETZAL_PROPERTIES_HPP)
#define QUETZAL_PROPERTIES_HPP
//------------------------------------------------------------------------------
// quetzal
// Properties.hpp
//------------------------------------------------------------------------------

#include <unordered_map>

namespace quetzal
{

    class Properties
    {
    public:

        std::string property(const std::string& name) const;
        void set_property(const std::string& name, const std::string& value);

        bool contains_property(const std::string& name) const;

        void remove_property(const std::string& name);
        void clear_properties();

    protected:

        Properties() = default;
        Properties(const Properties&) = default;
        Properties(Properties&&) noexcept = default;
        virtual ~Properties() = default;

        Properties& operator=(const Properties&) = default;
        Properties& operator=(Properties&&) = default;

    private:

        std::unordered_map<std::string, std::string> m_values;
    };

} // namespace quetzal

#endif // QUETZAL_PROPERTIES_HPP
