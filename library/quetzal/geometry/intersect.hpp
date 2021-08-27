#if !defined(QUETZAL_GEOMETRY_INTERSECT_HPP)
#define QUETZAL_GEOMETRY_INTERSECT_HPP
//------------------------------------------------------------------------------
// geometry
// intersect.hpp
//
// Functions that have polygon parameters only support planar convex polygons
//
//------------------------------------------------------------------------------

#include "Line.hpp"
#include "Intersection.hpp"
#include "Point.hpp"
#include "Polygon.hpp"
#include "Plane.hpp"
#include "Ray.hpp"
#include "Segment.hpp"
#include "distance.hpp"
#include "relationships.hpp"
#include "quetzal/math/DimensionReducer.hpp"
#include "quetzal/math/Vector.hpp"
#include "quetzal/math/floating_point.hpp"
#include <vector>
#include <cassert>

#include "quetzal/math/VectorTraits.hpp" // ...

namespace quetzal::geometry
{

    template<typename Traits>
    bool intersects(const Point<Traits>& pointA, const Point<Traits>& pointB);

    template<typename Traits>
    bool intersects(const Point<Traits>& point, const Line<Traits>& line);

    template<typename Traits>
    bool intersects(const Point<Traits>& point, const Ray<Traits>& ray);

    template<typename Traits>
    bool intersects(const Point<Traits>& point, const Segment<Traits>& segment);

    template<typename Traits>
    bool intersects(const Point<Traits>& point, const Plane<Traits>& plane);

    template<typename Traits>
    bool intersects(const Point<Traits>& point, const Polygon<Traits>& polygon);

    template<typename Traits>
    bool intersects(const Line<Traits>& line, const Point<Traits>& point);

    template<typename Traits>
    bool intersects(const Line<Traits>& lineA, const Line<Traits>& lineB);

    template<typename Traits>
    bool intersects(const Line<Traits>& line, const Ray<Traits>& ray);

    template<typename Traits>
    bool intersects(const Line<Traits>& line, const Segment<Traits>& segment);

    template<typename Traits>
    bool intersects(const Line<Traits>& line, const Plane<Traits>& plane);

    template<typename Traits>
    bool intersects(const Line<Traits>& line, const Polygon<Traits>& polygon);

    template<typename Traits>
    bool intersects(const Ray<Traits>& ray, const Point<Traits>& point);

    template<typename Traits>
    bool intersects(const Ray<Traits>& ray, const Line<Traits>& line);

    template<typename Traits>
    bool intersects(const Ray<Traits>& rayA, const Ray<Traits>& rayB);

    template<typename Traits>
    bool intersects(const Ray<Traits>& ray, const Segment<Traits>& segment);

    template<typename Traits>
    bool intersects(const Ray<Traits>& ray, const Plane<Traits>& plane);

    template<typename Traits>
    bool intersects(const Ray<Traits>& ray, const Polygon<Traits>& polygon);

    template<typename Traits>
    bool intersects(const Segment<Traits>& segment, const Point<Traits>& point);

    template<typename Traits>
    bool intersects(const Segment<Traits>& segment, const Line<Traits>& line);

    template<typename Traits>
    bool intersects(const Segment<Traits>& segment, const Ray<Traits>& ray);

    template<typename Traits>
    bool intersects(const Segment<Traits>& segmentA, const Segment<Traits>& segmentB);

    template<typename Traits>
    bool intersects(const Segment<Traits>& segment, const Plane<Traits>& plane);

    template<typename Traits>
    bool intersects(const Segment<Traits>& segment, const Polygon<Traits>& polygon);

    template<typename Traits>
    bool intersects(const Plane<Traits>& plane, const Point<Traits>& point);

    template<typename Traits>
    bool intersects(const Plane<Traits>& plane, const Line<Traits>& line);

    template<typename Traits>
    bool intersects(const Plane<Traits>& plane, const Ray<Traits>& ray);

    template<typename Traits>
    bool intersects(const Plane<Traits>& plane, const Segment<Traits>& segment);

    template<typename Traits>
    bool intersects(const Plane<Traits>& planeA, const Plane<Traits>& planeB);

    template<typename Traits>
    bool intersects(const Plane<Traits>& plane, const Polygon<Traits>& polygon);

    template<typename Traits>
    bool intersects(const Polygon<Traits>& polygon, const Point<Traits>& point);

    template<typename Traits>
    bool intersects(const Polygon<Traits>& polygon, const Line<Traits>& line);

    template<typename Traits>
    bool intersects(const Polygon<Traits>& polygon, const Ray<Traits>& ray);

    template<typename Traits>
    bool intersects(const Polygon<Traits>& polygon, const Segment<Traits>& segment);

    template<typename Traits>
    bool intersects(const Polygon<Traits>& polygon, const Plane<Traits>& plane);

    template<typename Traits>
    bool intersects(const Polygon<Traits>& polygonA, const Polygon<Traits>& polygonB);

    template<typename Traits>
    Intersection<Traits> intersection(const Point<Traits>& pointA, const Point<Traits>& pointB);

    template<typename Traits>
    Intersection<Traits> intersection(const Point<Traits>& point, const Line<Traits>& line);

