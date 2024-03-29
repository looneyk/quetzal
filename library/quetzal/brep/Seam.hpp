#if !defined(QUETZAL_BREP_SEAM_HPP)
#define QUETZAL_BREP_SEAM_HPP
//------------------------------------------------------------------------------
// brep
// Seam.hpp
//------------------------------------------------------------------------------

#include "Flags.hpp"
#include "id.hpp"
#include "mesh_util.hpp"
#include "quetzal/common/ElementsDirect.hpp"
#include "quetzal/common/Properties.hpp"
#include <iostream>
#include <cassert>

namespace quetzal::brep
{

    template<typename Traits>
    class Mesh;

    template<typename Traits, typename M>
    class Perimeter;

    //--------------------------------------------------------------------------
    template<typename Traits, typename M = Mesh<Traits>>
    class Seam : public Flags
    {
    public:

        using mesh_type = M;
        using surface_type = mesh_type::surface_type;
        using halfedge_type = mesh_type::halfedge_type;
        using halfedges_type = ElementsDirect<Seam, halfedge_type, id_type>;
        using size_type = Traits::size_type;

        Seam();
        Seam(mesh_type& mesh, id_type idNext = nullid, id_type idPrev = nullid, id_type idHalfedge = nullid, id_type idPerimeter = nullid);
        Seam(const Seam&) = default;
        Seam(Seam&&) noexcept = default;
        ~Seam() = default;

        Seam& operator=(const Seam&) = default;
        Seam& operator=(Seam&&) = default;

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

        id_type halfedge_id() const;
        void set_halfedge_id(id_type idHalfedge);

        const halfedge_type& halfedge() const; // ? ...
        halfedge_type& halfedge();

        // Virtual container interface, iterators for halfedges on this seam
        size_type halfedge_count() const;
        const halfedges_type& halfedges() const;
        halfedges_type& halfedges();

        id_type perimeter_id() const;

        const Perimeter<Traits, M>& perimeter() const;
        Perimeter<Traits, M>& perimeter();

        id_type surface_id() const;

        const surface_type& surface() const;
        surface_type& surface();

        const Properties& properties() const;
        Properties& properties();

        bool border() const;

        // Internal use
        const mesh_type& mesh() const;
        mesh_type& mesh();
        void set_mesh(mesh_type& mesh);
        void check_mesh(const mesh_type* const pmesh) const;

    private:

        mesh_type* m_pmesh;
        id_type m_idPartner;
        id_type m_idNext;
        id_type m_idPrev;
        id_type m_idHalfedge;
        id_type m_idPerimeter;
        halfedges_type m_halfedges;
        Properties m_properties;

        static halfedges_type::size_function_type m_halfedges_size;
        static halfedges_type::terminal_function_type m_halfedges_first;
        static halfedges_type::terminal_function_type m_halfedges_last;
        static halfedges_type::terminal_function_type m_halfedges_end;
        static halfedges_type::iterate_function_type m_halfedges_forward;
        static halfedges_type::iterate_function_type m_halfedges_reverse;
        static halfedges_type::element_function_type m_halfedges_element;
        static halfedges_type::const_element_function_type m_halfedges_const_element;
    };

