#if !defined(QUETZAL_BREP_PERIMETER_HPP)
#define QUETZAL_BREP_PERIMETER_HPP
//------------------------------------------------------------------------------
// brep
// Perimeter.hpp
//------------------------------------------------------------------------------

#include "Flags.hpp"
#include "Seam.hpp"
#include "id.hpp"
#include "mesh_util.hpp"
#include "quetzal/common/ElementsDirect.hpp"
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
        using surface_type = mesh_type::surface_type;
        using halfedge_type = mesh_type::halfedge_type;
        using halfedges_type = ElementsDirect<Perimeter, halfedge_type, id_type>;
        using seam_type = Seam<Traits, M>;
        using seams_type = std::vector<seam_type>;
        using size_type = mesh_type::size_type;

        Perimeter();
        Perimeter(mesh_type& mesh, id_type idSurface);
        Perimeter(const Perimeter&) = default;
        Perimeter(Perimeter&&) noexcept = default;
        ~Perimeter() = default;

        Perimeter& operator=(const Perimeter&) = default;
        Perimeter& operator=(Perimeter&&) = default;

        id_type surface_id() const;
        void set_surface_id(id_type idSurface);

        const surface_type& surface() const;
        surface_type& surface();

        id_type halfedge_id() const;

        // Virtual container interface, iterators for halfedges that make up this perimeter
        size_type halfedge_count() const;
        const halfedges_type& halfedges() const;
        halfedges_type& halfedges();

        size_type seam_count() const;
        const seams_type& seams() const;
        seams_type& seams();

