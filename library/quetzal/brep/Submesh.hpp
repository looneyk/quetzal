#if !defined(QUETZAL_BREP_SUBMESH_HPP)
#define QUETZAL_BREP_SUBMESH_HPP
//------------------------------------------------------------------------------
// brep
// Submesh.hpp
//------------------------------------------------------------------------------

#include "Flags.hpp"
#include "id.hpp"
#include "quetzal/common/Elements.hpp"
#include "quetzal/common/Properties.hpp"
#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <cassert>

namespace quetzal::brep
{

    template<typename Traits>
    class Mesh;

    //--------------------------------------------------------------------------
    template<typename Traits, typename M = Mesh<Traits>>
    class Submesh : public Flags
    {
    public:

        using traits_type = Traits;
        using mesh_type = M;
        using value_type = traits_type::value_type;
        using point_type = traits_type::point_type;
        using vector_type = mesh_type::vector_type;
        using vertex_type = mesh_type::vertex_type;
        using face_type = mesh_type::face_type;
        using face_ids_type = std::set<id_type>;
        using faces_type = Elements<mesh_type, face_type, face_ids_type::iterator>;
        using surface_type = mesh_type::surface_type;
        using surface_ids_type = std::set<id_type>;
        using surfaces_type = Elements<mesh_type, surface_type, surface_ids_type::iterator>;
        using index_type = mesh_type::index_type;
        using attributes_type = mesh_type::submesh_attributes_type;
        using size_type = traits_type::size_type;

        Submesh();
        Submesh(mesh_type& mesh, id_type id, const std::string& name, const attributes_type& attributes, const Properties& properties = {});
        Submesh(const Submesh&) = default;
        Submesh(Submesh&&) noexcept = default;
        ~Submesh() = default;

        Submesh& operator=(const Submesh&) = default;
        Submesh& operator=(Submesh&&) = default;

        id_type id() const;

        const std::string& name() const;
        void set_name(const std::string& name);

        size_type face_count() const;

        const face_ids_type& face_ids() const;
        face_ids_type& face_ids();

        const face_type& face(id_type id) const;
        face_type& face(id_type id);

        const faces_type& faces() const;
        faces_type& faces();

        bool contains_face(id_type idFace) const;
        void add_face(id_type idFace); // add face to list
        void remove_face(id_type idFace); // remove face from list
        void link_face(id_type idFace); // add face to list and set face submesh
        void unlink_face(id_type idFace); // remove face from list and clear face submesh

        size_type surface_count() const;

        const surface_ids_type& surface_ids() const;
        surface_ids_type& surface_ids();

        const surface_type& surface(id_type id) const;
        surface_type& surface(id_type id);

        id_type surface_id(const std::string& name) const;

        const surface_type& surface(const std::string& name) const;
        surface_type& surface(const std::string& name);

        const surfaces_type& surfaces() const;
        surfaces_type& surfaces();

        const attributes_type& attributes() const;
        attributes_type& attributes();
        void set_attributes(const attributes_type& attributes);

        const Properties& properties() const;
        Properties& properties();

        bool contains_surface(const std::string& name) const;
        bool contains_surface(id_type idSurface) const;
        void add_surface(id_type idSurface); // add surface to list
        void remove_surface(id_type idSurface); // remove surface from list
        void link_surface(id_type idSurface); // add surface to list and set surface submesh
        void unlink_surface(id_type idSurface); // remove surface from list and clear surface submesh

        void remove_surfaces();

        // renames only references in this submesh, not those in mesh
        void rename_surface(id_type idSurface, const std::string& replacement);
        void rename_surfaces(std::function<std::string(const std::string&)> renamer);

        size_type surface_index_count() const;
        const index_type& surface_index() const; // debug, logging, ...
        index_type& surface_index(); // debug, logging, ...
        void regenerate_surface_index(); // Surfaces with the same name should have already been coalesced

// not as implemented, but same as mesh ...
//        void append(const Submesh& submesh);

        bool empty() const;
        void clear(); // Clears contents, but does not change id or name

        bool unmarked() const;
        void reset() const override; // Flags

        // Internal use, only by Mesh
        void set_mesh(mesh_type& mesh);
        void set_id(id_type id);
        void check_mesh(const mesh_type* const pmesh) const;

    private:

        mesh_type* m_pmesh;
        id_type m_id;
        std::string m_name;

        face_ids_type m_face_ids;
        faces_type m_faces;

        surface_ids_type m_surface_ids;
        surfaces_type m_surfaces;
        index_type m_surface_index;

