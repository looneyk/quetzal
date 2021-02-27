#if !defined(QUETZAL_BREP_MESH_HPP)
#define QUETZAL_BREP_MESH_HPP
//------------------------------------------------------------------------------
// brep
// Mesh.hpp
//------------------------------------------------------------------------------

#include "Face.hpp"
#include "Flags.hpp"
#include "Halfedge.hpp"
#include "Seam.hpp"
#include "Submesh.hpp"
#include "Surface.hpp"
#include "Vertex.hpp"
#include "id.hpp"
#include "quetzal/common/Elements.hpp"
#include "quetzal/common/Properties.hpp"
#include <functional>
#include <limits>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include <cassert>

#include "validation.hpp" // ...

namespace quetzal::brep
{

    template<typename Traits>
    class Mesh;

    template<typename Traits>
    void swap(Mesh<Traits>& lhs, Mesh<Traits>& rhs);

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Mesh : public Flags
    {
    public:

        using traits_type = Traits;
        using vector_traits = typename traits_type::vector_traits;

        using vertex_attributes_type = typename traits_type::vertex_attributes_type;
        using face_attributes_type = typename traits_type::face_attributes_type;
        using surface_attributes_type = typename traits_type::surface_attributes_type;
        using submesh_attributes_type = typename traits_type::submesh_attributes_type;
        using value_type = typename traits_type::value_type;
        using point_type = typename traits_type::point_type;
        using vector_type = typename traits_type::vector_type;
        using texcoord_type = typename traits_type::texcoord_type;
        using halfedge_type = Halfedge<traits_type>;
        using halfedge_store_type = std::vector<halfedge_type>;
        using halfedges_type = Elements<Mesh<traits_type>, halfedge_type, id_type>;
        using vertex_type = Vertex<traits_type>;
        using vertex_store_type = std::vector<vertex_type>;
        using vertices_type = Elements<Mesh<traits_type>, vertex_type, id_type>;
        using face_type = Face<traits_type>;
        using face_store_type = std::vector<face_type>;
        using faces_type = Elements<Mesh<traits_type>, face_type, id_type>;
        using surface_type = Surface<traits_type>;
        using surface_store_type = std::vector<surface_type>;
        using surfaces_type = Elements<Mesh<traits_type>, surface_type, id_type>;
        using submesh_type = Submesh<traits_type>;
        using submesh_store_type = std::vector<submesh_type>;
        using submeshes_type = Elements<Mesh<traits_type>, submesh_type, id_type>;
        using seam_type = Seam<traits_type>;
        using seam_store_type = std::vector<seam_type>;
        using seams_type = Elements<Mesh<traits_type>, seam_type, id_type>;
        using index_type = std::map<std::string, id_type>;
        using size_type = typename traits_type::size_type;

        // Use from Traits only? ...
        template<typename U>
        static value_type val(const U& u)
        {
            return traits_type::val(u);
        }

        using face_deleter_type = std::function<void(face_type&)>;

        explicit Mesh(const std::string& name = "");
        Mesh(const Mesh& other);
        Mesh(Mesh&& other);
        ~Mesh() = default;

        Mesh& operator=(Mesh other);
        Mesh& operator=(Mesh&& other);

        id_type id() const;

        const std::string& name() const;
        void set_name(const std::string& name);

        const halfedge_type& halfedge(id_type id) const;
        halfedge_type& halfedge(id_type id);

        size_type halfedge_count() const;
        const halfedges_type& halfedges() const;
        halfedges_type& halfedges();

        size_type halfedge_store_count() const;
        const halfedge_store_type& halfedge_store() const;
        halfedge_store_type& halfedge_store();

        id_type create_halfedge(id_type idPartner, id_type idNext, id_type idPrev, id_type idVertex, id_type idFace);
        id_type create_halfedge_vertex(id_type idPartner, id_type idNext, id_type idPrev, id_type idFace, const vertex_attributes_type& attributes);

        const vertex_type& vertex(id_type id) const;
        vertex_type& vertex(id_type id);

        size_type vertex_count() const;
        const vertices_type& vertices() const;
        vertices_type& vertices();

        size_type vertex_store_count() const;
        const vertex_store_type& vertex_store() const;
        vertex_store_type& vertex_store();

        id_type create_vertex(id_type idHalfedge, const vertex_attributes_type& attributes = vertex_attributes_type());

        const face_type& face(id_type id) const;
        face_type& face(id_type id);

        size_type face_count() const;
        const faces_type& faces() const;
        faces_type& faces();

        size_type face_store_count() const;
        const face_store_type& face_store() const;
        face_store_type& face_store();

        id_type create_face(id_type idSurface, id_type idHalfedge, const face_attributes_type& attributes = face_attributes_type());

        void detach_face(id_type idFace); // Detach face from any connected surface and/or submesh, does not delete face
        void remove_face(id_type idFace); // Delete face leaving halfedges and vertices
        void delete_face(id_type idFace); // Delete all face components leaving an open area with border edges

        id_type surface_id(id_type idSubmesh, const std::string& name) const;

        const surface_type& surface(id_type idSubmesh, const std::string& name) const;
        surface_type& surface(id_type idSubmesh, const std::string& name);

        const surface_type& surface(id_type id) const;
        surface_type& surface(id_type id);

        size_type surface_count() const;
        const surfaces_type& surfaces() const;
        surfaces_type& surfaces();

        size_type surface_store_count() const;
        const surface_store_type& surface_store() const;
        surface_store_type& surface_store();

        size_type surface_index_count() const;
        const index_type& surface_index() const; // debug, logging, ...
        index_type& surface_index(); // debug, logging, ...

        bool contains_surface(id_type idSubmesh, const std::string& name) const;
        id_type create_surface(id_type idSubmesh, const std::string& name, const surface_attributes_type& attributes = surface_attributes_type());
        void detach_surface(id_type idSurface); // Detach faces from surface, detach surface from submesh, does not delete surface
        void remove_surface(id_type idSurface); // Detach faces from surface and delete empty surface
        void delete_surface(id_type idSurface); // Delete surface and all its faces
        void rename_surface(id_type idSurface, const std::string& replacement);
        void move_surface(id_type idSurface, const std::string& destination);

        // name and destination are extended surface names
        // destination can have a single component, in which case it refers to a submesh
        id_type surface_id(const std::string& name) const;
        const surface_type& surface(const std::string& name) const;
        surface_type& surface(const std::string& name);
        bool contains_surface(const std::string& name) const;
        void remove_surface(const std::string& name);
        void move_surface(const std::string& name, const std::string& destination);

        void remove_surfaces();
        void rename_surfaces(std::function<std::string(const std::string&)> renamer);

        id_type submesh_id(const std::string& name) const;

        const submesh_type& submesh(const std::string& name) const;
        submesh_type& submesh(const std::string& name);

        const submesh_type& submesh(id_type id) const;
        submesh_type& submesh(id_type id);

        size_type submesh_count() const;
        const submeshes_type& submeshes() const;
        submeshes_type& submeshes();

        size_type submesh_store_count() const;
        const submesh_store_type& submesh_store() const;
        submesh_store_type& submesh_store();

        size_type submesh_index_count() const;
        const index_type& submesh_index() const; // debug, logging, ...
        index_type& submesh_index(); // debug, logging, ...

        bool contains_submesh(const std::string& name) const;
        id_type create_submesh(const std::string& name, const submesh_attributes_type& attributes = submesh_attributes_type());
        void detach_submesh(id_type idSubmesh); // Detach faces and surfaces from submesh, does not delete submesh
        void remove_submesh(id_type idSubmesh); // Detach faces from submesh and delete empty submesh and surfaces
        void delete_submesh(id_type idSubmesh); // Delete submesh and all its faces and surfaces
        void rename_submesh(id_type idSubmesh, const std::string& replacement);

        void remove_submesh(const std::string& name);

        void remove_submeshes();
        void rename_submeshes(std::function<std::string(const std::string&)> renamer);

        // Manage surface face inclusion
        void add_surface_face(id_type idSubmesh, const std::string& name, id_type idFace); // Creates a new surface if it doesn't already exist

        // Manage submesh surface inclusion
        void add_submesh_surface(const std::string& name, id_type idSurface); // Creates a new submesh if it doesn't already exist
        void rename_submesh_surfaces(id_type idSubmesh, std::function<std::string(const std::string&)> renamer);

        const seam_type& seam(id_type id) const;
        seam_type& seam(id_type id);

        size_type seam_count() const;
        const seams_type& seams() const;
        seams_type& seams();

        size_type seam_store_count() const;
        const seam_store_type& seam_store() const;
        seam_store_type& seam_store();

        id_type create_seam(const std::string& name, id_type idPartner, id_type idNext, id_type idPrev, id_type idHalfedge);

        void clear_seams(id_type idSurface);
        void generate_seams();

        const Properties& properties() const;
        Properties& properties();

        // These link... functions unused, reference implementations? could use of these simplify connectivity?
        bool linked_face_surface(id_type idFace, id_type idSurface) const;
        bool linked_face_submesh(id_type idFace, id_type idSubmesh) const;
        bool linked_surface_submesh(id_type idSurface, id_type idSubmesh) const;
        bool linked(id_type idFace, id_type idSurface, id_type idSubmesh) const;

        void link(id_type idFace, id_type idSurface, id_type idSubmesh);

        void link_face_surface(id_type idFace, id_type idSurface);
        void link_face_submesh(id_type idFace, id_type idSubmesh);
        void link_surface_submesh(id_type idSurface, id_type idSubmesh);

        // should these have remove or detach semantics? currently remove, but detach seems a more appropriate complement to link ...
        void unlink_face(id_type idFace);
        void unlink_surface(id_type idSurface);
        void unlink_submesh(id_type idSubmesh);

        void append(const Mesh& mesh);
        void pack();

        bool empty() const;
        void clear(); // Clears contents, but does not change id or name

        bool unmarked() const;
        void reset() const override; // Flags

        void reassign_mesh();
        void regenerate_surface_index(); // Surfaces with the same name should have already been coalesced
        void regenerate_submesh_index(); // Submeshes with the same name should have already been coalesced

        bool check() const;
        size_type error_count() const;

        void check_mesh() const;

        friend void swap<Traits>(Mesh& lhs, Mesh& rhs);

    private:

        std::string extended_surface_name(id_type idSubmesh, const std::string& name) const; // qualified_? ...
        std::array<std::string, 2> split_surface_name(const std::string& name);

        id_type m_id;
        std::string m_name;

        halfedge_store_type m_halfedge_store;
        halfedges_type m_halfedges;

        vertex_store_type m_vertex_store;
        vertices_type m_vertices;

        face_store_type m_face_store;
        faces_type m_faces;

        surface_store_type m_surface_store;
        surfaces_type m_surfaces;
        index_type m_surface_index;

        submesh_store_type m_submesh_store;
        submeshes_type m_submeshes;
        index_type m_submesh_index;

        seam_store_type m_seam_store;
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

        static typename vertices_type::size_function_type m_vertices_size;
        static typename vertices_type::terminal_function_type m_vertices_first;
        static typename vertices_type::terminal_function_type m_vertices_last;
        static typename vertices_type::terminal_function_type m_vertices_end;
        static typename vertices_type::iterate_function_type m_vertices_forward;
        static typename vertices_type::iterate_function_type m_vertices_reverse;
        static typename vertices_type::element_function_type m_vertices_element;
        static typename vertices_type::const_element_function_type m_vertices_const_element;

        static typename faces_type::size_function_type m_faces_size;
        static typename faces_type::terminal_function_type m_faces_first;
        static typename faces_type::terminal_function_type m_faces_last;
        static typename faces_type::terminal_function_type m_faces_end;
        static typename faces_type::iterate_function_type m_faces_forward;
        static typename faces_type::iterate_function_type m_faces_reverse;
        static typename faces_type::element_function_type m_faces_element;
        static typename faces_type::const_element_function_type m_faces_const_element;

        static typename surfaces_type::size_function_type m_surfaces_size;
        static typename surfaces_type::terminal_function_type m_surfaces_first;
        static typename surfaces_type::terminal_function_type m_surfaces_last;
        static typename surfaces_type::terminal_function_type m_surfaces_end;
        static typename surfaces_type::iterate_function_type m_surfaces_forward;
        static typename surfaces_type::iterate_function_type m_surfaces_reverse;
        static typename surfaces_type::element_function_type m_surfaces_element;
        static typename surfaces_type::const_element_function_type m_surfaces_const_element;

        static typename submeshes_type::size_function_type m_submeshes_size;
        static typename submeshes_type::terminal_function_type m_submeshes_first;
        static typename submeshes_type::terminal_function_type m_submeshes_last;
        static typename submeshes_type::terminal_function_type m_submeshes_end;
        static typename submeshes_type::iterate_function_type m_submeshes_forward;
        static typename submeshes_type::iterate_function_type m_submeshes_reverse;
        static typename submeshes_type::element_function_type m_submeshes_element;
        static typename submeshes_type::const_element_function_type m_submeshes_const_element;

        static typename seams_type::size_function_type m_seams_size;
        static typename seams_type::terminal_function_type m_seams_first;
        static typename seams_type::terminal_function_type m_seams_last;
        static typename seams_type::terminal_function_type m_seams_end;
        static typename seams_type::iterate_function_type m_seams_forward;
        static typename seams_type::iterate_function_type m_seams_reverse;
        static typename seams_type::element_function_type m_seams_element;
        static typename seams_type::const_element_function_type m_seams_const_element;
    };

