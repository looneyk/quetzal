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
#include "quetzal/geometry/HalfSpace.hpp"
#include "quetzal/geometry/Line.hpp"
#include "quetzal/geometry/Polygon.hpp"
#include "quetzal/geometry/intersect.hpp"
#include <string>
#include <queue>

#include "validation.hpp"
#include <iostream>

namespace quetzal::brep
{

    template<typename M>
    void split(M& mesh, const geometry::HalfSpace<typename M::vector_traits>& halfspace, const std::string& nameSurface = "split");

    // Set all connected surfaces and faces to a new submesh
    template<typename M>
    void set_submesh_connected(M& mesh, id_type idFace, id_type idSubmesh);

} // namespace quetzal::brep

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::split(M& mesh, const geometry::HalfSpace<typename M::vector_traits>& halfspace, const std::string& nameSurface)
{
    mesh.reset();

    // Mark exterior vertices
    // Vertices in halfspace plane are considered interior

    for (auto& vertex : mesh.vertices())
    {
        if (vertex.checked())
        {
            continue;
        }

        bool bExterior = halfspace.exterior(vertex.attributes().position());

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

        // Don't split if non-exterior point already intersects halfspace plane
        if (!halfspace.boundary(mesh.halfedge(idHalfedgeTransitionIn).next().attributes().position()))
        {
            geometry::Line<typename M::vector_traits> line = to_line(mesh.halfedge(idHalfedgeTransitionIn));
            geometry::Intersection<typename M::vector_traits> intersection = geometry::intersection(line, halfspace.plane());
            assert(intersection.locus() == geometry::Locus::Point);
            typename M::point_type position = intersection.point();
            split_edge(mesh, idHalfedgeTransitionIn, position);
        }
        else if (mesh.halfedge(idHalfedgeTransitionIn).next().id() == idHalfedgeTransitionOut)
        {
            // The only halfspace plane intersection is this vertex, so this is actually an exterior face, mark it as such
            mesh.halfedge(idHalfedgeTransitionOut).vertex().set_marked();
            face.set_marked();
            continue;
        }
        else if (halfspace.boundary(mesh.halfedge(idHalfedgeTransitionOut).attributes().position()))
        {
            // Both inward and outward transitioning halfedges intersect with the halfspace plane
            // If all vertices between idHalfedgeTransitionIn and idHalfedgeTransitionOut are in halfspace plane or exterior, then this is an exterior face

            bool bExterior = true;
            for (id_type idHalfedge = mesh.halfedge(idHalfedgeTransitionIn).next().id(); idHalfedge != idHalfedgeTransitionOut; idHalfedge = mesh.halfedge(idHalfedge).next_id())
            {
                bool bIncident = halfspace.boundary(mesh.halfedge(idHalfedge).attributes().position());
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

        idHalfedgeTransitionIn = mesh.halfedge(idHalfedgeTransitionIn).next_id(); // Adjust so vertex is on halfspace plane
        assert(halfspace.boundary(mesh.halfedge(idHalfedgeTransitionIn).attributes().position()));

        assert(!mesh.halfedge(idHalfedgeTransitionIn).vertex().marked());
        assert(!mesh.halfedge(idHalfedgeTransitionOut).vertex().marked());

        if (!halfspace.boundary(mesh.halfedge(idHalfedgeTransitionOut).attributes().position()))
        {
            geometry::Line<typename M::vector_traits> line = to_line(mesh.halfedge(idHalfedgeTransitionOut));
            geometry::Intersection<typename M::vector_traits> intersection = geometry::intersection(line, halfspace.plane());
            assert(intersection.locus() == geometry::Locus::Point);
            typename M::point_type position = intersection.point();
            split_edge(mesh, idHalfedgeTransitionOut, position);

            idHalfedgeTransitionOut = mesh.halfedge(idHalfedgeTransitionOut).next_id(); // Adjust so vertex is on halfspace plane
        }

        mesh.halfedge(idHalfedgeTransitionOut).vertex().set_marked();

        assert(halfspace.boundary(mesh.halfedge(idHalfedgeTransitionOut).attributes().position()));

        // Split face into interior and exterior faces
        split_face(mesh, idHalfedgeTransitionIn, idHalfedgeTransitionOut);
        mesh.halfedge(idHalfedgeTransitionOut).prev().vertex().set_marked();
        mesh.halfedge(idHalfedgeTransitionOut).face().set_marked();
    }

    // Delete all vertices, edges, and faces that are exterior to the halfspace plane, store remaining border halfedges

    // borderHalfedges[idHalfedge] = idSubmesh
    BorderHalfedges borderHalfedges0;
    BorderHalfedges borderHalfedges1;

    for (auto& face : mesh.faces())
    {
        if (face.marked())
        {
            for (auto& halfedge : face.halfedges())
            {
                assert(halfedge.vertex().marked());
            }

            continue;
        }

        for (auto& halfedge : face.halfedges())
        {
            assert(!halfedge.vertex().marked());
            assert(!halfedge.border());

            if (halfedge.partner().vertex().marked())
            {
                borderHalfedges0.insert({halfedge.id(), face.submesh_id()});
                borderHalfedges1.insert({halfedge.partner_id(), face.submesh_id()});
                halfedge.partner().set_border();
                halfedge.set_border();
            }
        }
    }

    assert(borderHalfedges0.size() == borderHalfedges1.size());
    if (borderHalfedges0.empty()) // No intersection
    {
        return;
    }

    // Close off borders and assign each to a new submesh
    // This will still potentially create disconnected submeshes with one section per surface, may need better naming ...

    auto idsSurface = close_border(mesh, borderHalfedges0, nameSurface, halfspace.plane().normal());
    for (id_type idSurface : idsSurface)
    {
        id_type idSubmesh = mesh.get_submesh_id(mesh.surface(idSurface).submesh().name() + "_0");
        set_submesh_connected(mesh, mesh.surface(idSurface).faces().front().id(), idSubmesh);
    }

    idsSurface = close_border(mesh, borderHalfedges1, nameSurface, -halfspace.plane().normal());
    for (id_type idSurface : idsSurface)
    {
        id_type idSubmesh = mesh.get_submesh_id(mesh.surface(idSurface).submesh().name() + "_1");
        set_submesh_connected(mesh, mesh.surface(idSurface).faces().front().id(), idSubmesh);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::set_submesh_connected(M& mesh, id_type idFace, id_type idSubmesh)
{
    mesh.reset();

    std::queue<id_type> idsFace({idFace});
    while (!idsFace.empty())
    {
        auto& f = mesh.face(idsFace.front());
        idsFace.pop();

        if (f.marked())
        {
            continue;
        }

        if (f.submesh_id() != idSubmesh)
        {
            std::string nameSurface = f.surface().name();

            id_type idSurface = mesh.get_surface_id(idSubmesh, nameSurface, f.surface().attributes());
            mesh.move_face(f.id(), idSurface);
        }

        f.set_marked();
        
        for (auto& halfedge : f.halfedges())
        {
            if (!halfedge.partner().face().marked())
            {
                idsFace.push(halfedge.partner().face_id());
            }
        }
    }

    return;
}

#endif // QUETZAL_BREP_MESH_SPLIT_HPP
