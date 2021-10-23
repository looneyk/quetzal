#if !defined(QUETZAL_BREP_MESH_CONNECTION_HPP)
#define QUETZAL_BREP_MESH_CONNECTION_HPP
//------------------------------------------------------------------------------
// brep
// mesh_connection.hpp
//------------------------------------------------------------------------------

#include "mesh_geometry.hpp"
#include "mesh_texcoord.hpp"
#include "quetzal/common/string_util.hpp"
#include <map>

namespace quetzal::brep
{

    // Connect bodies A and B at coincident matching surfaces
    // Remove the original surfaces and their faces
    template<typename M>
    void weld(M& mesh, const std::string& nameSurfaceA, const std::string& nameSurfaceB);

    // Connect bodies A and B at coincident matching (partner) faces
    // Corresponding face halfedge partners will form new pairs
    // Remove the adjoining faces
    template<typename M>
    void weld(M& mesh, id_type idFaceA, id_type idFaceB);

    // Currently only supports surfaces that consist of a single face
    template<typename M>
    void attach(M& mesh, const std::string& nameSurfaceA, const std::string& nameSurfaceB, bool bSurfacesDistinct = false);

    // Connect bodies A and B at coplanar faces using vertices and halfedges of faces A and B
    // Face B must fit completely within face A
    // No new halfedges or vertices are added, only faces if there are any holes in B
    // Works by repurposing base B halfedges to form hole in face A and hole halfedges in base B to form new faces
    template<typename M>
    void attach(M& mesh, id_type idFaceA, id_type idFaceB, bool bSurfacesDistinct = false);

} // namespace quetzal::brep

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
    assert(faceA.id() == idFaceA);
    assert(faceB.id() == idFaceB);
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
void quetzal::brep::attach(M& mesh, const std::string& nameSurfaceA, const std::string& nameSurfaceB, bool bSurfacesDistinct)
{
    auto& surfaceA = mesh.surface(nameSurfaceA);
    auto& surfaceB = mesh.surface(nameSurfaceB);

    // so far this only handles the single face per surface case ...
    assert(surfaceA.face_count() == surfaceB.face_count());
    assert(surfaceA.face_count() == 1);

    attach(mesh, surfaceA.faces().front().id(), surfaceB.faces().front().id(), bSurfacesDistinct);
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
    typename M::face_attributes_type af = faceA.attributes();
    auto normal = af.normal();

    // Fix face id's and normals
    for (auto& halfedge : faceB.halfedges())
    {
        halfedge.set_face_id(idFaceA);
        halfedge.attributes().set_normal(normal); // vertex ...
    }

    faceA.create_hole(idHalfedgeB);

    if constexpr (M::vertex_attributes_type::contains(geometry::AttributesFlags::Texcoord0))
    {
        interpolate_face_texcoords(faceA, faceB);
    }

    // submesh should be reassigned, but faces should retain their surface membership ...
    // retain existing submesh as well? ...
    id_type idSurface = faceA.surface_id();
    id_type idSubmesh = faceA.submesh_id();
    size_t i = 0;

    for (const auto& hole : faceB.holes())
    {
        id_type idFace = mesh.create_face(idSurface, hole.halfedge_id(), af);
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

    // All of faceB's halfedges and vertices have been repurposed
    mesh.unlink_face(idFaceB);
    faceB.set_deleted();
    return;
}

#endif // QUETZAL_BREP_MESH_CONNECTION_HPP
