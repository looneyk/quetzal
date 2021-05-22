#if !defined(QUETZAL_BREP_MESH_INVERSION_HPP)
#define QUETZAL_BREP_MESH_INVERSION_HPP
//------------------------------------------------------------------------------
// brep
// mesh_inversion.hpp
//------------------------------------------------------------------------------

#include "id.hpp"
#include <functional>

namespace quetzal
{

namespace brep
{

    // transform_texcoord_type ...
    // null/identity_transform_texcoord ...

    // Reverses orientation of each face using invert function of attribute type
    template<typename M>
    void invert(M& mesh, std::function<typename M::texcoord_type(const typename M::texcoord_type&)> transform_texcoord = [](const typename M::texcoord_type& texcoord) -> typename M::texcoord_type {return {M::val(1) - texcoord.x(), texcoord.y()};});

    // Reverses orientation of each face using invert function of attribute type
    // Reassigns vertices leaving partners unchanged
    // bReset parameter is used to trigger reset on first, non-recursive call; not a user parameter
    template<typename M>
    void invert_border_section(M& mesh, id_type idHalfedge, std::function<typename M::texcoord_type(const typename M::texcoord_type&)> transform_texcoord = [](const typename M::texcoord_type& texcoord) -> typename M::texcoord_type {return {M::val(1) - texcoord.x(), texcoord.y()};}, bool bReset = true);

    // Reverses orientation of each face using invert function of attribute type
    template<typename M>
    void invert_submesh(M& mesh, id_type idSubmesh, std::function<typename M::texcoord_type(const typename M::texcoord_type&)> transform_texcoord = [](const typename M::texcoord_type& texcoord) -> typename M::texcoord_type {return {M::val(1) - texcoord.x(), texcoord.y()};});

    // Reverses orientation of each face using invert function of attribute type
    template<typename M>
    void invert_surface(M& mesh, id_type idSurface, std::function<typename M::texcoord_type(const typename M::texcoord_type&)> transform_texcoord = [](const typename M::texcoord_type& texcoord) -> typename M::texcoord_type {return {M::val(1) - texcoord.x(), texcoord.y()};});

    // Reverses orientation using invert function of attribute type
    template<typename M>
    void invert_face(M& mesh, id_type idFace, std::function<typename M::texcoord_type(const typename M::texcoord_type&)> transform_texcoord = [](const typename M::texcoord_type& texcoord) -> typename M::texcoord_type {return {M::val(1) - texcoord.x(), texcoord.y()};});

    template<typename M>
    void reverse_winding_order(M& mesh, id_type idFace);

} // namespace brep

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::invert(M& mesh, std::function<typename M::texcoord_type(const typename M::texcoord_type&)> transform_texcoord)
{
    for (auto& surface : mesh.surfaces())
    {
        invert_surface(mesh, surface.id(), transform_texcoord);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::invert_border_section(M& mesh, id_type idHalfedge, std::function<typename M::texcoord_type(const typename M::texcoord_type&)> transform_texcoord, bool bReset)
{
    if (bReset)
    {
        mesh.reset();
    }

    id_type idFace = mesh.halfedge(idHalfedge).face_id();
    invert_face(mesh, idFace, transform_texcoord);
    mesh.face(idFace).set_marked();

    for (const auto& halfedge : mesh.face(idFace).halfedges())
    {
        if (!halfedge.border())
        {
            if (!halfedge.partner().face().marked())
            {
                invert_border_section(mesh, halfedge.partner_id(), transform_texcoord, false);
            }
        }
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::invert_submesh(M& mesh, id_type idSubmesh, std::function<typename M::texcoord_type(const typename M::texcoord_type&)> transform_texcoord)
{
    auto& submesh = mesh.submesh(idSubmesh);

    for (id_type idSurface : submesh.surface_ids())
    {
        invert_surface(mesh, idSurface, transform_texcoord);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::invert_surface(M& mesh, id_type idSurface, std::function<typename M::texcoord_type(const typename M::texcoord_type&)> transform_texcoord)
{
    auto& surface = mesh.surface(idSurface);

    for (id_type idFace : surface.face_ids())
    {
        invert_face(mesh, idFace, transform_texcoord);
    }

    surface.attributes().invert();
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::invert_face(M& mesh, id_type idFace, std::function<typename M::texcoord_type(const typename M::texcoord_type&)> transform_texcoord)
{
    auto& face = mesh.face(idFace);
    assert(!face.deleted());

    for (auto& halfedge : face.halfedges())
    {
        halfedge.attributes().invert(transform_texcoord);
    }

    face.attributes().invert();
    reverse_winding_order(mesh, idFace);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::reverse_winding_order(M& mesh, id_type idFace)
{
    auto& face = mesh.face(idFace);
    assert(!face.deleted());
    assert(face.halfedge_count() >= 3);

    const id_type idHalfedge0 = face.halfedge_id();
    const id_type idVertex0 = face.halfedge().vertex_id();

    id_type idHalfedgeNext = idHalfedge0;
    do
    {
        auto& halfedge = mesh.halfedge(idHalfedgeNext);
        idHalfedgeNext = halfedge.next_id();

        id_type idVertex = idHalfedgeNext == idHalfedge0 ? idVertex0 : halfedge.next().vertex_id();
        mesh.vertex(idVertex).set_halfedge_id(halfedge.id());

        halfedge.set_vertex_id(idVertex);
        halfedge.set_next_id(halfedge.prev_id());
        halfedge.set_prev_id(idHalfedgeNext);
    }
    while (idHalfedgeNext != idHalfedge0);

    return;
}

#endif // QUETZAL_BREP_MESH_INVERSION_HPP
