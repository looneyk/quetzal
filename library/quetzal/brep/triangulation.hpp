#if !defined(QUETZAL_BREP_TRIANGULATION_HPP)
#define QUETZAL_BREP_TRIANGULATION_HPP
//------------------------------------------------------------------------------
// brep
// triangulation.hpp
//------------------------------------------------------------------------------

#include "id.hpp"
#include "mesh_geometry.hpp"
#include "mesh_util.hpp"
#include "quetzal/math/DimensionReducer.hpp"
#include "quetzal/triangulation/triangulation.hpp"

#include <iostream> // ...

namespace quetzal::brep
{

    // Uses only existing vertex positions
    template<typename M>
    void triangulate(M& mesh);

    template<typename M>
    void triangulate_face_quad(M& mesh, id_type idFace);

    // Uses only existing vertex positions
    template<typename M>
    void triangulate_face_ear(M& mesh, id_type idFace);

    // Uses only existing vertex positions
    template<typename M>
    void triangulate_face_cdt(M& mesh, id_type idFace);

    // Delaunay triangulation of all faces in a surface
    template<typename M>
    void triangulate_surface(M& mesh, id_type idSSurface);

    // Uses only existing vertex positions
    // Simplification that only works when it is known that the polygon is convex
    template<typename M>
    void triangulate_convex(M& mesh);

    // Uses only existing vertex positions
    // Simplification that only works when it is known that all polygons in the mesh are convex
    template<typename M>
    void triangulate_face_convex(M& mesh, id_type idFace);

    // Adds a central vertex and a face for each edge
    // Not guaranteed if the face is not convex
    template<typename M>
    void triangulate_central_vertex(M& mesh, bool bSurfacesDistinct = false);

    // Adds a central vertex and a face for each edge
    // Not guaranteed if the face is not convex
    template<typename M>
    void triangulate_face_central_vertex(M& mesh, id_type idFace, bool bSurfacesDistinct = false);

