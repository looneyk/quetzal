#if !defined(QUETZAL_BREP_MESH_CONNECTION_HPP)
#define QUETZAL_BREP_MESH_CONNECTION_HPP
//------------------------------------------------------------------------------
// brep
// mesh_connection.hpp
//------------------------------------------------------------------------------

#include "mesh_geometry.hpp"
#include "mesh_util.hpp"
#include "quetzal/common/string_util.hpp"
#include "quetzal/math/DimensionReducer.hpp"
#include "quetzal/geometry/Line.hpp"
#include "quetzal/geometry/Plane.hpp"
#include "quetzal/geometry/Polygon.hpp"
#include "quetzal/geometry/intersect.hpp"
#include <map>

namespace quetzal::brep
{

    // Connect bodies A and B at coincident faces associated with halfedges A and B respectively
    // Halfedge A and B will be deleted, but their partners will form a new pair
    // Remove the adjoining faces
    template<typename M>
    void weld(M& mesh, id_type idFaceA, id_type idFaceB);

    // Connect bodies A and B at coincident matching surfaces; remove the original surfaces and their faces
    template<typename M>
    void weld(M& mesh, const std::string& nameSurfaceA, const std::string& nameSurfaceB);

    // Connect bodies A and B at coplanar faces using vertices and halfedges of faces A and B
    // Face B must fit completely within face A
    // No new halfedges or vertices are added, only faces if there are any holes in B
    // Works by repurposing base B halfedges to form hole in face A and hole halfedges in base B to form new faces
    template<typename M>
    void attach(M& mesh, id_type idFaceA, id_type idFaceB, bool bSurfacesDistinct = false);

    template<typename M>
    void attach(M& mesh, const std::string& nameSurfaceA, const std::string& nameSurfaceB, bool bSurfacesDistinct = false);

    template<typename M>
    void clip(M& mesh, const geometry::Plane<typename M::vector_traits>& plane, const std::string& nameSurface = "clip");

