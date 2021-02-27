#if !defined(QUETZAL_BREP_SURFACE_HPP)
#define QUETZAL_BREP_SURFACE_HPP
//------------------------------------------------------------------------------
// brep
// Surface.hpp
//------------------------------------------------------------------------------

#include "Flags.hpp"
#include "id.hpp"
#include "mesh_util.hpp"
#include "quetzal/common/Elements.hpp"
#include "quetzal/common/Properties.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <cassert>

namespace quetzal::brep
{

    template<typename Traits>
    class Mesh;

    //--------------------------------------------------------------------------
    template<typename Traits, typename M = Mesh<Traits>>
    class Surface : public Flags
    {
    public:

        using traits_type = Traits;
        using mesh_type = M;
        using attributes_type = typename mesh_type::surface_attributes_type;
        using vector_type = typename mesh_type::vector_type;
        using texcoord_type = typename mesh_type::texcoord_type;
        using vertex_type = typename mesh_type::vertex_type;
        using face_type = typename mesh_type::face_type;
        using face_ids_type = std::set<id_type>;
        using faces_type = Elements<mesh_type, face_type, typename face_ids_type::iterator>;
        using halfedge_type = typename mesh_type::halfedge_type;
        using halfedges_type = Elements<mesh_type, halfedge_type, id_type>;
        using seam_type = typename mesh_type::seam_type;
        using seam_ids_type = std::vector<id_type>;
        using seams_type = Elements<mesh_type, seam_type, size_t>; // Using iterator as with faces_type did not work ...
        using submesh_type = typename mesh_type::submesh_type;
        using size_type = typename Traits::size_type;

        Surface();
        Surface(mesh_type& mesh, id_type id, const std::string& name, id_type idSubmesh, const attributes_type& attributes);
        Surface(const Surface&) = default;
        Surface(Surface&&) noexcept = default;
        ~Surface() = default;

        Surface& operator=(const Surface&) = default;
        Surface& operator=(Surface&&) = default;

        id_type id() const;

        const std::string& name() const;
        void set_name(const std::string& name);

        std::string extended_name() const;

        id_type submesh_id() const;
        void set_submesh_id(id_type idSubmesh);

        const submesh_type& submesh() const;
        submesh_type& submesh();

        const face_ids_type& face_ids() const;
        face_ids_type& face_ids();

        const seam_ids_type& seam_ids() const;
        seam_ids_type& seam_ids();

        const attributes_type& attributes() const;
        attributes_type& attributes();
        void set_attributes(const attributes_type& attributes);

        const Properties& properties() const;
        Properties& properties();

        // Virtual container interface, iterators for faces making up this surface
        size_type face_count() const;
        const faces_type& faces() const;
        faces_type& faces();

        // Virtual container interface, iterators for halfedges on the seams of this surface
        size_type halfedge_count() const;
        const halfedges_type& halfedges() const;
        halfedges_type& halfedges();

        // Virtual container interface, iterators for the distinct seams of this surface
        // Multiple seams with the same adjacent surface appear separately
        size_type seam_count() const;
        const seams_type& seams() const;
        seams_type& seams();

        std::vector<id_type> find_seam_ids(id_type idSurfacePartner);
        std::vector<id_type> find_seam_ids(const std::string& nameSurfacePartner);

        bool empty() const;
        void clear(); // Clears contents, but does not change id or name

        bool unmarked() const;
        void reset() const override; // Flags

        bool contains_face(id_type idFace) const;
        void add_face(id_type idFace); // add face to list
        void remove_face(id_type idFace); // remove face from list
        void link_face(id_type idFace); // add face to list and set face surface
        void unlink_face(id_type idFace); // remove face from list and clear face surface

        void append(const Surface& surface, id_type idFaceOffset);

        void set_regenerate_seams(bool b = true);

        // Internal use, only by Mesh
        void set_mesh(mesh_type& mesh);
        void set_id(id_type id);
        const mesh_type* mesh() const;
        void check_mesh(const mesh_type* const pmesh) const;

    private:

        void check_regenerate_seams();
        void generate_seams();
        id_type first_surface_halfedge_id() const;

        mesh_type* m_pmesh;
        id_type m_id;
        std::string m_name;
        id_type m_idSubmesh;
        face_ids_type m_face_ids;
        seam_ids_type m_seam_ids;
        attributes_type m_attributes;
        Properties m_properties;

