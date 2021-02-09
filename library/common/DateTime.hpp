#if !defined(QUETZAL_DATETIME_HPP)
#define QUETZAL_DATETIME_HPP
//------------------------------------------------------------------------------
// DateTime.hpp
//------------------------------------------------------------------------------

#include <string>
#include <chrono>
#include <iosfwd>

namespace quetzal
{

    //--------------------------------------------------------------------------
    class DateTime
    {
    public:

        DateTime();
        DateTime(const DateTime& other);
        ~DateTime();

        DateTime& operator=(DateTime other);

        // Default format is ISO 8601
        std::string to_string(const std::string& format = "") const;
        void set(const std::string& t, const std::string& format = "");

        int days_until(const DateTime& dt) const; // Assumes that dt is later

    private:

        std::chrono::system_clock::time_point m_t;
    };

    std::ostream& operator<<(std::ostream& os, const DateTime& dt);
	
} // namespace quetzal

#endif // QUETZAL_DATETIME_HPP
