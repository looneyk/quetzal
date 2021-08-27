#if !defined(QUETZAL_BREP_MESH_GEOMETRY_HPP)
#define QUETZAL_BREP_MESH_GEOMETRY_HPP
//------------------------------------------------------------------------------
// brep
// mesh_geometry.hpp
//------------------------------------------------------------------------------

#include "Face.hpp"
#include "Halfedge.hpp"
#include "Mesh.hpp"
#include "Submesh.hpp"
#include "Surface.hpp"
#include "quetzal/geometry/Line.hpp"
#include "quetzal/geometry/Plane.hpp"
#include "quetzal/geometry/Point.hpp"
#include "quetzal/geometry/Polygon.hpp"
#include "quetzal/geometry/Segment.hpp"
#include "quetzal/geometry/intersect.hpp"
#include "quetzal/geometry/triangle_util.hpp"
#include "quetzal/math/DimensionReducer.hpp"

namespace quetzal::brep
{

    // Returns -1, 0, 1: reflex, straight, convex
    template<typename Traits> requires (Traits::dimension >= 2 && Traits::dimension <= 3)
    int orientation(const Halfedge<Traits>& halfedge);

    template<typename Traits>
    bool convex(const Halfedge<Traits>& halfedge);

    template<typename Traits>
    bool colinear(const Halfedge<Traits>& halfedge);

    template<typename Traits>
    bool reflex(const Halfedge<Traits>& halfedge);

    template<typename Traits>
    bool ear(const Halfedge<Traits>& halfedge, const Mesh<Traits>& mesh);

    // Unit vector
    template<typename Traits>
    typename Traits::vector_type direction(const Halfedge<Traits>& halfedge);

    template<typename Traits>
    typename Traits::value_type length(const Halfedge<Traits>& halfedge);

    template<typename Traits>
    math::Vector<typename Traits::vector_traits> to_vector(const Halfedge<Traits>& halfedge);

    template<typename Traits>
    geometry::Line<typename Traits::vector_traits> to_line(const Halfedge<Traits>& halfedge);

    template<typename Traits>
    geometry::Segment<typename Traits::vector_traits> to_segment(const Halfedge<Traits>& halfedge);

    template<typename Traits>
    bool planar(const Face<Traits>& face);

    template<typename Traits>
    bool convex(const Face<Traits>& face);

    template<typename Traits>
    bool degenerate(const Face<Traits>& face);

    template<typename Traits>
    typename Traits::point_type centroid(const Face<Traits>& face);

    template<typename Traits>
    geometry::Plane<typename Traits::vector_traits> to_plane(const Face<Traits>& face);

    template<typename Traits>
    geometry::Polygon<typename Traits::vector_traits> to_polygon(const Face<Traits>& face);

    template<typename Traits>
    geometry::Polygon<typename Traits::vector_traits::reduced_traits> to_polygon(const Face<Traits>& face, const math::DimensionReducer<typename Traits::vector_traits>& dr);

    // area functions calculate the total signed area based on winding order (CCW positive, CW negative)

    template<typename Traits>
    typename Traits::value_type face_area(const Face<Traits>& face);

    // Works with Surface, Submesh, and Mesh
    template<typename S>
    typename S::traits_type::value_type surface_area(const S& s);

    template<typename Traits>
    int face_compare(const Face<Traits>& face, const typename Traits::point_type& point);

    template<typename Traits>
    bool face_contains(const Face<Traits>& face, const typename Traits::point_type& point);

    // Works with Surface, Submesh, and Mesh
    template<typename S>
    bool surface_contains(const S& s, const typename S::traits_type::point_type& point);

    template<typename Traits>
    bool solid_contains(const Submesh<Traits>& submesh, const typename Traits::point_type& point);

    template<typename Traits>
    bool solid_contains(const Mesh<Traits>& mesh, const typename Traits::point_type& point);

} // namespace quetzal::brep

