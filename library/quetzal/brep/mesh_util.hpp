#if !defined(QUETZAL_BREP_MESH_UTIL_HPP)
#define QUETZAL_BREP_MESH_UTIL_HPP
//------------------------------------------------------------------------------
// brep
// mesh_util.hpp
//------------------------------------------------------------------------------

#include "Face.hpp"
#include "mesh_geometry.hpp"
#include "quetzal/common/string_util.hpp"
#include "quetzal/math/DimensionReducer.hpp"
#include "quetzal/math/math_util.hpp"
#include "quetzal/geometry/Attributes.hpp"
#include "quetzal/geometry/Points.hpp"
#include "quetzal/geometry/Polygon.hpp"
#include "quetzal/geometry/intersect.hpp"
#include <array>

//#include "validation.hpp" // ...

namespace quetzal::brep
{

    template<typename M>
    quetzal::id_type next_surface_halfedge_id(const M& mesh, id_type idHalfedge);

    template<typename M>
    quetzal::id_type prev_surface_halfedge_id(const M& mesh, id_type idHalfedge);

    template<typename M>
    void add_face_surfaces(M& mesh, const std::string& prefix = "");

    template<typename M>
    void remove_degenerate_face(M& mesh, id_type idFace);

    template<typename M>
    void remove_degenerate_faces(M& mesh);

    // Create single face from all faces that contain this vertex, return the id of the resulting face
    template<typename M>
    id_type remove_vertex(M& mesh, id_type idVertex);

    // Create single face from the two faces adjacent to this edge, return the id of the resulting face
    template<typename M>
    id_type remove_edge(M& mesh, id_type idHalfedge);

    // Delete all components connected to this at any level
    // Misnamed, not just submesh, but anything connected ...
    // bReset parameter is used to trigger reset on first, non-recursive call; not a user parameter
    template<typename M>
    void remove_submesh(M& mesh, id_type idHalfedge, bool bReset = true);

    template<typename M>
    void update_halfedge(typename M::halfedge_type& halfedge, M& mesh, id_type idHalfedgeOffset, id_type idVertexOffset, id_type idFaceOffset);

    template<typename M>
    void update_vertex(typename M::vertex_type& vertex, M& mesh, id_type idHalfedgeOffset, id_type idVertexOffset);

    template<typename M>
    void update_face(typename M::face_type& face, M& mesh, id_type idHalfedgeOffset, id_type idFaceOffset, id_type idSurface, id_type idSubmesh);

//    template<typename M>
//    void update_surface(typename M::surface_type& surface, M& mesh, id_type idFaceOffset, id_type idSubmesh);

    // Original halfedges remain associated with their original vertices
    template<typename M>
    void split_edge(M& mesh, id_type idHalfedge, const typename M::vertex_attributes_type& attributes, const typename M::vertex_attributes_type& attributesPartner);

    // Split at position
    // Original halfedges remain associated with their original vertices
    // Intermediate vertex values are interpolated from endpoints
    template<typename M>
    void split_edge(M& mesh, id_type idHalfedge, const typename M::point_type& position);

    // Original halfedges remain associated with their original vertices
    // Intermediate vertex values are interpolated from endpoints
    // t ranges from 0 to 1 from vertex to end of edge
    template<typename M>
    void split_edge(M& mesh, id_type idHalfedge, const typename M::value_type& t);

    // Split into n segments
    // Original halfedges remain associated with their original vertices
    // Intermediate vertex values are interpolated from endpoints
    template<typename M>
    void split_edge(M& mesh, id_type idHalfedge, size_t n = 2);

    // id's are face halfedges whose vertices should be connected to produce two faces
    // Original halfedge and vertex id relationships are preserved
    // idHalfedgeA remains part of its original face
    template<typename M>
    void split_face(M& mesh, id_type idHalfedgeA, id_type idHalfedgeB);

    // id's are face halfedges whose vertices should be connected to produce two faces with a succession of edges at the given points
    // Original halfedge and vertex id relationships are preserved
    // idHalfedgeA remains part of its original face
    template<typename M>
    void split_face(M& mesh, id_type idHalfedgeA, id_type idHalfedgeB, const geometry::Points<typename M::vector_traits>& points);

    template<typename M>
    bool border_external(const M& mesh, id_type idHalfedge, const typename M::vector_type& normal);

    template<typename M>
    size_t border_edge_count(const M& mesh, id_type idHalfedge);

    template<typename M>
    size_t hole_edge_count(const M& mesh, id_type idHalfedge);