//        seam_type& seam(id_type idSeam); // only useful on generation? ...

        std::vector<id_type> find_seams(id_type idSurfacePartner);
        std::vector<id_type> find_seams(const std::string& nameSurfacePartner);

        id_type create_seam(id_type idNext, id_type idPrev, id_type idHalfedge, id_type idPerimeter);

        const Properties& properties() const;
        Properties& properties();

        // Internal use
        const mesh_type& mesh() const;
        mesh_type& mesh();
        void set_mesh(mesh_type& mesh);
        void check_mesh(const mesh_type* const pmesh) const;

    private:

        mesh_type* m_pmesh;
        id_type m_idSurface;
        halfedges_type m_halfedges;
        seams_type m_seams;
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
    std::ostream& operator<<(std::ostream& os, const Perimeter<Traits, M>& perimeter);

} // namespace quetzal::brep

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::brep::Perimeter<Traits, M>::Perimeter() :
    Flags(),
    m_pmesh(nullptr),
    m_idSurface(nullid),
    m_halfedges(m_halfedges_size, m_halfedges_first, m_halfedges_last, m_halfedges_end, m_halfedges_forward, m_halfedges_reverse, m_halfedges_element, m_halfedges_const_element),
    m_seams(),
    m_properties()
{
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
//quetzal::brep::Perimeter<Traits, M>::Perimeter(mesh_type& mesh, id_type idSurface, id_type idHalfedge) :
quetzal::brep::Perimeter<Traits, M>::Perimeter(mesh_type& mesh, id_type idSurface) :
    Flags(),
    m_pmesh(&mesh),
    m_idSurface(idSurface),
    m_halfedges(m_halfedges_size, m_halfedges_first, m_halfedges_last, m_halfedges_end, m_halfedges_forward, m_halfedges_reverse, m_halfedges_element, m_halfedges_const_element),
    m_seams(),
    m_properties()
{
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
//    return m_idHalfedge;
    assert(!m_seams.empty());
    return m_seams.front().halfedge_id();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::size_type quetzal::brep::Perimeter<Traits, M>::halfedge_count() const
{
    return m_halfedges.size();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Perimeter<Traits, M>::halfedges_type& quetzal::brep::Perimeter<Traits, M>::halfedges() const
{
    m_halfedges.set_source(*this);
    return m_halfedges;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::halfedges_type& quetzal::brep::Perimeter<Traits, M>::halfedges()
{
    m_halfedges.set_source(*this);
    return m_halfedges;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::size_type quetzal::brep::Perimeter<Traits, M>::seam_count() const
{
    surface().check_regenerate_perimeters();
    return m_seams.size();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename const quetzal::brep::Perimeter<Traits, M>::seams_type& quetzal::brep::Perimeter<Traits, M>::seams() const
{
    surface().check_regenerate_perimeters();
    return m_seams;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::seams_type& quetzal::brep::Perimeter<Traits, M>::seams()
{
    surface().check_regenerate_perimeters();
    return m_seams;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
std::vector<quetzal::id_type> quetzal::brep::Perimeter<Traits, M>::find_seams(id_type idSurfacePartner)
{
    surface().check_regenerate_perimeters();
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
std::vector<quetzal::id_type> quetzal::brep::Perimeter<Traits, M>::find_seams(const std::string& nameSurfacePartner)
{
    surface().check_regenerate_perimeters();
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
quetzal::id_type quetzal::brep::Perimeter<Traits, M>::create_seam(id_type idNext, id_type idPrev, id_type idHalfedge, id_type idPerimeter)
{
    id_type idSeam = m_seams.size();
    m_seams.emplace_back(*m_pmesh, idNext, idPrev, idHalfedge, idPerimeter);
    return idSeam;
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
const typename quetzal::brep::Perimeter<Traits, M>::mesh_type& quetzal::brep::Perimeter<Traits, M>::mesh() const
{
    assert(m_pmesh != nullptr);
    return *m_pmesh;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::mesh_type& quetzal::brep::Perimeter<Traits, M>::mesh()
{
    assert(m_pmesh != nullptr);
    return *m_pmesh;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Perimeter<Traits, M>::set_mesh(mesh_type& mesh)
{
    m_pmesh = &mesh;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Perimeter<Traits, M>::check_mesh(const mesh_type* const pmesh) const
{
    assert(m_pmesh == pmesh);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::halfedges_type::size_function_type quetzal::brep::Perimeter<Traits, M>::m_halfedges_size = [](const Perimeter<Traits, M>& perimeter) -> size_t
{
    size_t n = 0;
    for (auto i = perimeter.halfedges().cbegin(); i != perimeter.halfedges().cend(); ++i)
    {
        ++n;
    }

    return n;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Perimeter<Traits, M>::m_halfedges_first = [](const Perimeter<Traits, M>& perimeter) -> id_type
{
    return perimeter.halfedge_id();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Perimeter<Traits, M>::m_halfedges_last = [](const Perimeter<Traits, M>& perimeter) -> id_type
{
    return prev_surface_halfedge_id(perimeter.mesh(), perimeter.halfedge_id());
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Perimeter<Traits, M>::m_halfedges_end = [](const Perimeter<Traits, M>& perimeter) -> id_type
{
    perimeter;
    return nullid;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::halfedges_type::iterate_function_type quetzal::brep::Perimeter<Traits, M>::m_halfedges_forward = [](const Perimeter<Traits, M>& perimeter, id_type i) -> id_type
{
    assert(i != m_halfedges_end(perimeter));
    i = next_surface_halfedge_id(perimeter.mesh(), i);
    return i != m_halfedges_first(perimeter) ? i : nullid;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::halfedges_type::iterate_function_type quetzal::brep::Perimeter<Traits, M>::m_halfedges_reverse = [](const Perimeter<Traits, M>& perimeter, id_type i) -> id_type
{
    assert(i != m_halfedges_end(perimeter));
    i = prev_surface_halfedge_id(perimeter.mesh(), i);
    return i != m_halfedges_last(perimeter) ? i : nullid;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::halfedges_type::element_function_type quetzal::brep::Perimeter<Traits, M>::m_halfedges_element = [](Perimeter<Traits, M>& perimeter, id_type i) -> quetzal::brep::Perimeter<Traits, M>::halfedge_type&
{
    auto& halfedge = perimeter.mesh().halfedge(i);
    assert(!halfedge.deleted());
    return halfedge;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Perimeter<Traits, M>::halfedges_type::const_element_function_type quetzal::brep::Perimeter<Traits, M>::m_halfedges_const_element = [](const Perimeter<Traits, M>& perimeter, id_type i) -> const quetzal::brep::Perimeter<Traits, M>::halfedge_type&
{
    const auto& halfedge = perimeter.mesh().halfedge(i);
    assert(!halfedge.deleted());
    return halfedge;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
std::ostream& quetzal::brep::operator<<(std::ostream& os, const Perimeter<Traits, M>& perimeter)
{
    os << static_cast<Flags>(perimeter);

    if (!perimeter.deleted())
    {
        os << "\t" << perimeter.halfedge_count() << " edges";
        os << "\ts " << perimeter.surface_id();
        os << "\th " << perimeter.halfedge_id();
        os << "\t" << perimeter.properties();
    }

    os << std::endl;
    return os;
}

#endif // QUETZAL_BREP_PERIMETER_HPP
