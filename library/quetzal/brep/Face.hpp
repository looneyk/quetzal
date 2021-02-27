#if !defined(QUETZAL_BREP_FACE_HPP)
#define QUETZAL_BREP_FACE_HPP
//------------------------------------------------------------------------------
// brep
// Face.hpp
//------------------------------------------------------------------------------

#include "Flags.hpp"
#include "id.hpp"
#include "quetzal/common/Elements.hpp"
#include "quetzal/common/Properties.hpp"
#include <iostream>
#include <vector>
#include <cassert>

namespace quetzal::brep
{

    template<typename Traits>
    class Mesh;

    //--------------------------------------------------------------------------
    template<typename Traits, typename M = Mesh<Traits>>
    class Face : public Flags
    {
    public:

        using mesh_type = M;
        using halfedge_type = typename mesh_type::halfedge_type;
        using surface_type = typename mesh_type::surface_type;
        using submesh_type = typename mesh_type::submesh_type;
        using hole_ids_type = std::vector<id_type>; // halfedge ids ...
        using halfedges_type = Elements<mesh_type, halfedge_type, id_type>;
        using attributes_type = typename mesh_type::face_attributes_type;
        using size_type = typename mesh_type::size_type;

        Face();
        Face(mesh_type& mesh, id_type id, id_type idSurface, id_type idSubmesh, id_type idHalfedge);
        Face(mesh_type& mesh, id_type id, id_type idSurface, id_type idSubmesh, id_type idHalfedge, const attributes_type& attributes);
        Face(const Face&) = default;
        Face(Face&&) noexcept = default;
        ~Face() = default;

        Face& operator=(const Face&) = default;
        Face& operator=(Face&&) = default;

        id_type id() const;

        id_type partner_id() const;
        void set_partner_id(id_type idHalfedge);

        const Face& partner() const;
        Face& partner();

        id_type halfedge_id() const;
        void set_halfedge_id(id_type idHalfedge);

        const halfedge_type& halfedge() const;
        halfedge_type& halfedge();

        id_type surface_id() const;
        void set_surface_id(id_type idSurface);

        const surface_type& surface() const;
        surface_type& surface();

        id_type submesh_id() const;
        void set_submesh_id(id_type idSubmesh);

        const submesh_type& submesh() const;
        submesh_type& submesh();

        const hole_ids_type& hole_ids() const;
        hole_ids_type& hole_ids();

        const attributes_type& attributes() const;
        attributes_type& attributes();
        void set_attributes(const attributes_type& attributes);

        const Properties& properties() const;
        Properties& properties();

        // Virtual container interface, iterators for halfedges connected to this face
        const halfedges_type& halfedges() const;
        halfedges_type& halfedges();

        bool border() const;
        void set_border();

        size_type vertex_count() const;
        size_type hole_count() const;

        bool check() const;

        // Internal use, only by Mesh
        void set_mesh(mesh_type& mesh);
        void set_id(id_type id);
        const mesh_type* mesh() const;
        void check_mesh(const mesh_type* const pmesh) const;

    private:

        mesh_type* m_pmesh;
        id_type m_id;
        id_type m_idPartner;
        id_type m_idHalfedge;
        id_type m_idSurface;
        id_type m_idSubmesh;
        attributes_type m_attributes;
        Properties m_properties;

        hole_ids_type m_hole_ids;

        Elements<mesh_type, halfedge_type, id_type> m_halfedges;

        static typename halfedges_type::size_function_type m_halfedges_size;
        static typename halfedges_type::terminal_function_type m_halfedges_first;
        static typename halfedges_type::terminal_function_type m_halfedges_last;
        static typename halfedges_type::terminal_function_type m_halfedges_end;
        static typename halfedges_type::iterate_function_type m_halfedges_forward;
        static typename halfedges_type::iterate_function_type m_halfedges_reverse;
        static typename halfedges_type::element_function_type m_halfedges_element;
        static typename halfedges_type::const_element_function_type m_halfedges_const_element;
    };