        attributes_type m_attributes;
        Properties m_properties;

        static faces_type::size_function_type m_faces_size;
        static faces_type::terminal_function_type m_faces_first;
        static faces_type::terminal_function_type m_faces_last;
        static faces_type::terminal_function_type m_faces_end;
        static faces_type::iterate_function_type m_faces_forward;
        static faces_type::iterate_function_type m_faces_reverse;
        static faces_type::element_function_type m_faces_element;
        static faces_type::const_element_function_type m_faces_const_element;

        static surfaces_type::size_function_type m_surfaces_size;
        static surfaces_type::terminal_function_type m_surfaces_first;
        static surfaces_type::terminal_function_type m_surfaces_last;
        static surfaces_type::terminal_function_type m_surfaces_end;
        static surfaces_type::iterate_function_type m_surfaces_forward;
        static surfaces_type::iterate_function_type m_surfaces_reverse;
        static surfaces_type::element_function_type m_surfaces_element;
        static surfaces_type::const_element_function_type m_surfaces_const_element;
    };

    template<typename Traits, typename M>
    std::ostream& operator<<(std::ostream& os, const Submesh<Traits, M>& submesh);

} // namespace quetzal::brep

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::brep::Submesh<Traits, M>::Submesh() :
    Flags(),
    m_pmesh(nullptr),
    m_id(nullid),
    m_name(),
    m_face_ids(),
    m_faces(*m_pmesh, nullid, m_faces_size, m_faces_first, m_faces_last, m_faces_end, m_faces_forward, m_faces_reverse, m_faces_element, m_faces_const_element),
    m_surface_ids(),
    m_surfaces(*m_pmesh, nullid, m_surfaces_size, m_surfaces_first, m_surfaces_last, m_surfaces_end, m_surfaces_forward, m_surfaces_reverse, m_surfaces_element, m_surfaces_const_element),
    m_surface_index(),
    m_attributes(),
    m_properties()
{
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::brep::Submesh<Traits, M>::Submesh(mesh_type& mesh, id_type id, const std::string& name, const attributes_type& attributes, const Properties& properties) :
    Flags(),
    m_pmesh(&mesh),
    m_id(id),
    m_name(name),
    m_face_ids(),
    m_faces(mesh, id, m_faces_size, m_faces_first, m_faces_last, m_faces_end, m_faces_forward, m_faces_reverse, m_faces_element, m_faces_const_element),
    m_surface_ids(),
    m_surfaces(mesh, id, m_surfaces_size, m_surfaces_first, m_surfaces_last, m_surfaces_end, m_surfaces_forward, m_surfaces_reverse, m_surfaces_element, m_surfaces_const_element),
    m_surface_index(),
    m_attributes(attributes),
    m_properties(properties)
{
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Submesh<Traits, M>::id() const
{
    return m_id;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const std::string& quetzal::brep::Submesh<Traits, M>::name() const
{
    return m_name;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Submesh<Traits, M>::set_name(const std::string& name)
{
    m_name = name;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::size_type quetzal::brep::Submesh<Traits, M>::face_count() const
{
    return m_face_ids.size();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Submesh<Traits, M>::face_ids_type& quetzal::brep::Submesh<Traits, M>::face_ids() const
{
    return m_face_ids;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::face_ids_type& quetzal::brep::Submesh<Traits, M>::face_ids()
{
    return m_face_ids;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Submesh<Traits, M>::face_type& quetzal::brep::Submesh<Traits, M>::face(id_type id) const
{
    return m_pmesh->face(id);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::face_type& quetzal::brep::Submesh<Traits, M>::face(id_type id)
{
    return m_pmesh->face(id);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Submesh<Traits, M>::faces_type& quetzal::brep::Submesh<Traits, M>::faces() const
{
    return m_faces;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::faces_type& quetzal::brep::Submesh<Traits, M>::faces()
{
    return m_faces;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
bool quetzal::brep::Submesh<Traits, M>::contains_face(id_type idFace) const
{
    assert(m_pmesh != nullptr);
    return m_face_ids.contains(idFace);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Submesh<Traits, M>::add_face(id_type idFace)
{
    assert(!m_face_ids.contains(idFace));
    m_face_ids.insert(idFace);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Submesh<Traits, M>::remove_face(id_type idFace)
{
    assert(m_face_ids.contains(idFace));
    m_face_ids.erase(idFace);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Submesh<Traits, M>::link_face(id_type idFace)
{
    assert(m_pmesh != nullptr);
    add_face(idFace);
    m_pmesh->face(idFace).set_submesh_id(m_id);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Submesh<Traits, M>::unlink_face(id_type idFace)
{
    assert(m_pmesh != nullptr);
    remove_face(idFace);
    m_pmesh->face(idFace).set_submesh_id(nullid);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::size_type quetzal::brep::Submesh<Traits, M>::surface_count() const
{
    return m_surface_ids.size();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Submesh<Traits, M>::surface_ids_type& quetzal::brep::Submesh<Traits, M>::surface_ids() const
{
    return m_surface_ids;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::surface_ids_type& quetzal::brep::Submesh<Traits, M>::surface_ids()
{
    return m_surface_ids;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Submesh<Traits, M>::surface_type& quetzal::brep::Submesh<Traits, M>::surface(id_type id) const
{
    assert(m_pmesh != nullptr);
    return m_pmesh->surface(id);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::surface_type& quetzal::brep::Submesh<Traits, M>::surface(id_type id)
{
    assert(m_pmesh != nullptr);
    return m_pmesh->surface(id);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Submesh<Traits, M>::surface_id(const std::string& name) const
{
    auto i = m_surface_index.find(name);
    if (i == m_surface_index.end())
    {
        return nullid;
    }

    return i->second;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Submesh<Traits, M>::surface_type& quetzal::brep::Submesh<Traits, M>::surface(const std::string& name) const
{
    assert(contains_surface(name));
    return surface(m_surface_index.find(name)->second);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::surface_type& quetzal::brep::Submesh<Traits, M>::surface(const std::string& name)
{
    assert(contains_surface(name));
    return surface(m_surface_index.find(name)->second);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Submesh<Traits, M>::surfaces_type& quetzal::brep::Submesh<Traits, M>::surfaces() const
{
    return m_surfaces;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::surfaces_type& quetzal::brep::Submesh<Traits, M>::surfaces()
{
    return m_surfaces;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename const quetzal::brep::Submesh<Traits, M>::attributes_type& quetzal::brep::Submesh<Traits, M>::attributes() const
{
    return m_attributes;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::attributes_type& quetzal::brep::Submesh<Traits, M>::attributes()
{
    return m_attributes;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Submesh<Traits, M>::set_attributes(const attributes_type& attributes)
{
    m_attributes = attributes;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename const quetzal::Properties& quetzal::brep::Submesh<Traits, M>::properties() const
{
    return m_properties;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::Properties& quetzal::brep::Submesh<Traits, M>::properties()
{
    return m_properties;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
bool quetzal::brep::Submesh<Traits, M>::contains_surface(const std::string& name) const
{
    return m_surface_index.contains(name);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
bool quetzal::brep::Submesh<Traits, M>::contains_surface(id_type idSurface) const
{
    assert(m_pmesh != nullptr);
    return m_surface_ids.contains(idSurface);
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Submesh<Traits, M>::add_surface(id_type idSurface)
{
    assert(idSurface != nullid);

    assert(!m_surface_ids.contains(idSurface));
    assert(!m_surface_index.contains(m_pmesh->surface(idSurface).name()));

    m_surface_ids.insert(idSurface);
    m_surface_index[m_pmesh->surface(idSurface).name()] = idSurface;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Submesh<Traits, M>::remove_surface(id_type idSurface)
{
    assert(m_pmesh != nullptr);
    assert(idSurface != nullid);
    assert(m_surface_ids.contains(idSurface));
    assert(m_surface_index.contains(m_pmesh->surface(idSurface).name()));

    m_surface_ids.erase(idSurface);
    m_surface_index.erase(m_pmesh->surface(idSurface).name());
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Submesh<Traits, M>::link_surface(id_type idSurface)
{
    assert(m_pmesh != nullptr);
    add_surface(idSurface);
    m_pmesh->surface(idSurface).set_submesh_id(m_id);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Submesh<Traits, M>::unlink_surface(id_type idSurface)
{
    assert(m_pmesh != nullptr);
    remove_surface(idSurface);
    m_pmesh->surface(idSurface).set_submesh_id(nullid);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Submesh<Traits, M>::remove_surfaces()
{
    for (auto& surface : m_surfaces)
    {
        for (auto& face : surface.faces())
        {
            face.set_surface_id(nullid);
        }
    }

    m_surface_index.clear();
    m_surface_ids.clear();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Submesh<Traits, M>::rename_surface(id_type idSurface, const std::string& replacement)
{
    surface_type& s = surface(idSurface);
    assert(!s.deleted());

    m_surface_index.erase(s.name());
    m_surface_index[replacement] = s.id();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Submesh<Traits, M>::rename_surfaces(std::function<std::string(const std::string&)> renamer)
{
    for (auto& surface : m_surfaces)
    {
        surface.set_name(renamer(surface.name()));
    }

    regenerate_surface_index();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::size_type quetzal::brep::Submesh<Traits, M>::surface_index_count() const
{
    return m_surface_index.size();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Submesh<Traits, M>::index_type& quetzal::brep::Submesh<Traits, M>::surface_index() const
{
    return m_surface_index;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::index_type& quetzal::brep::Submesh<Traits, M>::surface_index()
{
    return m_surface_index;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Submesh<Traits, M>::regenerate_surface_index()
{
    m_surface_index.clear();

    for (const auto& surface : m_surfaces)
    {
        m_surface_index[surface.name()] = surface.id();
    }

    return;
}

/*
//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Submesh<Traits, M>::append(const Submesh& submesh)
{
    m_face_ids.insert(m_face_ids.end(), submesh.face_ids().begin(), submesh.face_ids().end());
    m_surface_ids.insert(m_surface_ids.end(), submesh.surface_ids().begin(), submesh.surface_ids().end());
    // much more than this would be necessary here, this should be done at mesh level ...
    return;
}
*/

//--------------------------------------------------------------------------
template<typename Traits, typename M>
bool quetzal::brep::Submesh<Traits, M>::empty() const
{
    assert(m_pmesh != nullptr);
    assert(m_surface_ids.empty() && m_face_ids.empty() || !m_surface_ids.empty() && !m_face_ids.empty());

    return m_surface_ids.empty();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Submesh<Traits, M>::clear()
{
    m_face_ids.clear();
    m_surface_ids.clear();
    m_surface_index.clear();
    return;
}

//--------------------------------------------------------------------------
template<typename Traits, typename M>
bool quetzal::brep::Submesh<Traits, M>::unmarked() const
{
    assert(m_pmesh != nullptr);

    if (marked())
    {
        return false;
    }

    for (const auto& surface : m_surfaces)
    {
        if (surface.marked())
        {
            return false;
        }
    }

    for (const auto& face : m_faces)
    {
        if (face.marked())
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Submesh<Traits, M>::reset() const
{
    assert(m_pmesh != nullptr);

    for (auto& face : m_faces)
    {
        face.reset();
    }

    for (auto& surface : m_surfaces)
    {
        surface.reset();
    }

    Flags::reset();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Submesh<Traits, M>::set_mesh(mesh_type& mesh)
{
    m_pmesh = &mesh;
    m_faces.set_source(mesh);
    m_surfaces.set_source(mesh);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Submesh<Traits, M>::set_id(id_type id)
{
    m_id = id;
    m_faces.set_id(id);
    m_surfaces.set_id(id);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Submesh<Traits, M>::check_mesh(const mesh_type* const pmesh) const
{
    assert(m_pmesh == pmesh);
    assert(m_faces.check_source(pmesh));
    assert(m_surfaces.check_source(pmesh));
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::faces_type::size_function_type quetzal::brep::Submesh<Traits, M>::m_faces_size = [](const mesh_type& mesh, id_type id) -> size_t
{
    return mesh.submesh(id).face_ids().size();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::faces_type::terminal_function_type quetzal::brep::Submesh<Traits, M>::m_faces_first = [](const mesh_type& mesh, id_type id) -> std::set<id_type>::iterator
{
    return mesh.submesh(id).face_ids().begin();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::faces_type::terminal_function_type quetzal::brep::Submesh<Traits, M>::m_faces_last = [](const mesh_type& mesh, id_type id) -> std::set<id_type>::iterator
{
    assert(!mesh.submesh(id).face_ids().empty());
    return --mesh.submesh(id).face_ids().end();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::faces_type::terminal_function_type quetzal::brep::Submesh<Traits, M>::m_faces_end = [](const mesh_type& mesh, id_type id) -> std::set<id_type>::iterator
{
    return mesh.submesh(id).face_ids().end();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::faces_type::iterate_function_type quetzal::brep::Submesh<Traits, M>::m_faces_forward = [](const mesh_type& mesh, id_type id, std::set<id_type>::iterator i) -> std::set<id_type>::iterator
{
    assert(i != m_faces_end(mesh, id));
    return std::next(i);
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::faces_type::iterate_function_type quetzal::brep::Submesh<Traits, M>::m_faces_reverse = [](const mesh_type& mesh, id_type id, std::set<id_type>::iterator i) -> std::set<id_type>::iterator
{
    assert(i != m_faces_first(mesh, id));
    return std::prev(i);
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::faces_type::element_function_type quetzal::brep::Submesh<Traits, M>::m_faces_element = [](mesh_type& mesh, id_type id, std::set<id_type>::iterator i) -> quetzal::brep::Submesh<Traits, M>::face_type&
{
    assert(i != m_faces_end(mesh, id));
    auto& face = mesh.face(*i);
    assert(!face.deleted());
    return face;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::faces_type::const_element_function_type quetzal::brep::Submesh<Traits, M>::m_faces_const_element = [](const mesh_type& mesh, id_type id, std::set<id_type>::iterator i) -> const quetzal::brep::Submesh<Traits, M>::face_type&
{
    assert(i != m_faces_end(mesh, id));
    const auto& face = mesh.face(*i);
    assert(!face.deleted());
    return face;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::surfaces_type::size_function_type quetzal::brep::Submesh<Traits, M>::m_surfaces_size = [](const mesh_type& mesh, id_type id) -> size_t
{
    return mesh.submesh(id).surface_ids().size();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::surfaces_type::terminal_function_type quetzal::brep::Submesh<Traits, M>::m_surfaces_first = [](const mesh_type& mesh, id_type id) -> std::set<id_type>::iterator
{
    return mesh.submesh(id).surface_ids().begin();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::surfaces_type::terminal_function_type quetzal::brep::Submesh<Traits, M>::m_surfaces_last = [](const mesh_type& mesh, id_type id) -> std::set<id_type>::iterator
{
    assert(!mesh.submesh(id).surface_ids().empty());
    return --mesh.submesh(id).surface_ids().end();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::surfaces_type::terminal_function_type quetzal::brep::Submesh<Traits, M>::m_surfaces_end = [](const mesh_type& mesh, id_type id) -> std::set<id_type>::iterator
{
    return mesh.submesh(id).surface_ids().end();
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::surfaces_type::iterate_function_type quetzal::brep::Submesh<Traits, M>::m_surfaces_forward = [](const mesh_type& mesh, id_type id, std::set<id_type>::iterator i) -> std::set<id_type>::iterator
{
    assert(i != m_surfaces_end(mesh, id));
    return std::next(i);
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::surfaces_type::iterate_function_type quetzal::brep::Submesh<Traits, M>::m_surfaces_reverse = [](const mesh_type& mesh, id_type id, std::set<id_type>::iterator i) -> std::set<id_type>::iterator
{
    assert(i != m_surfaces_first(mesh, id));
    return std::prev(i);
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::surfaces_type::element_function_type quetzal::brep::Submesh<Traits, M>::m_surfaces_element = [](mesh_type& mesh, id_type id, std::set<id_type>::iterator i) -> quetzal::brep::Submesh<Traits, M>::surface_type&
{
    assert(i != m_surfaces_end(mesh, id));
    auto& surface = mesh.surface(*i);
    assert(!surface.deleted());
    return surface;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Submesh<Traits, M>::surfaces_type::const_element_function_type quetzal::brep::Submesh<Traits, M>::m_surfaces_const_element = [](const mesh_type& mesh, id_type id, std::set<id_type>::iterator i) -> const quetzal::brep::Submesh<Traits, M>::surface_type&
{
    assert(i != m_surfaces_end(mesh, id));
    const auto& surface = mesh.surface(*i);
    assert(!surface.deleted());
    return surface;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
std::ostream& quetzal::brep::operator<<(std::ostream& os, const Submesh<Traits, M>& submesh)
{
    os << static_cast<Flags>(submesh);
    os << "\t" << submesh.id();

    if (!submesh.deleted())
    {
        os << "\t" << submesh.name();

        os << "\t" << submesh.face_count() << " faces (";
        for (id_type idFace : submesh.face_ids())
        {
            os << " " << idFace;
        }
        os << ")";

        os << "\t" << submesh.surface_count() << " surfaces (";
        for (id_type idSurface : submesh.surface_ids())
        {
            os << " " << idSurface;
        }
        os << ")";

        os << "\t" << submesh.attributes();
        os << "\t" << submesh.properties();
    }

    os << std::endl;
    return os;
}

#endif // QUETZAL_BREP_SUBMESH_HPP
