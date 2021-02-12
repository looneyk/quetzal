#if !defined(QUETZAL_BREP_VERTEX_HPP)
#define QUETZAL_BREP_VERTEX_HPP
//------------------------------------------------------------------------------
// brep
// Vertex.hpp
//------------------------------------------------------------------------------

#include "Flags.hpp"
#include "id.hpp"
#include "quetzal/common/Elements.hpp"
#include "quetzal/common/Properties.hpp"
#include <iostream>
#include <set>
#include <cassert>

namespace quetzal::brep
{

    template<typename Traits>
    class Mesh;

    //--------------------------------------------------------------------------
    template<typename Traits, typename M = Mesh<Traits>>
    class Vertex : public Flags, public Properties
    {
    public:

        using mesh_type = M;
        using halfedge_type = typename mesh_type::halfedge_type;
        using halfedge_ids_type = std::set<id_type>;
        using halfedges_type = Elements<mesh_type, halfedge_type, id_type>;
        using attributes_type = typename mesh_type::vertex_attributes_type;
        using size_type = typename mesh_type::size_type;

        Vertex();
        Vertex(mesh_type& mesh, id_type id, id_type idHalfedge, const attributes_type& attributes);
        Vertex(mesh_type& mesh, id_type id, id_type idHalfedge, attributes_type&& attributes);
        Vertex(const Vertex&) = default;
        Vertex(Vertex&&) noexcept = default;
        ~Vertex() = default;

        Vertex& operator=(const Vertex&) = default;
        Vertex& operator=(Vertex&&) = default;

        id_type id() const;

        id_type halfedge_id() const;
        void set_halfedge_id(id_type idHalfedge);

        const halfedge_type& halfedge() const;
        halfedge_type& halfedge();

        const attributes_type& attributes() const;
        attributes_type& attributes();
        void set_attributes(const attributes_type& attributes);

        const halfedge_ids_type& halfedge_ids() const; // Not implemented, only makes sense in the unique vertex position case ...
        halfedge_ids_type& halfedge_ids(); // Not implemented, only makes sense in the unique vertex position case ...

        // Virtual container interface, iterators for halfedges based at this vertex position
        const halfedges_type& halfedges() const;
        halfedges_type& halfedges();

        size_type halfedge_count() const;

        bool check() const;

        // Internal use, only by Mesh
        void set_mesh(mesh_type& mesh);
        void set_id(id_type id);
        const mesh_type* mesh() const;
        void check_mesh(const mesh_type* const pmesh) const;

    private:

