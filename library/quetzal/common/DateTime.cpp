//------------------------------------------------------------------------------
// DateTime.cpp
//------------------------------------------------------------------------------

#include "DateTime.hpp"
#include <iostream>

using namespace std;

//------------------------------------------------------------------------------
quetzal::DateTime::DateTime() :
    m_t(chrono::system_clock::now())
{
}

//------------------------------------------------------------------------------
quetzal::DateTime::DateTime(const DateTime& other) :
    m_t(other.m_t)
{
}

//------------------------------------------------------------------------------
quetzal::DateTime::~DateTime()
{
}

//------------------------------------------------------------------------------
quetzal::DateTime& quetzal::DateTime::operator=(DateTime other)
{
    using std::swap;

    swap(m_t, other.m_t);

    return *this;
}

//------------------------------------------------------------------------------
string quetzal::DateTime::to_string(const string& format) const
{
    time_t tt = chrono::system_clock::to_time_t(m_t);
//    struct tm t = *localtime(&tt);
struct tm t;
localtime_s(&t, &tt);

    char cc[64];
    strftime(cc, size(cc), !format.empty() ? format.c_str() : "%F %T", &t);

    return cc;
}

//------------------------------------------------------------------------------
void quetzal::DateTime::set(const std::string& s, const std::string& format)
{
    struct tm t {};

//    strptime(s.c_str(), !format.empty() ? format.c_str() : "%F %T", &t); // ...
    s;
    format;

    m_t = chrono::system_clock::from_time_t(mktime(&t));

    return;
}

//------------------------------------------------------------------------------
int quetzal::DateTime::days_until(const DateTime& dt) const
{
    return chrono::duration_cast<chrono::hours>(dt.m_t - m_t).count() / 24;
}

//------------------------------------------------------------------------------
ostream& quetzal::operator<<(ostream& os, const DateTime& dt)
{
    os << dt.to_string();
    return os;
}