    template<typename M>
    std::vector<id_type> border_halfedges(const M& mesh, id_type idHalfedge);

    // Returns the partner id's of the virtual edges in order around the face that would fill this open area
    template<typename M>
    id_type next_border_halfedge(const M& mesh, id_type idHalfedge);

    // Returns the partner id's of the virtual edges in order around the face that would fill this open area
    template<typename M>
    id_type prev_border_halfedge(const M& mesh, id_type idHalfedge);

    // The create_border_ functions do not calculate texcoords

    // Create a face closing off the border starting at the given halfedge; does not handle internal borders
    template<typename M>
    id_type create_border_face(M& mesh, id_type idHalfedge, const typename M::vector_type& normal, id_type idSurface = nullid);

    // Create a face closing off the border starting at the first halfedge and adding a halfedge to bridge the gap between the first and last halfedges
    template<typename M>
    id_type create_border_face(M& mesh, id_type idHalfedgeFirst, id_type idHalfedgeLast, const typename M::vector_type& normal, id_type idSurface = nullid);

    // Create the halfedges and vertices of a face that would close off the border starting at the given border halfedge without creating a new face; handle an internal border
    template<typename M>
    id_type create_border_hole(M& mesh, id_type idHalfedge, const typename M::vector_type& normal, id_type idFace = nullid);

    // Create the halfedges and vertices of a face that would close off the border starting at the given border halfedge and with holes starting at the given halfedges
    template<typename M>
    id_type create_border_with_holes_face(M& mesh, id_type idHalfedge, const std::vector<id_type> idHalfedgeHoles, const typename M::vector_type& normal, id_type idSurface = nullid);

    // Create a 2D polygon to use for face/hole association
    template<typename M>
    geometry::Polygon<typename M::vector_traits::reduced_traits> create_border_polygon(const M& mesh, id_type idHalfedgeBorder, const math::DimensionReducer<typename M::vector_traits>& dr);

    // id's are face halfedges whose vertices should be connected with a pair of halfedges
    // No new faces are generated but face ids can be given to new halfedges
    // Handles only vertex position and halfedge connectivity
    template<typename M>
    void connect(M& mesh, id_type idHalfedgeA, id_type idHalfedgeB, id_type idFaceA = nullid, id_type idFaceB = nullid);

    // id's are face halfedges whose vertices should be connected with a pair of halfedges
    // No new faces are generated but face ids can be given to new halfedges
    // Handles only vertex position and halfedge connectivity
    template<typename M>
    void connect(M& mesh, id_type idHalfedgeA, id_type idHalfedgeB, id_type idFaceA, id_type idFaceB, const geometry::Points<typename M::vector_traits>& points);

    // Ensures that no vertices belong to more than one face (vertices are not shared between halfedges/faces)
    template<typename M>
    void prepare_face_vertices(M& mesh);

    template<typename M>
    std::array<id_type, 2> find_partner_halfedges(M& mesh, id_type idFaceA, id_type idFaceB);

} // namespace quetzal::brep

//------------------------------------------------------------------------------
template<typename M>
quetzal::id_type quetzal::brep::next_surface_halfedge_id(const M& mesh, id_type idHalfedge)
{
    assert(mesh.halfedge(idHalfedge).surface_seam());

    id_type idHalfedgeNext = mesh.halfedge(idHalfedge).next_id();
    while (!mesh.halfedge(idHalfedgeNext).surface_seam())
    {
        idHalfedgeNext = mesh.halfedge(idHalfedgeNext).partner().next_id();
    }

    return idHalfedgeNext;
}

