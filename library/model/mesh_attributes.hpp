#if !defined(QUETZAL_MODEL_MESH_ATTRIBUTES_HPP)
#define QUETZAL_MODEL_MESH_ATTRIBUTES_HPP
//------------------------------------------------------------------------------
// model
// mesh_attributes.hpp
//------------------------------------------------------------------------------

#include "brep/Mesh.hpp"
#include "brep/mesh_geometry.hpp"
#include "brep/triangulation.hpp"
#include "common/id.hpp"
#include "math/Matrix.hpp"
#include "math/Vector.hpp"
#include "math/floating_point.hpp"
#include "math/transformation_matrix.hpp"
#include <functional>
#include <limits>
#include <vector>
#include <type_traits>

namespace quetzal
{

namespace model
{

    // Returns first valid vertex normal
    // instead use normalized weighted sum of vertex normals? ...
    // face normal should be non-zero and in approximate direction for reflex testing ...
    // Returns 0 vector for degenerate faces
    template<typename Traits>
    typename Traits::vector_type face_normal(const brep::Face<Traits>& face);

    // face normal should be non zero and in approximate direction for reflex testing ...
    // or there will be the possibility of producing opposite direction for reflex vertices in non-convex polygons ...
    // Returns 0 vector for colinear adjacent edges
    template<typename Traits>
    typename Traits::vector_type face_vertex_normal(const brep::Halfedge<Traits>& halfdedge);

    // Calculate smooth surface normal at this vertex
    // There must be no reflex vertices
    template<typename Traits>
    typename Traits::vector_type surface_vertex_normal(const brep::Vertex<Traits>& vertex);

    // transform works with both mesh and submesh

    // For use with translation and/or uniform scaling only, no need to transform normals
    template<typename M>
    void transform_positions(M& m, const math::Matrix<typename M::value_type>& matrix);

    // Applies matrix to positions and transpose inverse matrix to normals
    template<typename M>
    void transform(M& mesh, const math::Matrix<typename M::value_type>& matrix);

    template<typename M>
    void transform(M& mesh, const math::Matrix<typename M::value_type>& matrixPosition, const math::Matrix<typename M::value_type>& matrixNormal);

    template<typename M>
    void transform(M& m, std::function<void(typename M::vertex_attributes_type&)> fav, std::function<void(typename M::face_attributes_type&)> faf = [](typename M::face_attributes_type&) -> void {});

    template<typename M>
    void transform_border_section_positions(M& mesh, id_type idHalfedge, const math::Matrix<typename M::value_type>& matrix, bool bReset = true);

    // Calculate a common face normal and apply it to all vertices in the face
    // There must be no shared vertices
    template<typename M>
    void calculate_face_normals(M& mesh);

    // Calculate a common face normal and apply it to all vertices in the face
    // There must be no reflex vertices
    template<typename Traits>
    void calculate_face_normal(brep::Face<Traits>& face);

    // Calculate normals for the surface, each face, and each vertex in the mesh/submesh
    template<typename M>
    void calculate_surface_normals(M& m);

    // Calculate normals for the surface, each face, and each vertex in the surface
    template<typename Traits>
    void calculate_surface_normals(brep::Surface<Traits>& surface);

    // Calculate a common normal for the surface, each face, and each vertex in the surface
    template<typename Traits>
    void calculate_planar_surface_normals(brep::Surface<Traits>& surface);

    // There must be no shared vertices
    template<typename M>
    void calculate_spherical_face_normals(M& mesh);

    template<typename M>
    void calculate_spherical_face_normal(M& mesh, id_type idFace);

    template<typename M>
    void calculate_spherical_normals(M& mesh);

    // calculate normals directed away from the z-axis taking into account the slope relative to it (dr/dz)
    // may not be a need for disk structure or even the nAzimuth parameter, just needs to be applied before any non-cylindrical vertices are created ...
    template<typename M>
    void calculate_axial_vertex_normals(M& mesh, size_t nAzimuth);

    // Set normals for all vertices in a planar surface to the normal of the first face in the surface
    // additions: average face normal, calculate surface normal, set face normals to common value too, ...
    template<typename M>
    void set_planar_surface_vertex_normals(M& mesh, id_type idSurface);

    template<typename M>
    void apply_triangular_face_texcoords(M& mesh);

    // The following functions either take or produce matching halfedges that could be partners

    template<typename M>
    void unify_vertex_positions(M& mesh);

    template<typename V>
    void unify_vertex_position(V& vertex, const typename V::mesh_type::point_type& position);

