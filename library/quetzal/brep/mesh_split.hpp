#if !defined(QUETZAL_BREP_MESH_SPLIT_HPP)
#define QUETZAL_BREP_MESH_SPLIT_HPP
//------------------------------------------------------------------------------
// brep
// mesh_split.hpp
//------------------------------------------------------------------------------

#include "mesh_geometry.hpp"
#include "mesh_texcoord.hpp"
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

    template<typename M>
    void split(M& mesh, const geometry::Plane<typename M::vector_traits>& plane, const std::string& nameSubmesh0, const std::string& nameSubmesh1, const std::string& nameSurface = "split");

    // internal

    using BorderHalfedges = std::map<id_type, id_type>;
    using BorderHalfedge = BorderHalfedges::value_type;

    // In addition to creating the split face, this creates or renames the submesh
    template<typename M>
    void close_split(M& mesh, BorderHalfedges borderHalfedges, const typename M::vector_type& normal, const std::string& nameSurface, const std::string& nameSubmesh);

    // Set all connected surfaces and faces to the new submesh
    // Calls set_submesh_connected_face recursively
    template<typename F>
    void set_submesh_connected(F& face, id_type idSubmesh);

    template<typename F>
    void set_submesh_connected_face(F& face, id_type idSubmesh);

} // namespace quetzal::brep

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::split(M& mesh, const geometry::Plane<typename M::vector_traits>& plane, const std::string& nameSubmesh0, const std::string& nameSubmesh1, const std::string& nameSurface)
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

    // borderHalfedges[idHalfedge] = idSubmesh
    BorderHalfedges borderHalfedges0;
    BorderHalfedges borderHalfedges1;

    for (auto& face : mesh.faces())
    {
        bool bExterior = face.halfedge().vertex().marked();

        for (auto& halfedge : face.halfedges())
        {
            assert(bExterior && halfedge.vertex().marked() || !bExterior && !halfedge.vertex().marked());

            if (halfedge.vertex().marked())
            {
                if (!halfedge.border() && !halfedge.partner().vertex().marked())
                {
                    borderHalfedges0.insert({halfedge.partner_id(), face.submesh_id()});
                    borderHalfedges1.insert({halfedge.id(), face.submesh_id()});
                    halfedge.partner().set_border();
                    halfedge.set_border();
                }
            }
        }
    }

    if (borderHalfedges0.empty()) // No intersection
    {
        return;
    }

    close_split(mesh, borderHalfedges0, plane.normal(), nameSurface, nameSubmesh0);
    close_split(mesh, borderHalfedges1, -plane.normal(), nameSurface, nameSubmesh1);

    mesh.pack();
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::close_split(M& mesh, BorderHalfedges borderHalfedges, const typename M::vector_type& normal, const std::string& nameSurface, const std::string& nameSubmesh)
{
std::cout << "close_split\t" << borderHalfedges.size() << " borderHalfedges\t[" << normal << "]" << std::endl;
    // For each border polygon, store the initial halfedge in the appropriate set

    std::vector<BorderHalfedge> borderHalfedgesExternal;
    std::vector<BorderHalfedge> borderHalfedgesInternal;

    while (!borderHalfedges.empty())
    {
        BorderHalfedge borderHalfedge = *borderHalfedges.begin();
        id_type idHalfedge = borderHalfedge.first;
        assert(!mesh.halfedge(idHalfedge).deleted());

        id_type id = idHalfedge;
        do
        {
            borderHalfedges.erase(id);
            id = next_border_halfedge(mesh, id);
        } while (id != idHalfedge);

        if (border_external(mesh, idHalfedge, normal))
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

    std::map<id_type, BorderFace> borderFaces; // probably could be unordered_map ...

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
            borderFace.m_dr.init(normal);
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

    // Create faces and holes for this split face and reassign submesh

//    std::map<std::string, size_t> submeshInstances;

    for (const auto& value : borderFaces)
    {
        const BorderFace& borderFace = value.second;
        id_type idSubmeshOrig = borderFace.m_idSubmesh;

        id_type idSurface = nullid;
        if (mesh.contains_surface(idSubmeshOrig, nameSurface))
        {
            idSurface = mesh.surface_id(idSubmeshOrig, nameSurface);
        }
        else
        {
            idSurface = mesh.create_surface(idSubmeshOrig, nameSurface, {normal});
        }

        create_border_with_holes_face(mesh, borderFace.m_idHalfedgeBorderExternal, borderFace.m_idHalfedgesBorderInternal, normal, idSurface);

        if constexpr (M::vertex_attributes_type::contains(geometry::AttributesFlags::Texcoord0))
        {
            calculate_surface_texcoords(mesh, idSurface);
        }
/*
        // Create a separate submesh for this side of the split
        // handle the possibility that submesh is not singly connected (instances) ...

        auto& surface = mesh.surface(idSurface);

        id_type idSubmesh = nullid;
        if (mesh.contains_submesh(nameSubmesh))
        {
            idSubmesh = mesh.submesh_id(nameSubmesh);
        }
        else
        {
            idSubmesh = mesh.create_submesh(nameSubmesh);
        }

        // Move all connected surfaces and faces to the new submesh
        set_submesh_connected(surface.faces().front(), idSubmesh);
*/
    }

    return;
}

//------------------------------------------------------------------------------
template<typename F>
void quetzal::brep::set_submesh_connected(F& face, id_type idSubmesh)
{
    face.mesh().reset();
    set_submesh_connected_face(face, idSubmesh);
    return;
}

//------------------------------------------------------------------------------
template<typename F>
void quetzal::brep::set_submesh_connected_face(F& face, id_type idSubmesh)
{
    auto& mesh = face.mesh();
    std::string nameSurface = face.surface().name();

    id_type idSurface = nullid;
    if (mesh.contains_surface(idSubmesh, nameSurface))
    {
        idSurface = mesh.surface_id(idSubmesh, nameSurface);
    }
    else
    {
        idSurface = mesh.create_surface(idSubmesh, nameSurface, face.surface().attributes());
    }

    id_type idFace = face.id();

    auto& surfaceOrig = face.surface();
    surfaceOrig.unlink_face(idFace);
    if (surfaceOrig.empty())
    {
        mesh.remove_surface(surfaceOrig.id());
    }

    mesh.surface(idSurface).link_face(idFace);

    auto& submeshOrig = face.submesh();
    submeshOrig.unlink_face(idFace);
    if (submeshOrig.empty())
    {
        mesh.remove_submesh(submeshOrig.id());
    }

    mesh.submesh(idSubmesh).link_face(idFace);

    face.set_marked();

    for (auto& halfedge : face.halfedges())
    {
        if (!halfedge.face().marked())
        {
            set_submesh_connected_face(halfedge.face(), idSubmesh);
        }
    }

    return;
}

#endif // QUETZAL_BREP_MESH_SPLIT_HPP
