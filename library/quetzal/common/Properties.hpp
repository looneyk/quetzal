#if !defined(QUETZAL_PROPERTIES_HPP)
#define QUETZAL_PROPERTIES_HPP
//------------------------------------------------------------------------------
// quetzal
// Properties.hpp
//------------------------------------------------------------------------------

#include <iosfwd>
#include <unordered_map>
#include <utility>

namespace quetzal
{

    //--------------------------------------------------------------------------
    class Properties
    {
    public:

        using values_type = std::unordered_map<std::string, std::string>;
        using iterator = values_type::iterator;
        using const_iterator = values_type::const_iterator;

        Properties() = default;
        Properties(const Properties&) = default;
        Properties(Properties&&) noexcept = default;
        virtual ~Properties() = default;

        Properties& operator=(const Properties&) = default;
        Properties& operator=(Properties&&) = default;

        std::string get(const std::string& name) const;
        void set(const std::string& name, const std::string& value);
        void set(const Properties& properties);

        bool contains(const std::string& name) const;

        void erase(const std::string& name);
        void clear();

        void print(std::ostream& os) const;

        friend void swap(Properties& lhs, Properties& rhs) noexcept
        {
            using std::swap;
            swap(lhs.m_values, rhs.m_values);
        }

        iterator begin() { return m_values.begin(); }
        iterator end() { return m_values.end(); }

        const_iterator begin() const { return m_values.begin(); }
        const_iterator end() const { return m_values.end(); }

        const_iterator cbegin() const { return m_values.begin(); }
        const_iterator cend() const { return m_values.end(); }

    private:

        values_type m_values;
    };

    std::ostream& operator<<(std::ostream& os, const Properties& properties);

} // namespace quetzal

#endif // QUETZAL_PROPERTIES_HPP