    // Set all vertex positions the same for each pair of vertices in each face defined by one halfedge in the given pair of potential halfedge partners
    template<typename M>
    void unify_vertex_pairs(M& mesh, id_type idHalfedgeA, id_type idHalfedgeB);

    template<typename M>
    id_type adjust_matching_face(M& mesh, id_type idFaceA, id_type idFaceB);

} // namespace model

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::vector_type quetzal::model::face_normal(const brep::Face<Traits>& face)
{
    for (const auto& halfedge : face.halfedges())
    {
        auto normal = face_vertex_normal(halfedge);
        if (!vector_eq0(normal))
        {
            return normal;
        }
    }

    return face.attributes().normal();
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::vector_type quetzal::model::face_vertex_normal(const brep::Halfedge<Traits>& halfedge)
{
    assert(!halfedge.deleted());

    auto position = halfedge.attributes().position();
    auto positionNext = halfedge.next().attributes().position();
    auto positionPrev = halfedge.prev().attributes().position();

    auto cross_product = cross(positionNext - position, positionPrev - position);
    if (vector_eq0(cross_product))
    {
        return {};
    }

    typename Traits::value_type dot_product = dot(cross_product, halfedge.face().attributes().normal());
    if (math::float_lt0(dot_product))
    {
        cross_product = -cross_product;
    }

    return normalize(cross_product);

}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::vector_type quetzal::model::surface_vertex_normal(const brep::Vertex<Traits>& vertex)
{
    id_type idSurface = vertex.halfedge().face().surface_id();

    typename Traits::point_type position = vertex.attributes().position();

    typename Traits::vector_type normalTotalWeighted;

    for (auto& halfedge : vertex.halfedges())
    {
        assert(vector_eq(halfedge.attributes().position(), position));

        if (halfedge.face().surface_id() != idSurface)
        {
            continue;
        }

        typename Traits::vector_type vNext = halfedge.next().attributes().position() - position;
        typename Traits::vector_type vPrev = halfedge.prev().attributes().position() - position;
        typename Traits::vector_type normal = normalize(cross(vNext, vPrev));
        typename Traits::value_type weight = angle(vNext, vPrev); // function parameter for alternate weighting algorithm ...
        normalTotalWeighted += weight * normal;
    }

    return normalize(normalTotalWeighted);
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::transform_positions(M& m, const math::Matrix<typename M::value_type>& matrix)
{
    m.reset();

    if constexpr (std::is_same_v<M, brep::Mesh<typename M::traits_type>>) // requires ...
    {
        for (auto& vertex : m.vertices())
        {
            if (vertex.marked())
            {
                continue;
            }

            auto position = vertex.attributes().position() * matrix;

            for (auto& halfedge : vertex.halfedges())
            {
//                assert(vector_eq(halfedge.vertex().attributes().position(), vertex.attributes().position()));
                halfedge.vertex().attributes().set_position(position);
                halfedge.vertex().set_marked();
            }
        }
    }
    else
    {
        for (auto& face : m.faces())
        {
            for (auto& halfedgeFace : face.halfedges())
            {
                auto& vertex = halfedgeFace.vertex();
                if (vertex.marked())
                {
                    continue;
                }

                auto positionOrig = vertex.attributes().position();
                auto position = vertex.attributes().position() * matrix;

                for (auto& halfedge : vertex.halfedges())
                {
                    assert(vector_eq(halfedge.vertex().attributes().position(), positionOrig));
                    halfedge.vertex().attributes().set_position(position);
                    halfedge.vertex().set_marked();
                }
            }
        }
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::transform(M& mesh, const math::Matrix<typename M::value_type>& matrix)
{
    transform(mesh, matrix, transpose(inverse(matrix)));
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::transform(M& m, const math::Matrix<typename M::value_type>& matrixPosition, const math::Matrix<typename M::value_type>& matrixNormal)
{
    if constexpr (std::is_same_v<M, brep::Mesh<typename M::traits_type>>) // requires ...
    {
        for (auto& vertex : m.vertices())
        {
            vertex.attributes().transform(matrixPosition, matrixNormal);
        }

        for (auto& face : m.faces())
        {
            face.attributes().transform(matrixPosition, matrixNormal);
        }
    }
    else
    {
        m.reset();

        for (auto& face : m.faces())
        {
            for (auto& halfedgeFace : face.halfedges())
            {
                auto& vertex = halfedgeFace.vertex();
                if (vertex.marked())
                {
                    continue;
                }

                auto position = vertex.attributes().position() * matrixPosition;

                for (auto& halfedge : vertex.halfedges())
                {
                    halfedge.vertex().attributes().set_position(position);
                    halfedge.vertex().attributes().set_normal(normalize(halfedge.vertex().attributes().normal() * matrixNormal));
                    halfedge.vertex().set_marked();
                }
            }

            face.attributes().transform(matrixPosition, matrixNormal);
        }
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::transform(M& m, std::function<void(typename M::vertex_attributes_type&)> fav, std::function<void(typename M::face_attributes_type&)> faf)
{
    for (auto& face : m.faces())
    {
        for (auto& halfedge : face.halfedges())
        {
            fav(halfedge.vertex().attributes());
        }

        faf(face.attributes());
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::transform_border_section_positions(M& mesh, id_type idHalfedge, const math::Matrix<typename M::value_type>& matrix, bool bReset)
{
    if (bReset)
    {
        mesh.reset();
    }

    auto& face = mesh.halfedge(idHalfedge).face();
    if (face.marked())
    {
        return;
    }

    face.set_marked();

    for (auto& halfedge : face.halfedges())
    {
        if (!halfedge.border())
        {
            transform_border_section_positions(mesh, halfedge.partner_id(), matrix, false);
        }

        halfedge.attributes().position() *= matrix;
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::calculate_face_normals(M& mesh)
{
    for (auto& face : mesh.faces())
    {
        calculate_face_normal(face);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::model::calculate_face_normal(brep::Face<Traits>& face)
{
    if (face.vertex_count() == 3)
    {
        // for best accuracy, choose the vertex that forms an angle closest to 1/2 Pi ...

        const auto& positionPrev = face.halfedge().prev().attributes().position();
        const auto& position = face.halfedge().attributes().position();
        const auto& positionNext = face.halfedge().next().attributes().position();

        auto normal = normalize(cross(positionNext - position, positionPrev - position));
        face.attributes().set_normal(normal);
        for (auto& halfedge : face.halfedges())
        {
            halfedge.attributes().set_normal(normal);
        }

        return;
    }

    typename Traits::vector_type normalSum;

    // can anything be done about reflex vertices here? not without at least approximate normal ...
    // while all primitives now have usable normals, can't necessarily count on new work ...

    for (auto& halfedge : face.halfedges())
    {
        if (!colinear(halfedge))
        {
            const auto& positionPrev = halfedge.prev().attributes().position();
            const auto& position = halfedge.attributes().position();
            const auto& positionNext = halfedge.next().attributes().position();
            auto normal = normalize(cross(positionNext - position, positionPrev - position));
            normalSum += normal;
        }
    }

    // Set all normals to the same value for consistency
    auto normal = normalize(normalSum);
    face.attributes().set_normal(normal);
    for (auto& halfedge : face.halfedges())
    {
        halfedge.attributes().set_normal(normal);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::calculate_surface_normals(M& m)
{
    for (auto& surface : m.surfaces())
    {
        calculate_surface_normals(surface);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::model::calculate_surface_normals(brep::Surface<Traits>& surface)
{
    assert(!surface.empty());

    // improve normal consistency in this special case ...
    // handle other special cases, for example, planar surfaces should have a single normal that is the same for all vertices, faces, and the surface ...
    if (surface.face_count() == 1) // && surface.faces().front().vertex_count() == 3 ...
    {
        auto& face = surface.faces().front();
        calculate_face_normal(face);
        face.surface().attributes().set_normal(face.attributes().normal());
        return;
    }

    surface.reset();

    for (auto& face : surface.faces())
    {
        typename Traits::vector_type normalFaceTotal;

        for (auto& fh : face.halfedges())
        {
            if (fh.vertex().marked())
            {
                normalFaceTotal += fh.attributes().normal();
                continue;
            }

            typename Traits::vector_type normalVertex = surface_vertex_normal(fh.vertex()); // calculate the common surface normal at that vertex position
            normalFaceTotal += normalVertex;

            for (auto& vh : fh.vertex().halfedges())
            {
                if (vh.face().surface_id() != surface.id())
                {
                    continue;
                }

                auto& vertex = vh.vertex();
                assert(!vertex.marked());

                vertex.attributes().set_normal(normalVertex);
                vertex.set_marked();
            }
        }

        typename Traits::vector_type normalFace = normalize(normalFaceTotal); // face normal is simple average of surface vertex normals ...
        face.attributes().set_normal(normalFace);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::model::calculate_planar_surface_normals(brep::Surface<Traits>& surface)
{
    assert(!surface.empty());

    typename Traits::vector_type normal = face_normal(surface.faces().front());
    surface.attributes().set_normal(normal);

    for (auto& face : surface.faces())
    {
        face.attributes().set_normal(normal);

        for (auto& halfedge : face.halfedges())
        {
            halfedge.attributes().set_normal(normal);
        }
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::calculate_spherical_face_normals(M& mesh)
{
    for (auto& face : mesh.faces())
    {
        calculate_spherical_face_normal(mesh, face.id());
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::calculate_spherical_face_normal(M& mesh, id_type idFace)
{
    auto& face = mesh.face(idFace);
    auto normal = normalize(centroid(face));
    face.attributes().set_normal(normal);

    // Set all vertex normals to the face normal
    for (auto& halfedge : face.halfedges())
    {
        halfedge.attributes().set_normal(normal);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::calculate_spherical_normals(M& mesh)
{
    for (auto& vertex : mesh.vertices())
    {
        vertex.attributes().set_normal(normalize(vertex.attributes().position()));
    }

    for (auto& face : mesh.faces())
    {
        face.attributes().set_normal(normalize(centroid(face)));
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::calculate_axial_vertex_normals(M& mesh, size_t nAzimuth)
{
    using value_type = typename M::value_type;
    using point_type = typename M::point_type;
    using vector_type = typename M::vector_type;

    size_t nVertices = mesh.vertex_store_count();

    // Body vertices have layers both above and below
    size_t nBody = nVertices - 2 * (nAzimuth + 1);
    for (size_t i = 0; i < nBody; ++i)
    {
        size_t j = i + nAzimuth + 1;

        const point_type& position = mesh.vertex(j).attributes().position();
        const point_type& above = mesh.vertex(j + nAzimuth + 1).attributes().position();
        const point_type& below = mesh.vertex(j - nAzimuth - 1).attributes().position();

        value_type r = sqrt(position.x() * position.x() + position.y() * position.y());
        value_type rAbove = sqrt(above.x() * above.x() + above.y() * above.y());
        value_type rBelow = sqrt(below.x() * below.x() + below.y() * below.y());

        value_type drAbove = rAbove - r;
        value_type dzAbove = above.z() - position.z();
        value_type thetaAbove = atan2(-drAbove, dzAbove); // Normal angle based on edge parameters

        value_type drBelow = r - rBelow;
        value_type dzBelow = position.z() - below.z();
        value_type thetaBelow = atan2(-drBelow, dzBelow); // Normal angle based on edge parameters

        value_type theta = (thetaAbove + thetaBelow) / value_type(2);

        point_type projection(position.x(), position.y(), value_type(0));
        vector_type right(position.y(), -position.x(), value_type(0));
        if (r == value_type(0))
        {
            if (rBelow == value_type(0))
            {
//                right = {above.y(), -above.x(), value_type(0)};
                projection.set(above.x(), above.y(), value_type(0));
                right.set(above.y(), -above.x(), value_type(0));
            }
            else
            {
//                right = {below.y(), -below.x(), value_type(0)};
                projection.set(below.x(), below.y(), value_type(0));
                right.set(below.y(), -below.x(), value_type(0));
            }
        }

        mesh.vertex(j).attributes().set_normal(normalize(projection * rotation_axis(right, theta)));
    }

    // Bottom vertices have layer only above
    for (size_t i = 0; i < nAzimuth + 1; ++i)
    {
        const point_type& position = mesh.vertex(i).attributes().position();
        const point_type& above = mesh.vertex(i + nAzimuth + 1).attributes().position();

        value_type r = sqrt(position.x() * position.x() + position.y() * position.y());
        value_type rAbove = sqrt(above.x() * above.x() + above.y() * above.y());

        value_type dr = rAbove - r;
        value_type dz = above.z() - position.z();

        value_type theta = atan2(-dr, dz); // Normal angle based on edge parameters

        point_type projection(position.x(), position.y(), value_type(0));
        vector_type right(position.y(), -position.x(), value_type(0));
        if (r == value_type(0))
        {
//            right = {above.y(), -above.x(), value_type(0)};
            projection.set(above.x(), above.y(), value_type(0));
            right.set(above.y(), -above.x(), value_type(0));
        }

        mesh.vertex(i).attributes().set_normal(normalize(projection * rotation_axis(right, theta)));
    }

    // Top vertices have layer only below
    for (size_t i = nVertices - nAzimuth - 1; i < nVertices; ++i)
    {
        const point_type& position = mesh.vertex(i).attributes().position();
        const point_type& below = mesh.vertex(i - nAzimuth - 1).attributes().position();

        value_type r = sqrt(position.x() * position.x() + position.y() * position.y());
        value_type rBelow = sqrt(below.x() * below.x() + below.y() * below.y());

        value_type dr = r - rBelow;
        value_type dz = position.z() - below.z();

        value_type theta = atan2(-dr, dz); // Normal angle based on edge parameters

        point_type projection(position.x(), position.y(), value_type(0));
        vector_type right(position.y(), -position.x(), value_type(0));
        if (r == value_type(0))
        {
//            right = {below.y(), -below.x(), value_type(0)};
            projection.set(below.x(), below.y(), value_type(0));
            right.set(below.y(), -below.x(), value_type(0));
        }

        mesh.vertex(i).attributes().set_normal(normalize(projection * rotation_axis(right, theta)));
    }

    for (auto& face : mesh.faces())
    {
        vector_type normalSum;
        size_t n = 0;
        for (const auto& halfedge : face.halfedges())
        {
            normalSum += halfedge.vertex().attributes().normal();
            ++n;
        }

        face.attributes().set_normal(normalize(normalSum));
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::set_planar_surface_vertex_normals(M& mesh, id_type idSurface)
{
    typename M::surface_type& surface = mesh.surface(idSurface);
    assert(!surface.empty());

    typename M::vector_type normal = surface.faces().front().attributes().normal();

    for (auto& face : surface.face())
    {
        for (auto& halfedge : face.halfedges())
        {
            halfedge.attributes().set_normal(normal);
        }
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::apply_triangular_face_texcoords(M& mesh)
{
    for (auto& face : mesh.faces())
    {
        assert(face.vertex_count() == 3);

        face.halfedge().attributes().set_texcoord({M::traits_type::val(0), M::traits_type::val(1)});
        face.halfedge().next().attributes().set_texcoord({M::traits_type::val(1), M::traits_type::val(1)});
        face.halfedge().next().next().attributes().set_texcoord({M::traits_type::val(0.5), M::traits_type::val(0)});
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::unify_vertex_positions(M& mesh)
{
    mesh.reset();

    for (auto& vertex : mesh.vertices())
    {
        if (vertex.marked())
        {
            continue;
        }

        unify_vertex_position(vertex, vertex.attributes().position());
        vertex.set_marked();
    }

    return;
}

//------------------------------------------------------------------------------
template<typename V>
void quetzal::model::unify_vertex_position(V& vertex, const typename V::mesh_type::point_type& position)
{
    for (auto& halfedge : vertex.halfedges())
    {
        assert(vector_eq(halfedge.attributes().position(), position, 1000)); // 1000 ...
        halfedge.attributes().set_position(position);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::unify_vertex_pairs(M& mesh, id_type idHalfedgeA, id_type idHalfedgeB)
{
    typename M::face_type& faceA = mesh.halfedge(idHalfedgeA).face();
    typename M::face_type& faceB = mesh.halfedge(idHalfedgeB).face();
    assert(faceA.vertex_count() == faceB.vertex_count());

    size_t n = faceA.vertex_count();
    for (size_t i = 0; i < n; ++i)
    {
        typename M::point_type position = mesh.halfedge(idHalfedgeA).attributes().position();

        unify_vertex_position(mesh.halfedge(idHalfedgeA).vertex(), position);
        unify_vertex_position(mesh.halfedge(idHalfedgeB).next().vertex(), position);

        idHalfedgeA = mesh.halfedge(idHalfedgeA).next_id();
        idHalfedgeB = mesh.halfedge(idHalfedgeB).prev_id();
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
quetzal::id_type quetzal::model::adjust_matching_face(M& mesh, id_type idFaceA, id_type idFaceB)
{
    typename M::face_type& faceA = mesh.face(idFaceA);
    typename M::face_type& faceB = mesh.face(idFaceB);

    const auto idHalfedges = brep::find_partner_halfedges(faceA, faceB);
    assert(idHalfedges[0] != nullid && idHalfedges[1] != nullid);
    assert(idHalfedges[0] == faceA.halfedge_id());

    id_type idHalfedgeB = idHalfedges[1];
    id_type idHalfedge = idHalfedgeB;

    // Adjust both face A and face B positions to exactly match face A halfedge vertex position
    for (auto& halfedgeA : faceA.halfedges())
    {
        const auto position = halfedgeA.attributes().position();
        auto& halfedgeB = mesh.halfedge(idHalfedge);

        unify_vertex_position(halfedgeA.vertex(), position);
        unify_vertex_position(halfedgeB.next().vertex(), position);

        idHalfedge = halfedgeB.prev_id();
    }

    return idHalfedgeB;
}

#endif // QUETZAL_MODEL_MESH_ATTRIBUTES_HPP
