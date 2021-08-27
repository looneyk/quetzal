#if !defined(QUETZAL_BREP_MESH_TEXCOORD_HPP)
#define QUETZAL_BREP_MESH_TEXCOORD_HPP
//------------------------------------------------------------------------------
// brep
// mesh_texcoord.hpp
//------------------------------------------------------------------------------

#include "Face.hpp"
#include "mesh_geometry.hpp"
#include "quetzal/math/DimensionReducer.hpp"
#include "quetzal/math/transformation_matrix.hpp"
#include "quetzal/geometry/AxisAlignedBoundingBox.hpp"
#include "quetzal/geometry/Polygon.hpp"

namespace quetzal::brep
{

    template<typename M>
    void calculate_surface_texcoords(M& mesh, id_type idSurface);

    template<typename M>
    void calculate_planar_surface_texcoords(M& mesh, id_type idSurface);

    template<typename M>
    void calculate_face_texcoords(M& mesh, id_type idFace);

    // Add texcoords to face by interpolating from texcoords of faceReference
    template<typename Traits>
    void interpolate_face_texcoords(const Face<Traits>& faceReference, Face<Traits>& face);

    // Add texcoords to hole by interpolating from texcoords of faceReference
    template<typename Traits>
    void interpolate_hole_texcoords(const Face<Traits>& faceReference, Hole<Traits>& hole);

