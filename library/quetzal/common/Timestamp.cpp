//------------------------------------------------------------------------------
// quetzal
// Timestamp.cpp
//------------------------------------------------------------------------------

#include "Timestamp.hpp"
#include <thread>

using namespace std;

//------------------------------------------------------------------------------
quetzal::Timestamp::Timestamp() :
    m_t(clock_type::now())
{
}

//------------------------------------------------------------------------------
quetzal::Timestamp& quetzal::Timestamp::operator+=(double d)
{
    m_t += duration_type(d);
    return *this;
}

//------------------------------------------------------------------------------
quetzal::Timestamp& quetzal::Timestamp::operator-=(double d)
{
    m_t -= duration_type(d);
    return *this;
}

//------------------------------------------------------------------------------
void quetzal::Timestamp::update()
{
    m_t = clock_type::now();
    return;
}

//------------------------------------------------------------------------------
void quetzal::Timestamp::sleep_until() const
{
    this_thread::sleep_until(m_t);
    return;
}

//------------------------------------------------------------------------------
quetzal::Timestamp quetzal::Timestamp::now()
{
    return Timestamp();
}

//------------------------------------------------------------------------------
double quetzal::Timestamp::since(const Timestamp& t)
{
    return now() - t;
}

//------------------------------------------------------------------------------
quetzal::Timestamp quetzal::operator+(Timestamp t, double d)
{
    return t += d;
}

//------------------------------------------------------------------------------
quetzal::Timestamp quetzal::operator+(double d, Timestamp t)
{
    return t += d;
}

//------------------------------------------------------------------------------
quetzal::Timestamp quetzal::operator-(Timestamp t, double d)
{
    return t -= d;
}

//------------------------------------------------------------------------------
double quetzal::operator-(const Timestamp& lhs, const Timestamp& rhs)
{
    return (lhs.m_t - rhs.m_t).count();
}
