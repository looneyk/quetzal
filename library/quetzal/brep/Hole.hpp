#if !defined(QUETZAL_BREP_HOLE_HPP)
#define QUETZAL_BREP_HOLE_HPP
//------------------------------------------------------------------------------
// brep
// Hole.hpp
//------------------------------------------------------------------------------

#include "Flags.hpp"
#include "id.hpp"
#include "quetzal/common/ElementsDirect.hpp"
#include "quetzal/common/Properties.hpp"
#include <iostream>
#include <cassert>

namespace quetzal::brep
{

    template<typename Traits>
    class Mesh;

    //--------------------------------------------------------------------------
    template<typename Traits, typename M = Mesh<Traits>>
    class Hole : public Flags
    {
    public:

        using mesh_type = M;
        using face_type = typename mesh_type::face_type;
        using halfedge_type = typename mesh_type::halfedge_type;
        using halfedges_type = ElementsDirect<Hole, halfedge_type, id_type>;

        Hole();
        Hole(mesh_type& mesh, id_type idFace, id_type idHalfedge);
        Hole(const Hole&) = default;
        Hole(Hole&&) noexcept = default;
        ~Hole() = default;

        Hole& operator=(const Hole&) = default;
        Hole& operator=(Hole&&) = default;

        id_type face_id() const;
        void set_face_id(id_type idFace);

        const face_type& face() const;
        face_type& face();

        id_type halfedge_id() const;
        void set_halfedge_id(id_type idHalfedge);

        const halfedge_type& halfedge() const;
        halfedge_type& halfedge();

        // Virtual container interface, iterators for halfedges that make up this hole
        const halfedges_type& halfedges() const;
        halfedges_type& halfedges();

        const Properties& properties() const;
        Properties& properties();

        bool check() const;

        // Internal use, only by Mesh
        void set_mesh(mesh_type& mesh);
        const mesh_type& mesh() const;
        mesh_type& mesh();
        void check_mesh(const mesh_type* const pmesh) const;

    private:

        mesh_type* m_pmesh;
        id_type m_idFace;
        id_type m_idHalfedge;
        halfedges_type m_halfedges;
        Properties m_properties;

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
    std::ostream& operator<<(std::ostream& os, const Hole<Traits, M>& hole);

} // namespace quetzal::brep

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::brep::Hole<Traits, M>::Hole() :
    Flags(),
    m_pmesh(nullptr),
    m_idFace(nullid),
    m_idHalfedge(nullid),
    m_halfedges(m_halfedges_size, m_halfedges_first, m_halfedges_last, m_halfedges_end, m_halfedges_forward, m_halfedges_reverse, m_halfedges_element, m_halfedges_const_element),
    m_properties()
{
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::brep::Hole<Traits, M>::Hole(mesh_type& mesh, id_type idFace, id_type idHalfedge) :
    Flags(),
    m_pmesh(&mesh),
    m_idFace(idFace),
    m_idHalfedge(idHalfedge),
    m_halfedges(m_halfedges_size, m_halfedges_first, m_halfedges_last, m_halfedges_end, m_halfedges_forward, m_halfedges_reverse, m_halfedges_element, m_halfedges_const_element),
    m_properties()
{
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Hole<Traits, M>::face_id() const
{
    return m_idFace;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Hole<Traits, M>::set_face_id(id_type idFace)
{
    m_idFace = idFace;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Hole<Traits, M>::face_type& quetzal::brep::Hole<Traits, M>::face() const
{
    assert(m_pmesh != nullptr);
    return m_pmesh->face(m_idFace);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Hole<Traits, M>::face_type& quetzal::brep::Hole<Traits, M>::face()
{
    assert(m_pmesh != nullptr);
    return m_pmesh->face(m_idFace);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Hole<Traits, M>::halfedge_id() const
{
    return m_idHalfedge;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Hole<Traits, M>::set_halfedge_id(id_type idHalfedge)
{
    m_idHalfedge = idHalfedge;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Hole<Traits, M>::halfedge_type& quetzal::brep::Hole<Traits, M>::halfedge() const
{
    assert(m_pmesh != nullptr);
    return m_pmesh->halfedge(m_idHalfedge);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Hole<Traits, M>::halfedge_type& quetzal::brep::Hole<Traits, M>::halfedge()
{
    assert(m_pmesh != nullptr);
    return m_pmesh->halfedge(m_idHalfedge);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Hole<Traits, M>::halfedges_type& quetzal::brep::Hole<Traits, M>::halfedges() const
{
    m_halfedges.set_source(*this);
    return m_halfedges;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Hole<Traits, M>::halfedges_type& quetzal::brep::Hole<Traits, M>::halfedges()
{
    m_halfedges.set_source(*this);
    return m_halfedges;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename const quetzal::Properties& quetzal::brep::Hole<Traits, M>::properties() const
{
    return m_properties;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::Properties& quetzal::brep::Hole<Traits, M>::properties()
{
    return m_properties;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
bool quetzal::brep::Hole<Traits, M>::check() const
{
    assert(m_pmesh != nullptr);

    if (deleted())
    {
        return true;
    }

    return m_idFace < m_pmesh->face_store_count() && !face().deleted()
        && m_idHalfedge < m_pmesh->halfedge_store_count() && !halfedge().deleted();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Hole<Traits, M>::set_mesh(mesh_type& mesh)
{
    m_pmesh = &mesh;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Hole<Traits, M>::mesh_type& quetzal::brep::Hole<Traits, M>::mesh() const
{
    assert(m_pmesh != nullptr);
    return *m_pmesh;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Hole<Traits, M>::mesh_type& quetzal::brep::Hole<Traits, M>::mesh()
{
    assert(m_pmesh != nullptr);
    return *m_pmesh;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Hole<Traits, M>::check_mesh(const mesh_type* const pmesh) const
{
    assert(m_pmesh == pmesh);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Hole<Traits, M>::halfedges_type::size_function_type quetzal::brep::Hole<Traits, M>::m_halfedges_size = [](const Hole<Traits, M>& hole) -> size_t
{
    size_t n = 0;
    for (auto i = hole.halfedges().cbegin(); i != hole.halfedges().cend(); ++i)
    {
        ++n;
    }

    return n;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Hole<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Hole<Traits, M>::m_halfedges_first = [](const Hole<Traits, M>& hole) -> id_type
{
    return hole.halfedge_id();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Hole<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Hole<Traits, M>::m_halfedges_last = [](const Hole<Traits, M>& hole) -> id_type
{
    return hole.halfedge().prev_id();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Hole<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Hole<Traits, M>::m_halfedges_end = [](const Hole<Traits, M>& hole) -> id_type
{
    hole;
    return nullid;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Hole<Traits, M>::halfedges_type::iterate_function_type quetzal::brep::Hole<Traits, M>::m_halfedges_forward = [](const Hole<Traits, M>& hole, id_type i) -> id_type
{
    i = hole.mesh().halfedge(i).next_id();
    return i != m_halfedges_first(hole) ? i : nullid;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Hole<Traits, M>::halfedges_type::iterate_function_type quetzal::brep::Hole<Traits, M>::m_halfedges_reverse = [](const Hole<Traits, M>& hole, id_type i) -> id_type
{
    i = hole.mesh().halfedge(i).prev_id();
    return i != m_halfedges_last(hole) ? i : nullid;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Hole<Traits, M>::halfedges_type::element_function_type quetzal::brep::Hole<Traits, M>::m_halfedges_element = [](Hole<Traits, M>& hole, id_type i) -> typename quetzal::brep::Hole<Traits, M>::halfedge_type&
{
    auto& halfedge = hole.mesh().halfedge(i);
    assert(!halfedge.deleted());
    return halfedge;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Hole<Traits, M>::halfedges_type::const_element_function_type quetzal::brep::Hole<Traits, M>::m_halfedges_const_element = [](const Hole<Traits, M>& hole, id_type i) -> const typename quetzal::brep::Hole<Traits, M>::halfedge_type&
{
    const auto& halfedge = hole.mesh().halfedge(i);
    assert(!halfedge.deleted());
    return halfedge;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
std::ostream& quetzal::brep::operator<<(std::ostream& os, const Hole<Traits, M>& hole)
{
    os << static_cast<Flags>(hole);

    if (!hole.deleted())
    {
        os << "\t" << hole.halfedge_count() << " edges";
        os << "\tf " << hole.face_id();
        os << "\th " << hole.halfedge_id();
        os << "\t" << hole.properties();
    }

    os << std::endl;
    return os;
}

#endif // QUETZAL_BREP_HOLE_HPP