        faces_type m_faces;
        halfedges_type m_halfedges;
        seams_type m_seams;

        bool m_bRegenerateSeams;
        bool m_bGeneratingSeams;

        static typename faces_type::size_function_type m_faces_size;
        static typename faces_type::terminal_function_type m_faces_first;
        static typename faces_type::terminal_function_type m_faces_last;
        static typename faces_type::terminal_function_type m_faces_end;
        static typename faces_type::iterate_function_type m_faces_forward;
        static typename faces_type::iterate_function_type m_faces_reverse;
        static typename faces_type::element_function_type m_faces_element;
        static typename faces_type::const_element_function_type m_faces_const_element;

        static typename halfedges_type::size_function_type m_halfedges_size;
        static typename halfedges_type::terminal_function_type m_halfedges_first;
        static typename halfedges_type::terminal_function_type m_halfedges_last;
        static typename halfedges_type::terminal_function_type m_halfedges_end;
        static typename halfedges_type::iterate_function_type m_halfedges_forward;
        static typename halfedges_type::iterate_function_type m_halfedges_reverse;
        static typename halfedges_type::element_function_type m_halfedges_element;
        static typename halfedges_type::const_element_function_type m_halfedges_const_element;

        static typename seams_type::size_function_type m_seams_size;
        static typename seams_type::terminal_function_type m_seams_first;
        static typename seams_type::terminal_function_type m_seams_last;
        static typename seams_type::terminal_function_type m_seams_end;
        static typename seams_type::iterate_function_type m_seams_forward;
        static typename seams_type::iterate_function_type m_seams_reverse;
        static typename seams_type::element_function_type m_seams_element;
        static typename seams_type::const_element_function_type m_seams_const_element;
    };