    template<typename Traits, typename M>
    std::ostream& operator<<(std::ostream& os, const Face<Traits, M>& face);

} // namespace quetzal::brep

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::brep::Face<Traits, M>::Face() :
    Flags(),
    m_pmesh(nullptr),
    m_id(nullid),
    m_idPartner(nullid),
    m_idHalfedge(nullid),
    m_idSurface(nullid),
    m_idSubmesh(nullid),
    m_attributes(),
    m_properties(),
    m_hole_ids(),
    m_halfedges(*m_pmesh, nullid, m_halfedges_size, m_halfedges_first, m_halfedges_last, m_halfedges_end, m_halfedges_forward, m_halfedges_reverse, m_halfedges_element, m_halfedges_const_element)
{
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::brep::Face<Traits, M>::Face(mesh_type& mesh, id_type id, id_type idSurface, id_type idSubmesh, id_type idHalfedge) :
    Flags(),
    m_pmesh(&mesh),
    m_id(id),
    m_idPartner(nullid),
    m_idHalfedge(idHalfedge),
    m_idSurface(idSurface),
    m_idSubmesh(idSubmesh),
    m_attributes(),
    m_properties(),
    m_hole_ids(),
    m_halfedges(mesh, id, m_halfedges_size, m_halfedges_first, m_halfedges_last, m_halfedges_end, m_halfedges_forward, m_halfedges_reverse, m_halfedges_element, m_halfedges_const_element)
{
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::brep::Face<Traits, M>::Face(mesh_type& mesh, id_type id, id_type idSurface, id_type idSubmesh, id_type idHalfedge, const attributes_type& attributes) :
    Flags(),
    m_pmesh(&mesh),
    m_id(id),
    m_idPartner(nullid),
    m_idHalfedge(idHalfedge),
    m_idSurface(idSurface),
    m_idSubmesh(idSubmesh),
    m_attributes(attributes),
    m_properties(),
    m_hole_ids(),
    m_halfedges(mesh, id, m_halfedges_size, m_halfedges_first, m_halfedges_last, m_halfedges_end, m_halfedges_forward, m_halfedges_reverse, m_halfedges_element, m_halfedges_const_element)
{
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Face<Traits, M>::id() const
{
    return m_id;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Face<Traits, M>::partner_id() const
{
    return m_idPartner;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Face<Traits, M>::set_partner_id(id_type idPartner)
{
    m_idPartner = idPartner;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Face<Traits, M>& quetzal::brep::Face<Traits, M>::partner() const
{
    assert(m_pmesh != nullptr);
    return m_pmesh->face(m_idPartner);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Face<Traits, M>& quetzal::brep::Face<Traits, M>::partner()
{
    assert(m_pmesh != nullptr);
    return m_pmesh->face(m_idPartner);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Face<Traits, M>::halfedge_id() const
{
    return m_idHalfedge;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Face<Traits, M>::set_halfedge_id(id_type idHalfedge)
{
    m_idHalfedge = idHalfedge;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Face<Traits, M>::halfedge_type& quetzal::brep::Face<Traits, M>::halfedge() const
{
    assert(m_pmesh != nullptr);
    return m_pmesh->halfedge(m_idHalfedge);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Face<Traits, M>::halfedge_type& quetzal::brep::Face<Traits, M>::halfedge()
{
    assert(m_pmesh != nullptr);
    return m_pmesh->halfedge(m_idHalfedge);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Face<Traits, M>::surface_id() const
{
    return m_idSurface;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Face<Traits, M>::set_surface_id(id_type idSurface)
{
    m_idSurface = idSurface;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Face<Traits, M>::surface_type& quetzal::brep::Face<Traits, M>::surface() const
{
    assert(m_pmesh != nullptr);
    return m_pmesh->surface(m_idSurface);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Face<Traits, M>::surface_type& quetzal::brep::Face<Traits, M>::surface()
{
    assert(m_pmesh != nullptr);
    return m_pmesh->surface(m_idSurface);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Face<Traits, M>::submesh_id() const
{
    return m_idSubmesh;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Face<Traits, M>::set_submesh_id(id_type idSubmesh)
{
    m_idSubmesh = idSubmesh;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Face<Traits, M>::submesh_type& quetzal::brep::Face<Traits, M>::submesh() const
{
    assert(m_pmesh != nullptr);
    return m_pmesh->submesh(m_idSubmesh);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Face<Traits, M>::submesh_type& quetzal::brep::Face<Traits, M>::submesh()
{
    assert(m_pmesh != nullptr);
    return m_pmesh->submesh(m_idSubmesh);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Face<Traits, M>::hole_ids_type& quetzal::brep::Face<Traits, M>::hole_ids() const
{
    return m_hole_ids;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Face<Traits, M>::hole_ids_type& quetzal::brep::Face<Traits, M>::hole_ids()
{
    return m_hole_ids;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename const quetzal::brep::Face<Traits, M>::attributes_type& quetzal::brep::Face<Traits, M>::attributes() const
{
    return m_attributes;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Face<Traits, M>::attributes_type& quetzal::brep::Face<Traits, M>::attributes()
{
    return m_attributes;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Face<Traits, M>::set_attributes(const attributes_type& attributes)
{
    m_attributes = attributes;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename const quetzal::Properties& quetzal::brep::Face<Traits, M>::properties() const
{
    return m_properties;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::Properties& quetzal::brep::Face<Traits, M>::properties()
{
    return m_properties;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Face<Traits, M>::halfedges_type& quetzal::brep::Face<Traits, M>::halfedges() const
{
    return m_halfedges;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Face<Traits, M>::halfedges_type& quetzal::brep::Face<Traits, M>::halfedges()
{
    return m_halfedges;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
bool quetzal::brep::Face<Traits, M>::border() const
{
    return m_idPartner == nullid;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Face<Traits, M>::set_border()
{
    m_idPartner = nullid;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Face<Traits, M>::size_type quetzal::brep::Face<Traits, M>::vertex_count() const
{
    return m_halfedges.size();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Face<Traits, M>::size_type quetzal::brep::Face<Traits, M>::hole_count() const
{
    return m_hole_ids.size();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
bool quetzal::brep::Face<Traits, M>::check() const
{
    assert(m_pmesh != nullptr);

    if (deleted())
    {
        return true;
    }

    return m_id < m_pmesh->face_store_count()
        && m_idHalfedge < m_pmesh->halfedge_store_count()
        && !halfedge().deleted();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Face<Traits, M>::set_mesh(mesh_type& mesh)
{
    m_pmesh = &mesh;
    m_halfedges.set_source(mesh);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Face<Traits, M>::set_id(id_type id)
{
    m_id = id;
    m_halfedges.set_id(id);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Face<Traits, M>::mesh_type* quetzal::brep::Face<Traits, M>::mesh() const
{
    return m_pmesh;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Face<Traits, M>::check_mesh(const mesh_type* const pmesh) const
{
    assert(m_pmesh == pmesh);
    assert(m_halfedges.mesh() == pmesh);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Face<Traits, M>::halfedges_type::size_function_type quetzal::brep::Face<Traits, M>::m_halfedges_size = [](const mesh_type& mesh, id_type id) -> size_t
{
    size_t n = 0;

    const auto& face = mesh.face(id);
    for (auto i = face.halfedges().cbegin(); i != face.halfedges().cend(); ++i)
    {
        ++n;
    }

    return n;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Face<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Face<Traits, M>::m_halfedges_first = [](const mesh_type& mesh, id_type id) -> id_type
{
    return mesh.face(id).halfedge_id();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Face<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Face<Traits, M>::m_halfedges_last = [](const mesh_type& mesh, id_type id) -> id_type
{
    return mesh.face(id).halfedge().prev_id();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Face<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Face<Traits, M>::m_halfedges_end = [](const mesh_type& mesh, id_type id) -> id_type
{
    mesh;
    id;
    return nullid;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Face<Traits, M>::halfedges_type::iterate_function_type quetzal::brep::Face<Traits, M>::m_halfedges_forward = [](const mesh_type& mesh, id_type id, id_type i) -> id_type
{
    i = mesh.halfedge(i).next_id();
    return i != m_halfedges_first(mesh, id) ? i : nullid;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Face<Traits, M>::halfedges_type::iterate_function_type quetzal::brep::Face<Traits, M>::m_halfedges_reverse = [](const mesh_type& mesh, id_type id, id_type i) -> id_type
{
    i = mesh.halfedge(i).prev_id();
    return i != m_halfedges_last(mesh, id) ? i : nullid;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Face<Traits, M>::halfedges_type::element_function_type quetzal::brep::Face<Traits, M>::m_halfedges_element = [](mesh_type& mesh, id_type id, id_type i) -> typename quetzal::brep::Face<Traits, M>::halfedge_type&
{
    id;
    auto& halfedge = mesh.halfedge(i);
    assert(!halfedge.deleted());
    return halfedge;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Face<Traits, M>::halfedges_type::const_element_function_type quetzal::brep::Face<Traits, M>::m_halfedges_const_element = [](const mesh_type& mesh, id_type id, id_type i) -> const typename quetzal::brep::Face<Traits, M>::halfedge_type&
{
    id;
    const auto& halfedge = mesh.halfedge(i);
    assert(!halfedge.deleted());
    return halfedge;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
std::ostream& quetzal::brep::operator<<(std::ostream& os, const Face<Traits, M>& face)
{
    os << static_cast<Flags>(face);
    os << "\t" << face.id();

    if (!face.deleted())
    {
        os << "\t" << face.vertex_count() << " edges";
        os << "\t" << face.halfedge_id();

        os << "\t";
        if (face.partner_id() != nullid)
        {
            os << face.partner_id();
        }
        else
        {
            os << "border";
        }

        os << "\ts ";
        if (face.surface_id() != nullid)
        {
            os << face.surface_id() << " " << face.surface().name();
        }
        else
        {
            os << "nullid";
        }

        os << "\to ";
        if (face.submesh_id() != nullid)
        {
            os << face.submesh_id() << " " << face.submesh().name();
        }
        else
        {
            os << "nullid";
        }

        os << "\t" << face.attributes();
        os << "\t" << face.properties();
    }

    os << std::endl;
    return os;
}

#endif // QUETZAL_BREP_FACE_HPP