    template<typename Traits>
    Intersection<Traits> intersection(const Point<Traits>& point, const Ray<Traits>& ray);

    template<typename Traits>
    Intersection<Traits> intersection(const Point<Traits>& point, const Segment<Traits>& segment);

    template<typename Traits>
    Intersection<Traits> intersection(const Point<Traits>& point, const Plane<Traits>& plane);

    template<typename Traits>
    Intersection<Traits> intersection(const Point<Traits>& point, const Polygon<Traits>& polygon);

    template<typename Traits>
    Intersection<Traits> intersection(const Line<Traits>& line, const Point<Traits>& point);

    template<typename Traits>
    Intersection<Traits> intersection(const Line<Traits>& lineA, const Line<Traits>& lineB);

    template<typename Traits>
    Intersection<Traits> intersection(const Line<Traits>& line, const Ray<Traits>& ray);

    template<typename Traits>
    Intersection<Traits> intersection(const Line<Traits>& line, const Segment<Traits>& segment);

    template<typename Traits>
    Intersection<Traits> intersection(const Line<Traits>& line, const Plane<Traits>& plane);

    template<typename Traits>
    Intersection<Traits> intersection(const Line<Traits>& line, const Polygon<Traits>& polygon);

    template<typename Traits>
    Intersection<Traits> intersection(const Ray<Traits>& ray, const Point<Traits>& point);

    template<typename Traits>
    Intersection<Traits> intersection(const Ray<Traits>& ray, const Line<Traits>& line);

    template<typename Traits>
    Intersection<Traits> intersection(const Ray<Traits>& rayA, const Ray<Traits>& rayB);

    template<typename Traits>
    Intersection<Traits> intersection(const Ray<Traits>& ray, const Segment<Traits>& segment);

    template<typename Traits>
    Intersection<Traits> intersection(const Ray<Traits>& ray, const Plane<Traits>& plane);

    template<typename Traits>
    Intersection<Traits> intersection(const Ray<Traits>& ray, const Polygon<Traits>& polygon);

    template<typename Traits>
    Intersection<Traits> intersection(const Segment<Traits>& segment, const Point<Traits>& point);

    template<typename Traits>
    Intersection<Traits> intersection(const Segment<Traits>& segment, const Line<Traits>& line);

    template<typename Traits>
    Intersection<Traits> intersection(const Segment<Traits>& segment, const Ray<Traits>& ray);

    template<typename Traits>
    Intersection<Traits> intersection(const Segment<Traits>& segmentA, const Segment<Traits>& segmentB);

    template<typename Traits>
    Intersection<Traits> intersection(const Segment<Traits>& segment, const Plane<Traits>& plane);

    template<typename Traits>
    Intersection<Traits> intersection(const Segment<Traits>& segment, const Polygon<Traits>& polygon);

    template<typename Traits>
    Intersection<Traits> intersection(const Plane<Traits>& plane, const Point<Traits>& point);

    template<typename Traits>
    Intersection<Traits> intersection(const Plane<Traits>& plane, const Line<Traits>& line);

    template<typename Traits>
    Intersection<Traits> intersection(const Plane<Traits>& plane, const Ray<Traits>& ray);

    template<typename Traits>
    Intersection<Traits> intersection(const Plane<Traits>& plane, const Segment<Traits>& segment);

    template<typename Traits>
    Intersection<Traits> intersection(const Plane<Traits>& planeA, const Plane<Traits>& planeB);

    template<typename Traits>
    Intersection<Traits> intersection(const Plane<Traits>& plane, const Polygon<Traits>& polygon);

    template<typename Traits>
    Intersection<Traits> intersection(const Polygon<Traits>& polygon, const Point<Traits>& point);

    template<typename Traits>
    Intersection<Traits> intersection(const Polygon<Traits>& polygon, const Line<Traits>& line);

    template<typename Traits>
    Intersection<Traits> intersection(const Polygon<Traits>& polygon, const Ray<Traits>& ray);

    template<typename Traits>
    Intersection<Traits> intersection(const Polygon<Traits>& polygon, const Segment<Traits>& segment);

    template<typename Traits>
    Intersection<Traits> intersection(const Polygon<Traits>& polygon, const Plane<Traits>& plane);

    template<typename Traits>
    Intersection<Traits> intersection(const Polygon<Traits>& polygonA, const Polygon<Traits>& polygonB);

    template<typename Traits>
    Intersection<Traits> intersection(const Plane<Traits>& planeA, const Plane<Traits>& planeB, const Plane<Traits>& planeC);