    template<typename Traits, typename M>
    std::ostream& operator<<(std::ostream& os, const Seam<Traits, M>& Seam);

} // namespace quetzal::brep

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::brep::Seam<Traits, M>::Seam() :
    Flags(),
    m_pmesh(nullptr),
    m_idPartner(nullid),
    m_idNext(nullid),
    m_idPrev(nullid),
    m_idHalfedge(nullid),
    m_idPerimeter(nullid),
    m_halfedges(m_halfedges_size, m_halfedges_first, m_halfedges_last, m_halfedges_end, m_halfedges_forward, m_halfedges_reverse, m_halfedges_element, m_halfedges_const_element),
    m_properties()
{
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::brep::Seam<Traits, M>::Seam(mesh_type& mesh, id_type idNext, id_type idPrev, id_type idHalfedge, id_type idPerimeter) :
    Flags(),
    m_pmesh(&mesh),
    m_idPartner(nullid),
    m_idNext(idNext),
    m_idPrev(idPrev),
    m_idHalfedge(idHalfedge),
    m_idPerimeter(idPerimeter),
    m_halfedges(m_halfedges_size, m_halfedges_first, m_halfedges_last, m_halfedges_end, m_halfedges_forward, m_halfedges_reverse, m_halfedges_element, m_halfedges_const_element),
    m_properties()
{
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
quetzal::id_type quetzal::brep::Seam<Traits, M>::perimeter_id() const
{
    return m_idPerimeter;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const quetzal::brep::Perimeter<Traits, M>&  quetzal::brep::Seam<Traits, M>::perimeter() const
{
    return surface().perimeter(m_idPerimeter);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::brep::Perimeter<Traits, M>&  quetzal::brep::Seam<Traits, M>::perimeter()
{
    return surface.perimeter(m_idPerimeter);
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
    m_halfedges.set_source(*this);
    return m_halfedges;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>::halfedges_type& quetzal::brep::Seam<Traits, M>::halfedges()
{
    m_halfedges.set_source(*this);
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
typename const quetzal::Properties& quetzal::brep::Seam<Traits, M>::properties() const
{
    return m_properties;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::Properties& quetzal::brep::Seam<Traits, M>::properties()
{
    return m_properties;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
bool quetzal::brep::Seam<Traits, M>::border() const
{
    return m_idPartner == nullid;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Seam<Traits, M>::mesh_type& quetzal::brep::Seam<Traits, M>::mesh() const
{
    assert(m_pmesh != nullptr);
    return *m_pmesh;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>::mesh_type& quetzal::brep::Seam<Traits, M>::mesh()
{
    assert(m_pmesh != nullptr);
    return *m_pmesh;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Seam<Traits, M>::set_mesh(mesh_type& mesh)
{
    m_pmesh = &mesh;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Seam<Traits, M>::check_mesh(const mesh_type* const pmesh) const
{
    assert(m_pmesh == pmesh);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>::halfedges_type::size_function_type quetzal::brep::Seam<Traits, M>::m_halfedges_size = [](const Seam<Traits, M>& seam) -> size_t
{
    size_t n = 0;
    for (auto i = seam.halfedges().cbegin(); i != seam.halfedges().cend(); ++i)
    {
        ++n;
    }

    return n;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Seam<Traits, M>::m_halfedges_first = [](const Seam<Traits, M>& seam) -> id_type
{
    return seam.halfedge_id();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Seam<Traits, M>::m_halfedges_last = [](const Seam<Traits, M>& seam) -> id_type
{
    return prev_surface_halfedge_id(seam.mesh(), seam.next().halfedge_id());
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Seam<Traits, M>::m_halfedges_end = [](const Seam<Traits, M>& seam) -> id_type
{
    seam;
    return nullid;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>::halfedges_type::iterate_function_type quetzal::brep::Seam<Traits, M>::m_halfedges_forward = [](const Seam<Traits, M>& seam, id_type i) -> id_type
{
    assert(i != m_halfedges_end(seam));
    i = next_surface_halfedge_id(seam.mesh(), i);
    return i != m_halfedges_first(seam.next()) ? i : nullid;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>::halfedges_type::iterate_function_type quetzal::brep::Seam<Traits, M>::m_halfedges_reverse = [](const Seam<Traits, M>& seam, id_type i) -> id_type
{
    assert(i != m_halfedges_end(seam));
    i = prev_surface_halfedge_id(seam.mesh(), i);
    return i != m_halfedges_last(seam.prev()) ? i : nullid;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>::halfedges_type::element_function_type quetzal::brep::Seam<Traits, M>::m_halfedges_element = [](Seam<Traits, M>& seam, id_type i) -> quetzal::brep::Seam<Traits, M>::halfedge_type&
{
    auto& halfedge = seam.mesh().halfedge(i);
    assert(!halfedge.deleted());
    return halfedge;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Seam<Traits, M>::halfedges_type::const_element_function_type quetzal::brep::Seam<Traits, M>::m_halfedges_const_element = [](const Seam<Traits, M>& seam, id_type i) -> const quetzal::brep::Seam<Traits, M>::halfedge_type&
{
    const auto& halfedge = seam.mesh().halfedge(i);
    assert(!halfedge.deleted());
    return halfedge;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
std::ostream& quetzal::brep::operator<<(std::ostream& os, const Seam<Traits, M>& seam)
{
    os << static_cast<Flags>(seam);

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
        os << "\th" << seam.halfedge_id();
        os << "\tp" << seam.perimeter_id();
        os << "\t" << seam.properties();
    }

    os << std::endl;
    return os;
}

#endif // QUETZAL_BREP_SEAM_HPP
