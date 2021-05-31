#if !defined(QUETZAL_BREP_PERIMETER_HPP)
#define QUETZAL_BREP_PERIMETER_HPP
//------------------------------------------------------------------------------
// brep
// Perimeter.hpp
//------------------------------------------------------------------------------

#include "Flags.hpp"
#include "Seam.hpp"
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
    class Perimeter : public Flags
    {
    public:

        using mesh_type = M;
        using surface_type = typename mesh_type::surface_type;
        using halfedge_type = typename mesh_type::halfedge_type;
        using halfedges_type = Elements<mesh_type, halfedge_type, id_type>;
        using seam_type = Seam<Traits, M>;
        using seams_type = std::vector<seam_type>;
        using size_type = typename mesh_type::size_type;

        Perimeter();
        Perimeter(mesh_type& mesh, id_type id, id_type idSurface, id_type idHalfedge);
        Perimeter(const Perimeter&) = default;
        Perimeter(Perimeter&&) noexcept = default;
        ~Perimeter() = default;

        Perimeter& operator=(const Perimeter&) = default;
        Perimeter& operator=(Perimeter&&) = default;

        id_type id() const;

        id_type surface_id() const;
        void set_surface_id(id_type idSurface);

        const surface_type& surface() const;
        surface_type& surface();

        id_type halfedge_id() const;
        void set_halfedge_id(id_type idHalfedge);

        const halfedge_type& halfedge() const; // ? ...
        halfedge_type& halfedge();

        // Virtual container interface, iterators for halfedges that make up this perimeter
        size_type halfedge_count() const;
        const halfedges_type& halfedges() const;
        halfedges_type& halfedges();

        size_type seam_count() const;
        const seams_type& seams() const;
        seams_type& seams();

        const Properties& properties() const;
        Properties& properties();

        bool check() const;

        // Internal use, only by Mesh
        void set_mesh(mesh_type& mesh);
        void set_id(id_type id);
        const mesh_type* mesh() const;
        void check_mesh(const mesh_type* const pmesh) const;

    private:

        mesh_type* m_pmesh;
        id_type m_id;
        id_type m_idSurface;
        id_type m_idHalfedge;
        halfedges_type m_halfedges;
        seams_type m_seams;
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
    std::ostream& operator<<(std::ostream& os, const Perimeter<Traits, M>& perimeter);

} // namespace quetzal::brep

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::brep::Perimeter<Traits, M>::Perimeter() :
    Flags(),
    m_pmesh(nullptr),
    m_id(nullid),
    m_idSurface(nullid),
    m_idHalfedge(nullid),
    m_halfedges(*m_pmesh, nullid, m_halfedges_size, m_halfedges_first, m_halfedges_last, m_halfedges_end, m_halfedges_forward, m_halfedges_reverse, m_halfedges_element, m_halfedges_const_element),
    m_seams(),
    m_properties()
{
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::brep::Perimeter<Traits, M>::Perimeter(mesh_type& mesh, id_type id, id_type idSurface, id_type idHalfedge) :
    Flags(),
    m_pmesh(&mesh),
    m_id(id),
    m_idSurface(idSurface),
    m_idHalfedge(idHalfedge),
    m_halfedges(mesh, id, m_halfedges_size, m_halfedges_first, m_halfedges_last, m_halfedges_end, m_halfedges_forward, m_halfedges_reverse, m_halfedges_element, m_halfedges_const_element),
    m_seams(),
    m_properties()
{
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Perimeter<Traits, M>::id() const
{
    return m_id;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Perimeter<Traits, M>::surface_id() const
{
    return m_idSurface;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Perimeter<Traits, M>::set_surface_id(id_type idSurface)
{
    m_idSurface = idSurface;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Perimeter<Traits, M>::surface_type& quetzal::brep::Perimeter<Traits, M>::surface() const
{
    assert(m_pmesh != nullptr);
    return m_pmesh->surface(m_idSurface);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::surface_type& quetzal::brep::Perimeter<Traits, M>::surface()
{
    assert(m_pmesh != nullptr);
    return m_pmesh->surface(m_idSurface);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Perimeter<Traits, M>::halfedge_id() const
{
    return m_idHalfedge;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Perimeter<Traits, M>::set_halfedge_id(id_type idHalfedge)
{
    m_idHalfedge = idHalfedge;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::size_type quetzal::brep::Perimeter<Traits, M>::halfedge_count() const
{
    return m_halfedges.size();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Perimeter<Traits, M>::halfedge_type& quetzal::brep::Perimeter<Traits, M>::halfedge() const
{
    assert(m_pmesh != nullptr);
    return m_pmesh->halfedge(m_idHalfedge);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::halfedge_type& quetzal::brep::Perimeter<Traits, M>::halfedge()
{
    assert(m_pmesh != nullptr);
    return m_pmesh->halfedge(m_idHalfedge);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::size_type quetzal::brep::Perimeter<Traits, M>::seam_count() const
{
    surface().check_regenerate_seams();
    return m_seams.size();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename const quetzal::brep::Perimeter<Traits, M>::seams_type& quetzal::brep::Perimeter<Traits, M>::seams() const
{
    surface().check_regenerate_seams();
    return m_seams;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::seams_type& quetzal::brep::Perimeter<Traits, M>::seams()
{
    surface().check_regenerate_seams();
    return m_seams;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename const quetzal::Properties& quetzal::brep::Perimeter<Traits, M>::properties() const
{
    return m_properties;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::Properties& quetzal::brep::Perimeter<Traits, M>::properties()
{
    return m_properties;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Perimeter<Traits, M>::halfedges_type& quetzal::brep::Perimeter<Traits, M>::halfedges() const
{
    return m_halfedges;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::halfedges_type& quetzal::brep::Perimeter<Traits, M>::halfedges()
{
    return m_halfedges;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
bool quetzal::brep::Perimeter<Traits, M>::check() const
{
    assert(m_pmesh != nullptr);

    if (deleted())
    {
        return true;
    }

    return m_idHalfedge < m_pmesh->halfedge_store_count()
        && !halfedge().deleted();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Perimeter<Traits, M>::set_mesh(mesh_type& mesh)
{
    m_pmesh = &mesh;
    m_halfedges.set_source(mesh);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Perimeter<Traits, M>::set_id(id_type id)
{
    m_id = id;
    m_halfedges.set_id(id);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Perimeter<Traits, M>::mesh_type* quetzal::brep::Perimeter<Traits, M>::mesh() const
{
    return m_pmesh;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Perimeter<Traits, M>::check_mesh(const mesh_type* const pmesh) const
{
    assert(m_pmesh == pmesh);
    assert(m_halfedges.mesh() == pmesh);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::halfedges_type::size_function_type quetzal::brep::Perimeter<Traits, M>::m_halfedges_size = [](const mesh_type& mesh, id_type id) -> size_t
{
    size_t n = 0;

    const auto& perimeter = mesh.perimeter(id);
    for (auto i = perimeter.halfedges().cbegin(); i != perimeter.halfedges().cend(); ++i)
    {
        ++n;
    }

    return n;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Perimeter<Traits, M>::m_halfedges_first = [](const mesh_type& mesh, id_type id) -> id_type
{
    return mesh.perimeter(id).halfedge_id();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Perimeter<Traits, M>::m_halfedges_last = [](const mesh_type& mesh, id_type id) -> id_type
{
    return mesh.perimeter(id).halfedge().prev_id();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Perimeter<Traits, M>::m_halfedges_end = [](const mesh_type& mesh, id_type id) -> id_type
{
    mesh;
    id;
    return nullid;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::halfedges_type::iterate_function_type quetzal::brep::Perimeter<Traits, M>::m_halfedges_forward = [](const mesh_type& mesh, id_type id, id_type i) -> id_type
{
    i = mesh.halfedge(i).next_id();
    return i != m_halfedges_first(mesh, id) ? i : nullid;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::halfedges_type::iterate_function_type quetzal::brep::Perimeter<Traits, M>::m_halfedges_reverse = [](const mesh_type& mesh, id_type id, id_type i) -> id_type
{
    i = mesh.halfedge(i).prev_id();
    return i != m_halfedges_last(mesh, id) ? i : nullid;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::halfedges_type::element_function_type quetzal::brep::Perimeter<Traits, M>::m_halfedges_element = [](mesh_type& mesh, id_type id, id_type i) -> typename quetzal::brep::Perimeter<Traits, M>::halfedge_type&
{
    id;
    auto& halfedge = mesh.halfedge(i);
    assert(!halfedge.deleted());
    return halfedge;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::halfedges_type::const_element_function_type quetzal::brep::Perimeter<Traits, M>::m_halfedges_const_element = [](const mesh_type& mesh, id_type id, id_type i) -> const typename quetzal::brep::Perimeter<Traits, M>::halfedge_type&
{
    id;
    const auto& halfedge = mesh.halfedge(i);
    assert(!halfedge.deleted());
    return halfedge;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
std::ostream& quetzal::brep::operator<<(std::ostream& os, const Perimeter<Traits, M>& perimeter)
{
    os << static_cast<Flags>(perimeter);
    os << "\t" << perimeter.id();

    if (!perimeter.deleted())
    {
        os << "\t" << perimeter.vertex_count() << " edges";
        os << "\tf " << perimeter.surface_id();
        os << "\t" << perimeter.properties();
    }

    os << std::endl;
    return os;
}

#endif // QUETZAL_BREP_PERIMETER_HPP
