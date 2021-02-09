#if !defined(QUETZAL_BREP_SEAM_HPP)
#define QUETZAL_BREP_SEAM_HPP
//------------------------------------------------------------------------------
// brep
// Seam.hpp
//------------------------------------------------------------------------------

#include "Flags.hpp"
#include "id.hpp"
#include "common/Elements.hpp"
#include "mesh_util.hpp"
#include <iostream>
#include <cassert>

namespace quetzal
{

namespace brep
{

    template<typename Traits>
    class Mesh;

    //--------------------------------------------------------------------------
    template<typename Traits, typename M = Mesh<Traits>>
    class Seam : public Flags
    {
    public:

        using mesh_type = M;
        using surface_type = typename mesh_type::surface_type;
        using halfedge_type = typename mesh_type::halfedge_type;
        using halfedges_type = Elements<mesh_type, halfedge_type, id_type>;
        using size_type = typename Traits::size_type;

        Seam();
        Seam(mesh_type& mesh, id_type id, const std::string& name, id_type idPartner = nullid, id_type idNext = nullid, id_type idPrev = nullid, id_type idHalfedge = nullid);
        Seam(const Seam&) = default;
        Seam(Seam&&) noexcept = default;
        ~Seam() = default;

        Seam& operator=(const Seam&) = default;
        Seam& operator=(Seam&&) = default;

        id_type id() const;

        const std::string& name() const;
        void set_name(const std::string& name);

        std::string extended_name() const;

        id_type partner_id() const;
        void set_partner_id(id_type idSeam);

        const Seam& partner() const;
        Seam& partner();

        id_type next_id() const;
        void set_next_id(id_type idSeam);

        const Seam& next() const;
        Seam& next();

        id_type prev_id() const;
        void set_prev_id(id_type idSeam);

        const Seam& prev() const;
        Seam& prev();

        // with halfedges iterator, do we really need halfedge on its own? ...
        // no, it's only used in iterators

        id_type halfedge_id() const;
        void set_halfedge_id(id_type idHalfedge);

        const halfedge_type& halfedge() const;
        halfedge_type& halfedge();

        // Virtual container interface, iterators for halfedges on this seam
        size_type halfedge_count() const;
        const halfedges_type& halfedges() const;
        halfedges_type& halfedges();

        id_type surface_id() const;
        id_type partner_surface_id() const;

        const surface_type& surface() const;
        surface_type& surface();

        bool border() const;

        // Internal use, only by Mesh
        void set_mesh(mesh_type& mesh);
        void set_id(id_type id);
        const mesh_type* mesh() const;
        void check_mesh(const mesh_type* const pmesh) const;

    private:

        mesh_type* m_pmesh;
        id_type m_id;
        std::string m_name;
        id_type m_idPartner;
        id_type m_idNext;
        id_type m_idPrev;
        id_type m_idHalfedge;
        halfedges_type m_halfedges;

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
    std::ostream& operator<<(std::ostream& os, const Seam<Traits, M>& Seam);

} // namespace brep

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::brep::Seam<Traits, M>::Seam() :
    Flags(),
    m_pmesh(nullptr),
    m_id(nullid),
    m_name(),
    m_idPartner(nullid),
    m_idNext(nullid),
    m_idPrev(nullid),
    m_idHalfedge(nullid),
    m_halfedges(*m_pmesh, nullid, m_halfedges_size, m_halfedges_first, m_halfedges_last, m_halfedges_end, m_halfedges_forward, m_halfedges_reverse, m_halfedges_element, m_halfedges_const_element)
{
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::brep::Seam<Traits, M>::Seam(mesh_type& mesh, id_type id, const std::string& name, id_type idPartner, id_type idNext, id_type idPrev, id_type idHalfedge) :
    Flags(),
    m_pmesh(&mesh),
    m_id(id),
    m_name(name),
    m_idPartner(idPartner),
    m_idNext(idNext),
    m_idPrev(idPrev),
    m_idHalfedge(idHalfedge),
    m_halfedges(mesh, id, m_halfedges_size, m_halfedges_first, m_halfedges_last, m_halfedges_end, m_halfedges_forward, m_halfedges_reverse, m_halfedges_element, m_halfedges_const_element)
{
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Seam<Traits, M>::id() const
{
    return m_id;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const std::string& quetzal::brep::Seam<Traits, M>::name() const
{
    return m_name;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Seam<Traits, M>::set_name(const std::string& name)
{
    m_name = name;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
std::string quetzal::brep::Seam<Traits, M>::extended_name() const
{
    const auto& s = surface();

    if (surface_id() == nullid)
    {
        return "//" + m_name;
    }

    return (s.submesh_id() != nullid ? s.submesh().name() : "") + "/" + s.name() + "/" + m_name;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Seam<Traits, M>::partner_id() const
{
    return m_idPartner;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Seam<Traits, M>::set_partner_id(id_type idSeam)
{
    m_idPartner = idSeam;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Seam<Traits, M>& quetzal::brep::Seam<Traits, M>::partner() const
{
    assert(m_pmesh != nullptr);
    return m_pmesh->Seam(m_idPartner);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>& quetzal::brep::Seam<Traits, M>::partner()
{
    assert(m_pmesh != nullptr);
    return m_pmesh->Seam(m_idPartner);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Seam<Traits, M>::next_id() const
{
    return m_idNext;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Seam<Traits, M>::set_next_id(id_type idSeam)
{
    m_idNext = idSeam;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Seam<Traits, M>& quetzal::brep::Seam<Traits, M>::next() const
{
    assert(m_pmesh != nullptr);
    return m_pmesh->seam(m_idNext);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>& quetzal::brep::Seam<Traits, M>::next()
{
    assert(m_pmesh != nullptr);
    return m_pmesh->seam(m_idNext);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Seam<Traits, M>::prev_id() const
{
    return m_idPrev;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Seam<Traits, M>::set_prev_id(id_type idSeam)
{
    m_idPrev = idSeam;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Seam<Traits, M>& quetzal::brep::Seam<Traits, M>::prev() const
{
    assert(m_pmesh != nullptr);
    return m_pmesh->seam(m_idPrev);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>& quetzal::brep::Seam<Traits, M>::prev()
{
    assert(m_pmesh != nullptr);
    return m_pmesh->seam(m_idPrev);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Seam<Traits, M>::halfedge_id() const
{
    return m_idHalfedge;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Seam<Traits, M>::set_halfedge_id(id_type idHalfedge)
{
    m_idHalfedge = idHalfedge;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Seam<Traits, M>::halfedge_type& quetzal::brep::Seam<Traits, M>::halfedge() const
{
    assert(m_pmesh != nullptr);
    return m_pmesh->halfedge(m_idHalfedge);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>::halfedge_type& quetzal::brep::Seam<Traits, M>::halfedge()
{
    assert(m_pmesh != nullptr);
    return m_pmesh->halfedge(m_idHalfedge);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>::size_type quetzal::brep::Seam<Traits, M>::halfedge_count() const
{
    return m_halfedges.size();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Seam<Traits, M>::halfedges_type& quetzal::brep::Seam<Traits, M>::halfedges() const
{
    return m_halfedges;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>::halfedges_type& quetzal::brep::Seam<Traits, M>::halfedges()
{
    return m_halfedges;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Seam<Traits, M>::surface_id() const
{
    return halfedge().face().surface_id();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Seam<Traits, M>::partner_surface_id() const
{
    if (border())
    {
        return nullid;
    }

    return halfedge().partner().face().surface_id();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Seam<Traits, M>::surface_type& quetzal::brep::Seam<Traits, M>::surface() const
{
    assert(m_pmesh != nullptr);
    return m_pmesh->surface(surface_id());
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>::surface_type& quetzal::brep::Seam<Traits, M>::surface()
{
    assert(m_pmesh != nullptr);
    return m_pmesh->surface(surface_id());
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
bool quetzal::brep::Seam<Traits, M>::border() const
{
    return m_idPartner == nullid;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Seam<Traits, M>::set_mesh(mesh_type& mesh)
{
    m_pmesh = &mesh;
    m_halfedges.set_source(mesh);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Seam<Traits, M>::set_id(id_type id)
{
    m_id = id;
    m_halfedges.set_id(id);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Seam<Traits, M>::mesh_type* quetzal::brep::Seam<Traits, M>::mesh() const
{
    return m_pmesh;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Seam<Traits, M>::check_mesh(const mesh_type* const pmesh) const
{
    assert(m_pmesh == pmesh);
    assert(m_halfedges.mesh() == pmesh);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>::halfedges_type::size_function_type quetzal::brep::Seam<Traits, M>::m_halfedges_size = [](const mesh_type& mesh, id_type id) -> size_t
{
    id_type idHalfedgeLast = m_halfedges_last(mesh, id);
    size_t n = 1;
    for (id_type idHalfedge = mesh.seam(id).halfedge_id(); idHalfedge != idHalfedgeLast; idHalfedge = m_halfedges_forward(mesh, id, idHalfedge))
    {
        ++n;
    }

    return n;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Seam<Traits, M>::m_halfedges_first = [](const mesh_type& mesh, id_type id) -> id_type
{
    return mesh.seam(id).halfedge_id();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Seam<Traits, M>::m_halfedges_last = [](const mesh_type& mesh, id_type id) -> id_type
{
    return prev_surface_halfedge_id(mesh, mesh.seam(id).next().halfedge_id());
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Seam<Traits, M>::m_halfedges_end = [](const mesh_type& mesh, id_type id) -> id_type
{
    mesh;
    id;
    return nullid;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>::halfedges_type::iterate_function_type quetzal::brep::Seam<Traits, M>::m_halfedges_forward = [](const mesh_type& mesh, id_type id, id_type i) -> id_type
{
    assert(i != m_halfedges_end(mesh, id));

    id_type idHalfedgeNext = next_surface_halfedge_id(mesh, i);
    if (idHalfedgeNext == mesh.seam(id).halfedge_id() || mesh.halfedge(idHalfedgeNext).surface_id() != mesh.halfedge(i).surface_id())
    {
        return m_halfedges_end(mesh, id);
    }

    return idHalfedgeNext;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>::halfedges_type::iterate_function_type quetzal::brep::Seam<Traits, M>::m_halfedges_reverse = [](const mesh_type& mesh, id_type id, id_type i) -> id_type
{
    assert(i != m_halfedges_end(mesh, id));

    id_type idHalfedgePrev = prev_surface_halfedge_id(mesh, i);
    if (idHalfedgePrev == mesh.seam(id).halfedge_id() || mesh.halfedge(idHalfedgePrev).surface_id() != mesh.halfedge(i).surface_id())
    {
        return m_halfedges_end(mesh, id);
    }

    return idHalfedgePrev;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>::halfedges_type::element_function_type quetzal::brep::Seam<Traits, M>::m_halfedges_element = [](mesh_type& mesh, id_type id, id_type i) -> typename quetzal::brep::Seam<Traits, M>::halfedge_type&
{
    id;
    auto& halfedge = mesh.halfedge(i);
    assert(!halfedge.deleted());
    return halfedge;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>::halfedges_type::const_element_function_type quetzal::brep::Seam<Traits, M>::m_halfedges_const_element = [](const mesh_type& mesh, id_type id, id_type i) -> const typename quetzal::brep::Seam<Traits, M>::halfedge_type&
{
    id;
    const auto& halfedge = mesh.halfedge(i);
    assert(!halfedge.deleted());
    return halfedge;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
std::ostream& quetzal::brep::operator<<(std::ostream& os, const Seam<Traits, M>& seam)
{
    os << static_cast<Flags>(seam);
    os << "\t" << seam.id();

    if (!seam.deleted())
    {
        os << "\t";
        if (seam.partner_id() != nullid)
        {
            os << seam.partner_id();
        }
        else
        {
            os << "border";
        }

        os << "\t" << seam.next_id();
        os << "\t" << seam.prev_id();
        os << "\t" << seam.halfedge_id();
        os << "\t" << seam.surface_id();
    }

    os << std::endl;
    return os;
}

#endif // QUETZAL_BREP_SEAM_HPP
