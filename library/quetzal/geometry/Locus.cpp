//------------------------------------------------------------------------------
// geometry
// Locus.hpp
//------------------------------------------------------------------------------

#include "Locus.hpp"
#include <iostream>
#include <cassert>

using namespace std;

//------------------------------------------------------------------------------
ostream& quetzal::geometry::operator<<(ostream& os, const Locus& locus)
{
    switch (locus)
    {
        case Locus::Empty:
            os << "Empty";
            break;

        case Locus::Point:
            os << "Point";
            break;

        case Locus::Line:
            os << "Line";
            break;

        case Locus::Ray:
            os << "Ray";
            break;

        case Locus::Segment:
            os << "Segment";
            break;

        case Locus::Polyline:
            os << "Polyline";
            break;

        case Locus::Plane:
            os << "Plane";
            break;

        case Locus::Circle:
            os << "Circle";
            break;

        case Locus::Polygon:
            os << "Polygon";
            break;

        case Locus::Surface:
            os << "Surface";
            break;

        case Locus::Sphere:
            os << "Sphere";
            break;

        case Locus::Polyhedron:
            os << "Polyhedron";
            break;

        default:
            assert(false);
    }

    return os;
}
