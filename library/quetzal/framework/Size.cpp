//------------------------------------------------------------------------------
// quetzal framework
// Size.cpp
//------------------------------------------------------------------------------

#include "Size.hpp"
#include <iostream>

using namespace std;

//------------------------------------------------------------------------------
quetzal::framework::Size::Size(int x_, int y_) :
    x(x_),
    y(y_)
{
}

//------------------------------------------------------------------------------
bool quetzal::framework::Size::operator==(const Size& other)
{
    return other.x == x && other.y == y;
}

//------------------------------------------------------------------------------
bool quetzal::framework::Size::operator!=(const Size& other)
{
    return !(*this == other);
}

//------------------------------------------------------------------------------
double quetzal::framework::Size::aspect_ratio() const
{
    return static_cast<double>(x) / static_cast<double>(y);
}

//------------------------------------------------------------------------------
ostream& quetzal::framework::operator<<(ostream& os, const Size& size)
{
    os << size.x << " " << size.y;
    return  os;
}