    // Calculate texcoord at position by interpolating from texcoords of face
    template<typename Traits>
    typename Face<Traits>::mesh_type::vertex_attributes_type::texcoord_type interpolate_face_texcoord(const Face<Traits>& face, const typename Traits::point_type& position);

} // namespace quetzal::brep

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::calculate_surface_texcoords(M& mesh, id_type idSurface)
{
    // same as below, but need to flatten surface instead of using dr

    calculate_planar_surface_texcoords(mesh, idSurface); // calculate_surface_texcoords not implemented ...
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::calculate_planar_surface_texcoords(M& mesh, id_type idSurface)
{
    using value_type = M::value_type;
    using texcoord_type = M::vertex_attributes_type::texcoord_type;

    typename M::surface_type& surface = mesh.surface(idSurface);
	math::DimensionReducer<typename M::vector_traits> dr(surface.attributes().normal());

    std::vector<texcoord_type> points;
    for (const auto& face : surface.faces())
    {
        for (const auto& halfedge : face.halfedges())
        {
            points.emplace_back(dr.reduce(halfedge.attributes().position()));
        }

        for (auto& hole : face.holes())
        {
            for (auto& halfedge : hole.halfedges())
            {
                points.emplace_back(dr.reduce(halfedge.attributes().position()));
            }
        }
    }

    geometry::AxisAlignedBoundingBox<texcoord_type::traits_type> aabb(points);
    auto extent = aabb.extent();

    auto to_texcoord = [&](const texcoord_type& position) -> texcoord_type { return {(position.x() - aabb.lower().x()) / extent.x(), value_type(1) - (position.y() - aabb.lower().y()) / extent.y()}; };

    size_t i = 0;
    for (auto& face : surface.faces())
    {
        for (auto& halfedge : face.halfedges())
        {
            halfedge.attributes().set_texcoord(to_texcoord(points[i]));
            ++i;
        }

        for (auto& hole : face.holes())
        {
            for (auto& halfedge : hole.halfedges())
            {
                halfedge.attributes().set_texcoord(to_texcoord(points[i]));
                ++i;
            }
        }
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::calculate_face_texcoords(M& mesh, id_type idFace)
{
    using value_type = M::value_type;
    using texcoord_type = M::vertex_attributes_type::texcoord_type;

    typename M::face_type& face = mesh.face(idFace);

	math::DimensionReducer<typename M::vector_traits> dr(face.attributes().normal());

    std::vector<texcoord_type> points;
    for (const auto& halfedge : face.halfedges())
    {
        texcoord_type position = dr.reduce(halfedge.attributes().position());
        points.emplace_back(position);
    }

    geometry::AxisAlignedBoundingBox<texcoord_type::traits_type> aabb(points);
    auto extent = aabb.extent();

    auto to_texcoord = [&](const texcoord_type& position) -> texcoord_type { return {(position.x() - aabb.lower().x()) / extent.x(), value_type(1) - (position.y() - aabb.lower().y()) / extent.y()}; };

    size_t i = 0;
    for (auto& halfedge : face.halfedges())
    {
        halfedge.attributes().set_texcoord(to_texcoord(points[i]));
        ++i;
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::interpolate_face_texcoords(const Face<Traits>& faceReference, Face<Traits>& face)
{
    math::DimensionReducer<typename Traits::vector_traits> dr(faceReference.attributes().normal());
    auto polygon = to_polygon(faceReference, dr);

    // consider changing the vertex used for 2 ...
    // if texcoords have u or v of 0 or 1, then those vertices should be chosen ...
    // need to make sure these form an orthogonal basis ...
    auto point0 = polygon.vertex(0);
    auto point1 = polygon.vertex(1);
    auto point2 = polygon.vertex(2);
    auto texcoord0 = faceReference.halfedge().attributes().texcoord();
    auto texcoord1 = faceReference.halfedge().next().attributes().texcoord();
    auto texcoord2 = faceReference.halfedge().next().next().attributes().texcoord();

    auto matrix = math::linear_mapping(point1 - point0, point2 - point0, texcoord1 - texcoord0, texcoord2 - texcoord0);

    for (auto& halfedge : face.halfedges())
    {
        auto point = dr.reduce(halfedge.attributes().position());
        halfedge.attributes().set_texcoord((point - point0) * matrix + texcoord0);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::interpolate_hole_texcoords(const Face<Traits>& faceReference, Hole<Traits>& hole)
{
    // this produces texcoords out of range 0..1 ...

    math::DimensionReducer<typename Traits::vector_traits> dr(faceReference.attributes().normal());
    auto polygon = to_polygon(faceReference, dr);

    // consider changing the vertex used for 2 ...
    // if texcoords have u or v of 0 or 1, then those vertices should be chosen ...
    // need to make sure these form an orthogonal basis ...
    auto point0 = polygon.vertex(0);
    auto point1 = polygon.vertex(1);
    auto point2 = polygon.vertex(2);
    auto texcoord0 = faceReference.halfedge().attributes().texcoord();
    auto texcoord1 = faceReference.halfedge().next().attributes().texcoord();
    auto texcoord2 = faceReference.halfedge().next().next().attributes().texcoord();

    auto matrix = math::linear_mapping(point1 - point0, point2 - point0, texcoord1 - texcoord0, texcoord2 - texcoord0);

    for (auto& halfedge : hole.halfedges())
    {
        auto point = dr.reduce(halfedge.attributes().position());
        auto zzz = (point - point0) * matrix + texcoord0;
        halfedge.attributes().set_texcoord((point - point0) * matrix + texcoord0);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Face<Traits>::mesh_type::vertex_attributes_type::texcoord_type quetzal::brep::interpolate_face_texcoord(const Face<Traits>& face, const typename Traits::point_type& position)
{
    math::DimensionReducer<typename Traits::vector_traits> dr(face.attributes().normal());
    auto polygon = to_polygon(face, dr);

    // consider changing the vertex used for 2 ...
    // if texcoords have u or v of 0 or 1, then those vertices should be chosen ...
    auto point0 = polygon.vertex(0);
    auto point1 = polygon.vertex(1);
    auto point2 = polygon.vertex(2);
    auto texcoord0 = face.halfedge().attributes().texcoord();
    auto texcoord1 = face.halfedge().next().attributes().texcoord();
    auto texcoord2 = face.halfedge().next().next().attributes().texcoord();

    auto matrix = math::linear_mapping(point1 - point0, point2 - point0, texcoord1 - texcoord0, texcoord2 - texcoord0);

    auto point = dr.reduce(position);
    return (point - point0) * matrix + texcoord0;
}

#endif // QUETZAL_BREP_MESH_TEXCOORD_HPP
