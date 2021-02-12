//------------------------------------------------------------------------------
// quetzal framework
// Position.cpp
//------------------------------------------------------------------------------

#include "Position.hpp"
#include <iostream>

using namespace std;

//------------------------------------------------------------------------------
quetzal::framework::Position::Position(int x_, int y_) :
    x(x_),
    y(y_)
{
}

//------------------------------------------------------------------------------
bool quetzal::framework::Position::operator==(const Position& other)
{
    return other.x == x && other.y == y;
}

//------------------------------------------------------------------------------
bool quetzal::framework::Position::operator!=(const Position& other)
{
    return !(*this == other);
}

//------------------------------------------------------------------------------
ostream& quetzal::framework::operator<<(ostream& os, const Position& position)
{
    os << position.x << " " << position.y;
    return  os;
}