    // take submesh name(s) and return pair of new submesh ids ...
    template<typename M>
    void split(M& mesh, const geometry::Plane<typename M::vector_traits>& plane, const std::string& nameSurface0 = "split0", const std::string& nameSurface1 = "split1");

} // namespace quetzal::brep

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::weld(M& mesh, id_type idFaceA, id_type idFaceB)
{
    auto [idHalfedgeA, idHalfedgeB] = find_partner_halfedges(mesh, idFaceA, idFaceB);
    assert(idHalfedgeA != nullid);
    assert(idHalfedgeB != nullid);
    assert(!mesh.halfedge(idHalfedgeA).deleted());
    assert(!mesh.halfedge(idHalfedgeB).deleted());

    auto& faceA = mesh.halfedge(idHalfedgeA).face();
    auto& faceB = mesh.halfedge(idHalfedgeB).face();
    assert(!faceA.deleted());
    assert(!faceB.deleted());
    assert(faceA.halfedge_count() == faceB.halfedge_count());

    for (auto& halfedgeA : faceA.halfedges())
    {
        auto& halfedgeB = mesh.halfedge(idHalfedgeB);

        assert(vector_eq(halfedgeA.attributes().position(), halfedgeB.next().attributes().position()));
        assert(vector_eq(halfedgeB.attributes().position(), halfedgeA.next().attributes().position()));

        // Fine even if A and B are partners
        halfedgeA.partner().set_partner_id(halfedgeB.partner_id());
        halfedgeB.partner().set_partner_id(halfedgeA.partner_id());

        halfedgeA.set_border();
        halfedgeB.set_border();

        idHalfedgeB = halfedgeB.prev_id();
    }

    mesh.delete_face(faceA.id());
    mesh.delete_face(faceB.id());
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::weld(M& mesh, const std::string& nameSurfaceA, const std::string& nameSurfaceB)
{
    auto& surfaceA = mesh.surface(nameSurfaceA);
    auto& surfaceB = mesh.surface(nameSurfaceB);
    assert(surfaceA.face_count() == surfaceB.face_count());

    auto sort_faces = [&mesh](const typename M::surface_type& surface) -> std::map<typename M::point_type, id_type>
    { 
        std::map<typename M::point_type, id_type> index;
        for (const auto& face : surface.faces())
        {
            index[centroid(face)] = face.id();
        }
        return index;
    };

    auto indexFacesA = sort_faces(surfaceA);
    auto indexFacesB = sort_faces(surfaceB);
    assert(indexFacesA.size() == indexFacesB.size());

    for (auto iA = indexFacesA.begin(), iB = indexFacesB.begin(); iA != indexFacesA.end(); ++iA, ++iB)
    {
        assert(vector_eq(iA->first, iB->first));
        weld(mesh, iA->second, iB->second);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::attach(M& mesh, id_type idFaceA, id_type idFaceB, bool bSurfacesDistinct)
{
    id_type idHalfedgeA = mesh.face(idFaceA).halfedge_id();
    id_type idHalfedgeB = mesh.face(idFaceB).halfedge_id();
    assert(!mesh.halfedge(idHalfedgeA).deleted());
    assert(!mesh.halfedge(idHalfedgeB).deleted());

    auto& faceA = mesh.face(idFaceA);
    auto& faceB = mesh.face(idFaceB);
    assert(!faceA.deleted());
    assert(!faceB.deleted());

    // Apply face A normal to transferred vertices
    auto normal = faceA.attributes().normal();

    // Fix face id's and normals
    for (auto& halfedge : faceB.halfedges())
    {
        halfedge.set_face_id(idFaceA);
        halfedge.attributes().set_normal(normal); // vertex ...
    }

    faceA.create_hole(idHalfedgeB);

    interpolate_face_texcoords(faceA, faceB);

    // submesh should be reassigned, but faces should retain their surface membership ...
    // retain existing submesh as well? ...
    id_type idSurface = faceA.surface_id();
    id_type idSubmesh = faceA.submesh_id();
    size_t i = 0;

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
            calculate_surface_texcoords(mesh, idSurfaceHole);
            ++i;
        }
        else
        {
            interpolate_face_texcoords(faceA, face);
        }
    }

    mesh.remove_face(idFaceB);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::attach(M& mesh, const std::string& nameSurfaceA, const std::string& nameSurfaceB, bool bSurfacesDistinct)
{
    auto& surfaceA = mesh.surface(nameSurfaceA);
    auto& surfaceB = mesh.surface(nameSurfaceB);

    // this only handles single face per surface case ...
    assert(surfaceA.face_count() == surfaceB.face_count());
    assert(surfaceA.face_count() == 1);

    attach(mesh, surfaceA.faces().front().id(), surfaceB.faces().front().id(), bSurfacesDistinct);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::clip(M& mesh, const geometry::Plane<typename M::vector_traits>& plane, const std::string& nameSurface)
{
    mesh.reset();

    // Mark exterior vertices
    // Vertices in clip plane are considered interior

    for (auto& vertex : mesh.vertices())
    {
        if (vertex.checked())
        {
            continue;
        }

        bool bExterior = plane.exterior(vertex.attributes().position());

        for (auto& halfedge : vertex.halfedges())
        {
            halfedge.vertex().set_marked(bExterior);
            halfedge.vertex().set_checked();
        }
    }

    // Split intersecting faces

    id_type nFacesOrig = mesh.face_store_count();
    for (size_t nFace = 0; nFace < nFacesOrig; ++nFace)
    {
        auto& face = mesh.face(nFace);
        if (face.deleted())
        {
            continue;
        }

        bool bAllInterior = true;
        bool bAllExterior = true;
        for (const auto& halfedge : face.halfedges())
        {
            if (halfedge.vertex().marked())
            {
                bAllInterior = false;
            }
            else
            {
                bAllExterior = false;
            }
        }

        if (bAllInterior)
        {
            continue;
        }
        if (bAllExterior)
        {
            face.set_marked();
            continue;
        }

        id_type idHalfedgeTransitionOut = nullid;
        id_type idHalfedgeTransitionIn = nullid;

        for (const auto& halfedge : face.halfedges())
        {
            bool bMarked0 = halfedge.vertex().marked();
            bool bMarked1 = halfedge.next().vertex().marked();

            if (bMarked0 && !bMarked1)
            {
                idHalfedgeTransitionIn = halfedge.id();
            }
            else if (!bMarked0 && bMarked1)
            {
                idHalfedgeTransitionOut = halfedge.id();
            }
        }

        assert(idHalfedgeTransitionOut != nullid);
        assert(idHalfedgeTransitionIn != nullid);

        // Don't split if non-exterior point already intersects plane
        if (!plane.contains(mesh.halfedge(idHalfedgeTransitionIn).next().attributes().position()))
        {
            geometry::Line<typename M::vector_traits> line = to_line(mesh.halfedge(idHalfedgeTransitionIn));
            geometry::Intersection<typename M::vector_traits> intersection = geometry::intersection(line, plane);
            assert(intersection.locus() == geometry::Locus::Point);
            typename M::point_type position = intersection.point();
            split_edge(mesh, idHalfedgeTransitionIn, position);
        }
        else if (mesh.halfedge(idHalfedgeTransitionIn).next().id() == idHalfedgeTransitionOut)
        {
            // The only plane intersection is this vertex, so this is actually an exterior face, mark it as such
            mesh.halfedge(idHalfedgeTransitionOut).vertex().set_marked();
            face.set_marked();
            continue;
        }
        else if (plane.contains(mesh.halfedge(idHalfedgeTransitionOut).attributes().position()))
        {
            // Both inward and outward transitioning halfedges intersect with the clip plane
            // If all vertices between idHalfedgeTransitionIn and idHalfedgeTransitionOut are in plane or exterior, then this is an exterior face

            bool bExterior = true;
            for (id_type idHalfedge = mesh.halfedge(idHalfedgeTransitionIn).next().id(); idHalfedge != idHalfedgeTransitionOut; idHalfedge = mesh.halfedge(idHalfedge).next_id())
            {
                bool bIncident = plane.contains(mesh.halfedge(idHalfedge).attributes().position());
                if (!mesh.halfedge(idHalfedge).marked() && !bIncident)
                {
                    bExterior = false;
                    break;
                }
            }

            if (bExterior)
            {
                for (id_type idHalfedge = mesh.halfedge(idHalfedgeTransitionIn).next().id(); idHalfedge != idHalfedgeTransitionOut; idHalfedge = mesh.halfedge(idHalfedge).next_id())
                {
                    mesh.halfedge(idHalfedge).vertex().set_marked();
                }

                mesh.halfedge(idHalfedgeTransitionOut).vertex().set_marked();
                face.set_marked();
                continue;
            }
        }

        idHalfedgeTransitionIn = mesh.halfedge(idHalfedgeTransitionIn).next_id(); // Adjust so vertex is on clip plane
        assert(plane.contains(mesh.halfedge(idHalfedgeTransitionIn).attributes().position()));

        assert(!mesh.halfedge(idHalfedgeTransitionIn).vertex().marked());
        assert(!mesh.halfedge(idHalfedgeTransitionOut).vertex().marked());

        if (!plane.contains(mesh.halfedge(idHalfedgeTransitionOut).attributes().position()))
        {
            geometry::Line<typename M::vector_traits> line = to_line(mesh.halfedge(idHalfedgeTransitionOut));
            geometry::Intersection<typename M::vector_traits> intersection = geometry::intersection(line, plane);
            assert(intersection.locus() == geometry::Locus::Point);
            typename M::point_type position = intersection.point();
//            typename M::point_type position = plane.intersection(line); // ...
            split_edge(mesh, idHalfedgeTransitionOut, position);

            idHalfedgeTransitionOut = mesh.halfedge(idHalfedgeTransitionOut).next_id(); // Adjust so vertex is on clip plane
        }

        mesh.halfedge(idHalfedgeTransitionOut).vertex().set_marked();

        assert(plane.contains(mesh.halfedge(idHalfedgeTransitionOut).attributes().position()));

        // Split face into interior and exterior faces
        split_face(mesh, idHalfedgeTransitionIn, idHalfedgeTransitionOut);
        mesh.halfedge(idHalfedgeTransitionOut).prev().vertex().set_marked();
        mesh.halfedge(idHalfedgeTransitionOut).face().set_marked();
    }

    // Delete all vertices, edges, and faces that are exterior to the clipping plane, store remaining border halfedges

    std::map<id_type, id_type> borderHalfedges; // borderHalfedges[idHalfedge] = idSubmesh
    using BorderHalfedge = std::map<id_type, id_type>::value_type;

    for (auto& face : mesh.faces())
    {
        bool bExterior = false;
        for (auto& halfedge : face.halfedges())
        {
            if (halfedge.vertex().marked() || halfedge.next().vertex().marked())
            {
                if (halfedge.partner_id() != nullid)
                {
                    halfedge.partner().set_border();
                    borderHalfedges.insert({halfedge.partner_id(), face.submesh_id()});
                }

                halfedge.vertex().set_deleted();
                halfedge.set_deleted();

                bExterior = true;
            }
        }

        if (bExterior)
        {
            mesh.remove_face(face.id());
        }
    }

    if (borderHalfedges.empty()) // No intersection
    {
        return;
    }

    // For each border polygon, store the initial halfedge in the appropriate set

    std::vector<BorderHalfedge> borderHalfedgesExternal;
    std::vector<BorderHalfedge> borderHalfedgesInternal;

    while (!borderHalfedges.empty())
    {
        BorderHalfedge borderHalfedge = *borderHalfedges.begin();
        id_type idHalfedge = borderHalfedge.first;
        if (mesh.halfedge(idHalfedge).deleted())
        {
            borderHalfedges.erase(idHalfedge);
            continue;
        }

        id_type id = idHalfedge;
        do
        {
            borderHalfedges.erase(id);
            id = next_border_halfedge(mesh, id);
        } while (id != idHalfedge);

        if (border_external(mesh, idHalfedge, plane.normal()))
        {
            borderHalfedgesExternal.push_back(borderHalfedge);
        }
        else
        {
            borderHalfedgesInternal.push_back(borderHalfedge);
        }
    }

    // Associate holes with faces

    struct BorderFace
    {
        math::DimensionReducer<typename M::vector_traits> m_dr;
        geometry::Polygon<typename M::vector_traits::reduced_traits> m_polygon;
        typename M::value_type m_area;
        id_type m_idSubmesh;
        id_type m_idHalfedgeBorderExternal;
        std::vector<id_type> m_idHalfedgesBorderInternal;

        bool contains(M& mesh, BorderHalfedge borderHalfedge) const
        {
            id_type idSubmesh = borderHalfedge.second;
            assert(idSubmesh != nullid);
            if (idSubmesh != m_idSubmesh)
            {
                return false;
            }

            // Single point check is sufficient
            auto position = m_dr.reduce(mesh.halfedge(borderHalfedge.first).attributes().position());
            return geometry::intersects(position, m_polygon);
        };
    };

    std::map<id_type, BorderFace> borderFaces; // probably could be unordered_set ...

    if (borderHalfedgesExternal.size() == 1)
    {
        BorderHalfedge borderHalfedgeExternal = borderHalfedgesExternal.front();
        id_type idHalfedgeFace = borderHalfedgeExternal.first;
        BorderFace& borderFace = borderFaces[idHalfedgeFace];
        borderFace.m_idSubmesh = borderHalfedgeExternal.second;
        borderFace.m_idHalfedgeBorderExternal = idHalfedgeFace;

        std::vector<id_type> idHalfedgesBorderInternal;
        for (const auto& borderHalfedgeInternal : borderHalfedgesInternal)
        {
            assert(borderHalfedgeInternal.second == borderHalfedgeExternal.second); // should be true ...
            idHalfedgesBorderInternal.push_back(borderHalfedgeInternal.first);
        }
        borderFaces[idHalfedgeFace].m_idHalfedgesBorderInternal = idHalfedgesBorderInternal;
    }
    else
    {
        for (BorderHalfedge borderHalfedge : borderHalfedgesExternal)
        {
            id_type idHalfedgeFace = borderHalfedge.first;
            BorderFace& borderFace = borderFaces[idHalfedgeFace];
            borderFace.m_idSubmesh = borderHalfedge.second;
            borderFace.m_idHalfedgeBorderExternal = idHalfedgeFace;
            borderFace.m_dr.init(plane.normal());
            borderFace.m_polygon = create_border_polygon(mesh, idHalfedgeFace, borderFace.m_dr);
            assert(borderFace.m_polygon.vertex_count() > 0);
            borderFace.m_area = borderFace.m_polygon.area();
            assert(borderFace.m_area > M::traits_type::val(0));
        }

        // Associate each hole with its enclosing face
        for (BorderHalfedge borderHalfedgeInternal : borderHalfedgesInternal)
        {
            id_type idHalfedgeHole  = borderHalfedgeInternal.first;
            id_type idHalfedgeFaceMin = nullid;
            typename M::value_type areaMin = M::traits_type::val(0);

            for (BorderHalfedge borderHalfedgeExternal : borderHalfedgesExternal)
            {
                id_type idHalfedgeFace  = borderHalfedgeExternal.first;
                const auto& borderFace = borderFaces[idHalfedgeFace];
                if (borderFace.contains(mesh, borderHalfedgeInternal))
                {
                    typename M::value_type area = borderFace.m_area;
                    if (idHalfedgeFaceMin == nullid || area < areaMin)
                    {
                        idHalfedgeFaceMin = idHalfedgeFace;
                        areaMin = area;
                    }
                }
            }

            assert(idHalfedgeFaceMin != nullid);
            borderFaces[idHalfedgeFaceMin].m_idHalfedgesBorderInternal.push_back(idHalfedgeHole);
        }
    }

    // Create faces and holes for this clip face

    for (const auto& value : borderFaces)
    {
        const BorderFace& borderFace = value.second;
        id_type idSubmesh = borderFace.m_idSubmesh;

        id_type idSurface = nullid;
        if (mesh.contains_surface(idSubmesh, nameSurface))
        {
            idSurface = mesh.surface_id(idSubmesh, nameSurface);
        }
        else
        {
            idSurface = mesh.create_surface(idSubmesh, nameSurface, {plane.normal()});
        }

        create_border_with_holes_face(mesh, borderFace.m_idHalfedgeBorderExternal, borderFace.m_idHalfedgesBorderInternal, plane.normal(), idSurface);
    }

    mesh.pack();
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::split(M& mesh, const geometry::Plane<typename M::vector_traits>& plane, const std::string& nameSurface0, const std::string& nameSurface1)
{
    assert(false); // ...

    return;
}

#endif // QUETZAL_BREP_MESH_CONNECTION_HPP