//------------------------------------------------------------------------------
template<typename M>
quetzal::id_type quetzal::brep::prev_surface_halfedge_id(const M& mesh, id_type idHalfedge)
{
    assert(mesh.halfedge(idHalfedge).surface_seam());

    id_type idHalfedgePrev = mesh.halfedge(idHalfedge).prev_id();
    while (!mesh.halfedge(idHalfedgePrev).surface_seam())
    {
        idHalfedgePrev = mesh.halfedge(idHalfedgePrev).partner().prev_id();
    }

    return idHalfedgePrev;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::add_face_surfaces(M& mesh, const std::string& prefix)
{
    for (auto& face : mesh.faces())
    {
        std::string name = prefix + to_string(face.id());
        if (face.surface().name() != name)
        {
            mesh.add_surface_face(face.submesh_id(), name, face.id());
        }
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::remove_degenerate_face(M& mesh, id_type idFace)
{
    const auto& face = mesh.face(idFace);
    assert(!face.deleted());

    // remove_degenerate_face ...
    // order face vertices (find corners of bounding box and calculate positions of vertices along the diagonal)
    // add halfedges/vertices between each pair in sequence, connecting to successive edge and existing edges, except for those in original face

    // set degenerate face halfedge ids to nullid, then
    // mesh.delete_face(idFace);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::remove_degenerate_faces(M& mesh)
{
    std::vector<id_type> idsDegenerate;
    for (const auto& face : mesh.faces())
    {
        if (degenerate_face(mesh, face.id()))
        {
            idsDegenerate.push_back(id_type);
        }
    }

    for (id_type id : idsDegenerate)
    {
        remove_degenerate_face(mesh, id);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
quetzal::id_type quetzal::brep::remove_vertex(M& mesh, id_type idVertex)
{
    auto& vertex = mesh.vertex(idVertex);
    assert(!vertex.deleted());

    id_type idHalfedge = vertex.halfedge().next_id();
    id_type idFace = vertex.halfedge().face_id();
    id_type idSurace = vertex.halfedge().face().surface_id();

    // construct merged face data for use in new merged face

    // do this only for faces in idSurface ...
    // handle vertices on border and/or surface edge ...

    for (auto& halfedge : vertex.halfedges())
    {
        // Delete all but original face
        if (halfedge.face_id() != idFace)
        {
            mesh.remove_face(halfedge.face_id());
        }

        halfedge.next().set_face_id(idFace);
        halfedge.next().set_prev_id(halfedge.partner().prev_id());
        halfedge.prev().prev().set_next_id(halfedge.prev().partner().next_id());
        halfedge.prev().set_deleted();
        halfedge.set_deleted();
        halfedge.prev().vertex().set_deleted();
        halfedge.vertex().set_deleted();
    }

    mesh.face(idFace).set_halfedge_id(idHalfedge);
    return idFace;
}

//------------------------------------------------------------------------------
template<typename M>
quetzal::id_type quetzal::brep::remove_edge(M& mesh, id_type idHalfedge)
{
    typename M::halfedge_type& halfedge = mesh.halfedge(idHalfedge);
    assert(!halfedge.deleted());

    if (halfedge.border())
    {
        mesh.delete_face(halfedge.face_id());
        return nullid;
    }

    id_type idFace = halfedge.partner().face_id();

    halfedge.next().set_prev_id(halfedge.partner().prev_id());
    halfedge.prev().set_next_id(halfedge.partner().next_id());
    halfedge.partner().next().set_prev_id(halfedge.prev_id());
    halfedge.partner().prev().set_next_id(halfedge.next_id());

    halfedge.face().set_deleted();
    halfedge.partner().set_deleted();
    halfedge.set_deleted();

    id_type id = halfedge.next_id();
    do
    {
        mesh.halfedge(id).set_face_id(idFace);
        id = mesh.halfedge(id).next_id();
    } while (id != halfedge.next_id());

    return idFace;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::remove_submesh(M& mesh, id_type idHalfedge, bool bReset)
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

    // recursion here might be a problem with very large meshes ...

    id_type idFace = face.id();
    face.set_marked();
    for (auto& halfedge : face.halfedges())
    {
        if (halfedge.partner_id() != nullid)
        {
            remove_submesh(mesh, halfedge.partner_id(), false);
        }
    }

    mesh.delete_face(idFace);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::update_halfedge(typename M::halfedge_type& halfedge, M& mesh, id_type idHalfedgeOffset, id_type idVertexOffset, id_type idFaceOffset)
{
    halfedge.set_mesh(mesh);
    halfedge.set_id(halfedge.id() + idHalfedgeOffset);
    if (halfedge.partner_id() != nullid)
    {
        halfedge.set_partner_id(halfedge.partner_id() + idHalfedgeOffset);
    }
    halfedge.set_next_id(halfedge.next_id() + idHalfedgeOffset);
    halfedge.set_prev_id(halfedge.prev_id() + idHalfedgeOffset);
    halfedge.set_vertex_id(halfedge.vertex_id() + idVertexOffset);
    halfedge.set_face_id(halfedge.face_id() + idFaceOffset);

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::update_vertex(typename M::vertex_type& vertex, M& mesh, id_type idHalfedgeOffset, id_type idVertexOffset)
{
    vertex.set_mesh(mesh);
    vertex.set_id(vertex.id() + idVertexOffset);
    vertex.set_halfedge_id(vertex.halfedge_id() + idHalfedgeOffset);

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::update_face(typename M::face_type& face, M& mesh, id_type idHalfedgeOffset, id_type idFaceOffset, id_type idSurface, id_type idSubmesh)
{
    face.set_mesh(mesh);
    face.set_id(face.id() + idFaceOffset);
    face.set_halfedge_id(face.halfedge_id() + idHalfedgeOffset);

    for (auto& hole : face.holes())
    {
        hole.set_face_id(face.id());
        hole.set_halfedge_id(hole.halfedge_id() + idHalfedgeOffset);
    }

    face.set_surface_id(idSurface);
    face.set_submesh_id(idSubmesh);

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::split_edge(M& mesh, id_type idHalfedge, const typename M::vertex_attributes_type& av, const typename M::vertex_attributes_type& avPartner)
{
    assert(!mesh.halfedge(idHalfedge).deleted());
    assert(vector_eq(av.position(), avPartner.position()));

    const auto& halfedgeOrig = mesh.halfedge(idHalfedge);
    id_type idHalfedgeNext = halfedgeOrig.next_id();
    id_type idHalfedgeFace = halfedgeOrig.face_id();
    id_type idPartner = halfedgeOrig.partner_id();
    id_type idPartnerNext = halfedgeOrig.partner().next_id();
    id_type idPartnerFace = halfedgeOrig.partner().face_id();

    id_type idHalfedge0 = mesh.create_halfedge_vertex(idPartner, idHalfedgeNext, idHalfedge, idHalfedgeFace, av); // extension of halfedge
    id_type idHalfedge1 = mesh.create_halfedge_vertex(idHalfedge, idPartnerNext, idPartner, idPartnerFace, avPartner); // partner of halfedge

    auto& partner = mesh.halfedge(idPartner);
    partner.next().set_prev_id(idHalfedge1);
    partner.set_next_id(idHalfedge1);
    partner.set_partner_id(idHalfedge0);

    auto& halfedge = mesh.halfedge(idHalfedge);
    halfedge.next().set_prev_id(idHalfedge0);
    halfedge.set_next_id(idHalfedge0);
    halfedge.set_partner_id(idHalfedge1);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::split_edge(M& mesh, id_type idHalfedge, const typename M::point_type& position)
{
    auto halfedge = mesh.halfedge(idHalfedge);
    assert(!halfedge.deleted());

    const typename M::vertex_attributes_type av0 = halfedge.attributes();
    const typename M::vertex_attributes_type av1 = halfedge.next().attributes();
    const typename M::vertex_attributes_type avp0 = halfedge.partner().next().attributes();
    const typename M::vertex_attributes_type avp1 = halfedge.partner().attributes();

    assert(vector_eq(av0.position(), avp0.position()));
    assert(vector_eq(av1.position(), avp1.position()));

    auto t = to_segment(halfedge).projection_parameter(position);
    typename M::vertex_attributes_type av = lerp(av0, av1, t);
    typename M::vertex_attributes_type avPartner = lerp(avp0, avp1, t);

    // Restore original position to avoid floating point precision issue or if the split point was not colinear
    av.set_position(position);
    avPartner.set_position(position);

    split_edge(mesh, idHalfedge, av, avPartner);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::split_edge(M& mesh, id_type idHalfedge, const typename M::value_type& t)
{
    auto halfedge = mesh.halfedge(idHalfedge);
    assert(!halfedge.deleted());
    assert(t >= M::traits_type::val(0) && t <= M::traits_type::val(1));

    const typename M::vertex_attributes_type av0 = halfedge.attributes();
    const typename M::vertex_attributes_type av1 = halfedge.next().attributes();
    const typename M::vertex_attributes_type avp0 = halfedge.partner().next().attributes();
    const typename M::vertex_attributes_type avp1 = halfedge.partner().attributes();

    assert(vector_eq(av0.position(), avp0.position()));
    assert(vector_eq(av1.position(), avp1.position()));

    const typename M::vertex_attributes_type av = lerp(av0, av1, t);
    const typename M::vertex_attributes_type avPartner = lerp(avp0, avp1, t);

    // Set positions the same to avoid floating point precision issue
    avPartner.set_position(av.position());

    split_edge(mesh, idHalfedge, av, avPartner);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::split_edge(M& mesh, id_type idHalfedge, size_t n)
{
    auto halfedge = mesh.halfedge(idHalfedge);
    assert(!halfedge.deleted());
//    assert(n >= 2);
    if (n < 2) // should be assert here ...
    {
        return;
    }

    const typename M::vertex_attributes_type av0 = halfedge.attributes();
    const typename M::vertex_attributes_type av1 = halfedge.next().attributes();
    const typename M::vertex_attributes_type avp0 = halfedge.partner().next().attributes();
    const typename M::vertex_attributes_type avp1 = halfedge.partner().attributes();

    assert(vector_eq(av0.position(), avp0.position()));
    assert(vector_eq(av1.position(), avp1.position()));

    for (size_t i = 1; i < n; ++i)
    {
        auto t = M::traits_type::val(i) / M::traits_type::val(n);
        const typename M::vertex_attributes_type av = lerp(av0, av1, t);
        const typename M::vertex_attributes_type avp = lerp(avp0, avp1, t);
        split_edge(mesh, idHalfedge, av, avp);
        idHalfedge = mesh.halfedge(idHalfedge).next_id();
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::split_face(M& mesh, id_type idHalfedgeA, id_type idHalfedgeB)
{
    assert(!mesh.halfedge(idHalfedgeA).deleted());
    assert(!mesh.halfedge(idHalfedgeB).deleted());
    assert(idHalfedgeA != idHalfedgeB);
    assert(mesh.halfedge(idHalfedgeA).face_id() == mesh.halfedge(idHalfedgeB).face_id());

    id_type idFaceA = mesh.halfedge(idHalfedgeA).face_id();
    id_type idSurface = mesh.face(idFaceA).surface_id();

    mesh.face(idFaceA).set_halfedge_id(idHalfedgeA);
    id_type idFaceB = mesh.create_face(idSurface, idHalfedgeB, mesh.face(idFaceA).attributes());

    for (id_type id = idHalfedgeB; id != idHalfedgeA; id = mesh.halfedge(id).next_id())
    {
        mesh.halfedge(id).set_face_id(idFaceB);
    }

    assert(mesh.halfedge(idHalfedgeA).face_id() == idFaceA);
    assert(mesh.halfedge(idHalfedgeB).face_id() == idFaceB);

    connect(mesh, idHalfedgeA, idHalfedgeB, idFaceA, idFaceB);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::split_face(M& mesh, id_type idHalfedgeA, id_type idHalfedgeB, const geometry::Points<typename M::vector_traits>& points)
{
    assert(!mesh.halfedge(idHalfedgeA).deleted());
    assert(!mesh.halfedge(idHalfedgeB).deleted());
    assert(idHalfedgeA != idHalfedgeB); // ? ...
    assert(mesh.halfedge(idHalfedgeA).face_id() == mesh.halfedge(idHalfedgeB).face_id());
    assert(points.size() > 1);
    assert(to_segment(mesh.halfedge(idHalfedgeA)).contains(points.front()));
    assert(to_segment(mesh.halfedge(idHalfedgeB)).contains(points.back()));

    id_type idFaceA = mesh.halfedge(idHalfedgeA).face_id();
    id_type idSurface = mesh.face(idFaceA).surface_id();

    // split edges and reassign idHalfedge's where necessary
    if (!vector_eq(points.front(), mesh.halfedge(idHalfedgeA).attributes().position()))
    {
        idHalfedgeA = split_edge(mesh, idHalfedgeA, points.front());
    }

    if (!vector_eq(points.back(), mesh.halfedge(idHalfedgeB).attributes().position()))
    {
        idHalfedgeB = split_edge(mesh, idHalfedgeB, points.back());
    }

    mesh.face(idFaceA).set_halfedge_id(idHalfedgeA);
    id_type idFaceB = mesh.create_face(idSurface, idHalfedgeB, mesh.face(idFaceA).attributes());

    for (id_type id = idHalfedgeB; id != idHalfedgeA; id = mesh.halfedge(id).next_id())
    {
        mesh.halfedge(id).set_face_id(idFaceB);
    }

    assert(mesh.halfedge(idHalfedgeA).face_id() == idFaceA);
    assert(mesh.halfedge(idHalfedgeB).face_id() == idFaceB);

    connect(mesh, idHalfedgeA, idHalfedgeB, idFaceA, idFaceB, points);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::border_external(const M& mesh, id_type idHalfedge, const typename M::vector_type& normal)
{
    assert(!mesh.halfedge(idHalfedge).deleted());
    assert(mesh.halfedge(idHalfedge).border());

    typename M::value_type sumAnglesExterior = M::traits_type::val(0);
    id_type id = idHalfedge;

    do
    {
        const auto& halfedge = mesh.halfedge(id);

        typename M::point_type positionPrev = mesh.halfedge(prev_border_halfedge(mesh, id)).attributes().position();
        typename M::point_type position = halfedge.attributes().position();
        typename M::point_type positionNext = mesh.halfedge(next_border_halfedge(mesh, id)).attributes().position();

        typename M::value_type angleInterior = math::angle(positionNext - position, positionPrev - position, normal);

        sumAnglesExterior += math::Pi<typename M::value_type> - angleInterior;

        id = next_border_halfedge(mesh, id);
    } while (id != idHalfedge);

    return sumAnglesExterior > M::traits_type::val(0);
}

//------------------------------------------------------------------------------
template<typename M>
size_t quetzal::brep::border_edge_count(const M& mesh, id_type idHalfedge)
{
    assert(!mesh.halfedge(idHalfedge).deleted());
    assert(mesh.halfedge(idHalfedge).border());

    size_t n = 0;
    id_type id = idHalfedge;

    do
    {
        ++n;
        id = next_border_halfedge(mesh, id);
    } while (id != idHalfedge);

    return n;
}

//------------------------------------------------------------------------------
template<typename M>
size_t quetzal::brep::hole_edge_count(const M& mesh, id_type idHalfedge)
{
    assert(!mesh.halfedge(idHalfedge).deleted());

    size_t n = 0;
    id_type id = idHalfedge;

    do
    {
        ++n;
        id = mesh.halfedge(id).next_id();
    } while (id != idHalfedge);

    return n;
}

//------------------------------------------------------------------------------
template<typename M>
std::vector<quetzal::id_type> quetzal::brep::border_halfedges(const M& mesh, id_type idHalfedge)
{
    assert(!mesh.halfedge(idHalfedge).deleted());

    std::vector<id_type> ids;
    id_type id = idHalfedge;

    do
    {
        assert(mesh.halfedge(id).border());
        ids.push_back(id);
        id = next_border_halfedge(mesh, id);
    } while (id != idHalfedge);

    return ids;
}

//------------------------------------------------------------------------------
template<typename M>
quetzal::id_type quetzal::brep::next_border_halfedge(const M& mesh, id_type idHalfedge)
{
    assert(!mesh.halfedge(idHalfedge).deleted());

    id_type id = mesh.halfedge(idHalfedge).prev_id();
    while (!mesh.halfedge(id).border())
    {
        if (mesh.halfedge(id).partner_id() == idHalfedge)
        {
            return nullid;
        }

        id = mesh.halfedge(id).partner().prev_id();
    }

    return id;
}

//------------------------------------------------------------------------------
template<typename M>
quetzal::id_type quetzal::brep::prev_border_halfedge(const M& mesh, id_type idHalfedge)
{
    assert(!mesh.halfedge(idHalfedge).deleted());

    id_type id = mesh.halfedge(idHalfedge).next_id();
    while (!mesh.halfedge(id).border())
    {
        if (mesh.halfedge(id).partner_id() == idHalfedge)
        {
            return nullid;
        }

        id = mesh.halfedge(id).partner().next_id();
    }

    return id;
}

//------------------------------------------------------------------------------
template<typename M>
quetzal::id_type quetzal::brep::create_border_face(M& mesh, id_type idHalfedge, const typename M::vector_type& normal, id_type idSurface)
{
    assert(!mesh.halfedge(idHalfedge).deleted());
    assert(mesh.halfedge(idHalfedge).border());

    id_type nh = mesh.halfedge_store_count();
    id_type nhOrig = nh;
    id_type idFace = mesh.create_face(idSurface, nhOrig, {normal});

    id_type id = idHalfedge;
    do
    {
        id_type idPartner = next_border_halfedge(mesh, id);
        assert(idPartner != nullid);

        mesh.halfedge(idPartner).set_partner_id(nh);

        typename M::vertex_attributes_type av = mesh.halfedge(id).attributes();
        av.set_normal(normal);

        mesh.create_halfedge_vertex(idPartner, nh + 1, nh - 1, idFace, av);

        ++nh;

        id = idPartner;
    } while (id != idHalfedge);

    // Fix first-last links
    mesh.halfedge(nh - 1).set_next_id(nhOrig);
    mesh.halfedge(nhOrig).set_prev_id(nh - 1);

    return idFace;
}

//------------------------------------------------------------------------------
template<typename M>
quetzal::id_type quetzal::brep::create_border_face(M& mesh, id_type idHalfedgeFirst, id_type idHalfedgeLast, const typename M::vector_type& normal, id_type idSurface)
{
    assert(!mesh.halfedge(idHalfedgeFirst).deleted());
    assert(mesh.halfedge(idHalfedgeFirst).border());
    assert(!mesh.halfedge(idHalfedgeLast).deleted());
    assert(mesh.halfedge(idHalfedgeLast).border());
    assert(idHalfedgeFirst != idHalfedgeLast);

    id_type nh = mesh.halfedge_store_count();
    id_type nhOrig = nh;
    id_type idFace = mesh.create_face(idSurface, nh, {normal});

    id_type id = idHalfedgeFirst;
    for (;;)
    {
        mesh.halfedge(id).set_partner_id(nh);
        
        typename M::vertex_attributes_type av = mesh.halfedge(id).next().attributes();
        av.set_normal(normal);

        mesh.create_halfedge_vertex(id, nh + 1, nh - 1, idFace, av);

        ++nh;

        if (id == idHalfedgeLast)
        {
            break;
        }

        id = next_border_halfedge(mesh, id);
        assert(id != nullid);
    }

    // Add the connecting halfedge
    typename M::vertex_attributes_type av = mesh.halfedge(id).attributes();
    av.set_normal(normal);

    id_type idVertex = mesh.create_vertex(nh, av);
    mesh.create_halfedge(nullid, nhOrig, nh - 1, idVertex, idFace);

    // Correct the end halfedge linkage
    mesh.halfedge(nh - 1).set_next_id(nh);
    mesh.halfedge(nhOrig).set_prev_id(nh);

    return idFace;
}

//------------------------------------------------------------------------------
template<typename M>
quetzal::id_type quetzal::brep::create_border_hole(M& mesh, id_type idHalfedge, const typename M::vector_type& normal, id_type idFace)
{
    assert(!mesh.halfedge(idHalfedge).deleted());
    assert(mesh.halfedge(idHalfedge).border());

    id_type nh = mesh.halfedge_store_count();

    id_type idHalfedgeHole = nh;
    id_type id = idHalfedge;
    do
    {
        id_type idPartner = next_border_halfedge(mesh, id);

        mesh.halfedge(idPartner).set_partner_id(nh);

        auto av = mesh.halfedge(id).attributes();
        av.set_normal(normal);

        mesh.create_halfedge_vertex(idPartner, nh + 1, nh - 1, idFace, av);

        ++nh;

        id = idPartner;
    } while (id != idHalfedge);

    // Fix first-last links
    mesh.halfedge(nh - 1).set_next_id(idHalfedgeHole);
    mesh.halfedge(idHalfedgeHole).set_prev_id(nh - 1);

    return idHalfedgeHole;
}

//------------------------------------------------------------------------------
template<typename M>
quetzal::id_type quetzal::brep::create_border_with_holes_face(M& mesh, id_type idHalfedge, const std::vector<id_type> idHalfedgeHoles, const typename M::vector_type& normal, id_type idSurface)
{
    id_type idFace = create_border_face(mesh, idHalfedge, normal, idSurface);

    for (id_type idHalfedgeHole : idHalfedgeHoles)
    {
        create_border_hole(mesh, idHalfedgeHole, normal, idFace);
    }

    return idFace;
}

//------------------------------------------------------------------------------
template<typename M>
quetzal::geometry::Polygon<typename M::vector_traits::reduced_traits> quetzal::brep::create_border_polygon(const M& mesh, id_type idHalfedgeBorder, const math::DimensionReducer<typename M::vector_traits>& dr)
{
    assert(!mesh.halfedge(idHalfedgeBorder).deleted());
    assert(mesh.halfedge(idHalfedgeBorder).border());

    geometry::Polygon<typename M::vector_traits::reduced_traits> polygon;

    id_type id = idHalfedgeBorder;
    do
    {
        polygon.vertices().push_back(dr.reduce(mesh.halfedge(id).attributes().position()));
        id = next_border_halfedge(mesh, id);
    } while (id != idHalfedgeBorder);

    return polygon;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::connect(M& mesh, id_type idHalfedgeA, id_type idHalfedgeB, id_type idFaceA, id_type idFaceB)
{
    assert(!mesh.halfedge(idHalfedgeA).deleted());
    assert(!mesh.halfedge(idHalfedgeB).deleted());
    assert(idHalfedgeA != idHalfedgeB);

    id_type nh = mesh.halfedge_store_count();

    auto& halfedgeA = mesh.halfedge(idHalfedgeA);
    id_type idPrevA = halfedgeA.prev_id();
    halfedgeA.prev().set_next_id(nh + 1);
    halfedgeA.set_prev_id(nh);
    auto avB = halfedgeA.attributes();

    auto& halfedgeB = mesh.halfedge(idHalfedgeB);
    id_type idPrevB = halfedgeB.prev_id();
    halfedgeB.prev().set_next_id(nh);
    halfedgeB.set_prev_id(nh + 1);
    auto avA = halfedgeB.attributes();

    mesh.create_halfedge_vertex(nh + 1, idHalfedgeA, idPrevB, idFaceA, avA);
    mesh.create_halfedge_vertex(nh, idHalfedgeB, idPrevA, idFaceB, avB);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::connect(M& mesh, id_type idHalfedgeA, id_type idHalfedgeB, id_type idFaceA, id_type idFaceB, const geometry::Points<typename M::vector_traits>& points)
{
    assert(idHalfedgeA != idHalfedgeB);

    auto& halfedgeA = mesh.halfedge(idHalfedgeA);
    assert(!halfedgeA.deleted());
    assert(vector_eq(halfedgeA.attributes().position, points.front()));

    auto& halfedgeB = mesh.halfedge(idHalfedgeB);
    assert(!halfedgeB.deleted());
    assert(vector_eq(halfedgeB.attributes().position, points.back()));

    auto& faceA = mesh.face(idFaceA);
    auto& faceB = mesh.face(idFaceB);

    typename M::vertex_attributes_type avA;
    typename M::vertex_attributes_type avB;

    avA.set_normal(faceA.attributes().normal());
    avB.set_normal(faceB.attributes().normal());

    id_type nh = mesh.halfedge_store_count();

    id_type idNextA = idHalfedgeA;
    id_type idPrevB = halfedgeA.prev_id();

    for (auto i = std::next(points.begin()); i != points.end(); ++i)
    {
        auto position = *i;
        avA.set_position(position);
        avB.set_position(mesh.halfedge(idNextA).attributes().position());

        mesh.create_halfedge_vertex(nh + 1, idNextA, nh + 2, idFaceA, avA);
        mesh.create_halfedge_vertex(nh, nh + 3, idPrevB, idFaceB, avB);

        idNextA = nh;
        idPrevB = nh + 1;
        nh += 2;
    }

    mesh.halfedge(idHalfedgeB).prev().set_next_id(idNextA);
    mesh.halfedge(idNextA).set_prev_id(mesh.halfedge(idHalfedgeB).prev_id());

    mesh.halfedge(idHalfedgeB).set_prev_id(idPrevB);
    mesh.halfedge(idPrevB).set_next_id(idHalfedgeB);

    if constexpr (M::vertex_attributes_type::contains(geometry::AttributesFlags::Texcoord0))
    {
        for (id_type idA = mesh.halfedge(idHalfedgeA).prev_id(); mesh.halfedge(idA).partner_id() != mesh.halfedge(idHalfedgeB).prev_id; idA = mesh.halfedge(idA).prev_id())
        {
            // texcoord interpolation context in place of face for efficiency ...
            auto texcoord = interpolate_face_texcoord(faceA, mesh.halfedge(idA).attributes().position()); // same for both since originally a single face
            mesh.halfedge(idA).attributes().set_texcoord(texcoord);
            mesh.halfedge(idA).partner().next().attributes().set_texcoord(texcoord);
        }
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::prepare_face_vertices(M& mesh)
{
    typename M::vertices_type vertices_orig = move(mesh.vertex_store());
    mesh.vertex_store().clear();

    for (auto& face : mesh.faces())
    {
        for (auto& halfedge : face.halfedges())
        {
            halfedge.set_vertex_id(mesh.create_vertex(halfedge.id(), vertices_orig[halfedge.vertex_id()].attributes()));
        }
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
std::array<quetzal::id_type, 2> quetzal::brep::find_partner_halfedges(M& mesh, id_type idFaceA, id_type idFaceB)
{
    const auto& halfedgeA = mesh.face(idFaceA).halfedge();
    for (const auto& halfedgeB : mesh.face(idFaceB).halfedges())
    {
        if (coincident(halfedgeA.attributes(), halfedgeB.next().attributes()) && coincident(halfedgeA.next().attributes(), halfedgeB.attributes()))
        {
            return {halfedgeA.id(), halfedgeB.id()};
        }
    }

    return {nullid, nullid};
}

#endif // QUETZAL_BREP_MESH_UTIL_HPP