    template<typename Traits, typename M>
    std::ostream& operator<<(std::ostream& os, const Surface<Traits, M>& surface);

} // namespace quetzal::brep

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::brep::Surface<Traits, M>::Surface() :
    Flags(),
    m_pmesh(nullptr),
    m_id(nullid),
    m_name(),
    m_idSubmesh(nullid),
    m_face_ids(),
    m_seam_ids(),
    m_attributes(),
    m_properties(),
    m_faces(*m_pmesh, nullid, m_faces_size, m_faces_first, m_faces_last, m_faces_end, m_faces_forward, m_faces_reverse, m_faces_element, m_faces_const_element),
    m_halfedges(*m_pmesh, nullid, m_halfedges_size, m_halfedges_first, m_halfedges_last, m_halfedges_end, m_halfedges_forward, m_halfedges_reverse, m_halfedges_element, m_halfedges_const_element),
    m_seams(*m_pmesh, nullid, m_seams_size, m_seams_first, m_seams_last, m_seams_end, m_seams_forward, m_seams_reverse, m_seams_element, m_seams_const_element),
    m_bRegenerateSeams(false),
    m_bGeneratingSeams(false)
{
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::brep::Surface<Traits, M>::Surface(mesh_type& mesh, id_type id, const std::string& name, id_type idSubmesh, const attributes_type& attributes) :
    Flags(),
    m_pmesh(&mesh),
    m_id(id),
    m_name(name),
    m_idSubmesh(idSubmesh),
    m_face_ids(),
    m_seam_ids(),
    m_attributes(attributes),
    m_properties(),
    m_faces(mesh, id, m_faces_size, m_faces_first, m_faces_last, m_faces_end, m_faces_forward, m_faces_reverse, m_faces_element, m_faces_const_element),
    m_halfedges(mesh, id, m_halfedges_size, m_halfedges_first, m_halfedges_last, m_halfedges_end, m_halfedges_forward, m_halfedges_reverse, m_halfedges_element, m_halfedges_const_element),
    m_seams(mesh, id, m_seams_size, m_seams_first, m_seams_last, m_seams_end, m_seams_forward, m_seams_reverse, m_seams_element, m_seams_const_element),
    m_bRegenerateSeams(false),
    m_bGeneratingSeams(false)
{
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Surface<Traits, M>::id() const
{
    return m_id;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const std::string& quetzal::brep::Surface<Traits, M>::name() const
{
    return m_name;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::set_name(const std::string& name)
{
    m_name = name;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
std::string quetzal::brep::Surface<Traits, M>::extended_name() const
{
    return (m_idSubmesh != nullid ? submesh().name() : "") + "/" + m_name;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Surface<Traits, M>::submesh_id() const
{
    return m_idSubmesh;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::set_submesh_id(id_type idSubmesh)
{
    m_idSubmesh = idSubmesh;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Surface<Traits, M>::submesh_type& quetzal::brep::Surface<Traits, M>::submesh() const
{
    assert(m_pmesh != nullptr);
    return m_pmesh->submesh(m_idSubmesh);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::submesh_type& quetzal::brep::Surface<Traits, M>::submesh()
{
    assert(m_pmesh != nullptr);
    return m_pmesh->submesh(m_idSubmesh);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Surface<Traits, M>::face_ids_type& quetzal::brep::Surface<Traits, M>::face_ids() const
{
    return m_face_ids;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::face_ids_type& quetzal::brep::Surface<Traits, M>::face_ids()
{
    return m_face_ids;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Surface<Traits, M>::seam_ids_type& quetzal::brep::Surface<Traits, M>::seam_ids() const
{
    const_cast<Surface<Traits, M>*>(this)->check_regenerate_seams();
    return m_seam_ids;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::seam_ids_type& quetzal::brep::Surface<Traits, M>::seam_ids()
{
    check_regenerate_seams();
    return m_seam_ids;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename const quetzal::brep::Surface<Traits, M>::attributes_type& quetzal::brep::Surface<Traits, M>::attributes() const
{
    return m_attributes;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::attributes_type& quetzal::brep::Surface<Traits, M>::attributes()
{
    return m_attributes;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::set_attributes(const attributes_type& attributes)
{
    m_attributes = attributes;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename const quetzal::Properties& quetzal::brep::Surface<Traits, M>::properties() const
{
    return m_properties;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::Properties& quetzal::brep::Surface<Traits, M>::properties()
{
    return m_properties;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::size_type quetzal::brep::Surface<Traits, M>::face_count() const
{
    return m_face_ids.size();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Surface<Traits, M>::faces_type& quetzal::brep::Surface<Traits, M>::faces() const
{
    return m_faces;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::faces_type& quetzal::brep::Surface<Traits, M>::faces()
{
    return m_faces;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::size_type quetzal::brep::Surface<Traits, M>::halfedge_count() const
{
    return m_halfedges.size();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Surface<Traits, M>::halfedges_type& quetzal::brep::Surface<Traits, M>::halfedges() const
{
    return m_halfedges;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::halfedges_type& quetzal::brep::Surface<Traits, M>::halfedges()
{
    return m_halfedges;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::size_type quetzal::brep::Surface<Traits, M>::seam_count() const
{
    check_regenerate_seams();
    return m_seam_ids.size();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Surface<Traits, M>::seams_type& quetzal::brep::Surface<Traits, M>::seams() const
{
    check_regenerate_seams();
    return m_seams;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::seams_type& quetzal::brep::Surface<Traits, M>::seams()
{
    check_regenerate_seams();
    return m_seams;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
std::vector<quetzal::id_type> quetzal::brep::Surface<Traits, M>::find_seam_ids(id_type idSurfacePartner)
{
    check_regenerate_seams();
    std::vector<id_type> ids;

    for (const auto& seam : m_seams)
    {
        if (seam.partner_surface_id() == idSurfacePartner)
        {
            ids.push_back(seam.id());
        }
    }

    return ids;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
std::vector<quetzal::id_type> quetzal::brep::Surface<Traits, M>::find_seam_ids(const std::string& nameSurfacePartner)
{
    assert(m_pmesh != nullptr);
    check_regenerate_seams();
    std::vector<id_type> ids;

    for (const auto& seam : m_seams)
    {
        if (!seam.border() && m_pmesh->surface(seam.partner_surface_id()).name() == nameSurfacePartner)
        {
            ids.push_back(seam.id());
        }
    }

    return ids;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
bool quetzal::brep::Surface<Traits, M>::empty() const
{
    return m_face_ids.empty();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::clear()
{
    m_idSubmesh = nullid;
    m_face_ids.clear();
    m_seam_ids.clear();
    m_bRegenerateSeams = false;
    return;
}

//--------------------------------------------------------------------------
template<typename Traits, typename M>
bool quetzal::brep::Surface<Traits, M>::unmarked() const
{
    assert(m_pmesh != nullptr);

    if (marked())
    {
        return false;
    }

    for (const auto& face : m_faces)
    {
        if (face.marked())
        {
            return false;
        }
    }

    for (const auto& seam : m_seams)
    {
        if (seam.marked())
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::reset() const
{
    assert(m_pmesh != nullptr);

    for (auto& face : m_faces)
    {
        face.reset();
    }

    for (auto& seam : m_seams)
    {
        seam.reset();
    }

    Flags::reset();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
bool quetzal::brep::Surface<Traits, M>::contains_face(id_type idFace) const
{
    assert(m_pmesh != nullptr);
    return m_face_ids.contains(idFace);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::add_face(id_type idFace)
{
    assert(!m_face_ids.contains(idFace));
    m_face_ids.insert(idFace);
    set_regenerate_seams();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::remove_face(id_type idFace)
{
    assert(m_face_ids.contains(idFace));
    m_face_ids.erase(idFace);
    set_regenerate_seams();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::link_face(id_type idFace)
{
    assert(m_pmesh != nullptr);
    add_face(idFace);
    m_pmesh->face(idFace).set_surface_id(m_id);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::unlink_face(id_type idFace)
{
    assert(m_pmesh != nullptr);
    remove_face(idFace);
    m_pmesh->face(idFace).set_surface_id(nullid);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::append(const Surface& surface, id_type idFaceOffset)
{
    std::transform(surface.face_ids().begin(), surface.face_ids().end(), std::inserter(m_face_ids, m_face_ids.end()), [idFaceOffset](id_type id) -> size_type { return idFaceOffset + id; });
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::set_regenerate_seams(bool b)
{
    m_bRegenerateSeams = b;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::set_mesh(mesh_type& mesh)
{
    m_pmesh = &mesh;
    m_faces.set_source(mesh);
    m_halfedges.set_source(mesh);
    m_seams.set_source(mesh);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::set_id(id_type id)
{
    m_id = id;
    m_faces.set_id(id);
    m_halfedges.set_id(id);
    m_seams.set_id(id);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Surface<Traits, M>::mesh_type* quetzal::brep::Surface<Traits, M>::mesh() const
{
    return m_pmesh;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::check_mesh(const mesh_type* const pmesh) const
{
    assert(m_pmesh == pmesh);
    assert(m_faces.mesh() == pmesh);
    assert(m_halfedges.mesh() == pmesh);
    assert(m_seams.mesh() == pmesh);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::check_regenerate_seams()
{
    if (m_bRegenerateSeams && !m_bGeneratingSeams)
    {
        m_bGeneratingSeams = true;
        generate_seams();
        m_bRegenerateSeams = false;
        m_bGeneratingSeams = false;
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::generate_seams()
{
    assert(m_pmesh != nullptr);

    m_pmesh->clear_seams(m_id);
    m_seam_ids.clear();

    id_type idHalfedgeFirst = first_surface_halfedge_id();
    if (idHalfedgeFirst == nullid)
    {
        return;
    }

    assert(m_pmesh->halfedge(idHalfedgeFirst).surface_seam());

    id_type idSurfacePartner = m_pmesh->halfedge(idHalfedgeFirst).partner_surface_id();
    id_type idSeam0 = m_pmesh->seam_store_count();
    id_type idSeam = idSeam0;

    for (id_type idHalfedge = next_surface_halfedge_id(*m_pmesh, idHalfedgeFirst); idHalfedge != idHalfedgeFirst; idHalfedge = next_surface_halfedge_id(*m_pmesh, idHalfedge))
    {
        if (m_pmesh->halfedge(idHalfedge).partner_surface_id() != idSurfacePartner)
        {
            idSurfacePartner = m_pmesh->halfedge(idHalfedge).partner_surface_id();
            idSeam = m_pmesh->create_seam("", idSurfacePartner, idSeam + 2, idSeam, idHalfedge);
            m_seam_ids.push_back(idSeam);
        }
    }

    m_pmesh->seam(idSeam).set_next_id(idSeam0);
    m_pmesh->seam(idSeam0).set_prev_id(idSeam);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Surface<Traits, M>::first_surface_halfedge_id() const
{
    for (const auto& face : m_faces)
    {
        for (const auto& halfedge : face.halfedges())
        {
            if (halfedge.surface_seam())
            {
                return halfedge.id();
            }
        }
    }

    return nullid;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::faces_type::size_function_type quetzal::brep::Surface<Traits, M>::m_faces_size = [](const mesh_type& mesh, id_type id) -> size_t
{
    return mesh.surface(id).face_ids().size();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::faces_type::terminal_function_type quetzal::brep::Surface<Traits, M>::m_faces_first = [](const mesh_type& mesh, id_type id) -> face_ids_type::iterator
{
    return mesh.surface(id).face_ids().begin();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::faces_type::terminal_function_type quetzal::brep::Surface<Traits, M>::m_faces_last = [](const mesh_type& mesh, id_type id) -> face_ids_type::iterator
{
    assert(!mesh.surface(id).face_ids().empty());
    return --mesh.surface(id).face_ids().end();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::faces_type::terminal_function_type quetzal::brep::Surface<Traits, M>::m_faces_end = [](const mesh_type& mesh, id_type id) -> face_ids_type::iterator
{
    return mesh.surface(id).face_ids().end();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::faces_type::iterate_function_type quetzal::brep::Surface<Traits, M>::m_faces_forward = [](const mesh_type& mesh, id_type id, face_ids_type::iterator i) -> face_ids_type::iterator
{
    assert(i != m_faces_end(mesh, id));
    return std::next(i);
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::faces_type::iterate_function_type quetzal::brep::Surface<Traits, M>::m_faces_reverse = [](const mesh_type& mesh, id_type id, face_ids_type::iterator i) -> face_ids_type::iterator
{
    assert(i != m_faces_first(mesh, id));
    return std::prev(i);
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::faces_type::element_function_type quetzal::brep::Surface<Traits, M>::m_faces_element = [](mesh_type& mesh, id_type id, face_ids_type::iterator i) -> typename quetzal::brep::Surface<Traits, M>::face_type&
{
    assert(i != m_faces_end(mesh, id));
    auto& face = mesh.face(*i);
    assert(!face.deleted());
    return face;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::faces_type::const_element_function_type quetzal::brep::Surface<Traits, M>::m_faces_const_element = [](const mesh_type& mesh, id_type id, face_ids_type::iterator i) -> const typename quetzal::brep::Surface<Traits, M>::face_type&
{
    assert(i != m_faces_end(mesh, id));
    const auto& face = mesh.face(*i);
    assert(!face.deleted());
    return face;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::halfedges_type::size_function_type quetzal::brep::Surface<Traits, M>::m_halfedges_size = [](const mesh_type& mesh, id_type id) -> size_t
{
    size_t n = 0;

    const auto& surface = mesh.surface(id);
    for (auto i = surface.halfedges().cbegin(); i != surface.halfedges().cend(); ++i)
    {
        ++n;
    }

    return n;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Surface<Traits, M>::m_halfedges_first = [](const mesh_type& mesh, id_type id) -> id_type
{
    id_type idSeam = mesh.surface(id).seam_ids().front();
    return mesh.seam(idSeam).halfedge_id();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Surface<Traits, M>::m_halfedges_last = [](const mesh_type& mesh, id_type id) -> id_type
{
    return prev_surface_halfedge_id(mesh, m_halfedges_first(mesh, id));
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Surface<Traits, M>::m_halfedges_end = [](const mesh_type& mesh, id_type id) -> id_type
{
    mesh;
    id;
    return nullid;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::halfedges_type::iterate_function_type quetzal::brep::Surface<Traits, M>::m_halfedges_forward = [](const mesh_type& mesh, id_type id, id_type i) -> id_type
{
    id_type idHalfedge = next_surface_halfedge_id(mesh, i);
    return idHalfedge != m_halfedges_first(mesh, id) ? idHalfedge : nullid;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::halfedges_type::iterate_function_type quetzal::brep::Surface<Traits, M>::m_halfedges_reverse = [](const mesh_type& mesh, id_type id, id_type i) -> id_type
{
    id_type idHalfedge = prev_surface_halfedge_id(mesh, i);
    return idHalfedge != m_halfedges_last(mesh, id) ? idHalfedge : nullid;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::halfedges_type::element_function_type quetzal::brep::Surface<Traits, M>::m_halfedges_element = [](mesh_type& mesh, id_type id, id_type i) -> typename quetzal::brep::Surface<Traits, M>::halfedge_type&
{
    id;
    auto& halfedge = mesh.halfedge(i);
    assert(!halfedge.deleted());
    return halfedge;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::halfedges_type::const_element_function_type quetzal::brep::Surface<Traits, M>::m_halfedges_const_element = [](const mesh_type& mesh, id_type id, id_type i) -> const typename quetzal::brep::Surface<Traits, M>::halfedge_type&
{
    id;
    const auto& halfedge = mesh.halfedge(i);
    assert(!halfedge.deleted());
    return halfedge;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::seams_type::size_function_type quetzal::brep::Surface<Traits, M>::m_seams_size = [](const mesh_type& mesh, id_type id) -> size_t
{
    return mesh.surface(id).seam_ids().size();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::seams_type::terminal_function_type quetzal::brep::Surface<Traits, M>::m_seams_first = [](const mesh_type& mesh, id_type id) -> size_t
{
    return mesh.surface(id).seam_ids().size() > 0 ? 0 : m_seams_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::seams_type::terminal_function_type quetzal::brep::Surface<Traits, M>::m_seams_last = [](const mesh_type& mesh, id_type id) -> size_t
{
    return mesh.surface(id).seam_ids().size() > 0 ? mesh.surface(id).seam_ids().size() - 1 : m_seams_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::seams_type::terminal_function_type quetzal::brep::Surface<Traits, M>::m_seams_end = [](const mesh_type& mesh, id_type id) -> size_t
{
    mesh;
    id;
    return numeric_limits<size_t>::max();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::seams_type::iterate_function_type quetzal::brep::Surface<Traits, M>::m_seams_forward = [](const mesh_type& mesh, id_type id, size_t i) -> size_t
{
    assert(i != m_seams_end(mesh, id));
    const auto& surface = mesh.surface(id);
    size_t n = surface.seam_ids().size();
    assert(i < n);
    return ++i == surface.seam_ids().size() ? m_seams_end(mesh, id) : i;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::seams_type::iterate_function_type quetzal::brep::Surface<Traits, M>::m_seams_reverse = [](const mesh_type& mesh, id_type id, size_t i) -> size_t
{
    assert(i != m_seams_end(mesh, id));
    const auto& surface = mesh.surface(id);
    size_t n = surface.seam_ids().size();
    assert(i < n);
    return i == 0 ? m_seams_end(mesh, id) : i - 1;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::seams_type::element_function_type quetzal::brep::Surface<Traits, M>::m_seams_element = [](mesh_type& mesh, id_type id, size_t i) -> typename quetzal::brep::Surface<Traits, M>::seam_type&
{
    assert(i != m_seams_end(mesh, id));
    assert(i < mesh.surface(id).seam_ids().size());
    auto& seam = mesh.seam(mesh.surface(id).seam_ids()[i]);
    assert(!seam.deleted());
    return seam;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::seams_type::const_element_function_type quetzal::brep::Surface<Traits, M>::m_seams_const_element = [](const mesh_type& mesh, id_type id, size_t i) -> const typename quetzal::brep::Surface<Traits, M>::seam_type&
{
    assert(i != m_seams_end(mesh, id));
    assert(i < mesh.surface(id).seam_ids().size());
    const auto& seam = mesh.seam(mesh.surface(id).seam_ids()[i]);
    assert(!seam.deleted());
    return seam;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
std::ostream& quetzal::brep::operator<<(std::ostream& os, const Surface<Traits, M>& surface)
{
    os << static_cast<Flags>(surface);
    os << "\t" << surface.id();

    if (!surface.deleted())
    {
        os << "\t" << surface.name();
        os << "\to ";
        if (surface.submesh_id() != nullid)
        {
            os << surface.submesh_id() << " " << surface.submesh().name();
        }
        else
        {
            os << "nullid";
        }

        os << "\t" << surface.face_count() << " faces (";
        for (id_type idFace : surface.face_ids())
        {
            os << " " << idFace;
        }
        os << ")";

        os << "\t" << surface.attributes();
        os << "\t" << surface.properties();
    }

    os << std::endl;
    return os;
}

#endif // QUETZAL_BREP_SURFACE_HPP