//------------------------------------------------------------------------------
template<typename Traits> requires (Traits::dimension >= 2 && Traits::dimension <= 3)
int quetzal::brep::orientation(const Halfedge<Traits>& halfedge)
{
    if (colinear(halfedge))
    {
        return 0;
    }

    const auto& a = halfedge.prev().attributes().position();
    const auto& b = halfedge.attributes().position();
    const auto& c = halfedge.next().attributes().position();

    if constexpr (Traits::dimension == 3)
    {
        const auto& normal = halfedge.attributes().normal();
        return dot(c - b, cross(normal, b - a)) < Traits::val(0) ? -1 : 1;
    }
    else if constexpr (Traits::dimension == 2)
    {
        return dot(c - b, perp(b - a)) < Traits::val(0) ? -1 : 1;
    }
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::brep::convex(const Halfedge<Traits>& halfedge)
{
    return orientation(halfedge) > 0;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::brep::colinear(const Halfedge<Traits>& halfedge)
{
    const auto& a = halfedge.prev().attributes().position();
    const auto& b = halfedge.attributes().position();
    const auto& c = halfedge.next().attributes().position();
    return geometry::colinear(a, b, c);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::brep::reflex(const Halfedge<Traits>& halfedge)
{
    return orientation(halfedge) < 0;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::brep::ear(const Halfedge<Traits>& halfedge, const Mesh<Traits>& mesh)
{
    if (!convex(halfedge))
    {
        return false;
    }

    id_type idHalfedgeNext = halfedge.next_id();
    id_type idHalfedgePrev = halfedge.prev_id();

    const auto& position0 = halfedge.prev().attributes().position();
    const auto& position1 = halfedge.attributes().position();
    const auto& position2 = halfedge.next().attributes().position();

    for (id_type id = halfedge.next().next_id(); id != idHalfedgePrev; id = mesh.halfedge(id).next_id())
    {
        // could probably do without mesh using halfedge pointer ...

        if (triangle_contains(position0, position1, position2, mesh.halfedge(id).attributes().position()))
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::vector_type quetzal::brep::direction(const Halfedge<Traits>& halfedge)
{
    return normalize(halfedge.next().attributes().position() - halfedge.attributes().position());
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::brep::length(const Halfedge<Traits>& halfedge)
{
    return (halfedge.next().attributes().position() - halfedge.attributes().position()).norm();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::math::Vector<typename Traits::vector_traits> quetzal::brep::to_vector(const Halfedge<Traits>& halfedge)
{
    return halfedge.next().attributes().position() - halfedge.attributes().position();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Line<typename Traits::vector_traits> quetzal::brep::to_line(const Halfedge<Traits>& halfedge)
{
    return geometry::Line<typename Traits::vector_traits>(halfedge.attributes().position(), direction(halfedge));
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Segment<typename Traits::vector_traits> quetzal::brep::to_segment(const Halfedge<Traits>& halfedge)
{
    return geometry::Segment<typename Traits::vector_traits>(halfedge.attributes().position(), halfedge.next().attributes().position());
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::brep::planar(const Face<Traits>& face)
{
    geometry::Plane<typename Traits::vector_traits> plane = to_plane(face);

    for (const auto& halfedge : face.halfedges())
    {
        if (!intersects(halfedge.attributes().position(), plane))
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::brep::convex(const Face<Traits>& face)
{
    for (const auto& halfedge : face.halfedges())
    {
        if (!convex(halfedge))
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::brep::degenerate(const Face<Traits>& face)
{
    for (const auto& halfedge : face.halfedges())
    {
        if (!colinear(halfedge))
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::point_type quetzal::brep::centroid(const Face<Traits>& face)
{
    typename Traits::point_type position;
    size_t n = 0;

    for (const auto& halfedge : face.halfedges())
    {
        position += halfedge.attributes().position();
        ++n;
    }

    return position / Traits::val(n);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Plane<typename Traits::vector_traits> quetzal::brep::to_plane(const Face<Traits>& face)
{
    return geometry::Plane<typename Traits::vector_traits>(face.halfedge().attributes().position(), face.attributes().normal());
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polygon<typename Traits::vector_traits> quetzal::brep::to_polygon(const Face<Traits>& face)
{
    geometry::Polygon<typename Traits::vector_traits> polygon(face.halfedge_count());

    size_t i = 0;
    for (const auto& halfedge : face.halfedges())
    {
        polygon.set_vertex(i, halfedge.attributes().position());
        ++i;
    }

    return polygon;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polygon<typename Traits::vector_traits::reduced_traits> quetzal::brep::to_polygon(const Face<Traits>& face, const math::DimensionReducer<typename Traits::vector_traits>& dr)
{
    geometry::Polygon<typename Traits::vector_traits::reduced_traits> polygon(face.halfedge_count());

    size_t i = 0;
    for (const auto& halfedge : face.halfedges())
    {
        polygon.set_vertex(i, dr.reduce(halfedge.attributes().position()));
        ++i;
    }

    return polygon;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::brep::face_area(const Face<Traits>& face)
{
    assert(!face.deleted());
    assert(face.halfedge_count() >= 3);

    const typename Traits::vector_type normal = face.attributes().normal();

    if (face.halfedge_count() == 3)
    {
        const auto& halfedge = face.halfedge();
        const auto& position0 = halfedge.prev().attributes().position();
        const auto& position1 = halfedge.attributes().position();
        const auto& position2 = halfedge.next().attributes().position();

        return triangle_area(position0, position1, position2, normal);
    }

    return to_polygon(face).signed_area(normal);
}

//------------------------------------------------------------------------------
template<typename S>
typename S::traits_type::value_type quetzal::brep::surface_area(const S& s)
{
    typename S::traits_type::value_type area = S::traits_type::val(0);

    for (const auto& face : s.faces())
    {
        area += face_area(face);
    }

    return area;
}

//------------------------------------------------------------------------------
template<typename Traits>
int quetzal::brep::face_compare(const Face<Traits>& face, const typename Traits::point_type& point)
{
    assert(!face.deleted());
    assert(face.halfedge_count() >= 3);

    if (face.halfedge_count() == 3)
    {
        const auto& halfedge = face.halfedge();
        return geometry::triangle_compare(halfedge.prev().attributes().position(), halfedge.attributes().position(), halfedge.next().attributes().position(), point);
    }

    if (!to_plane(face).contains(point))
    {
        return 1;
    }

    math::DimensionReducer<typename Traits::vector_traits> dr(face.attributes().normal());
    return to_polygon(face, dr).compare(dr.reduce(point));
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::brep::face_contains(const Face<Traits>& face, const typename Traits::point_type& point)
{
    assert(!face.deleted());
    assert(face.halfedge_count() >= 3);

    if (face.halfedge_count() == 3)
    {
        const auto& halfedge = face.halfedge();
        return geometry::triangle_contains(halfedge.prev().attributes().position(), halfedge.attributes().position(), halfedge.next().attributes().position(), point);
    }

    if (!to_plane(face).contains(point))
    {
        return false;
    }

    math::DimensionReducer<typename Traits::vector_traits> dr(face.attributes().normal());
    return to_polygon(face, dr).contains(dr.reduce(point));
}

//------------------------------------------------------------------------------
template<typename S>
bool quetzal::brep::surface_contains(const S& s, const typename S::traits_type::point_type& point)
{
    for (const auto& face : s.faces())
    {
        if (face_contains(face, point))
        {
            return true;
        }
    }

    return false;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::brep::solid_contains(const Submesh<Traits>& submesh, const typename Traits::point_type& point)
{
    submesh.reset();

    geometry::Ray ray(point, {Traits::val(1), Traits::val(0), Traits::val(0)});
    size_t n = 0;

    for (const auto& face : submesh.faces())
    {
        if (intersects(ray, to_polygon(face)))
        {
            for (const auto& halfedge : face.halfedges())
            {
                // Skip next position so that each vertex is only checked once
                if (vector_eq(halfedge.next().attributes().position(), point))
                {
                    continue;
                }

                // Check for vertex and edge intersection so that adjoining vertices or edge can be marked so that intersection will only count once

                if (vector_eq(halfedge.attributes().position(), point))
                {
                    for (const auto& h : halfedge.vertex().halfedges())
                    {
                        h.face().set_marked();
                    }

                    break;
                }

                if (intersects(ray, to_segment(halfedge)))
                {
                    halfedge.partner().face().set_marked();
                    break;
                }
            }

            face.set_marked();
            ++n;
        }
    }

    return math::odd(n);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::brep::solid_contains(const Mesh<Traits>& mesh, const typename Traits::point_type& point)
{
    mesh.reset();

    geometry::Ray ray(point, {Traits::val(1), Traits::val(0), Traits::val(0)});
    size_t n = 0;

    for (const auto& face : mesh.faces())
    {
        if (intersects(ray, to_polygon(face)))
        {
            for (const auto& halfedge : face.halfedges())
            {
                // Skip next position so that each vertex is only checked once
                if (vector_eq(halfedge.next().attributes().position(), point))
                {
                    continue;
                }

                // Check for vertex and edge intersection so that adjoining vertices or edge can be marked so that intersection will only count once

                if (vector_eq(halfedge.attributes().position(), point))
                {
                    for (const auto& h : halfedge.vertex().halfedges())
                    {
                        h.face().set_marked();
                    }

                    break;
                }

                if (intersects(ray, to_segment(halfedge)))
                {
                    halfedge.partner().face().set_marked();
                    break;
                }
            }

            face.set_marked();
            ++n;
        }
    }

    return math::odd(n);
}

/* reactivate if needed ...
//------------------------------------------------------------------------------
template<typename S>
quetzal::id_type quetzal::brep::face_containing_point(const S& s, const typename S::traits_type::point_type& point)
{
    for (const auto& face : s.faces())
    {
        if (face_contains(face, point))
        {
            return face.id();
        }
    }

    return nullid;
}
*/

#endif // QUETZAL_BREP_MESH_GEOMETRY_HPP