    //--------------------------------------------------------------------------
    template<typename Traits>
    typename Mesh<Traits>::face_deleter_type face_deleter_basic = [](Face& face) -> void
    {
        face.set_deleted();
        return;
    };

    //--------------------------------------------------------------------------
    template<typename Traits>
    typename Mesh<Traits>::face_deleter_type face_deleter_full = [](Face& face) -> void
    {
        for (auto& halfedge : face.halfedges())
        {
            if (!halfedge.border())
            {
                halfedge.partner().set_border();
            }

            halfedge.vertex().set_deleted();
            halfedge.set_deleted();
        }

        face.set_deleted();
        return;
    };

} // namespace quetzal::brep

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::brep::Mesh<Traits>::Mesh(const std::string& name) :
    Flags(),
    m_id(nullid),
    m_name(name),
    m_halfedge_store(),
    m_halfedges(*this, nullid, m_halfedges_size, m_halfedges_first, m_halfedges_last, m_halfedges_end, m_halfedges_forward, m_halfedges_reverse, m_halfedges_element, m_halfedges_const_element),
    m_vertex_store(),
    m_vertices(*this, nullid, m_vertices_size, m_vertices_first, m_vertices_last, m_vertices_end, m_vertices_forward, m_vertices_reverse, m_vertices_element, m_vertices_const_element),
    m_face_store(),
    m_faces(*this, nullid, m_faces_size, m_faces_first, m_faces_last, m_faces_end, m_faces_forward, m_faces_reverse, m_faces_element, m_faces_const_element),
    m_surface_store(),
    m_surfaces(*this, nullid, m_surfaces_size, m_surfaces_first, m_surfaces_last, m_surfaces_end, m_surfaces_forward, m_surfaces_reverse, m_surfaces_element, m_surfaces_const_element),
    m_surface_index(),
    m_submesh_store(),
    m_submeshes(*this, nullid, m_submeshes_size, m_submeshes_first, m_submeshes_last, m_submeshes_end, m_submeshes_forward, m_submeshes_reverse, m_submeshes_element, m_submeshes_const_element),
    m_submesh_index(),
    m_seam_store(),
    m_seams(*this, nullid, m_seams_size, m_seams_first, m_seams_last, m_seams_end, m_seams_forward, m_seams_reverse, m_seams_element, m_seams_const_element),
    m_properties()
{
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::brep::Mesh<Traits>::Mesh(const Mesh& other) :
    Flags(other),
    m_id(other.m_id),
    m_name(other.m_name),
    m_halfedge_store(other.m_halfedge_store),
    m_halfedges(other.m_halfedges),
    m_vertex_store(other.m_vertex_store),
    m_vertices(other.m_vertices),
    m_face_store(other.m_face_store),
    m_faces(other.m_faces),
    m_surface_store(other.m_surface_store),
    m_surfaces(other.m_surfaces),
    m_surface_index(other.m_surface_index),
    m_submesh_store(other.m_submesh_store),
    m_submeshes(other.m_submeshes),
    m_submesh_index(other.m_submesh_index),
    m_seam_store(other.m_seam_store),
    m_seams(other.m_seams),
    m_properties()
{
    reassign_mesh();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::brep::Mesh<Traits>::Mesh(Mesh&& other) :
    Flags(other),
    m_id(other.m_id),
    m_name(std::move(other.m_name)),
    m_halfedge_store(std::move(other.m_halfedge_store)),
    m_halfedges(other.m_halfedges),
    m_vertex_store(std::move(other.m_vertex_store)),
    m_vertices(other.m_vertices),
    m_face_store(std::move(other.m_face_store)),
    m_faces(other.m_faces),
    m_surface_store(std::move(other.m_surface_store)),
    m_surfaces(other.m_surfaces),
    m_surface_index(std::move(other.m_surface_index)),
    m_submesh_store(std::move(other.m_submesh_store)),
    m_submeshes(other.m_submeshes),
    m_submesh_index(std::move(other.m_submesh_index)),
    m_seam_store(std::move(other.m_seam_store)),
    m_seams(other.m_seams),
    m_properties()
{
    reassign_mesh();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::brep::Mesh<Traits>& quetzal::brep::Mesh<Traits>::operator=(Mesh other)
{
    swap(*this, other);
    reassign_mesh();
    return *this;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::brep::Mesh<Traits>& quetzal::brep::Mesh<Traits>::operator=(Mesh&& other)
{
    Flags::operator=(other);
    m_id = other.m_id;
    m_name = std::move(other.m_name);
    m_halfedge_store = std::move(other.m_halfedge_store);
    m_vertex_store = std::move(other.m_vertex_store);
    m_face_store = std::move(other.m_face_store);
    m_surface_store = std::move(other.m_surface_store);
    m_surfaces = std::move(other.m_surfaces);
    m_surface_index = std::move(other.m_surface_index);
    m_submesh_store = std::move(other.m_submesh_store);
    m_submeshes = std::move(other.m_submeshes);
    m_submesh_index = std::move(other.m_submesh_index);
    m_seam_store = std::move(other.m_seam_store);
    m_seams = std::move(other.m_seams);
    m_properties = std::move(other.m_properties);

    reassign_mesh();
    return *this;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::id_type quetzal::brep::Mesh<Traits>::id() const
{
    return m_id;
}

//------------------------------------------------------------------------------
template<typename Traits>
const std::string& quetzal::brep::Mesh<Traits>::name() const
{
    return m_name;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::set_name(const std::string& name)
{
    m_name = name;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::halfedge_type& quetzal::brep::Mesh<Traits>::halfedge(id_type id) const
{
    assert(id != nullid);
    assert(id < m_halfedge_store.size());
    return m_halfedge_store[id];
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::halfedge_type& quetzal::brep::Mesh<Traits>::halfedge(id_type id)
{
    assert(id != nullid);
    assert(id < m_halfedge_store.size());
    return m_halfedge_store[id];
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::size_type quetzal::brep::Mesh<Traits>::halfedge_count() const
{
    size_type n = 0;

    for (const halfedge_type& halfedge : m_halfedge_store)
    {
        if (!halfedge.deleted())
        {
            ++n;
        }
    }

    return n;
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::halfedges_type& quetzal::brep::Mesh<Traits>::halfedges() const
{
    return m_halfedges;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::halfedges_type& quetzal::brep::Mesh<Traits>::halfedges()
{
    return m_halfedges;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::size_type quetzal::brep::Mesh<Traits>::halfedge_store_count() const
{
    return m_halfedge_store.size();
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::halfedge_store_type& quetzal::brep::Mesh<Traits>::halfedge_store() const
{
    return m_halfedge_store;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::halfedge_store_type& quetzal::brep::Mesh<Traits>::halfedge_store()
{
    return m_halfedge_store;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::id_type quetzal::brep::Mesh<Traits>::create_halfedge(id_type idPartner, id_type idNext, id_type idPrev, id_type idVertex, id_type idFace)
{
    id_type idHalfedge = m_halfedge_store.size();
    m_halfedge_store.emplace_back(*this, idHalfedge, idPartner, idNext, idPrev, idVertex, idFace);
    return idHalfedge;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::id_type quetzal::brep::Mesh<Traits>::create_halfedge_vertex(id_type idPartner, id_type idNext, id_type idPrev, id_type idFace, const vertex_attributes_type& attributes)
{
    id_type idHalfedge = m_halfedge_store.size();
    id_type idVertex = create_vertex(idHalfedge, attributes);
    m_halfedge_store.emplace_back(*this, idHalfedge, idPartner, idNext, idPrev, idVertex, idFace);
    return idHalfedge;
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::vertex_type& quetzal::brep::Mesh<Traits>::vertex(id_type id) const
{
    assert(id != nullid);
    assert(id < m_vertex_store.size());
    return m_vertex_store[id];
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::vertex_type& quetzal::brep::Mesh<Traits>::vertex(id_type id)
{
    assert(id != nullid);
    assert(id < m_vertex_store.size());
    return m_vertex_store[id];
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::size_type quetzal::brep::Mesh<Traits>::vertex_count() const
{
    size_type n = 0;

    for (const vertex_type& vertex : m_vertex_store)
    {
        if (!vertex.deleted())
        {
            ++n;
        }
    }

    return n;
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::vertices_type& quetzal::brep::Mesh<Traits>::vertices() const
{
    return m_vertices;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::vertices_type& quetzal::brep::Mesh<Traits>::vertices()
{
    return m_vertices;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::size_type quetzal::brep::Mesh<Traits>::vertex_store_count() const
{
    return m_vertex_store.size();
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::vertex_store_type& quetzal::brep::Mesh<Traits>::vertex_store() const
{
    return m_vertex_store;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::vertex_store_type& quetzal::brep::Mesh<Traits>::vertex_store()
{
    return m_vertex_store;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::id_type quetzal::brep::Mesh<Traits>::create_vertex(id_type idHalfedge, const vertex_attributes_type& attributes)
{
    id_type idVertex = m_vertex_store.size();
    m_vertex_store.emplace_back(*this, idVertex, idHalfedge, attributes);
    return idVertex;
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::face_type& quetzal::brep::Mesh<Traits>::face(id_type id) const
{
    assert(id != nullid);
    assert(id < m_face_store.size());
    return m_face_store[id];
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::face_type& quetzal::brep::Mesh<Traits>::face(id_type id)
{
    assert(id != nullid);
    assert(id < m_face_store.size());
    return m_face_store[id];
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::size_type quetzal::brep::Mesh<Traits>::face_count() const
{
    size_type n = 0;

    for (const face_type& face : m_face_store)
    {
        if (!face.deleted())
        {
            ++n;
        }
    }

    return n;
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::faces_type& quetzal::brep::Mesh<Traits>::faces() const
{
    return m_faces;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::faces_type& quetzal::brep::Mesh<Traits>::faces()
{
    return m_faces;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::size_type quetzal::brep::Mesh<Traits>::face_store_count() const
{
    return m_face_store.size();
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::face_store_type& quetzal::brep::Mesh<Traits>::face_store() const
{
    return m_face_store;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::face_store_type& quetzal::brep::Mesh<Traits>::face_store()
{
    return m_face_store;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::id_type quetzal::brep::Mesh<Traits>::create_face(id_type idSurface, id_type idHalfedge, const face_attributes_type& attributes)
{
    id_type idFace = m_face_store.size();
    id_type idSubmesh = idSurface == nullid ? nullid : surface(idSurface).submesh_id();
    m_face_store.emplace_back(*this, idFace, idSurface, idSubmesh, idHalfedge, attributes);

    if (idSurface != nullid)
    {
        surface(idSurface).add_face(idFace);
    }

    if (idSubmesh != nullid)
    {
        submesh(idSubmesh).add_face(idFace);
    }

    return idFace;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::detach_face(id_type idFace)
{
    face_type& f = face(idFace);

    if (f.surface_id() != nullid)
    {
        surface_type& s = f.surface();
        s.remove_face(idFace); // unlink ...

        if (s.empty())
        {
            if (s.submesh_id() != nullid)
            {
                m_surface_index.erase(s.extended_name());
                s.submesh().unlink_surface(s.id());
            }

            s.set_deleted();
        }

        f.set_surface_id(nullid);
    }

    if (f.submesh_id() != nullid)
    {
        submesh_type& o = f.submesh();
        o.remove_face(idFace); // unlink ...

        if (o.empty())
        {
            m_submesh_index.erase(o.name());
            o.set_deleted();
        }

        f.set_submesh_id(nullid);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::remove_face(id_type idFace)
{
    detach_face(idFace);
    face(idFace).set_deleted();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::delete_face(id_type idFace)
{
    face_type& f = face(idFace);

    for (halfedge_type& halfedge : f.halfedges())
    {
        if (!halfedge.border())
        {
            halfedge.partner().set_border();
        }

        halfedge.vertex().set_deleted();
        halfedge.set_deleted();
    }

    remove_face(idFace);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::id_type quetzal::brep::Mesh<Traits>::surface_id(id_type idSubmesh, const std::string& name) const
{
    auto i = m_surface_index.find(extended_surface_name(idSubmesh, name));
    if (i != m_surface_index.end())
    {
        assert(submesh(idSubmesh).surface_id(name) != nullid);
        return i->second;
    }

    assert(submesh(idSubmesh).surface_id(name) == nullid);
    return nullid;
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::surface_type& quetzal::brep::Mesh<Traits>::surface(id_type idSubmesh, const std::string& name) const
{
    assert(contains_surface(idSubmesh, name));
    assert(submesh(idSubmesh).surface_id(name) == nullid);

    id_type idSurface = m_surface_index[extended_surface_name(idSubmesh, name)];
    return surface(idSurface);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::surface_type& quetzal::brep::Mesh<Traits>::surface(id_type idSubmesh, const std::string& name)
{
    assert(contains_surface(idSubmesh, name));
    assert(submesh(idSubmesh).surface_id(name) == nullid);

    id_type idSurface = m_surface_index[extended_surface_name(idSubmesh, name)];
    return surface(idSurface);
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::surface_type& quetzal::brep::Mesh<Traits>::surface(id_type id) const
{
    assert(id != nullid);
    assert(id < surface_store_count());
    return m_surface_store[id];
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::surface_type& quetzal::brep::Mesh<Traits>::surface(id_type id)
{
    assert(id != nullid);
    assert(id < surface_store_count());
    return m_surface_store[id];
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::size_type quetzal::brep::Mesh<Traits>::surface_count() const
{
    size_type n = 0;

    for (const surface_type& surface : m_surface_store)
    {
        if (!surface.deleted())
        {
            ++n;
        }
    }

    return n;
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::surfaces_type& quetzal::brep::Mesh<Traits>::surfaces() const
{
    return m_surfaces;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::surfaces_type& quetzal::brep::Mesh<Traits>::surfaces()
{
    return m_surfaces;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::size_type quetzal::brep::Mesh<Traits>::surface_store_count() const
{
    return m_surface_store.size();
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::surface_store_type& quetzal::brep::Mesh<Traits>::surface_store() const
{
    return m_surface_store;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::surface_store_type& quetzal::brep::Mesh<Traits>::surface_store()
{
    return m_surface_store;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::size_type quetzal::brep::Mesh<Traits>::surface_index_count() const
{
    return m_surface_index.size();
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::index_type& quetzal::brep::Mesh<Traits>::surface_index() const
{
    return m_surface_index;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::index_type& quetzal::brep::Mesh<Traits>::surface_index()
{
    return m_surface_index;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::brep::Mesh<Traits>::contains_surface(id_type idSubmesh, const std::string& name) const
{
    assert(submesh(idSubmesh).contains_surface(name) == m_surface_index.contains(extended_surface_name(idSubmesh, name)));
    return m_surface_index.contains(extended_surface_name(idSubmesh, name));
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::id_type quetzal::brep::Mesh<Traits>::create_surface(id_type idSubmesh, const std::string& name, const surface_attributes_type& attributes)
{
    assert(!contains_surface(idSubmesh, name));
    assert(!submesh(idSubmesh).contains_surface(name));

    id_type idSurface = surface_store_count();
    m_surface_store.emplace_back(*this, idSurface, name, idSubmesh, attributes);

    if (idSubmesh != nullid)
    {
        submesh(idSubmesh).add_surface(idSurface);
    }

    m_surface_index[surface(idSurface).extended_name()] = idSurface;
    return idSurface;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::detach_surface(id_type idSurface)
{
    surface_type& s = surface(idSurface);
    assert(m_surface_index.contains(s.extended_name()));
    m_surface_index.erase(s.extended_name());

    for (auto& face : s.faces())
    {
        face.set_surface_id(nullid);
    }

    if (s.submesh_id() != nullid)
    {
        s.submesh().remove_surface(idSurface);

        if (s.submesh().empty())
        {
            m_submesh_index.erase(s.submesh().name());
            s.submesh().set_deleted();
        }
    }

    s.clear();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::remove_surface(id_type idSurface)
{
    detach_surface(idSurface);
    surface(idSurface).set_deleted();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::delete_surface(id_type idSurface)
{
    surface_type& s = surface(idSurface);
    for (id_type idFace : s.face_ids())
    {
        face(idFace).set_surface_id(nullid); // This ensures that face_ids is not modified (invalidating iterator) and that an empty surface will not be removed behind our backs
        delete_face(idFace);
    }

    remove_surface(idSurface);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::rename_surface(id_type idSurface, const std::string& replacement)
{
    surface_type& s = surface(idSurface);
    assert(m_surface_index.contains(s.extended_name()));

    m_surface_index.erase(s.extended_name());

    id_type idSurfaceReplacement = idSurface;
    if (!contains_surface(replacement))
    {
        s.set_name(replacement);
    }
    else
    {
        idSurfaceReplacement = surface_id(replacement);

        for (id_type idFace : s.face_ids())
        {
            surface(idSurfaceReplacement).link_face(idFace);
        }

        s.clear();
        s.set_deleted();
    }

    s.submesh().rename_surface(idSurface, replacement);
    m_surface_index[extended_surface_name(s.submesh_id(), replacement)] = s.id();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::move_surface(id_type idSurface, const std::string& destination)
{
    auto [nameSubmeshDest, nameSurfaceDest] = split_surface_name(destination);
    assert(!nameSubmeshDest.empty());

    id_type idSubmeshDest = submesh_id(nameSubmeshDest);
    if (idSubmeshDest == nullid)
    {
        idSubmeshDest = create_submesh(nameSubmeshDest); // attributes and properties from mesh.surface(idSurface).submesh() ...
    }

    if (nameSurfaceDest.empty())
    {
        nameSurfaceDest = surface(idSurface).name();
    }

    id_type idSurfaceDest = surface_id(idSubmeshDest, nameSurfaceDest);
    if (idSurfaceDest == nullid)
    {
        idSurfaceDest = create_surface(idSubmeshDest, nameSurfaceDest, surface(idSurface).attributes()); // properties ...
    }

    surface_type& s = surface(idSurface);
    submesh_type& o = s.submesh();

    assert(idSubmeshDest != o.id());
    assert(idSurfaceDest != idSurface);

    m_surface_index.erase(s.extended_name());

    o.unlink_surface(idSurface);
    for (id_type idFace : s.face_ids())
    {
        o.unlink_face(idFace);
    }

    for (id_type idFace : s.face_ids())
    {
        surface(idSurfaceDest).link_face(idFace);
        submesh(idSubmeshDest).link_face(idFace);
    }

    m_surface_index[surface(idSurfaceDest).extended_name()] = idSurfaceDest;

    if (o.empty())
    {
        m_submesh_index.erase(o.name());
        o.clear();
        o.set_deleted();
    }

    s.clear();
    s.set_deleted();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::id_type quetzal::brep::Mesh<Traits>::surface_id(const std::string& name) const
{
    auto i = m_surface_index.find(name);
    if (i != m_surface_index.end())
    {
        return i->second;
    }

    return nullid;
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::surface_type& quetzal::brep::Mesh<Traits>::surface(const std::string& name) const
{
    assert(contains_surface(name));
    return surface(m_surface_index.find(name)->second);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::surface_type& quetzal::brep::Mesh<Traits>::surface(const std::string& name)
{
    assert(contains_surface(name));
    return surface(m_surface_index[name]);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::brep::Mesh<Traits>::contains_surface(const std::string& name) const
{
    return m_surface_index.contains(name);
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::remove_surface(const std::string& name)
{
    assert(contains_surface(name));
    remove_surface(surface_id(name));
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::move_surface(const std::string& name, const std::string& destination)
{
    assert(contains_surface(name));
    move_surface(surface_id(name), destination);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::remove_surfaces()
{
    for (face_type& face : m_face_store)
    {
        face.set_surface_id(nullid);
    }

    for (submesh_type& submesh : m_submeshes)
    {
        submesh.surface_index().clear();
        submesh.surface_ids().clear();

        // for each surface, for each face, unlink it from submesh ...
    }

    m_surface_index.clear();
    m_surface_store.clear();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::rename_surfaces(std::function<std::string(const std::string&)> renamer)
{
    for (auto& surface : m_surfaces)
    {
        surface.set_name(renamer(surface.name()));
    }

    for (auto& submesh : m_submeshes)
    {
        submesh.regenerate_surface_index();
    }

    regenerate_surface_index();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::id_type quetzal::brep::Mesh<Traits>::submesh_id(const std::string& name) const
{
    auto i = m_submesh_index.find(name);
    if (i != m_submesh_index.end())
    {
        return i->second;
    }

    return nullid;
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::submesh_type& quetzal::brep::Mesh<Traits>::submesh(const std::string& name) const
{
    assert(contains_submesh(name));
    return submesh(m_submesh_index[name]);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::submesh_type& quetzal::brep::Mesh<Traits>::submesh(const std::string& name)
{
    assert(contains_submesh(name));
    return submesh(m_submesh_index[name]);
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::submesh_type& quetzal::brep::Mesh<Traits>::submesh(id_type id) const
{
    assert(id != nullid);
    assert(id < submesh_store_count());
    return m_submesh_store[id];
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::submesh_type& quetzal::brep::Mesh<Traits>::submesh(id_type id)
{
    assert(id != nullid);
    assert(id < submesh_store_count());
    return m_submesh_store[id];
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::size_type quetzal::brep::Mesh<Traits>::submesh_count() const
{
    size_type n = 0;

    for (const submesh_type& submesh : m_submesh_store)
    {
        if (!submesh.deleted())
        {
            ++n;
        }
    }

    return n;
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::submeshes_type& quetzal::brep::Mesh<Traits>::submeshes() const
{
    return m_submeshes;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::submeshes_type& quetzal::brep::Mesh<Traits>::submeshes()
{
    return m_submeshes;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::size_type quetzal::brep::Mesh<Traits>::submesh_store_count() const
{
    return m_submesh_store.size();
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::submesh_store_type& quetzal::brep::Mesh<Traits>::submesh_store() const
{
    return m_submesh_store;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::submesh_store_type& quetzal::brep::Mesh<Traits>::submesh_store()
{
    return m_submesh_store;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::size_type quetzal::brep::Mesh<Traits>::submesh_index_count() const
{
    return m_submesh_index.size();
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::index_type& quetzal::brep::Mesh<Traits>::submesh_index() const
{
    return m_submesh_index;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::index_type& quetzal::brep::Mesh<Traits>::submesh_index()
{
    return m_submesh_index;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::brep::Mesh<Traits>::contains_submesh(const std::string& name) const
{
    return m_submesh_index.contains(name);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::id_type quetzal::brep::Mesh<Traits>::create_submesh(const std::string& name, const submesh_attributes_type& attributes)
{
    assert(!contains_submesh(name));

    id_type idSubmesh = submesh_store_count();
    m_submesh_store.emplace_back(*this, idSubmesh, name, attributes);

    m_submesh_index[name] = idSubmesh;
    return idSubmesh;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::detach_submesh(id_type idSubmesh)
{
    submesh_type& o = submesh(idSubmesh);
    for (auto& face : o.faces())
    {
        face.set_submesh_id(nullid);
    }

    for (auto& surface : o.surfaces())
    {
        m_surface_index.erase(surface.extended_name());
        surface.set_submesh_id(nullid);
        m_surface_index[surface.extended_name()] = surface.id();
    }

    m_submesh_index.erase(o.name());
    o.clear();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::remove_submesh(id_type idSubmesh)
{
    detach_submesh(idSubmesh);
    submesh(idSubmesh).set_deleted();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::delete_submesh(id_type idSubmesh)
{
    submesh_type& o = submesh(idSubmesh);
    for (id_type idFace : o.face_ids())
    {
        face(idFace).set_submesh_id(nullid); // This ensures that face_ids is not modified (invalidating iterator) and that an empty submesh will not be removed behind our backs
        delete_face(idFace);
    }

    remove_submesh(idSubmesh);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::rename_submesh(id_type idSubmesh, const std::string& replacement)
{
    assert(submesh(idSubmesh).name() != replacement);

    submesh_type& o = submesh(idSubmesh);
    m_submesh_index.erase(o.name());

    for (const auto& surface : o.surfaces())
    {
        m_surface_index.erase(surface.extended_name());
    }

    id_type idSubmeshReplacement = idSubmesh;
    if (!contains_submesh(replacement))
    {
        o.set_name(replacement);
    }
    else
    {
        idSubmeshReplacement = submesh_id(replacement);
        auto& oDest = submesh(idSubmeshReplacement);

        for (id_type idSurface : o.surface_ids())
        {
            auto& s = surface(idSurface);

            if (!oDest.contains_surface(s.name()))
            {
                oDest.link_surface(idSurface);

                for (id_type idFace : s.face_ids())
                {
                    oDest.link_face(idFace);
                }
            }
            else
            {
                for (id_type idFace : s.face_ids())
                {
                    oDest.link_face(idFace);
                    oDest.surface(s.name()).link_face(idFace);
                }

                s.clear();
                s.set_deleted();
            }
        }

        o.clear();
        o.set_deleted();
    }

    for (const surface_type& surface : submesh(idSubmeshReplacement).surfaces())
    {
        m_surface_index[surface.extended_name()] = surface.id();
    }

    m_submesh_index[replacement] = idSubmeshReplacement;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::remove_submesh(const std::string& name)
{
    assert(contains_submesh(name));
    remove_submesh(submesh_id(name));
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::remove_submeshes()
{
    for (face_type& face : m_face_store)
    {
        face.set_submesh_id(nullid);
    }

    for (surface_type& surface : m_surfaces)
    {
        surface.set_submesh_id(nullid);
    }

    m_submesh_store.clear();
    m_submesh_index.clear();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::rename_submeshes(std::function<std::string(const std::string&)> renamer)
{
    for (auto& submesh : m_submeshes)
    {
        submesh.set_name(renamer(submesh.name()));
    }

    regenerate_submesh_index();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::add_surface_face(id_type idSubmesh, const std::string& name, id_type idFace)
{
    assert(!face(idFace).deleted());

    id_type idSurface = nullid;
    if (contains_surface(idSubmesh, name))
    {
        idSurface = surface_id(idSubmesh, name);
    }
    else
    {
        idSurface = create_surface(idSubmesh, name, face(idFace).attributes());
    }

    if (face(idFace).surface_id() != idSurface)
    {
        if (face(idFace).surface_id() != nullid)
        {
            surface_type& s = face(idFace).surface();
            assert(!s.deleted());

            s.remove_face(idFace);
            if (s.empty())
            {
                remove_surface(s.id());
            }
        }

        surface(idSurface).link_face(idFace);

        if (surface(idSurface).submesh_id() != nullid && face(idFace).submesh_id() != surface(idSurface).submesh_id())
        {
            surface(idSurface).submesh().link_face(idFace);
        }
    }

    assert(surface(idSurface).contains_face(idFace));
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::add_submesh_surface(const std::string& name, id_type idSurface)
{
    assert(!surface(idSurface).deleted());

    if (surface(idSurface).submesh_id() != nullid)
    {
        submesh_type& o = surface(idSurface).submesh();
        o.remove_face(idSurface);
        if (o.empty())
        {
            remove_submesh(o.name());
        }
    }

    id_type idSubmesh = nullid;
    if (contains_submesh(name))
    {
        idSubmesh = submesh_id(name);
    }
    else
    {
        idSubmesh = create_submesh(name);
    }

    submesh(idSubmesh).link_surface(idSurface);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::rename_submesh_surfaces(id_type idSubmesh, std::function<std::string(const std::string&)> renamer)
{
    for (auto& surface : submesh(idSubmesh).surfaces())
    {
        rename_surface(surface.id(), renamer(surface.name()));
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::seam_type& quetzal::brep::Mesh<Traits>::seam(id_type id) const
{
    assert(id != nullid);
    assert(id < m_seam_store.size());
    return m_seam_store[id];
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::seam_type& quetzal::brep::Mesh<Traits>::seam(id_type id)
{
    assert(id != nullid);
    assert(id < m_seam_store.size());
    return m_seam_store[id];
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::size_type quetzal::brep::Mesh<Traits>::seam_count() const
{
    size_type n = 0;

    for (const seam_type& seam : m_seam_store)
    {
        if (!seam.deleted())
        {
            ++n;
        }
    }

    return n;
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::seams_type& quetzal::brep::Mesh<Traits>::seams() const
{
    return m_seams;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::seams_type& quetzal::brep::Mesh<Traits>::seams()
{
    return m_seams;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::size_type quetzal::brep::Mesh<Traits>::seam_store_count() const
{
    return m_seam_store.size();
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::brep::Mesh<Traits>::seam_store_type& quetzal::brep::Mesh<Traits>::seam_store() const
{
    return m_seam_store;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::seam_store_type& quetzal::brep::Mesh<Traits>::seam_store()
{
    return m_seam_store;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::id_type quetzal::brep::Mesh<Traits>::create_seam(const std::string& name, id_type idPartner, id_type idNext, id_type idPrev, id_type idHalfedge)
{
    id_type idSeam = m_seam_store.size();
    m_seam_store.emplace_back(*this, idSeam, name, idPartner, idNext, idPrev, idHalfedge);
    return idSeam;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::clear_seams(id_type idSurface)
{
    for (auto& seam : m_seams)
    {
        if (seam.surface_id() == idSurface)
        {
            seam.set_deleted();
        }
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::generate_seams()
{
    m_seam_store.clear();

    for (auto& surface : m_surfaces)
    {
        surface.generate_seams();
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename const quetzal::Properties& quetzal::brep::Mesh<Traits>::properties() const
{
    return m_properties;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::Properties& quetzal::brep::Mesh<Traits>::properties()
{
    return m_properties;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::brep::Mesh<Traits>::linked_face_surface(id_type idFace, id_type idSurface) const
{
    if (idFace == nullid || idSurface == nullid)
    {
        return false;
    }

    return face(idFace).surface_id() == idSurface && surface(idSurface).contains_face(idFace);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::brep::Mesh<Traits>::linked_face_submesh(id_type idFace, id_type idSubmesh) const
{
    if (idFace == nullid || idSubmesh == nullid)
    {
        return false;
    }

    return face(idFace).submesh_id() == idSubmesh && submesh(idSubmesh).contains_face(idFace);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::brep::Mesh<Traits>::linked_surface_submesh(id_type idSurface, id_type idSubmesh) const
{
    if (idSurface == nullid || idSubmesh == nullid)
    {
        return false;
    }

    return surface(idSurface).submesh_id() == idSubmesh && submesh(idSubmesh).contains_surface(idSurface);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::brep::Mesh<Traits>::linked(id_type idFace, id_type idSurface, id_type idSubmesh) const
{
    return linked_face_surface(idFace, idSurface) && linked_face_submesh(idFace, idSubmesh) && linked_surface_submesh(idSurface, idSubmesh);
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::link(id_type idFace, id_type idSurface, id_type idSubmesh)
{
    assert(idFace != nullid);

    if (idSurface != nullid)
    {
        link_face_surface(idFace, idSurface);
    }

    if (idSubmesh != nullid)
    {
        link_face_submesh(idFace, idSubmesh);

        if (idSurface != nullid)
        {
            link_surface_submesh(idSurface, idSubmesh);
        }
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::link_face_surface(id_type idFace, id_type idSurface)
{
    assert(idFace != nullid);
    assert(idSurface != nullid);

    face(idFace).set_surface_id(idSurface);
    surface(idSurface).face_ids().insert(idFace);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::link_face_submesh(id_type idFace, id_type idSubmesh)
{
    assert(idFace != nullid);
    assert(idSubmesh != nullid);

    face(idFace).set_submesh_id(idSubmesh);
    submesh(idSubmesh).face_ids().insert(idFace);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::link_surface_submesh(id_type idSurface, id_type idSubmesh)
{
    assert(idSurface != nullid);
    assert(idSubmesh != nullid);

    surface(idSurface).set_submesh_id(idSubmesh);
    submesh(idSubmesh).surface_ids().insert(idSurface);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::unlink_face(id_type idFace)
{
    Face& f = face(idFace);

    if (f.surface_id() != nullid)
    {
        Surface& s = surface(f.surface_id());
        s.face_ids().erase(idFace);

        if (s.empty())
        {
            m_surface_index.erase(s.extended_name());
            s.set_deleted();
        }
    }

    if (f.submesh_id() != nullid)
    {
        Submesh& o = submesh(f.submesh_id());
        o.face_ids().erase(idFace);

        if (f.surface_id() != nullid)
        {
            o.surface_ids().erase(f.surface_id());
        }

        if (o.empty())
        {
            m_submesh_index.erase(o.name());
            o.set_deleted();
        }
    }

    f.set_deleted();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::unlink_surface(id_type idSurface)
{
    Surface& s = surface(idSurface);

    for (Face& f : s.faces())
    {
        assert(f.surface_id() == idSurface);
        f.set_surface_id(nullid);
    }

    if (s.submesh_id() != nullid)
    {
        Submesh& o = submesh(s.submesh_id());
        o.surface_ids().erase(idSurface);

        for (id_type idFace : s.face_ids())
        {
            o.face_ids().erase(idFace);
        }

        if (o.empty())
        {
            m_submesh_index.erase(o.name());
            o.set_deleted();
        }
    }

    s.clear();
    s.set_deleted(); // ? ...
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::unlink_submesh(id_type idSubmesh)
{
    Submesh& o = submesh(idSubmesh);

    for (Surface& s : o.surfaces())
    {
        assert(s.submesh_id() == idSubmesh);
        s.clear();
        s.set_deleted();
    }

    for (Face& f : o.faces())
    {
        assert(f.submesh_id() == idSubmesh);
        assert(f.surface_id() == nullid || f.surface().submesh_id() == idSubmesh);

//        f.clear() // ...
        f.set_surface_id(nullid);
        f.set_submesh_id(nullid);
        f.set_deleted();
    }

    o.clear();
    o.set_deleted(); // ? ...
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::append(const Mesh& mesh)
{
    if (empty())
    {
        *this = mesh;
        return;
    }

    size_type nh = halfedge_store_count();
    size_type nv = vertex_store_count();
    size_type nf = face_store_count();
    size_type ns = surface_store_count();
//    no? ...
//    ne? ...
// consider what to do about seam here ...

    // Add the new geometry (include deleted elements except for surfaces and submeshes ...)
    m_halfedge_store.insert(m_halfedge_store.end(), mesh.halfedge_store().begin(), mesh.halfedge_store().end());
    m_vertex_store.insert(m_vertex_store.end(), mesh.vertex_store().begin(), mesh.vertex_store().end());
    m_face_store.insert(m_face_store.end(), mesh.face_store().begin(), mesh.face_store().end()); // faces()? ...

    for (const auto& oOther : mesh.submeshes())
    {
        const std::string& name = oOther.name();
        assert(!name.empty());

        if (!contains_submesh(name))
        {
            create_submesh(name);
        }
    }

    for (const auto& sOther : mesh.surfaces())
    {
        const std::string& name = sOther.name();
        assert(!name.empty());

        id_type idSubmesh = sOther.submesh_id() == nullid ? nullid : submesh_id(sOther.submesh().name());

        if (!contains_surface(idSubmesh, name))
        {
            create_surface(idSubmesh, name, sOther.attributes());
        }
    }

    // Adjust id's to reflect their new positions

    for (auto i = next(m_vertex_store.begin(), nv); i != m_vertex_store.end(); ++i)
    {
        update_vertex(*i, *this, nh, nv);
    }

    for (auto i = next(m_halfedge_store.begin(), nh); i != m_halfedge_store.end(); ++i)
    {
        update_halfedge(*i, *this, nh, nv, nf);
    }

    for (auto i = next(m_face_store.begin(), nf); i != m_face_store.end(); ++i)
    {
        id_type idSubmesh = i->submesh_id() == nullid ? nullid : submesh_id(mesh.submesh(i->submesh_id()).name());
        id_type idSurface = i->surface_id() == nullid ? nullid : surface_id(idSubmesh, mesh.surface(i->surface_id()).name());
        update_face(*i, *this, nh, nf, idSurface, idSubmesh);
    }

    for (const auto& sOther : mesh.surfaces())
    {
        id_type idSubmesh = sOther.submesh_id() == nullid ? nullid : submesh_id(sOther.submesh().name());

        Surface<Traits>& s = surface(surface_id(idSubmesh, sOther.name()));

        if (s.id() > ns)
        {
            s.set_submesh_id(idSubmesh);
        }

        for (id_type idFace : sOther.face_ids())
        {
            s.link_face(nf + idFace);
        }
    }

    for (const auto& oOther : mesh.submeshes())
    {
        Submesh<Traits>& o = submesh(submesh_id(oOther.name()));

        for (id_type idSurfaceOther : oOther.surface_ids())
        {
            id_type idSurface = surface_id(o.id(), mesh.surface(idSurfaceOther).name());
            if (!o.contains_surface(idSurface))
            {
                o.add_surface(idSurface);
            }
            if (idSurface > ns) // ...
{
assert(surface(idSurface).submesh_id() != nullid);
//            if (surface(idSurface).submesh_id() == nullid)
            {
                surface(idSurface).set_submesh_id(o.id());
            }
}
//            o.link_surface(idSurface);
        }

        for (id_type idFaceOther : oOther.face_ids())
        {
            o.link_face(nf + idFaceOther);
        }
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::pack()
{
    id_type n = 0;

    // Pack halfedges and store new positions
    std::unordered_map<id_type, id_type> halfedge_mapping;
    n = 0;
    for (halfedge_type& halfedge : m_halfedges)
    {
        halfedge_mapping[halfedge.id()] = n;
        m_halfedge_store[n] = halfedge;
        ++n;
    }
    m_halfedge_store.resize(n);

    // Pack vertices and store new positions
    std::unordered_map<id_type, id_type> vertex_mapping;
    n = 0;
    for (vertex_type& vertex : m_vertices)
    {
        vertex_mapping[vertex.id()] = n;
        m_vertex_store[n] = vertex;
        ++n;
    }
    m_vertex_store.resize(n);

    // Pack faces and store new positions
    std::unordered_map<id_type, id_type> face_mapping;
    n = 0;
    for (face_type& face : m_faces)
    {
        face_mapping[face.id()] = n;
        m_face_store[n] = face;
        ++n;
    }
    m_face_store.resize(n);

    // Pack surfaces and store new positions
    std::unordered_map<id_type, id_type> surface_mapping;
    n = 0;
    for (surface_type& surface : m_surfaces)
    {
        surface_mapping[surface.id()] = n;
        m_surface_store[n] = surface;
        ++n;
    }
    m_surface_store.resize(n);

    // Pack submeshes and store new positions
    std::unordered_map<id_type, id_type> submesh_mapping;
    n = 0;
    for (submesh_type& submesh : m_submeshes)
    {
        submesh_mapping[submesh.id()] = n;
        m_submesh_store[n] = submesh;
        ++n;
    }
    m_submesh_store.resize(n);

    // Pack seams and store new positions
    std::unordered_map<id_type, id_type> seam_mapping;
    n = 0;
    for (seam_type& seam : m_seams)
    {
        seam_mapping[seam.id()] = n;
        m_seam_store[n] = seam;
        ++n;
    }
    m_seam_store.resize(n);

    // Adjust id's to reflect their new positions

    for (halfedge_type& halfedge : m_halfedge_store)
    {
        assert(halfedge_mapping.contains(halfedge.id()));
        halfedge.set_id(halfedge_mapping[halfedge.id()]);

        if (!halfedge.border())
        {
            assert(halfedge_mapping.contains(halfedge.partner_id()));
            halfedge.set_partner_id(halfedge_mapping[halfedge.partner_id()]);
        }

        assert(halfedge_mapping.contains(halfedge.next_id()));
        halfedge.set_next_id(halfedge_mapping[halfedge.next_id()]);

        assert(halfedge_mapping.contains(halfedge.prev_id()));
        halfedge.set_prev_id(halfedge_mapping[halfedge.prev_id()]);

        assert(vertex_mapping.contains(halfedge.vertex_id()));
        halfedge.set_vertex_id(vertex_mapping[halfedge.vertex_id()]);

        assert(face_mapping.contains(halfedge.face_id()));
        halfedge.set_face_id(face_mapping[halfedge.face_id()]);
    }

    for (vertex_type& vertex : m_vertex_store)
    {
        assert(vertex_mapping.contains(vertex.id()));
        vertex.set_id(vertex_mapping[vertex.id()]);

        assert(halfedge_mapping.contains(vertex.halfedge_id()));
        vertex.set_halfedge_id(halfedge_mapping[vertex.halfedge_id()]);
    }

    for (face_type& face : m_face_store)
    {
        assert(face_mapping.contains(face.id()));
        face.set_id(face_mapping[face.id()]);

        assert(halfedge_mapping.contains(face.halfedge_id()));
        face.set_halfedge_id(halfedge_mapping[face.halfedge_id()]);

        for (id_type& idHalfedgeHole : face.hole_ids())
        {
            assert(halfedge_mapping.contains(idHalfedgeHole));
            idHalfedgeHole = halfedge_mapping[idHalfedgeHole];
        }

        if (face.surface_id() != nullid)
        {
            assert(surface_mapping.contains(face.surface_id()));
            face.set_surface_id(surface_mapping[face.surface_id()]);
        }

        if (face.submesh_id() != nullid)
        {
            assert(submesh_mapping.contains(face.submesh_id()));
            face.set_submesh_id(submesh_mapping[face.submesh_id()]);
        }
    }

    for (surface_type& surface : m_surface_store)
    {
        assert(surface_mapping.contains(surface.id()));
        surface.set_id(surface_mapping[surface.id()]);

        const typename surface_type::face_ids_type face_ids = std::move(surface.face_ids());
        surface.face_ids().clear();

        for (id_type idFace : face_ids)
        {
            assert(face_mapping.contains(idFace));
            surface.face_ids().insert(face_mapping[idFace]);
        }

        surface.set_regenerate_seams();

        if (surface.submesh_id() != nullid)
        {
            assert(submesh_mapping.contains(surface.submesh_id()));
            surface.set_submesh_id(submesh_mapping[surface.submesh_id()]);
        }
    }

    for (seam_type& seam : m_seam_store)
    {
        assert(seam_mapping.contains(seam.id()));
        seam.set_id(seam_mapping[seam.id()]);

        if (!seam.border())
        {
            assert(seam_mapping.contains(seam.partner_id()));
            seam.set_partner_id(seam_mapping[seam.partner_id()]);
        }

        assert(seam_mapping.contains(seam.next_id()));
        seam.set_next_id(seam_mapping[seam.next_id()]);

        assert(seam_mapping.contains(seam.prev_id()));
        seam.set_prev_id(seam_mapping[seam.prev_id()]);

        assert(halfedge_mapping.contains(seam.halfedge_id()));
        seam.set_halfedge_id(halfedge_mapping[seam.halfedge_id()]);
    }

    for (submesh_type& submesh : m_submesh_store)
    {
        assert(submesh_mapping.contains(submesh.id()));
        submesh.set_id(submesh_mapping[submesh.id()]);

        const typename submesh_type::face_ids_type face_ids = std::move(submesh.face_ids());
        submesh.face_ids().clear();

        for (id_type idFace : face_ids)
        {
            assert(face_mapping.contains(idFace));
            submesh.face_ids().insert(face_mapping[idFace]);
        }

        const typename submesh_type::surface_ids_type surface_ids = std::move(submesh.surface_ids());
        submesh.surface_ids().clear();

        for (id_type idSurface : surface_ids)
        {
            assert(surface_mapping.contains(idSurface));
            submesh.surface_ids().insert(surface_mapping[idSurface]);
        }

        submesh.regenerate_surface_index();
    }

    regenerate_surface_index();
    regenerate_submesh_index();
    return;
}

//--------------------------------------------------------------------------
template<typename Traits>
bool quetzal::brep::Mesh<Traits>::empty() const
{
    return halfedge_count() == 0 || vertex_count() == 0 || face_count() == 0;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::clear()
{
    m_halfedge_store.clear();
    m_vertex_store.clear();
    m_face_store.clear();
    m_surface_store.clear();
    m_surface_index.clear();
    m_submesh_store.clear();
    m_submesh_index.clear();
    m_seam_store.clear();
    return;
}

//--------------------------------------------------------------------------
template<typename Traits>
bool quetzal::brep::Mesh<Traits>::unmarked() const
{
    if (marked())
    {
        return false;
    }

    for (halfedge_type& halfedge : m_halfedges)
    {
        if (halfedge.marked())
        {
            return false;
        }
    }

    for (vertex_type& vertex : m_vertices)
    {
        if (vertex.marked())
        {
            return false;
        }
    }

    for (face_type& face : m_faces)
    {
        if (face.marked())
        {
            return false;
        }
    }

    for (surface_type& surface : m_surfaces)
    {
        if (surface.marked())
        {
            return false;
        }
    }

    for (submesh_type& submesh : m_submeshes)
    {
        if (submesh.marked())
        {
            return false;
        }
    }

    for (seam_type& seam : m_seams)
    {
        if (seam.marked())
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::reset() const
{
    for (const halfedge_type& halfedge : m_halfedge_store)
    {
        halfedge.Flags::reset();
    }

    for (const vertex_type& vertex : m_vertex_store)
    {
        vertex.Flags::reset();
    }

    for (const face_type& face : m_face_store)
    {
        face.Flags::reset();
    }

    for (const surface_type& surface : m_surfaces)
    {
        surface.Flags::reset();
    }

    for (const submesh_type& submesh : m_submeshes)
    {
        submesh.Flags::reset();
    }

    for (const seam_type& seam : m_seam_store)
    {
        seam.Flags::reset();
    }

    Flags::reset();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::reassign_mesh()
{
    for (halfedge_type& halfedge : m_halfedge_store)
    {
        halfedge.set_mesh(*this);
    }
    m_halfedges.set_source(*this);

    for (vertex_type& vertex : m_vertex_store)
    {
        vertex.set_mesh(*this);
    }
    m_vertices.set_source(*this);

    for (face_type& face : m_face_store)
    {
        face.set_mesh(*this);
    }
    m_faces.set_source(*this);

    for (surface_type& surface : m_surface_store)
    {
        surface.set_mesh(*this);
    }
    m_surfaces.set_source(*this);

    for (submesh_type& submesh : m_submesh_store)
    {
        submesh.set_mesh(*this);
    }
    m_submeshes.set_source(*this);

    for (seam_type& seam : m_seam_store)
    {
        seam.set_mesh(*this);
    }
    m_seams.set_source(*this);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::regenerate_surface_index()
{
    m_surface_index.clear();

    for (const surface_type& surface : m_surfaces)
    {
        m_surface_index[surface.extended_name()] = surface.id();
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::regenerate_submesh_index()
{
    m_submesh_index.clear();

    for (const submesh_type& submesh : m_submeshes)
    {
        m_submesh_index[submesh.name()] = submesh.id();
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::brep::Mesh<Traits>::check() const
{
//    return error_count() == 0;
return true; // ...
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::size_type quetzal::brep::Mesh<Traits>::error_count() const
{
    size_type n = 0;

    for (const halfedge_type halfedge : m_halfedge_store)
    {
        n += halfedge.error_count();
    }

    for (const vertex_type vertex : m_vertex_store)
    {
        n += vertex.check() ? 0 : 1;
    }

    for (const face_type face : m_face_store)
    {
        n += face.check() ? 0 : 1;
    }

    return n;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::Mesh<Traits>::check_mesh() const
{
    for (const halfedge_type& halfedge : m_halfedge_store)
    {
        halfedge.check_mesh(this);
    }
    m_halfedges.check_mesh(this);

    for (const vertex_type& vertex : m_vertex_store)
    {
        vertex.check_mesh(this);
    }
    m_vertices.check_mesh(this);

    for (const face_type& face : m_face_store)
    {
        face.check_mesh(this);
    }
    m_faces.check_mesh(this);

    for (const surface_type& surface : m_surface_store)
    {
        surface.check_mesh(this);
    }
    m_surfaces.check_mesh(this);

    for (const submesh_type& submesh : m_submesh_store)
    {
        submesh.check_mesh(this);
    }
    m_submeshes.check_mesh(this);

    for (const seam_type& seam : m_seam_store)
    {
        seam.check_mesh(this);
    }
    m_seams.check_mesh(this);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::swap(Mesh<Traits>& lhs, Mesh<Traits>& rhs)
{
    using std::swap;
    swap(static_cast<Flags&>(lhs), static_cast<Flags&>(rhs));
    swap(lhs.m_id, rhs.m_id);
    swap(lhs.m_name, rhs.m_name);
    swap(lhs.m_halfedge_store, rhs.m_halfedge_store);
    swap(lhs.m_vertex_store, rhs.m_vertex_store);
    swap(lhs.m_face_store, rhs.m_face_store);
    swap(lhs.m_surface_store, rhs.m_surface_store);
    swap(lhs.m_surface_index, rhs.m_surface_index);
    swap(lhs.m_submesh_store, rhs.m_submesh_store);
    swap(lhs.m_submesh_index, rhs.m_submesh_index);
    swap(lhs.m_seam_store, rhs.m_seam_store);
    swap(lhs.properties(), rhs.properties());

    lhs.reassign_mesh();
    rhs.reassign_mesh();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
std::string quetzal::brep::Mesh<Traits>::extended_surface_name(id_type idSubmesh, const std::string& name) const
{
    return (idSubmesh != nullid ? submesh(idSubmesh).name() : "") + "/" + name;
}

//------------------------------------------------------------------------------
template<typename Traits>
std::array<std::string, 2> quetzal::brep::Mesh<Traits>::split_surface_name(const std::string& name)
{
    auto pos = name.find_first_of("/");
    if (pos != std::string::npos)
    {
        return {name.substr(0, pos), name.substr(pos + 1)};
    }

    return {name, ""};
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::halfedges_type::size_function_type quetzal::brep::Mesh<Traits>::m_halfedges_size = [](const Mesh<Traits>& mesh, id_type id) -> size_t
{
    id;
    return mesh.halfedge_count();
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::halfedges_type::terminal_function_type quetzal::brep::Mesh<Traits>::m_halfedges_first = [](const Mesh<Traits>& mesh, id_type id) -> id_type
{
    for (const auto& halfedge : mesh.halfedge_store())
    {
        if (!halfedge.deleted())
        {
            return halfedge.id();
        }
    }

    return m_halfedges_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::halfedges_type::terminal_function_type quetzal::brep::Mesh<Traits>::m_halfedges_last = [](const Mesh<Traits>& mesh, id_type id) -> id_type
{
    for (auto i = mesh.halfedge_store().crbegin(); i != mesh.halfedge_store().crend(); ++i)
    {
        if (!i->deleted())
        {
            return i->id();
        }
    }

    return m_halfedges_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::halfedges_type::terminal_function_type quetzal::brep::Mesh<Traits>::m_halfedges_end = [](const Mesh<Traits>& mesh, id_type id) -> id_type
{
    mesh;
    id;
    return nullid;
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::halfedges_type::iterate_function_type quetzal::brep::Mesh<Traits>::m_halfedges_forward = [](const Mesh<Traits>& mesh, id_type id, id_type i) -> id_type
{
    assert(i != m_halfedges_end(mesh, id));
    assert(i < mesh.halfedge_store_count());
    for (++i; i < mesh.halfedge_store_count(); ++i)
    {
        if (!mesh.halfedge(i).deleted())
        {
            return i;
        }
    }

    return m_halfedges_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::halfedges_type::iterate_function_type quetzal::brep::Mesh<Traits>::m_halfedges_reverse = [](const Mesh<Traits>& mesh, id_type id, id_type i) -> id_type
{
    assert(i != m_halfedges_end(mesh, id));
    assert(i < mesh.halfedge_store_count());
    while (i > 0)
    {
        if (!mesh.halfedge(--i).deleted())
        {
            return i;
        }
    }

    return m_halfedges_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::halfedges_type::element_function_type quetzal::brep::Mesh<Traits>::m_halfedges_element = [](Mesh<Traits>& mesh, id_type id, id_type i) -> typename quetzal::brep::Mesh<Traits>::halfedge_type&
{
    id;
    auto& halfedge = mesh.halfedge(i);
    assert(!halfedge.deleted());
    return halfedge;
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::halfedges_type::const_element_function_type quetzal::brep::Mesh<Traits>::m_halfedges_const_element = [](const Mesh<Traits>& mesh, id_type id, id_type i) -> const typename quetzal::brep::Mesh<Traits>::halfedge_type&
{
    id;
    const auto& halfedge = mesh.halfedge(i);
    assert(!halfedge.deleted());
    return halfedge;
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::vertices_type::size_function_type quetzal::brep::Mesh<Traits>::m_vertices_size = [](const Mesh<Traits>& mesh, id_type id) -> size_t
{
    id;
    return mesh.vertex_count();
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::vertices_type::terminal_function_type quetzal::brep::Mesh<Traits>::m_vertices_first = [](const Mesh<Traits>& mesh, id_type id) -> id_type
{
    for (const auto& vertex : mesh.vertex_store())
    {
        if (!vertex.deleted())
        {
            return vertex.id();
        }
    }

    return m_vertices_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::vertices_type::terminal_function_type quetzal::brep::Mesh<Traits>::m_vertices_last = [](const Mesh<Traits>& mesh, id_type id) -> id_type
{
    for (auto i = mesh.vertex_store().crbegin(); i != mesh.vertex_store().crend(); ++i)
    {
        if (!i->deleted())
        {
            return i->id();
        }
    }

    return m_vertices_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::vertices_type::terminal_function_type quetzal::brep::Mesh<Traits>::m_vertices_end = [](const Mesh<Traits>& mesh, id_type id) -> id_type
{
    mesh;
    id;
    return nullid;
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::vertices_type::iterate_function_type quetzal::brep::Mesh<Traits>::m_vertices_forward = [](const Mesh<Traits>& mesh, id_type id, id_type i) -> id_type
{
    assert(i != m_vertices_end(mesh, id));
    assert(i < mesh.vertex_store_count());
    for (++i; i < mesh.vertex_store_count(); ++i)
    {
        if (!mesh.vertex(i).deleted())
        {
            return i;
        }
    }

    return m_vertices_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::vertices_type::iterate_function_type quetzal::brep::Mesh<Traits>::m_vertices_reverse = [](const Mesh<Traits>& mesh, id_type id, id_type i) -> id_type
{
    assert(i != m_vertices_end(mesh, id));
    assert(i < mesh.vertex_store_count());
    while (i > 0)
    {
        if (!mesh.vertex(--i).deleted())
        {
            return i;
        }
    }

    return m_vertices_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::vertices_type::element_function_type quetzal::brep::Mesh<Traits>::m_vertices_element = [](Mesh<Traits>& mesh, id_type id, id_type i) -> typename quetzal::brep::Mesh<Traits>::vertex_type&
{
    id;
    auto& vertex = mesh.vertex(i);
    assert(!vertex.deleted());
    return vertex;
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::vertices_type::const_element_function_type quetzal::brep::Mesh<Traits>::m_vertices_const_element = [](const Mesh<Traits>& mesh, id_type id, id_type i) -> const typename quetzal::brep::Mesh<Traits>::vertex_type&
{
    id;
    const auto& vertex = mesh.vertex(i);
    assert(!vertex.deleted());
    return vertex;
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::faces_type::size_function_type quetzal::brep::Mesh<Traits>::m_faces_size = [](const Mesh<Traits>& mesh, id_type id) -> size_t
{
    id;
    return mesh.face_count();
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::faces_type::terminal_function_type quetzal::brep::Mesh<Traits>::m_faces_first = [](const Mesh<Traits>& mesh, id_type id) -> id_type
{
    for (const auto& face : mesh.face_store())
    {
        if (!face.deleted())
        {
            return face.id();
        }
    }

    return m_faces_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::faces_type::terminal_function_type quetzal::brep::Mesh<Traits>::m_faces_last = [](const Mesh<Traits>& mesh, id_type id) -> id_type
{
    for (auto i = mesh.face_store().crbegin(); i != mesh.face_store().crend(); ++i)
    {
        if (!i->deleted())
        {
            return i->id();
        }
    }

    return m_faces_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::faces_type::terminal_function_type quetzal::brep::Mesh<Traits>::m_faces_end = [](const Mesh<Traits>& mesh, id_type id) -> id_type
{
    mesh;
    id;
    return nullid;
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::faces_type::iterate_function_type quetzal::brep::Mesh<Traits>::m_faces_forward = [](const Mesh<Traits>& mesh, id_type id, id_type i) -> id_type
{
    assert(i != m_faces_end(mesh, id));
    assert(i < mesh.face_store_count());
    for (++i; i < mesh.face_store_count(); ++i)
    {
        if (!mesh.face(i).deleted())
        {
            return i;
        }
    }

    return m_faces_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::faces_type::iterate_function_type quetzal::brep::Mesh<Traits>::m_faces_reverse = [](const Mesh<Traits>& mesh, id_type id, id_type i) -> id_type
{
    assert(i != m_faces_end(mesh, id));
    assert(i < mesh.face_store_count());
    while (i > 0)
    {
        if (!mesh.face(--i).deleted())
        {
            return i;
        }
    }

    return m_faces_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::faces_type::element_function_type quetzal::brep::Mesh<Traits>::m_faces_element = [](Mesh<Traits>& mesh, id_type id, id_type i) -> typename quetzal::brep::Mesh<Traits>::face_type&
{
    id;
    auto& face = mesh.face(i);
    assert(!face.deleted());
    return face;
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::faces_type::const_element_function_type quetzal::brep::Mesh<Traits>::m_faces_const_element = [](const Mesh<Traits>& mesh, id_type id, id_type i) -> const typename quetzal::brep::Mesh<Traits>::face_type&
{
    id;
    const auto& face = mesh.face(i);
    assert(!face.deleted());
    return face;
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::surfaces_type::size_function_type quetzal::brep::Mesh<Traits>::m_surfaces_size = [](const Mesh<Traits>& mesh, id_type id) -> size_t
{
    id;
    return mesh.surface_count();
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::surfaces_type::terminal_function_type quetzal::brep::Mesh<Traits>::m_surfaces_first = [](const Mesh<Traits>& mesh, id_type id) -> id_type
{
    for (const auto& surface : mesh.surface_store())
    {
        if (!surface.deleted())
        {
            return surface.id();
        }
    }

    return m_surfaces_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::surfaces_type::terminal_function_type quetzal::brep::Mesh<Traits>::m_surfaces_last = [](const Mesh<Traits>& mesh, id_type id) -> id_type
{
    for (auto i = mesh.surface_store().crbegin(); i != mesh.surface_store().crend(); ++i)
    {
        if (!i->deleted())
        {
            return i->id();
        }
    }

    return m_surfaces_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::surfaces_type::terminal_function_type quetzal::brep::Mesh<Traits>::m_surfaces_end = [](const Mesh<Traits>& mesh, id_type id) -> id_type
{
    mesh;
    id;
    return nullid;
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::surfaces_type::iterate_function_type quetzal::brep::Mesh<Traits>::m_surfaces_forward = [](const Mesh<Traits>& mesh, id_type id, id_type i) -> id_type
{
    assert(i != m_surfaces_end(mesh, id));
    assert(i < mesh.surface_store_count());
    for (++i; i < mesh.surface_store_count(); ++i)
    {
        if (!mesh.surface(i).deleted())
        {
            return i;
        }
    }

    return m_surfaces_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::surfaces_type::iterate_function_type quetzal::brep::Mesh<Traits>::m_surfaces_reverse = [](const Mesh<Traits>& mesh, id_type id, id_type i) -> id_type
{
    assert(i != m_surfaces_end(mesh, id));
    assert(i < mesh.surface_store_count());
    while (i > 0)
    {
        if (!mesh.surface(--i).deleted())
        {
            return i;
        }
    }

    return m_surfaces_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::surfaces_type::element_function_type quetzal::brep::Mesh<Traits>::m_surfaces_element = [](Mesh<Traits>& mesh, id_type id, id_type i) -> typename quetzal::brep::Mesh<Traits>::surface_type&
{
    id;
    auto& surface = mesh.surface(i);
    assert(!surface.deleted());
    return surface;
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::surfaces_type::const_element_function_type quetzal::brep::Mesh<Traits>::m_surfaces_const_element = [](const Mesh<Traits>& mesh, id_type id, id_type i) -> const typename quetzal::brep::Mesh<Traits>::surface_type&
{
    id;
    const auto& surface = mesh.surface(i);
    assert(!surface.deleted());
    return surface;
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::submeshes_type::size_function_type quetzal::brep::Mesh<Traits>::m_submeshes_size = [](const Mesh<Traits>& mesh, id_type id) -> size_t
{
    id;
    return mesh.submesh_count();
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::submeshes_type::terminal_function_type quetzal::brep::Mesh<Traits>::m_submeshes_first = [](const Mesh<Traits>& mesh, id_type id) -> id_type
{
    for (const auto& submesh : mesh.submesh_store())
    {
        if (!submesh.deleted())
        {
            return submesh.id();
        }
    }

    return m_submeshes_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::submeshes_type::terminal_function_type quetzal::brep::Mesh<Traits>::m_submeshes_last = [](const Mesh<Traits>& mesh, id_type id) -> id_type
{
    for (auto i = mesh.submesh_store().crbegin(); i != mesh.submesh_store().crend(); ++i)
    {
        if (!i->deleted())
        {
            return i->id();
        }
    }

    return m_submeshes_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::submeshes_type::terminal_function_type quetzal::brep::Mesh<Traits>::m_submeshes_end = [](const Mesh<Traits>& mesh, id_type id) -> id_type
{
    mesh;
    id;
    return nullid;
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::submeshes_type::iterate_function_type quetzal::brep::Mesh<Traits>::m_submeshes_forward = [](const Mesh<Traits>& mesh, id_type id, id_type i) -> id_type
{
    assert(i != m_submeshes_end(mesh, id));
    assert(i < mesh.submesh_store_count());
    for (++i; i < mesh.submesh_store_count(); ++i)
    {
        if (!mesh.submesh(i).deleted())
        {
            return i;
        }
    }

    return m_submeshes_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::submeshes_type::iterate_function_type quetzal::brep::Mesh<Traits>::m_submeshes_reverse = [](const Mesh<Traits>& mesh, id_type id, id_type i) -> id_type
{
    assert(i != m_submeshes_end(mesh, id));
    assert(i < mesh.submesh_store_count());
    while (i > 0)
    {
        if (!mesh.submesh(--i).deleted())
        {
            return i;
        }
    }

    return m_submeshes_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::submeshes_type::element_function_type quetzal::brep::Mesh<Traits>::m_submeshes_element = [](Mesh<Traits>& mesh, id_type id, id_type i) -> typename quetzal::brep::Mesh<Traits>::submesh_type&
{
    id;
    auto& submesh = mesh.submesh(i);
    assert(!submesh.deleted());
    return submesh;
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::submeshes_type::const_element_function_type quetzal::brep::Mesh<Traits>::m_submeshes_const_element = [](const Mesh<Traits>& mesh, id_type id, id_type i) -> const typename quetzal::brep::Mesh<Traits>::submesh_type&
{
    id;
    const auto& submesh = mesh.submesh(i);
    assert(!submesh.deleted());
    return submesh;
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::seams_type::size_function_type quetzal::brep::Mesh<Traits>::m_seams_size = [](const Mesh<Traits>& mesh, id_type id) -> size_t
{
    id;
    return mesh.seam_count();
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::seams_type::terminal_function_type quetzal::brep::Mesh<Traits>::m_seams_first = [](const Mesh<Traits>& mesh, id_type id) -> id_type
{
    for (const auto& seam : mesh.seam_store())
    {
        if (!seam.deleted())
        {
            return seam.id();
        }
    }

    return m_seams_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::seams_type::terminal_function_type quetzal::brep::Mesh<Traits>::m_seams_last = [](const Mesh<Traits>& mesh, id_type id) -> id_type
{
    for (auto i = mesh.seam_store().crbegin(); i != mesh.seam_store().crend(); ++i)
    {
        if (!i->deleted())
        {
            return i->id();
        }
    }

    return m_seams_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::seams_type::terminal_function_type quetzal::brep::Mesh<Traits>::m_seams_end = [](const Mesh<Traits>& mesh, id_type id) -> id_type
{
    mesh;
    id;
    return nullid;
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::seams_type::iterate_function_type quetzal::brep::Mesh<Traits>::m_seams_forward = [](const Mesh<Traits>& mesh, id_type id, id_type i) -> id_type
{
    assert(i != m_seams_end(mesh, id));
    assert(i < mesh.seam_store_count());
    for (++i; i < mesh.seam_store_count(); ++i)
    {
        if (!mesh.seam(i).deleted())
        {
            return i;
        }
    }

    return m_seams_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::seams_type::iterate_function_type quetzal::brep::Mesh<Traits>::m_seams_reverse = [](const Mesh<Traits>& mesh, id_type id, id_type i) -> id_type
{
    assert(i != m_seams_end(mesh, id));
    assert(i < mesh.seam_store_count());
    while (i > 0)
    {
        if (!mesh.seam(--i).deleted())
        {
            return i;
        }
    }

    return m_seams_end(mesh, id);
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::seams_type::element_function_type quetzal::brep::Mesh<Traits>::m_seams_element = [](Mesh<Traits>& mesh, id_type id, id_type i) -> typename quetzal::brep::Mesh<Traits>::seam_type&
{
    id;
    auto& seam = mesh.seam(i);
    assert(!seam.deleted());
    return seam;
};

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::brep::Mesh<Traits>::seams_type::const_element_function_type quetzal::brep::Mesh<Traits>::m_seams_const_element = [](const Mesh<Traits>& mesh, id_type id, id_type i) -> const typename quetzal::brep::Mesh<Traits>::seam_type&
{
    id;
    const auto& seam = mesh.seam(i);
    assert(!seam.deleted());
    return seam;
};

#endif // QUETZAL_BREP_MESH_HPP
