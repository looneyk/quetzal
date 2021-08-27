#if !defined(QUETZAL_BREP_MESH_CONNECTION2_HPP)
#define QUETZAL_BREP_MESH_CONNECTION2_HPP
//------------------------------------------------------------------------------
// brep
// mesh_util.hpp
//------------------------------------------------------------------------------

#include "mesh_geometry.hpp"
#include "mesh_texcoord.hpp"
#include "quetzal/math/math_util.hpp"
#include "quetzal/geometry/intersect.hpp"
#include <algorithm>
#include <map>
#include <tuple>

#include "mesh_connection.hpp" // weld ...

namespace quetzal::brep
{

    // Currently only supports surfaces that consist of a single face
    template<typename M>
    void attach_x(M& mesh, const std::string& nameSurfaceA, const std::string& nameSurfaceB, bool bSurfacesDistinct = false);

    template<typename M>
    void attach_x(M& mesh, id_type idFaceA, id_type idFaceB, bool bSurfacesDistinct = false);

    // Implementation

    template<typename M>
    std::vector<std::array<id_type, 2>> create_intersection_faces(M& mesh, id_type idFaceA, id_type idFaceB);

    template<typename M>
    void mark_internal(const typename M::face_type& faceA, const typename M::face_type& faceB);

    template<typename M>
    struct EdgeIntersection
    {
        const M& mesh;
        id_type idHalfedgeA;
        id_type idHalfedgeB;
        std::array<typename M::point_type, 2> points;
        bool bSegment;
        // flag for position lies on one of the halfedges to save later check ...
    };

    template<typename M>
    using EdgeIntersections = std::vector<EdgeIntersection<M>>;

    template<typename M>
    EdgeIntersections<M> edge_intersections(const typename M::face_type& faceA, const typename M::face_type& faceB);

    template<typename M>
    id_type split_intersecting(M& mesh, id_type idHalfedge, id_type idHalfedgeOther, const typename M::vector_type& position);

    template<typename M>
    void update_intersections(M& mesh, EdgeIntersections<M>& intersections, id_type idHalfedgeOrig, id_type idHalfedge);

    using ledges_type = std::vector<std::pair<id_type, id_type>>;

    template<typename M>
    ledges_type find_ledges(const M& mesh, const std::map<id_type, id_type>& intersection_correlation);

    template<typename M>
    bool inward(const M& mesh, id_type idHalfedgeA, id_type idHalfedgeB);

    template<typename M>
    std::ostream& operator<<(std::ostream& os, const EdgeIntersection<M>& e)
    {
        auto marked = [&](id_type idHalfedge) -> bool { return e.mesh.halfedge(idHalfedge).vertex().marked(); };
        auto mark = [&](id_type idHalfedge) -> std::string { return marked(idHalfedge) ? "*" : ""; };
        os << "intersection " << e.idHalfedgeA << mark(e.idHalfedgeA) << " " << e.idHalfedgeB << mark(e.idHalfedgeB) << " {[" << e.points[0] << "], [" << e.points[1] << "]} " << e.bSegment;
        return os;
    }

    template<typename M>
    std::ostream& operator<<(std::ostream& os, const EdgeIntersections<M>& es)
    {
        for (const auto& e : es)
        {
            os << e << std::endl;
        }
        return os;
    }