        mesh_type* m_pmesh;
        id_type m_id;
        id_type m_idHalfedge; // no longer meaningful once/if halfedge_ids implemented ...
        halfedge_ids_type m_halfedge_ids;
        attributes_type m_attributes;

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
    std::ostream& operator<<(std::ostream& os, const Vertex<Traits, M>& vertex);

} // namespace quetzal::brep

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::brep::Vertex<Traits, M>::Vertex() :
    Flags(),
    Properties(),
    m_pmesh(nullptr),
    m_id(nullid),
    m_idHalfedge(nullid),
    m_halfedge_ids(),
    m_attributes(),
    m_halfedges(*m_pmesh, nullid, m_halfedges_size, m_halfedges_first, m_halfedges_last, m_halfedges_end, m_halfedges_forward, m_halfedges_reverse, m_halfedges_element, m_halfedges_const_element)
{
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::brep::Vertex<Traits, M>::Vertex(mesh_type& mesh, id_type id, id_type idHalfedge, const attributes_type& attributes) :
    Flags(),
    Properties(),
    m_pmesh(&mesh),
    m_id(id),
    m_idHalfedge(idHalfedge),
    m_halfedge_ids(),
    m_attributes(attributes),
    m_halfedges(mesh, id, m_halfedges_size, m_halfedges_first, m_halfedges_last, m_halfedges_end, m_halfedges_forward, m_halfedges_reverse, m_halfedges_element, m_halfedges_const_element)
{
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::brep::Vertex<Traits, M>::Vertex(mesh_type& mesh, id_type id, id_type idHalfedge, attributes_type&& attributes) :
    Flags(),
    Properties(),
    m_pmesh(&mesh),
    m_id(id),
    m_idHalfedge(idHalfedge),
    m_halfedge_ids(),
    m_attributes(attributes),
    m_halfedges(mesh, id, m_halfedges_size, m_halfedges_first, m_halfedges_last, m_halfedges_end, m_halfedges_forward, m_halfedges_reverse, m_halfedges_element, m_halfedges_const_element)
{
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Vertex<Traits, M>::id() const
{
    return m_id;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Vertex<Traits, M>::halfedge_id() const
{
    return m_idHalfedge;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Vertex<Traits, M>::set_halfedge_id(id_type idHalfedge)
{
    m_idHalfedge = idHalfedge;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Vertex<Traits, M>::halfedge_type& quetzal::brep::Vertex<Traits, M>::halfedge() const
{
    assert(m_pmesh != nullptr);
    return m_pmesh->halfedge(m_idHalfedge);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Vertex<Traits, M>::halfedge_type& quetzal::brep::Vertex<Traits, M>::halfedge()
{
    assert(m_pmesh != nullptr);
    return m_pmesh->halfedge(m_idHalfedge);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename const quetzal::brep::Vertex<Traits, M>::attributes_type& quetzal::brep::Vertex<Traits, M>::attributes() const
{
    return m_attributes;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Vertex<Traits, M>::attributes_type& quetzal::brep::Vertex<Traits, M>::attributes()
{
    return m_attributes;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Vertex<Traits, M>::set_attributes(const attributes_type& attributes)
{
    m_attributes = attributes;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Vertex<Traits, M>::halfedge_ids_type& quetzal::brep::Vertex<Traits, M>::halfedge_ids() const
{
    return m_halfedge_ids;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Vertex<Traits, M>::halfedge_ids_type& quetzal::brep::Vertex<Traits, M>::halfedge_ids()
{
    return m_halfedge_ids;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Vertex<Traits, M>::halfedges_type& quetzal::brep::Vertex<Traits, M>::halfedges() const
{
    return m_halfedges;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Vertex<Traits, M>::halfedges_type& quetzal::brep::Vertex<Traits, M>::halfedges()
{
    return m_halfedges;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Vertex<Traits, M>::size_type quetzal::brep::Vertex<Traits, M>::halfedge_count() const
{
    return m_halfedges.size();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
bool quetzal::brep::Vertex<Traits, M>::check() const
{
    assert(m_pmesh != nullptr);

    if (deleted())
    {
        return true;
    }

    return m_id < m_pmesh->vertex_store_count()
        && m_idHalfedge < m_pmesh->halfedge_store_count()
        && !halfedge().deleted();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Vertex<Traits, M>::set_mesh(mesh_type& mesh)
{
    m_pmesh = &mesh;
    m_halfedges.set_source(mesh);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Vertex<Traits, M>::set_id(id_type id)
{
    m_id = id;
    m_halfedges.set_id(id);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Vertex<Traits, M>::mesh_type* quetzal::brep::Vertex<Traits, M>::mesh() const
{
    return m_pmesh;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Vertex<Traits, M>::check_mesh(const mesh_type* const pmesh) const
{
    assert(m_pmesh == pmesh);
    assert(m_halfedges.mesh() == pmesh);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Vertex<Traits, M>::halfedges_type::size_function_type quetzal::brep::Vertex<Traits, M>::m_halfedges_size = [](const mesh_type& mesh, id_type id) -> size_t
{
    size_t n = 0;

    const auto& vertex = mesh.vertex(id);
    for (auto i = vertex.halfedges().cbegin(); i != vertex.halfedges().cend(); ++i)
    {
        ++n;
    }

    return n;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Vertex<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Vertex<Traits, M>::m_halfedges_first = [](const mesh_type& mesh, id_type id) -> id_type
{
    return mesh.vertex(id).halfedge_id();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Vertex<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Vertex<Traits, M>::m_halfedges_last = [](const mesh_type& mesh, id_type id) -> id_type
{
    return mesh.vertex(id).halfedge().partner().next_id();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Vertex<Traits, M>::halfedges_type::terminal_function_type quetzal::brep::Vertex<Traits, M>::m_halfedges_end = [](const mesh_type& mesh, id_type id) -> id_type
{
    mesh;
    id;
    return nullid;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Vertex<Traits, M>::halfedges_type::iterate_function_type quetzal::brep::Vertex<Traits, M>::m_halfedges_forward = [](const mesh_type& mesh, id_type id, id_type i) -> id_type
{
	i = mesh.halfedge(i).prev().partner_id();
    if (i == m_halfedges_first(mesh, id))
    {
        i = nullid;
    }

	return i;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Vertex<Traits, M>::halfedges_type::iterate_function_type quetzal::brep::Vertex<Traits, M>::m_halfedges_reverse = [](const mesh_type& mesh, id_type id, id_type i) -> id_type
{
    i = mesh.halfedge(i).partner().next_id();
    if (i == m_halfedges_last(mesh, id))
    {
        i = nullid;
    }

	return i;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Vertex<Traits, M>::halfedges_type::element_function_type quetzal::brep::Vertex<Traits, M>::m_halfedges_element = [](mesh_type& mesh, id_type id, id_type i) -> typename quetzal::brep::Vertex<Traits, M>::halfedge_type&
{
    id;
    auto& halfedge = mesh.halfedge(i);
    assert(!halfedge.deleted());
    return halfedge;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Vertex<Traits, M>::halfedges_type::const_element_function_type quetzal::brep::Vertex<Traits, M>::m_halfedges_const_element = [](const mesh_type& mesh, id_type id, id_type i) -> const typename quetzal::brep::Vertex<Traits, M>::halfedge_type&
{
    id;
    const auto& halfedge = mesh.halfedge(i);
    assert(!halfedge.deleted());
    return halfedge;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
std::ostream& quetzal::brep::operator<<(std::ostream& os, const Vertex<Traits, M>& vertex)
{
    os << static_cast<Flags>(vertex);
    os << "\t" << vertex.id();

    if (!vertex.deleted())
    {
        os << "\t" << vertex.halfedge_id();
        if (vertex.halfedge().border())
        {
            os << "\tborder";
        }
        else
        {
            os << "\t" << vertex.halfedge_count() << " edges";
        }

        os << "\t" << vertex.attributes();
    }

    os << std::endl;
    return os;
}

#endif // QUETZAL_BREP_VERTEX_HPP