    // Adds a new vertex at position and a face for each edge
    // Not guaranteed if the face is not convex
    template<typename M>
    void triangulate_face_central_vertex(M& mesh, id_type idFace, const typename M::point_type& position, bool bSurfacesDistinct = false);

} // namespace quetzal::brep

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::triangulate(M& mesh)
{
    size_t nFacesOrig = mesh.face_store_count();
    for (size_t i = 0; i < nFacesOrig; ++i)
    {
        const typename M::face_type& face = mesh.face(i);

        if (face.deleted() || (face.halfedge_count() == 3 && face.hole_count() == 0))
        {
            continue;
        }

        if (face.halfedge_count() == 4 && face.hole_count() == 0)
        {
            triangulate_face_quad(mesh, i);
            continue;
        }

        // check for other simplifications for speed ...

        triangulate_face_cdt(mesh, i);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::triangulate_face_quad(M& mesh, id_type idFace)
{
    const auto& face = mesh.face(idFace);
    assert(!face.deleted());
    assert(face.halfedge_count() == 4);
    assert(face.hole_count() == 0);

    id_type idHalfedges[4];
    typename M::point_type positions[4];
    size_t i = 0;
    for (const auto& halfedge : face.halfedges())
    {
        idHalfedges[i] = halfedge.id();
        positions[i] = halfedge.attributes().position();
        ++i;
    }
	assert(i == 4);

    typename M::value_type d02 = norm_squared(positions[0] - positions[2]);
    typename M::value_type d13 = norm_squared(positions[1] - positions[3]);

	// allow for choice, delaunay-like by default ...
    if (d02 < d13)
    {
        split_face(mesh, idHalfedges[0], idHalfedges[2]);
    }
    else
    {
        split_face(mesh, idHalfedges[1], idHalfedges[3]);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::triangulate_face_ear(M& mesh, id_type idFace)
{
    const auto& face = mesh.face(idFace);
    assert(!face.deleted());
    assert(face.halfedge_count() > 3);
    assert(face.hole_count() == 0);

// mark reflex vertices ...
    typename M::vector_type normal = face.attributes().normal();
    size_t nEdges = face.halfedge_count();
    size_t n = 0; // Counts number of failed attempts to find ear vertex

    id_type idHalfedge = face.halfedge_id();
    while (mesh.halfedge(idHalfedge).face().vertex_count() > 3)
    {
        id_type idHalfedgeNext = mesh.halfedge(idHalfedge).next_id();
        // a bit better by advancing to next vertex; this should be done away with and instead use better triangulation algorithm ...
        if (convex(mesh.halfedge(idHalfedgeNext).face()))
        {
            idHalfedgeNext = mesh.halfedge(idHalfedgeNext).next_id();
        }

        if (ear(mesh.halfedge(idHalfedge), mesh))
        {
            split_face(mesh, mesh.halfedge(idHalfedge).prev_id(), mesh.halfedge(idHalfedge).next_id());
            // update status of prev and next vertices ...
            n = 0;
        }

        idHalfedge = idHalfedgeNext;
        assert(++n < nEdges);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::triangulate_face_cdt(M& mesh, id_type idFace)
{
    const auto& face = mesh.face(idFace);
    assert(!face.deleted());

    typename M::face_attributes_type af = face.attributes();
    typename M::vector_type normal = af.normal();
    id_type idSubmesh = face.submesh_id();
    id_type idSurface = face.surface_id();

	math::DimensionReducer<typename M::vector_traits> dr(normal);

    std::vector<p2t::Point> polygon;
    for (const auto& halfedge : face.halfedges())
    {
        const auto position = dr.reduce(halfedge.attributes().position());
        polygon.emplace_back(halfedge.id(), position.x(), position.y());
    }

    p2t::CDT cdt(polygon);

    for (const auto& hole : face.holes())
    {
        std::vector<p2t::Point> p2tHole;
        for (const auto& halfedge : hole.halfedges())
        {
            const auto position = dr.reduce(halfedge.attributes().position());
            p2tHole.emplace_back(halfedge.id(), position.x(), position.y());
        }

        cdt.AddHole(p2tHole);
    }

    cdt.Triangulate();

    std::vector<p2t::Triangle> triangles = cdt.GetTriangles();
    assert(!triangles.empty());

    // Delete original face leaving its halfedges, vertices, and surface
    if (idSubmesh != nullid)
    {
        mesh.submesh(idSubmesh).remove_face(idFace);
    }
    if (idSurface != nullid)
    {
        mesh.surface(idSurface).remove_face(idFace);
    }
    mesh.face(idFace).set_deleted();

    id_type nhOrig = mesh.halfedge_store_count();
    id_type nh = nhOrig;

    // Store first halfedge of each new pair for partner lookup
    std::map<std::array<id_type, 2>, id_type> edges; // ...

    for (size_t i = 0; i < triangles.size(); ++i)
    {
        p2t::Triangle& triangle = triangles[i];

        id_type idHalfedgeOrig[3];
        id_type idHalfedgeFace[3];

        for (size_t j = 0; j < 3; ++j)
        {
            size_t jNext = (j + 1) % 3;

            id_type idHalfedge = triangle.GetPoint(j)->m_id;
            id_type idHalfedgeNext = triangle.GetPoint(jNext)->m_id;

            idHalfedgeOrig[j] = idHalfedge;

            if (idHalfedgeNext == mesh.halfedge(idHalfedge).next_id()) // Original edge
            {
                idHalfedgeFace[j] = idHalfedge;
            }
            else
            {
                idHalfedgeFace[j] = nh++;
            }
        }

        id_type idFaceTriangle = mesh.create_face(idSurface, idHalfedgeFace[0], af);

        for (size_t j = 0; j < 3; ++j)
        {
            size_t jNext = (j + 1) % 3;
            size_t jPrev = (j + 2) % 3;

            id_type idHalfedge = idHalfedgeFace[j];
            id_type idHalfedgeNext = idHalfedgeFace[jNext];
            id_type idHalfedgePrev = idHalfedgeFace[jPrev];

            if (idHalfedge < nhOrig) // Reuse original halfedge and vertex
            {
                auto& halfedge = mesh.halfedge(idHalfedge);
                halfedge.set_next_id(idHalfedgeNext);
                halfedge.set_prev_id(idHalfedgePrev);
                halfedge.set_face_id(idFaceTriangle);
            }
            else
            {
                auto iEdge = edges.find({idHalfedgeOrig[jNext], idHalfedgeOrig[j]});
                id_type idPartner = iEdge != edges.end() ? iEdge->second : nullid;
                if (idPartner != nullid)
                {
                    mesh.halfedge(idPartner).set_partner_id(idHalfedge);
                    edges.erase({idHalfedgeOrig[jNext], idHalfedgeOrig[j]});
                }
                else
                {
                    edges[{idHalfedgeOrig[j], idHalfedgeOrig[jNext]}] = idHalfedge;
                }

                assert(idHalfedge == mesh.halfedge_store_count());

                mesh.create_halfedge_vertex(idPartner, idHalfedgeNext, idHalfedgePrev, idFaceTriangle, mesh.halfedge(idHalfedgeOrig[j]).attributes());
            }
        }
    }

    assert(edges.empty());
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::triangulate_surface(M& mesh, id_type idSSurface)
{
    // triangulate_surface ...
    // add surface border as constrained edges

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::triangulate_convex(M& mesh)
{
    size_t nFacesOrig = mesh.face_store_count();
    for (size_t i = 0; i < nFacesOrig; ++i)
    {
        if (mesh.face(i).deleted() || mesh.face(i).vertex_count() == 3)
        {
            continue;
        }

        triangulate_face_convex(mesh, i);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::triangulate_face_convex(M& mesh, id_type idFace)
{
    const auto& face = mesh.face(idFace);
    assert(!face.deleted());
    assert(face.halfedge_count() > 3);
    assert(face.hole_count() == 0);

    id_type idHalfedge = face.halfedge_id();
    while (mesh.halfedge(idHalfedge).face().vertex_count() > 3)
    {
//        id_type idHalfedgeNext = mesh.halfedge(idHalfedge).next_id();
        // Advancing twice prevents cases where all triangulated faces share the same vertex
        id_type idHalfedgeNext = mesh.halfedge(idHalfedge).next().next_id();

        split_face(mesh, mesh.halfedge(idHalfedge).prev_id(), mesh.halfedge(idHalfedge).next_id());

        idHalfedge = idHalfedgeNext;
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::triangulate_central_vertex(M& mesh, bool bSurfacesDistinct)
{
    size_t nEdgesMin = 4; // 3 to allow the addition of a center point to triangular faces, parameter for simply triangulation or face generation when 3? ...
    id_type nFacesOrig = mesh.face_store_count();
    for (id_type id = 0; id < nFacesOrig; ++id)
    {
        if (!mesh.face(id).deleted() && mesh.face(id).halfedge_count() >= nEdgesMin)
        {
            triangulate_face_central_vertex(mesh, id, bSurfacesDistinct);
        }
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::triangulate_face_central_vertex(M& mesh, id_type idFace, bool bSurfacesDistinct)
{
    triangulate_face_central_vertex(mesh, idFace, centroid(mesh.face(idFace)), bSurfacesDistinct);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::triangulate_face_central_vertex(M& mesh, id_type idFace, const typename M::point_type& position, bool bSurfacesDistinct)
{
    const auto& face = mesh.face(idFace);
    assert(!face.deleted());
    assert(face.halfedge_count() >= 3);
    assert(face.hole_count() == 0);

    id_type idHalfedge = face.halfedge_id();
    id_type n = face.halfedge_count();
    bool bPlanar = face_contains(face, position);

    typename M::vector_type normal = face.attributes().normal();
    typename M::texcoord_type texcoord({M::val(0.5), M::val(0.0)});

    if (!bSurfacesDistinct)
    {
        // calculate proportional to position relative to face center (this assumes that position is centroid) ...
        texcoord.clear();

        for (const auto& halfedge : face.halfedges())
        {
            texcoord += halfedge.attributes().texcoord();
        }

        texcoord /= M::val(n);
    }

    typename M::vertex_attributes_type av(position, normal, texcoord);
    typename M::face_attributes_type af(normal);
    id_type idSurface = face.surface_id();

    for (id_type j = 0; j < n; ++j)
    {
        id_type nh = mesh.halfedge_store_count();

        id_type idHalfedgeNext = mesh.halfedge(idHalfedge).next_id();

        if (!bPlanar)
        {
            normal = normalize(cross(mesh.halfedge(idHalfedge).attributes().position() - position, mesh.halfedge(idHalfedge).next().attributes().position() - position));
        }

        typename M::vertex_attributes_type avBorder = mesh.halfedge(idHalfedge).next().attributes();
        avBorder.set_normal(normal);
        av.set_normal(normal);
        mesh.halfedge(idHalfedge).attributes().set_normal(normal);

        af.set_normal(normal);
        id_type idFace_ = mesh.create_face(idSurface, idHalfedge, af);
        id_type idHalfedge0 = mesh.create_halfedge_vertex((j == 0 ? nh - 1 + 2 * n : nh - 1), idHalfedge, nh + 1, idFace_, av);
        id_type idHalfedge1 = mesh.create_halfedge_vertex((j == n - 1 ? nh + 2 - 2 * n : nh + 2), nh, idHalfedge, idFace_, avBorder);

        mesh.halfedge(idHalfedge).set_next_id(idHalfedge1);
        mesh.halfedge(idHalfedge).set_prev_id(idHalfedge0);
        mesh.halfedge(idHalfedge).set_face_id(idFace_);

        if (bSurfacesDistinct)
        {
            mesh.halfedge(idHalfedge0).attributes().set_texcoord({0.5f, 0.0f});
            mesh.halfedge(idHalfedge1).attributes().set_texcoord({1.0, 1.0});
            mesh.halfedge(idHalfedge).attributes().set_texcoord({0.0, 1.0});
        }

        idHalfedge = idHalfedgeNext;
    }

    mesh.remove_face(idFace);
    return;
}

#endif // QUETZAL_BREP_TRIANGULATION_HPP