    template<typename M>
    void print_face(const typename M::face_type& face)
    {
        auto marked = [&face](id_type idHalfedge) -> bool { return face.mesh().halfedge(idHalfedge).vertex().marked(); };
        auto mark = [&](id_type idHalfedge) -> std::string { return marked(idHalfedge) ? "*" : ""; };
        std::cout << face.id() << std::endl;
        for (auto& halfedge : face.halfedges())
        {
            std::cout << "    " << halfedge.id() << mark(halfedge.id()) << " [" << halfedge.attributes().position() << "]" << std::endl;
        }
        return;
    }

} // namespace quetzal::brep

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::attach_x(M& mesh, const std::string& nameSurfaceA, const std::string& nameSurfaceB, bool bSurfacesDistinct)
{
    auto& surfaceA = mesh.surface(nameSurfaceA);
    auto& surfaceB = mesh.surface(nameSurfaceB);

    // so far this only handles the single face per surface case ...
    assert(surfaceA.face_count() == surfaceB.face_count());
    assert(surfaceA.face_count() == 1);

    attach_x(mesh, surfaceA.faces().front().id(), surfaceB.faces().front().id(), bSurfacesDistinct);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::attach_x(M& mesh, id_type idFaceA, id_type idFaceB, bool bSurfacesDistinct)
{
    auto intersections = create_intersection_faces(mesh, idFaceA, idFaceB);
    for (const auto& i : intersections)
    {
        weld(mesh, i[0], i[1]);
    }

bSurfacesDistinct;
/*
    // submesh should be reassigned, but faces should retain their surface membership ...
    // retain existing submesh as well? ...
    id_type idSurface = faceA.surface_id();
    id_type idSubmesh = faceA.submesh_id();
    size_t i = 0;

    // need to apply create_intersection_faces here too ...
    // but using intersection for hole face instead of removing it ...
    for (const auto& hole : faceB.holes())
    {
        id_type idFace = mesh.create_face(idSurface, hole.halfedge_id(), {normal});
        auto& face = mesh.face(idFace);

        for (auto& halfedge : face.halfedges())
        {
            halfedge.set_face_id(idFace);
            halfedge.attributes().set_normal(normal); // vertex ...
        }

        if (bSurfacesDistinct)
        {
            id_type idSurfaceHole = mesh.create_surface(idSubmesh, mesh.surface(idSurface).name() + to_string(i), mesh.surface(idSurface).attributes(), mesh.surface(idSurface).properties());
            face.set_surface_id(idSurfaceHole);

            if constexpr (M::vertex_attributes_type::contains(geometry::AttributesFlags::Texcoord0))
            {
                calculate_surface_texcoords(mesh, idSurfaceHole);
            }

            ++i;
        }
        else
        {
            if constexpr (M::vertex_attributes_type::contains(geometry::AttributesFlags::Texcoord0))
            {
                interpolate_face_texcoords(faceA, face);
            }
        }
    }
*/
    return;
}

//------------------------------------------------------------------------------
template<typename M>
std::vector<std::array<quetzal::id_type, 2>> quetzal::brep::create_intersection_faces(M& mesh, id_type idFaceA, id_type idFaceB)
{
    auto marked = [&mesh](id_type idHalfedge) -> bool { return mesh.halfedge(idHalfedge).vertex().marked(); };
    auto mark = [&](id_type idHalfedge) -> std::string { return marked(idHalfedge) ? "*" : ""; };

    // assert coplanar ...
    // assert reverse normals ...

    // Mark interior and inward directed halfedges

    auto& faceA = mesh.face(idFaceA);
    auto& faceB = mesh.face(idFaceB);

    mark_internal<M>(faceA, faceB);
std::cout << "A ";
print_face<M>(faceA);
std::cout << "B ";
print_face<M>(faceB);

    EdgeIntersections<M> intersections = edge_intersections<M>(faceA, faceB);
std::cout << intersections;

    // this does not handle multiple intersections at a point ...
    // make sure this handles an edge that is intersected more than once ...

    std::map<id_type, id_type> intersection_correlation;
    for (auto& e : intersections)
    {
        typename M::point_type point = e.bSegment ? e.points[1] : e.points[0];
        id_type idHalfedgeA = split_intersecting(mesh, e.idHalfedgeA, e.idHalfedgeB, point);
std::cout << "split A <" << idFaceA << "> " << e.idHalfedgeA << mark(e.idHalfedgeA) << " at " << idHalfedgeA << mark(idHalfedgeA) << " [" << point << "]" << std::endl;
        update_intersections(mesh, intersections, e.idHalfedgeA, idHalfedgeA);
std::cout << "  A ";
print_face<M>(faceA);
std::cout << intersections;

        point = e.bSegment ? e.points[0] : e.points[0]; // ...
        id_type idHalfedgeB = split_intersecting(mesh, e.idHalfedgeB, e.idHalfedgeA, point);
std::cout << "split B <" << idFaceB << "> " << e.idHalfedgeB << mark(e.idHalfedgeB) << " at " << idHalfedgeB << mark(idHalfedgeB) << " [" << point << "]" << std::endl;
        update_intersections(mesh, intersections, e.idHalfedgeB, idHalfedgeB);
std::cout << "  B ";
print_face<M>(faceB);
std::cout << intersections;

        assert(marked(idHalfedgeA) && marked(idHalfedgeB) || !marked(idHalfedgeA) && !marked(idHalfedgeB));

        intersection_correlation[idHalfedgeA] = idHalfedgeB;
        intersection_correlation[idHalfedgeB] = idHalfedgeA;
    }
std::cout << "A ";
print_face<M>(faceA);
std::cout << "B ";
print_face<M>(faceB);
std::cout << "intersection_correlation" << std::endl;
for (const auto& ic : intersection_correlation)
{
    std::cout << "  " << ic.first << mark(ic.first) << " f " << mesh.halfedge(ic.first).face_id() << "    " << ic.second << mark(ic.second) << " f " << mesh.halfedge(ic.second).face_id() << std::endl;
}

    // at this point, all intersection points have been added to both faces
    // now just add faces using the edges that are internal from the other face
    // (this should already be handling intersections at a vertex)

    // From intersection_correlation, save the beginning and ending point of each ledge, the part of the face that juts out over the other face
    ledges_type ledges = find_ledges(mesh, intersection_correlation);
std::cout << "ledges" << std::endl;
for (const auto& ledge : ledges)
{
    std::cout << "  " << ledge.first << mark(ledge.first) << ".." << ledge.second << mark(ledge.second) << std::endl;
}

    size_t nh = mesh.halfedge_store_count();

    for (const auto& ledge : ledges)
    {
std::cout << "for ledge" << std::endl;
        id_type idHalfedge = ledge.second;
std::cout << "  idHalfedge " << idHalfedge << mark(idHalfedge) << " face " << mesh.halfedge(idHalfedge).face_id() << std::endl;

        id_type idFaceOrig = mesh.halfedge(idHalfedge).face_id();
        id_type idFace = mesh.create_face(mesh.face(idFaceOrig).surface_id(), nh, mesh.face(idFaceOrig).attributes());
std::cout << "  idFaceOrig " << idFaceOrig << std::endl;
std::cout << "  idFace " << idFace << std::endl;

        id_type idNext = idHalfedge;
        id_type idPrev = mesh.halfedge(idHalfedge).prev_id();
std::cout << "  idNext " << idNext << mark(idNext) << std::endl;
std::cout << "  idPrev " << idPrev << mark(idPrev) << std::endl;

        mesh.face(idFaceOrig).set_halfedge_id(idNext);

        // Create edges to create ledge from edges of opposing face

        id_type idHalfedgeOther = intersection_correlation[idHalfedge];
        auto normal = mesh.face(idFaceOrig).attributes().normal();

        do
        {
std::cout << "  do" << std::endl;
std::cout << "    idHalfedgeOther " << idHalfedgeOther << mark(idHalfedgeOther) << " [" << mesh.halfedge(idHalfedgeOther).attributes().position() << "]" << std::endl;
            mesh.halfedge(idNext).set_prev_id(nh + 1);
            mesh.halfedge(idPrev).set_next_id(nh);

            auto av = mesh.halfedge(idHalfedgeOther).attributes();
            av.set_normal(normal);
            idPrev = mesh.create_halfedge_vertex(nh + 1, nullid, idPrev, idFace, av);
std::cout << "    idPrev " << idPrev << mark(idPrev) << " [" << av.position() << "]" << std::endl;

            av = mesh.halfedge(idHalfedgeOther).next().attributes();
            av.set_normal(normal);
            idNext = mesh.create_halfedge_vertex(nh, idNext, nullid, idFaceOrig, av);
std::cout << "    idNext " << idNext << mark(idNext) << " [" << av.position() << "]" << std::endl;

            idHalfedgeOther = mesh.halfedge(idHalfedgeOther).next_id();
            nh += 2;
std::cout << "    while marked(idHalfedgeOther) " << idHalfedgeOther << mark(idHalfedgeOther) << std::endl;
        } while (marked(idHalfedgeOther));

        // Connect last edge to pre-existing geometry and update face ids

        id_type idHalfedgeEnd = ledge.first;
std::cout << "  idHalfedgeEnd " << idHalfedgeEnd << mark(idHalfedgeEnd) << std::endl;

        mesh.halfedge(idNext).set_prev_id(mesh.halfedge(idHalfedgeEnd).prev_id());
        mesh.halfedge(idPrev).set_next_id(idHalfedgeEnd);

        mesh.halfedge(idHalfedgeEnd).prev().set_next_id(idNext);
        mesh.halfedge(idHalfedgeEnd).set_prev_id(idPrev);

std::cout << "  set face " << idFace << ":";
        for (id_type id = idHalfedgeEnd; mesh.halfedge(id).face_id() != idFace; id = mesh.halfedge(id).next_id())
        {
std::cout << " " << id << mark(id);
            mesh.halfedge(id).set_face_id(idFace);
        }
std::cout << std::endl;
    }

    return {{idFaceA, idFaceB}};
}

//--------------------------------------------------------------------------
template<typename M>
void quetzal::brep::mark_internal(const typename M::face_type& faceA, const typename M::face_type& faceB)
{
    faceA.reset();
    for (auto& halfedge : faceA.halfedges())
    {
        bool bNotExterior = face_compare(faceB, halfedge.attributes().position()) <= 0;
        halfedge.vertex().set_marked(bNotExterior);
        halfedge.vertex().set_checked(bNotExterior);
    }

    faceB.reset();
    for (auto& halfedge : faceB.halfedges())
    {
        bool bNotExterior = face_compare(faceA, halfedge.attributes().position()) <= 0;
        halfedge.vertex().set_marked(bNotExterior);
        halfedge.vertex().set_checked(bNotExterior);
    }

    return;
}

//--------------------------------------------------------------------------
template<typename M>
quetzal::brep::EdgeIntersections<M> quetzal::brep::edge_intersections(const typename M::face_type& faceA, const typename M::face_type& faceB)
{
    // Efficiency could be improved here, sweep algorithm ...

    EdgeIntersections<M> intersections;
    for (const auto& halfedgeA : faceA.halfedges())
    {
        for (const auto& halfedgeB : faceB.halfedges())
        {
            auto inter = geometry::intersection(to_segment(halfedgeA), to_segment(halfedgeB));
            if (inter.locus() == geometry::Locus::Empty)
            {
                continue;
            }
            else if (inter.locus() == geometry::Locus::Point)
            {
                // Segments are half open, exclude terminal points
                if (!vector_eq(inter.point(), halfedgeA.next().attributes().position())
                    && !vector_eq(inter.point(), halfedgeB.next().attributes().position()))
                {
                    std::array<typename M::point_type, 2> points = {inter.point(), typename M::point_type()};
                    intersections.emplace_back(faceA.mesh(), halfedgeA.id(), halfedgeB.id(), points, false);
                }
            }
            else if (inter.locus() == geometry::Locus::Segment)
            {
                std::array<typename M::point_type, 2> points = {inter.segment().endpoint(0), inter.segment().endpoint(1)};
                intersections.emplace_back(faceA.mesh(), halfedgeA.id(), halfedgeB.id(), points, true);

                // Segment intersections should be treated as external but on perimeter

                halfedgeA.vertex().set_marked(false);
                halfedgeA.vertex().set_checked(true);

                halfedgeB.vertex().set_marked(false);
                halfedgeB.vertex().set_checked(true);
            }
        }
    }

    return intersections;
}

//--------------------------------------------------------------------------
template<typename M>
quetzal::id_type quetzal::brep::split_intersecting(M& mesh, id_type idHalfedge, id_type idHalfedgeOther, const typename M::vector_type& position)
{
    auto marked = [&mesh](id_type idHalfedge) -> bool { return mesh.halfedge(idHalfedge).vertex().marked(); };
    auto mark = [&](id_type idHalfedge) -> std::string { return marked(idHalfedge) ? "*" : ""; };

    bool bMarked = true;
std::cout << "split_intersecting [" << position << "]" << std::endl;
std::cout << "    idHalfedge " << idHalfedge << mark(idHalfedge) << " [" << mesh.halfedge(idHalfedge).attributes().position() << "]" << std::endl;
std::cout << "    idHalfedgeOther " << idHalfedgeOther << mark(idHalfedgeOther) << " [" << mesh.halfedge(idHalfedgeOther).attributes().position() << "]" << std::endl;
    if (vector_eq(position, mesh.halfedge(idHalfedge).attributes().position()))
    {
        bMarked = mesh.halfedge(idHalfedge).next().vertex().checked();
    }
    else
    {
        split_edge(mesh, idHalfedge, position);
        idHalfedge = mesh.halfedge(idHalfedge).next_id();

        if (!marked(mesh.halfedge(idHalfedge).next_id()))
        {
            bMarked = inward(mesh, idHalfedge, idHalfedgeOther);
        }
    }

std::cout << "    idHalfedge " << idHalfedge << mark(idHalfedge) << std::endl;
    mesh.halfedge(idHalfedge).vertex().set_marked(bMarked);
    mesh.halfedge(idHalfedge).vertex().set_checked(true);
    return idHalfedge;
}

//--------------------------------------------------------------------------
template<typename M>
void quetzal::brep::update_intersections(M& mesh, EdgeIntersections<M>& intersections, id_type idHalfedgeOrig, id_type idHalfedge)
{
    auto marked = [&mesh](id_type idHalfedge) -> bool { return mesh.halfedge(idHalfedge).vertex().marked(); };
    auto mark = [&](id_type idHalfedge) -> std::string { return marked(idHalfedge) ? "*" : ""; };

std::cout << "update_intersections " << idHalfedgeOrig << mark(idHalfedgeOrig) << " to " << idHalfedge << mark(idHalfedge) << std::endl;
    for (auto& e : intersections)
    {
std::cout << "  " << e << std::endl;
        if (e.idHalfedgeA == idHalfedgeOrig)
        {
            if (intersects(to_segment(mesh.halfedge(idHalfedge)), to_segment(mesh.halfedge(e.idHalfedgeB))))
            {
                e.idHalfedgeA = idHalfedge;
            }
        }

        if (e.idHalfedgeB == idHalfedgeOrig)
        {
            if (intersects(to_segment(mesh.halfedge(idHalfedge)), to_segment(mesh.halfedge(e.idHalfedgeA))))
            {
                e.idHalfedgeB = idHalfedge;
            }
        }
    }

    return;
}

//--------------------------------------------------------------------------
template<typename M>
quetzal::brep::ledges_type quetzal::brep::find_ledges(const M& mesh, const std::map<id_type, id_type>& intersection_correlation)
{
    auto marked = [&mesh](id_type idHalfedge) -> bool { return mesh.halfedge(idHalfedge).vertex().marked(); };

    ledges_type ledges;

    for (const auto& ic : intersection_correlation)
    {
        if (!marked(ic.first))
        {
            id_type idHalfedge = ic.first;
            for (;;)
            {
                idHalfedge = mesh.halfedge(idHalfedge).next_id();
                if (intersection_correlation.contains(idHalfedge))
                {
                    assert(marked(idHalfedge));
                    ledges.emplace_back(ic.first, idHalfedge);
                    break;
                }
            }
        }
    }

    return ledges;
}

//--------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::inward(const M& mesh, id_type idHalfedgeA, id_type idHalfedgeB)
{
    const auto& halfedgeA = mesh.halfedge(idHalfedgeA);
    const auto& halfedgeB = mesh.halfedge(idHalfedgeB);
    auto va = to_vector(halfedgeA);
    auto vb = to_vector(halfedgeB);
    auto normal = halfedgeA.face().attributes().normal();
    return dot(cross(va, vb), normal) > M::val(0);
}

#endif // QUETZAL_BREP_MESH_CONNECTION2_HPP
