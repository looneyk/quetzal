#if !defined(QUETZAL_GEOMETRY_LOCUS_HPP)
#define QUETZAL_GEOMETRY_LOCUS_HPP
//------------------------------------------------------------------------------
// geometry
// Locus.hpp
//------------------------------------------------------------------------------

#include <iosfwd>

namespace quetzal
{

namespace geometry
{

    //--------------------------------------------------------------------------
    enum class Locus
    {
        Empty,
        Point,
        Line,
        Ray,
        Segment,
        Polyline,
        Plane,
        Circle,
        Polygon,
        Surface,
        Sphere,
        Polyhedron
    };

    std::ostream& operator<<(std::ostream& os, const Locus& locus);

} // namespace geometry

} // namespace quetzal

#endif // QUETZAL_GEOMETRY_LOCUS_HPP