    template<typename Traits>
    bool intersects_coplanar(const Point<Traits>& point, const Polygon<Traits>& polygon);

} // namespace quetzal::geometry

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Point<Traits>& pointA, const Point<Traits>& pointB)
{
    return vector_eq(pointA, pointB);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Point<Traits>& point, const Line<Traits>& line)
{
    return line.contains(point);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Point<Traits>& point, const Ray<Traits>& ray)
{
    return ray.contains(point);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Point<Traits>& point, const Segment<Traits>& segment)
{
    return segment.contains(point);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Point<Traits>& point, const Plane<Traits>& plane)
{
    return plane.contains(point);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Point<Traits>& point, const Polygon<Traits>& polygon)
{
    return polygon.contains(point);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Line<Traits>& line, const Point<Traits>& point)
{
    return intersects(point, line);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Line<Traits>& lineA, const Line<Traits>& lineB)
{
    if (parallel(lineA, lineB))
    {
        return lineB.contains(lineA.point());
    }

    if constexpr (Traits::dimension == 3)
    {
        return math::float_eq0(distance(lineA, lineB));
    }
    else if constexpr (Traits::dimension == 2)
    {
        return true;
    }
    else
    {
        static_assert(false, "Dimension out of range.");
    }
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Line<Traits>& line, const Ray<Traits>& ray)
{
    if (parallel(line, ray))
    {
        return line.contains(ray.point());
    }

    auto [s, t] = closest_point_parameters(line, Line<Traits>(ray.endpoint(), ray.direction()));
    return math::float_ge0(t) && math::float_eq0((line.point(s) - ray.endpoint(t)).norm_squared()); // pointA == pointB ...
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Line<Traits>& line, const Segment<Traits>& segment)
{
    if (parallel(line, segment))
    {
        return line.contains(segment.endpoint(0));
    }

    auto [s, t] = closest_point_parameters(line.point(), line.direction(), segment.endpoint(0), segment.vector());
    return math::float_clamped01(t) && math::float_eq0((line.point(s) - segment.point(t)).norm_squared()); // pointA == pointB ...
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Line<Traits>& line, const Plane<Traits>& plane)
{
    return !parallel(line, plane) || plane.contains(line.point());
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Line<Traits>& line, const Polygon<Traits>& polygon)
{
    if constexpr (Traits::dimension >= 3)
    {
        Intersection<Traits> inter = intersection(line, polygon.plane());
        if (inter.locus() == Locus::Empty)
        {
            return false;
        }
        else if (inter.locus() == Locus::Point)
        {
            return polygon.contains(inter.point()); // but no need for plane check ...
        }

        assert(inter.locus() == Locus::Line);
    }

    const Polygon<Traits>::vertices_type& vertices = polygon.vertices();

    for (size_t i = 0; i < polygon.vertex_count(); ++i)
    {
        Point<Traits> point0 = vertices[i];
        Point<Traits> point1 = vertices[(i + 1) % polygon.vertex_count()];

        if (intersects(line, Segment<Traits>(point0, point1)))
        {
            return true;
        }
    }

    return false;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Ray<Traits>& ray, const Point<Traits>& point)
{
    return intersects(point, ray);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Ray<Traits>& ray, const Line<Traits>& line)
{
    return intersects(line, ray);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Ray<Traits>& rayA, const Ray<Traits>& rayB)
{
    if (parallel(rayA, rayB))
    {
        return rayA.contains(rayB.point()) || rayB.contains(rayA.point());
    }

    auto [s, t] = closest_point_parameters(Line<Traits>(rayA.point(), rayA.direction()), Line<Traits>(rayB.point(), rayB.direction()));
    return math::float_ge0(s) && math::float_ge0(t) && math::float_eq0((rayA.point(s) - rayB.point(t)).norm_squared()); // pointA == pointB ...
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Ray<Traits>& ray, const Segment<Traits>& segment)
{
    if (parallel(ray, segment))
    {
        return ray.contains(segment.endpoint(0)) || ray.contains(segment.endpoint(1));
    }

    auto [s, t] = closest_point_parameters(ray.endpoint(), ray.direction(), segment.endpoint(0), segment.vector());
    return math::float_ge0(s) && math::float_clamped01(t) && math::float_eq0((ray.point(s) - segment.point(t)).norm_squared()); // pointA == pointB ...
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Ray<Traits>& ray, const Plane<Traits>& plane)
{
    if (parallel(ray, plane))
    {
        return plane.contains(ray.endpoint());
    }

    return math::float_ge0(dot(plane.point() - ray.endpoint(), ray.direction()));
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Ray<Traits>& ray, const Polygon<Traits>& polygon)
{
    if constexpr (Traits::dimension >= 3)
    {
        Intersection<Traits> inter = intersection(ray, polygon.plane());
        if (inter.locus() == Locus::Empty)
        {
            return false;
        }
        else if (inter.locus() == Locus::Point)
        {
            return polygon.contains(inter.point()); // but no need for plane check ...
        }

        assert(inter.locus() == Locus::Ray);
    }

    const Polygon<Traits>::vertices_type& vertices = polygon.vertices();

    for (size_t i = 0; i < polygon.vertex_count(); ++i)
    {
        Point<Traits> point0 = vertices[i];
        Point<Traits> point1 = vertices[(i + 1) % polygon.vertex_count()];

        if (intersects(ray, Segment<Traits>(point0, point1)))
        {
            return true;
        }
    }

    return false;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Segment<Traits>& segment, const Point<Traits>& point)
{
    return intersects(point, segment);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Segment<Traits>& segment, const Line<Traits>& line)
{
    return intersects(line, segment);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Segment<Traits>& segment, const Ray<Traits>& ray)
{
    return intersects(ray, segment);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Segment<Traits>& segmentA, const Segment<Traits>& segmentB)
{
    if (parallel(segmentA, segmentB))
    {
        return segmentA.contains(segmentB.endpoint(0)) || segmentA.contains(segmentB.endpoint(1))
            || segmentB.contains(segmentA.endpoint(0)) || segmentB.contains(segmentA.endpoint(1));
    }

    auto [s, t] = closest_point_parameters(segmentA.endpoint(0), segmentA.vector(), segmentB.endpoint(0), segmentB.vector());
    return math::float_clamped01(s) && math::float_clamped01(t) && math::float_eq0((segmentA.point(s) - segmentB.point(t)).norm_squared()); // pointA == pointB ...
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Segment<Traits>& segment, const Plane<Traits>& plane)
{
    auto p = plane.compare(segment.endpoint(0));
    auto q = plane.compare(segment.endpoint(1));
    return p != q || p == 0;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Segment<Traits>& segment, const Polygon<Traits>& polygon)
{
    if constexpr (Traits::dimension >= 3)
    {
        Intersection<Traits> inter = intersection(segment, polygon.plane());
        if (inter.locus() == Locus::Empty)
        {
            return false;
        }
        else if (inter.locus() == Locus::Point)
        {
            return polygon.contains(inter.point()); // but no need for plane check ...
        }

        assert(inter.locus() == Locus::Segment);
    }

    if (intersects(segment.endpoint(0), polygon) || intersects(segment.endpoint(1), polygon))
    {
        return true;
    }

    const Polygon<Traits>::vertices_type& vertices = polygon.vertices();

    for (size_t i = 0; i < polygon.vertex_count(); ++i)
    {
        Point<Traits> point0 = vertices[i];
        Point<Traits> point1 = vertices[(i + 1) % polygon.vertex_count()];

        if (intersects(segment, Segment<Traits>(point0, point1)))
        {
            return true;
        }
    }

    return false;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Plane<Traits>& plane, const Point<Traits>& point)
{
    return intersects(point, plane);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Plane<Traits>& plane, const Line<Traits>& line)
{
    return intersects(line, plane);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Plane<Traits>& plane, const Ray<Traits>& ray)
{
    return intersects(ray, plane);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Plane<Traits>& plane, const Segment<Traits>& segment)
{
    return intersects(segment, plane);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Plane<Traits>& planeA, const Plane<Traits>& planeB)
{
    return !parallel(planeA, planeB) || planeA.contains(planeB.point());
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Plane<Traits>& plane, const Polygon<Traits>& polygon)
{
    bool bPos = false;
    bool bNeg = false;

    for (size_t i = 0; i < polygon.vertex_count(); ++i)
    {
        typename Traits::value_type d = dot(polygon.vertex(i), plane.normal());
        if (math::float_eq0(d))
        {
            return true;
        }
        else if (math::float_gt0(d))
        {
            if (bNeg)
            {
                return true;
            }

            bPos = true;
        }
        else
        {
            if (bPos)
            {
                return true;
            }

            bNeg = true;
        }
    }

    return false;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Polygon<Traits>& polygon, const Point<Traits>& point)
{
    return intersects(point, polygon);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Polygon<Traits>& polygon, const Line<Traits>& line)
{
    return intersects(line, polygon);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Polygon<Traits>& polygon, const Ray<Traits>& ray)
{
    return intersects(ray, polygon);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Polygon<Traits>& polygon, const Segment<Traits>& segment)
{
    return intersects(segment, polygon);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Polygon<Traits>& polygon, const Plane<Traits>& plane)
{
    return intersects(plane, polygon);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects(const Polygon<Traits>& polygonA, const Polygon<Traits>& polygonB)
{
    if constexpr (Traits::dimension >= 3)
    {
        if (!intersects(polygonA.plane(), polygonB) || !intersects(polygonB.plane(), polygonA))
        {
            return false;
        }
    }

    const Polygon<Traits>::vertices_type& vertices = polygonA.vertices();

    for (size_t i = 0; i < polygonA.vertex_count(); ++i)
    {
        Point<Traits> point0 = vertices[i];
        Point<Traits> point1 = vertices[(i + 1) % polygonA.vertex_count()];

        if (intersects(Segment<Traits>(point0, point1), polygonB))
        {
            return true;
        }
    }

    return false;

}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Point<Traits>& pointA, const Point<Traits>& pointB)
{
    if (vector_eq(pointA, pointB))
    {
        return Intersection<Traits>(pointA);
    }

    return Intersection<Traits>();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Point<Traits>& point, const Line<Traits>& line)
{
    if (line.contains(point))
    {
        return Intersection<Traits>(point);
    }

    return Intersection<Traits>();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Point<Traits>& point, const Ray<Traits>& ray)
{
    if (ray.contains(point))
    {
        return Intersection<Traits>(point);
    }

    return Intersection<Traits>();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Point<Traits>& point, const Segment<Traits>& segment)
{
    if (segment.contains(point))
    {
        return Intersection<Traits>(point);
    }

    return Intersection<Traits>();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Point<Traits>& point, const Plane<Traits>& plane)
{
    if (plane.contains(point))
    {
        return Intersection<Traits>(point);
    }

    return Intersection<Traits>();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Point<Traits>& point, const Polygon<Traits>& polygon)
{
    if (intersects(point, polygon))
    {
        return Intersection<Traits>(point);
    }

    return Intersection<Traits>();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Line<Traits>& line, const Point<Traits>& point)
{
    return intersection(point, line);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Line<Traits>& lineA, const Line<Traits>& lineB)
{
    auto [s, t] = closest_point_parameters(lineA.point(), lineA.direction(), lineB.point(), lineB.direction());
    if (math::float_eq0((lineA.point(s) - lineB.point(t)).norm_squared())) // pointA == pointB ...
    {
        if (parallel(lineA, lineB))
        {
            return Intersection<Traits>(lineA);
        }

        return Intersection<Traits>(lineA.point(s));
    }

    return Intersection<Traits>();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Line<Traits>& line, const Ray<Traits>& ray)
{
    auto [s, t] = closest_point_parameters(line, Line<Traits>(ray.endpoint(), ray.direction()));
    if (math::float_ge0(t) && math::float_eq0((line.point(s) - ray.point(t)).norm_squared())) // pointA == pointB ...
    {
        if (parallel(line, ray))
        {
            return Intersection<Traits>(ray);
        }

        return Intersection<Traits>(line.point(s));
    }

    return Intersection<Traits>();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Line<Traits>& line, const Segment<Traits>& segment)
{
    auto [s, t] = closest_point_parameters(line.point(), line.direction(), segment.endpoint(0), segment.vector());
    if (math::float_clamped01(t) && math::float_eq0((line.point(s) - segment.point(t)).norm_squared())) // pointA == pointB ...
    {
        if (parallel(line, segment))
        {
            return Intersection<Traits>(segment);
        }

        return Intersection<Traits>(line.point(s));
    }

    return Intersection<Traits>();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Line<Traits>& line, const Plane<Traits>& plane)
{
    typename Traits::value_type num = dot(plane.point() - line.point(), plane.normal());
    typename Traits::value_type denom = dot(line.direction(), plane.normal());

    if (math::float_eq0(denom)) // parallel
//if (abs(denom < 0.1f)) // ...
    {
        if (math::float_eq0(num))
        {
            return Intersection<Traits>(line);
        }

        return Intersection<Traits>();
    }

assert(plane.contains(line.point(num / denom))); // ...
    return Intersection<Traits>(line.point(num / denom));
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Line<Traits>& line, const Polygon<Traits>& polygon)
{
    if constexpr (Traits::dimension >= 3)
    {
        Intersection<Traits> inter = intersection(line, polygon.plane());
        if (inter.locus() == Locus::Empty)
        {
            return false;
        }
        else if (inter.locus() == Locus::Point)
        {
            return intersection(inter.point(), polygon());
        }

        assert(inter.locus() == Locus::Line);
    }

    const Polygon<Traits>::vertices_type& vertices = polygon.vertices();

    // Non-convex polygons may have multiple intersections of any of these types ...

    std::vector<Intersection<Traits>> intersections;
    for (size_t i = 0; i < polygon.vertex_count(); ++i)
    {
        Point<Traits> point0 = vertices[i];
        Point<Traits> point1 = vertices[(i + 1) % polygon.vertex_count()];

        Intersection<Traits> inter = intersection(line, Segment<Traits>(point0, point1));
        if (inter.locus() == Locus::Segment)
        {
            assert(intersections.empty());
            return inter;
        }
        else if (inter.locus() == Locus::Point && !line.contains(point1))
        {
            // Don't consider second segment endpoint so vertex intersections don't get counted twice
            intersections.push_back(inter);
        }
    }

    if (intersections.size() == 1)
    {
        assert(intersections[0].locus() == Locus::Point);
        return intersections[0];
    }
    else if (intersections.size() == 2)
    {
        assert(intersections[0].locus() == Locus::Point);
        assert(intersections[1].locus() == Locus::Point);
        return Segment(intersections[0].point(), intersections[1].point());
    }

    return Intersection<Traits>();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Ray<Traits>& ray, const Point<Traits>& point)
{
    return intersection(point, ray);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Ray<Traits>& ray, const Line<Traits>& line)
{
    return intersection(line, ray);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Ray<Traits>& rayA, const Ray<Traits>& rayB)
{
    auto [s, t] = closest_point_parameters(Line<Traits>(rayA.point(), rayA.direction()), Line<Traits>(rayB.point(), rayB.direction()));
    if (math::float_ge0(s) && math::float_ge0(t) && math::float_eq0((rayA.point(s) - rayB.point(t)).norm_squared())) // pointA == pointB ...
    {
        if (parallel(rayA, rayB))
        {
            bool bIntersectsA = rayA.contains(rayB.point());
            bool bIntersectsB = rayB.contains(rayA.point());

            if (bIntersectsA && bIntersectsB)
            {
                return Intersection<Traits>(Segment<Traits>(rayA.point(), rayB.point()));
            }
            else if (bIntersectsB)
            {
                return Intersection<Traits>(rayA);
            }

            return Intersection<Traits>(rayB);
        }

        return Intersection<Traits>(rayA.point(s));
    }

    return Intersection<Traits>();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Ray<Traits>& ray, const Segment<Traits>& segment)
{
    auto [s, t] = closest_point_parameters(ray.endpoint(), ray.direction(), segment.endpoint(0), segment.vector());
    if (math::float_ge0(s) && math::float_clamped01(t) && math::float_eq0((ray.point(s) - segment.point(t)).norm_squared())) // pointA == pointB ...
    {
        if (parallel(ray, segment))
        {
            if (segment.contains(ray.endpoint()))
            {
                if (ray.contains(segment.endpoint(0)))
                {
                    return Intersection<Traits>(Segment(segment.endpoint(0), ray.endpoint()));
                }

                return Intersection<Traits>(Segment(segment.endpoint(1), ray.endpoint()));
            }

            return Intersection<Traits>(segment);
        }

        return Intersection<Traits>(ray.point(s));
    }

    return Intersection<Traits>();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Ray<Traits>& ray, const Plane<Traits>& plane)
{
    if (intersects(ray, plane))
    {
        return intersection(Line<Traits>(ray.endpoint(), ray.direction), plane);
    }

    return Intersection<Traits>();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Ray<Traits>& ray, const Polygon<Traits>& polygon)
{
    if constexpr (Traits::dimension >= 3)
    {
        Intersection<Traits> inter = intersection(ray, polygon.plane());
        if (inter.locus() == Locus::Empty)
        {
            return false;
        }
        else if (inter.locus() == Locus::Point)
        {
            return intersection(inter.point(), polygon());
        }

        assert(inter.locus() == Locus::Ray);
    }

    const Polygon<Traits>::vertices_type& vertices = polygon.vertices();

    // Non-convex polygons may have multiple intersections of any of these types ...

    std::vector<Intersection<Traits>> intersections;
    for (size_t i = 0; i < polygon.vertex_count(); ++i)
    {
        Point<Traits> point0 = vertices[i];
        Point<Traits> point1 = vertices[(i + 1) % polygon.vertex_count()];

        Intersection<Traits> inter = intersection(ray, Segment<Traits>(point0, point1));
        if (inter.locus() == Locus::Segment)
        {
            assert(intersections.empty());
            return inter;
        }
        else if (inter.locus() == Locus::Point && !ray.contains(point1))
        {
            // Don't consider second segment endpoint so vertex intersections don't get counted twice
            intersections.push_back(inter);
        }
    }

    if (intersections.size() == 1)
    {
        assert(intersections[0].locus() == Locus::Point);
        if (intersections[0].point() == ray.endpoint())
        {
            return intersections[0];
        }

        return Intersection<Traits>(Segment(ray.endpoint(), intersections[0].point()));
    }
    else if (intersections.size() == 2)
    {
        assert(intersections[0].locus() == Locus::Point);
        assert(intersections[1].locus() == Locus::Point);
        return Segment(intersections[0].point(), intersections[1].point());
    }

    return Intersection<Traits>();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Segment<Traits>& segment, const Point<Traits>& point)
{
    return intersection(point, segment);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Segment<Traits>& segment, const Line<Traits>& line)
{
    return intersection(line, segment);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Segment<Traits>& segment, const Ray<Traits>& ray)
{
    return intersection(ray, segment);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Segment<Traits>& segmentA, const Segment<Traits>& segmentB)
{
    auto [s, t] = closest_point_parameters(segmentA.endpoint(0), segmentA.vector(), segmentB.endpoint(0), segmentB.vector());
    if (math::float_clamped01(s) && math::float_clamped01(t) && math::float_eq0((segmentA.point(s) - segmentB.point(t)).norm_squared())) // pointA == pointB ...
    {
        if (parallel(segmentA, segmentB))
        {
            bool a0b = segmentB.contains(segmentA.endpoint(0));
            bool a1b = segmentB.contains(segmentA.endpoint(1));
            bool ab0 = segmentA.contains(segmentB.endpoint(0));
            bool ab1 = segmentA.contains(segmentB.endpoint(1));

            if (a0b && a1b)
            {
                return Intersection<Traits>(segmentA);
            }
            else if (ab0 && ab1)
            {
                return Intersection<Traits>(segmentB);
            }
            else
            {
                Segment<Traits> segment(a0b ? segmentA.endpoint(0) : segmentA.endpoint(1), ab0 ? segmentB.endpoint(0) : segmentB.endpoint(1));
                if (vector_eq(segment.endpoint(0), segment.endpoint(1)))
                {
                    return Intersection<Traits>(segment.endpoint(0));
                }

                return Intersection<Traits>(segment);
            }
        }

        return Intersection<Traits>(segmentA.point(s));
    }

    return Intersection<Traits>();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Segment<Traits>& segment, const Plane<Traits>& plane)
{
    Intersection<Traits> inter = intersection(Line<Traits>(segment.endpoint(0), normalize(segment.direction())), plane);
    if (inter.locus() == Locus::Point && intersects(segment, plane))
    {
        return inter;
    }
    else if (inter.locus() == Locus::Line)
    {
        return Intersection<Traits>(segment);
    }

    return Intersection<Traits>();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Segment<Traits>& segment, const Polygon<Traits>& polygon)
{
    if constexpr (Traits::dimension >= 3)
    {
        Intersection<Traits> inter = intersection(segment, polygon.plane());
        if (inter.locus() == Locus::Empty)
        {
            return Intersection<Traits>();
        }
        else if (inter.locus() == Locus::Point)
        {
            return intersection(inter.point(), polygon);
        }

        assert(inter.locus() == Locus::Segment);
    }

    const Polygon<Traits>::vertices_type& vertices = polygon.vertices();

    // Non-convex polygons may have multiple intersections of any of these types ...

    std::vector<Intersection<Traits>> intersections;
    for (size_t i = 0; i < polygon.vertex_count(); ++i)
    {
        Point<Traits> point0 = vertices[i];
        Point<Traits> point1 = vertices[(i + 1) % polygon.vertex_count()];

        Intersection<Traits> inter = intersection(segment, Segment<Traits>(point0, point1));
        if (inter.locus() == Locus::Segment)
        {
            assert(intersections.empty());
            return inter;
        }
        else if (inter.locus() == Locus::Point && !segment.contains(point1))
        {
            // Don't consider second segment endpoint so vertex intersections don't get counted twice
            intersections.push_back(inter);
        }
    }

    if (intersections.size() == 1)
    {
        assert(intersections[0].locus() == Locus::Point);
        if (intersections[0].point() == segment.endpoint(0)) // endpoint(0)? ...
        {
            return intersections[0];
        }

        return Intersection<Traits>(Segment(segment.endpoint(0), intersections[0].point()));
    }
    else if (intersections.size() == 2)
    {
        assert(intersections[0].locus() == Locus::Point);
        assert(intersections[1].locus() == Locus::Point);
        return Intersection<Traits>(Segment(intersections[0].point(), intersections[1].point()));
    }
    // could also be multiple segments and/or points, handle that above ...

    return Intersection<Traits>();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Plane<Traits>& plane, const Point<Traits>& point)
{
    return intersection(point, plane);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Plane<Traits>& plane, const Line<Traits>& line)
{
    return intersection(line, plane);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Plane<Traits>& plane, const Ray<Traits>& ray)
{
    return intersection(ray, plane);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Plane<Traits>& plane, const Segment<Traits>& segment)
{
    return intersection(segment, plane);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Plane<Traits>& planeA, const Plane<Traits>& planeB)
{
    using T = Traits::value_type;

    if (parallel(planeA, planeB))
    {
        if (planeA.contains(planeB.point()))
        {
            return Intersection<Traits>(planeA);
        }

        return Intersection<Traits>();
    }

    math::Vector<Traits> direction = cross(planeA.normal(), planeB.normal()); // normalize? ...

    // Determine max abs coordinate of cross product
    T ax = abs(direction[0]);
    T ay = abs(direction[1]);
    T az = abs(direction[2]);
    int iMax = (az >= ax && az >= ay ? 2 : (ay >= ax ? 1 : 0));

    // zero the max coord, and solve for the other two
    // the constants in the 2 plane equations, could be pre-stored  with plane
    T d1 = -dot(planeA.normal(), planeA.point());
    T d2 = -dot(planeB.normal(), planeB.point());

    Point<Traits> point;

    switch (iMax)
    {
        case 0: // intersect with x = 0
            point[0] = Traits::val(0);
            point[1] = (d2 * planeA.normal()[2] - d1 * planeB.normal()[2]) / direction[0];
            point[2] = (d1 * planeB.normal()[1] - d2 * planeA.normal()[1]) / direction[0];
            break;

        case 1: // intersect with y = 0
            point[0] = (d1 * planeB.normal()[2] - d2 * planeA.normal()[2]) / direction[1];
            point[1] = Traits::val(0);
            point[2] = (d2 * planeA.normal()[0] - d1 * planeB.normal()[0]) / direction[1];
            break;

        case 2: // intersect with z = 0
            point[0] = (d2 * planeA.normal()[1] - d1 * planeB.normal()[1]) / direction[2];
            point[1] = (d1 * planeB.normal()[0] - d2 * planeA.normal()[0]) / direction[2];
            point[2] = Traits::val(0);
            break;
    }

    return Intersection<Traits>(Line<Traits>(point, direction));
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Plane<Traits>& plane, const Polygon<Traits>& polygon)
{
    if (parallel(plane, polygon))
    {
        if (intersects(polygon.vertex(0), plane))
        {
            return Intersection<Traits>(polygon);
        }

        return Intersection<Traits>();
    }

    Intersection<Traits> inter = intersection(plane, polygon.plane());
    assert(inter.locus() == Locus::Line);

    return intersection(inter.line(), polygon);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Polygon<Traits>& polygon, const Point<Traits>& point)
{
    return intersection(point, polygon);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Polygon<Traits>& polygon, const Line<Traits>& line)
{
    return intersection(line, polygon);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Polygon<Traits>& polygon, const Ray<Traits>& ray)
{
    return intersection(ray, polygon);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Polygon<Traits>& polygon, const Segment<Traits>& segment)
{
    return intersection(segment, polygon);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Polygon<Traits>& polygon, const Plane<Traits>& plane)
{
    return intersection(plane, polygon);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Polygon<Traits>& polygonA, const Polygon<Traits>& polygonB)
{
polygonA;
polygonB;
    // none
    // point(s)
    // segment(s)
    // polygon
    assert(false); // ...
    return Intersection<Traits>(); // ...
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Plane<Traits>& planeA, const Plane<Traits>& planeB, const Plane<Traits>& planeC)
{
planeA;
planeB;
planeC;
    // none
    // point
    /*
        typename Traits::value_type dA = planeA.distance({});
        typename Traits::value_type dB = planeB.distance({});
        typename Traits::value_type dC = planeC.distance({});
        return (-dA * cross(planeB.normal(), planeC.normal()) - dB * cross(planeC.normal(), planeA.normal()) - dC * cross(planeA.normal(), planeB.normal()))
            / dot(planeA.normal(), cross(planeB.normal(), planeC.normal()));
    */
    // line(s)
    // plane
    assert(false); // ...
    return Intersection<Traits>(); // ...
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::intersects_coplanar(const Point<Traits>& point, const Polygon<Traits>& polygon)
{
/*
//    using planar_point_type = math::Vector<typename Traits::reduced_traits>; // 2 ...
    using planar_point_type = math::Vector<math::VectorTraits<typename Traits::value_type, 2>>;
    std::vector<planar_point_type> vertices;
    planar_point_type initial_point;

    if constexpr (Traits::dimension == 3)
    {
        math::DimensionReducer<Traits> dr(polygon.normal());
        for (const auto& vertex : polygon.vertices())
        {
            vertices.emplace_back(dr.reduce(vertex));
        }

        initial_point = dr.reduce(point);
    }
    else if constexpr (Traits::dimension == 2)
    {
        vertices = polygon.vertices();
        initial_point = point;
    }
    else
    {
        static_assert(false, "Dimension out of range.");
    }

    vertices.push_back(vertices[0]);
*/
//    Ray ray(initial_point, normalize(vertices[1] - vertices[0]));
    Ray ray(point, normalize(polygon.vertex(1) - polygon.vertex(0)));

    size_t n = 0;
    for (size_t i = 0; i < polygon.vertex_count(); ++i)
    {
//        Segment segment(vertices[i], vertices[i + 1]);
        Segment segment(polygon.vertex(i), polygon.vertex((i + 1) % polygon.vertex_count()));
        auto inter = intersection(ray, segment);
        if (inter.locus() == Locus::Segment)
        {
            if (intersects(ray.endpoint(), segment))
            {
                return true;
            }

            ++n;
        }
        else if (inter.locus() == Locus::Point)
        {
            if (vector_eq(inter.point(), ray.endpoint())) // for this check exact equality probably ok ...
            {
//                assert(inter.point() == ray.endpoint());
                return true;
            }

            // Don't consider second segment endpoint so vertex intersections don't get counted twice
            if (!vector_eq(inter.point(), segment.endpoint(1), 10000)) // see if this can be reduced ...
            {
                ++n;
            }
        }
        else
        {
            assert(inter.locus() == Locus::Empty);
        }

        // Also handle the edge case where the ray is tangent to the polygon at a vertex ...
        // tangent points should not be counted ...
        // if point0, then don't count if prev and next are both to the right or left of the ray ...
    }

assert(polygon.contains(point) == math::odd(n));
    return math::odd(n);
}

/*
//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Intersection<Traits> quetzal::geometry::intersection(const Segment<Traits>& segment, const Polygon<Traits>& polygon)
{
    math::Vector<Traits> normal = normalize(cross(polygon.vertex(2) - polygon.vertex(1), polygon.vertex(0) - polygon.vertex(1)));

    math::Vector<Traits> v0 = segment.endpoint(0) - polygon.vertex(0);
    math::Vector<Traits> v1 = segment.endpoint(1) - polygon.vertex(0);
    typename Traits::value_type dot0 = dot(v0, normal);
    typename Traits::value_type dot1 = dot(v1, normal);

    // Check for both points on the same side of the polygon, no intersection
    if (math::float_gt0(dot0) && math::float_gt0(dot1) || math::float_lt0(dot0) && math::float_lt0(dot1))
    {
        return Intersection<Traits>();
    }

    // Segment is in plane of polygon
    if (math::float_eq0(dot0) && math::float_eq0(dot1))
    {
        // count crossings ...
        // if crossings == 0, no intersection ...
        // find crossing points ...
        // if same, then point intersection ...
        // if not convex polygon (crossings > 2), then could be multiple segments ...
        // if 2 and endpoints are interior, then 2 segments ...
        return Intersection<Traits>(segment);
    }

    // Segment crosses plane of polygon. Check if segment/plane intersection point is within polygon
    Intersection<Traits> i = intersection(Line<Traits>(segment.endpoint(0), segment.direction()), Plane<Traits>(polygon.vertex(1), normal));
    assert(i.locus() == Locus::Point);

    if (intersects(i.point(), polygon))
    {
        return i;
    }

    return Intersection<Traits>();
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Plane<Traits>::line_type quetzal::geometry::Plane<Traits>::intersection(const Plane& plane) const
{
    value_type h0 = distance(Plane<Traits>::point_type());
    value_type h1 = plane.distance(Plane<Traits>::point_type());

    value_type ndotn = dot(m_normal, plane.normal());
    value_type denom = Traits::val(1) - ndotn * ndotn;

    value_type c0 = (h0 - h1 * ndotn) / denom;
    value_type c1 = (h1 - h0 * ndotn) / denom;

    return Line(c0 * m_normal + c1 * plane.normal(), cross(m_normal, plane.m_normal));
}
*/

#endif // QUETZAL_GEOMETRY_INTERSECT_HPP
