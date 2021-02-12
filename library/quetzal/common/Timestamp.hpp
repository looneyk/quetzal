#if !defined(QUETZAL_TIMESTAMP_HPP)
#define QUETZAL_TIMESTAMP_HPP
//------------------------------------------------------------------------------
// quetzal
// Timestamp.hpp
//------------------------------------------------------------------------------

#include <chrono>
#include <compare>

namespace quetzal
{

    //--------------------------------------------------------------------------
    class Timestamp
    {
    public:

        using clock_type = std::chrono::high_resolution_clock;
        using duration_type = std::chrono::duration<double>;

        Timestamp();
		Timestamp(const Timestamp&) = default;
        ~Timestamp() = default;

        Timestamp& operator=(const Timestamp&) = default;
        auto operator<=>(const Timestamp&) const = default;
        Timestamp& operator+=(double d);
        Timestamp& operator-=(double d);

        void update();
        void sleep_until() const;

        static Timestamp now();
        static double since(const Timestamp& t);

        friend double operator-(const Timestamp& lhs, const Timestamp& rhs);

    private:

        std::chrono::time_point<clock_type, duration_type> m_t;
    };

    Timestamp operator+(Timestamp t, double d);
    Timestamp operator+(double d, Timestamp t);
    Timestamp operator-(Timestamp t, double d);
    double operator-(const Timestamp& lhs, const Timestamp& rhs);

} // namespace quetzal

#endif // QUETZAL_TIMESTAMP_HPP
