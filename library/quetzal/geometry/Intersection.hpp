#if !defined(QUETZAL_GEOMETRY_INTERSECTION_HPP)
#define QUETZAL_GEOMETRY_INTERSECTION_HPP
//------------------------------------------------------------------------------
// geometry
// Intersection.hpp
//------------------------------------------------------------------------------

#include "Point.hpp"
#include "Line.hpp"
#include "Locus.hpp"
#include "Ray.hpp"
#include "Segment.hpp"
//#include "Polyline.hpp"
//#include "Plane.hpp"
//#include "Circle.hpp"
#include "Polygon.hpp"
//#include "Sphere.hpp"
//#include "Polyhedron.hpp"
#include <iostream>

namespace quetzal::geometry
{

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Intersection
    {
    public:

        using point_type = Point<Traits>;
        using line_type = Line<Traits>;
        using ray_type = Ray<Traits>;
        using segment_type = Segment<Traits>;
//        using plane_type = Plane<Traits>;
        using polygon_type = Polygon<Traits>;

        Intersection();
        explicit Intersection(const point_type& point);
        explicit Intersection(const line_type& line);
        explicit Intersection(const ray_type& ray);
        explicit Intersection(const segment_type& segment);
//        explicit Intersection(const plane_type& plane);
        explicit Intersection(const polygon_type& polygon);
        Intersection(const Intersection&) = default;
        Intersection(Intersection&&) = default;
        ~Intersection() = default;

        Intersection& operator=(const Intersection&) = default;
        Intersection& operator=(Intersection&&) = default;

        Locus locus() const;
        point_type point() const;
        line_type line() const;
        ray_type ray() const;
        segment_type segment() const;
//        plane_type plane() const;
        polygon_type polygon() const;

        bool empty() const;

    private:

        Locus m_locus;
        point_type m_point;
        line_type m_line;
        ray_type m_ray;
        segment_type m_segment;
//        plane_type m_plane;
        polygon_type m_polygon;
    };

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, const Intersection<Traits>& intersection);

} // namespace quetzal::geometry

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits>::Intersection() :
    m_locus(Locus::Empty),
    m_point(),
    m_line(),
    m_ray(),
    m_segment(),
//    m_plane(),
    m_polygon()
{
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits>::Intersection(const point_type& point) :
    m_locus(Locus::Point),
    m_point(point),
    m_line(),
    m_ray(),
    m_segment(),
//    m_plane(),
    m_polygon()
{
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits>::Intersection(const line_type& line) :
    m_locus(Locus::Line),
    m_point(),
    m_line(line),
    m_ray(),
    m_segment(),
//    m_plane(),
    m_polygon()
{
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits>::Intersection(const ray_type& ray) :
    m_locus(Locus::Ray),
    m_point(),
    m_line(),
    m_ray(ray),
    m_segment(),
//    m_plane(),
    m_polygon()
{
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits>::Intersection(const segment_type& segment) :
    m_locus(Locus::Segment),
    m_point(),
    m_line(),
    m_ray(),
    m_segment(segment),
//    m_plane(),
    m_polygon()
{
}

//------------------------------------------------------------------------------
/*
template<typename Traits>
quetzal::geometry::Intersection<Traits>::Intersection(const plane_type& plane) :
    m_locus(Locus::Plane),
    m_point(),
    m_line(),
    m_ray(),
    m_segment(),
    m_plane(plane),
    m_polygon()
{
}
*/

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits>::Intersection(const polygon_type& polygon) :
    m_locus(Locus::Polygon),
    m_point(),
    m_line(),
    m_ray(),
    m_segment(),
//    m_plane(),
    m_polygon(polygon)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Locus quetzal::geometry::Intersection<Traits>::locus() const
{
    return m_locus;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Point<Traits> quetzal::geometry::Intersection<Traits>::point() const
{
    assert(m_locus == Locus::Point);
    return m_point;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Line<Traits> quetzal::geometry::Intersection<Traits>::line() const
{
    assert(m_locus == Locus::Line);
    return m_line;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Ray<Traits> quetzal::geometry::Intersection<Traits>::ray() const
{
    assert(m_locus == Locus::Ray);
    return m_ray;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Segment<Traits> quetzal::geometry::Intersection<Traits>::segment() const
{
    assert(m_locus == Locus::Segment);
    return m_segment;
}

//------------------------------------------------------------------------------
/*
template<typename Traits>
typename quetzal::geometry::Plane<Traits> quetzal::geometry::Intersection<Traits>::plane() const
{
    assert(m_locus == Locus::Plane);
    return m_plane;
}
*/

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Polygon<Traits> quetzal::geometry::Intersection<Traits>::polygon() const
{
    assert(m_locus == Locus::Polygon);
    return m_polygon;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Intersection<Traits>::empty() const
{
    return m_locus == Locus::Empty;
}

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, const Intersection<Traits>& intersection)
{
    switch (intersection.locus())
    {
        case Locus::Empty:
            os << "Empty" << std::endl;
            break;

        case Locus::Point:
            os << "Point [" << intersection.point() << "]" << std::endl;
            break;

        case Locus::Line:
            os << "Line " << intersection.line() << std::endl;
            break;

        case Locus::Ray:
            os << "Ray " << intersection.ray() << std::endl;
            break;

        case Locus::Segment:
            os << "Segment " << intersection.segment() << std::endl;
            break;

/*
        case Locus::Plane:
            os << "Plane " << intersection.plane() << std::endl;
            break;
*/

        case Locus::Polygon:
            os << "Polygon " << intersection.polygon() << std::endl;
            break;
    }

    return os;
}

#endif // QUETZAL_GEOMETRY_INTERSECTION_HPP
