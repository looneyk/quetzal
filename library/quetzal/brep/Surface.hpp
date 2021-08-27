#if !defined(QUETZAL_BREP_SURFACE_HPP)
#define QUETZAL_BREP_SURFACE_HPP
//------------------------------------------------------------------------------
// brep
// Surface.hpp
//------------------------------------------------------------------------------

#include "Flags.hpp"
#include "Perimeter.hpp"
#include "Seam.hpp"
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
        using attributes_type = mesh_type::surface_attributes_type;
        using vector_type = mesh_type::vector_type;
        using vertex_type = mesh_type::vertex_type;
        using face_type = mesh_type::face_type;
        using face_ids_type = std::set<id_type>;
        using faces_type = Elements<mesh_type, face_type, face_ids_type::iterator>;
        using halfedge_type = mesh_type::halfedge_type;
        using perimeter_type = Perimeter<Traits, M>;
        using perimeters_type = std::vector<perimeter_type>;
        using submesh_type = mesh_type::submesh_type;
        using size_type = Traits::size_type;

        Surface();
        Surface(mesh_type& mesh, id_type id, const std::string& name, id_type idSubmesh, const attributes_type& attributes, const Properties& properties = {});
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

        const attributes_type& attributes() const;
        attributes_type& attributes();
        void set_attributes(const attributes_type& attributes);

        const Properties& properties() const;
        Properties& properties();

        // Virtual container interface, iterators for faces making up this surface
        size_type face_count() const;
        const faces_type& faces() const;
        faces_type& faces();

        size_type perimeter_count() const;
        const perimeters_type& perimeters() const;
        perimeters_type& perimeters();

//        perimeter_type& perimeter(id_type idPerimeter); // only useful on generation? ...

        std::vector<id_type> find_seams(id_type idSurfacePartner);
        std::vector<id_type> find_seams(const std::string& nameSurfacePartner);

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

        void set_regenerate_perimeters(bool b = true);

        // Internal use, only by Mesh
        void set_mesh(mesh_type& mesh);
        void set_id(id_type id);
        void check_mesh(const mesh_type* const pmesh) const;

    private:

        id_type create_perimeter();

        void check_regenerate_perimeters();
        void generate_perimeters();
        void generate_perimeter(id_type idHalfedge);
        std::array<id_type, 2> generate_seam(id_type idNext, id_type idPrev, id_type idHalfedge, id_type idPerimeter);

        mesh_type* m_pmesh;
        id_type m_id;
        std::string m_name;
        id_type m_idSubmesh;
        face_ids_type m_face_ids;
        attributes_type m_attributes;
        Properties m_properties;

        faces_type m_faces;
        perimeters_type m_perimeters;

        bool m_bRegeneratePerimeters;
        bool m_bGeneratingPerimeters;

        static faces_type::size_function_type m_faces_size;
        static faces_type::terminal_function_type m_faces_first;
        static faces_type::terminal_function_type m_faces_last;
        static faces_type::terminal_function_type m_faces_end;
        static faces_type::iterate_function_type m_faces_forward;
        static faces_type::iterate_function_type m_faces_reverse;
        static faces_type::element_function_type m_faces_element;
        static faces_type::const_element_function_type m_faces_const_element;
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
    m_attributes(),
    m_properties(),
    m_faces(*m_pmesh, nullid, m_faces_size, m_faces_first, m_faces_last, m_faces_end, m_faces_forward, m_faces_reverse, m_faces_element, m_faces_const_element),
    m_perimeters(),
    m_bRegeneratePerimeters(false),
    m_bGeneratingPerimeters(false)
{
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::brep::Surface<Traits, M>::Surface(mesh_type& mesh, id_type id, const std::string& name, id_type idSubmesh, const attributes_type& attributes, const Properties& properties) :
    Flags(),
    m_pmesh(&mesh),
    m_id(id),
    m_name(name),
    m_idSubmesh(idSubmesh),
    m_face_ids(),
    m_attributes(attributes),
    m_properties(properties),
    m_faces(mesh, id, m_faces_size, m_faces_first, m_faces_last, m_faces_end, m_faces_forward, m_faces_reverse, m_faces_element, m_faces_const_element),
    m_perimeters(),
    m_bRegeneratePerimeters(false),
    m_bGeneratingPerimeters(false)
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
typename quetzal::brep::Surface<Traits, M>::size_type quetzal::brep::Surface<Traits, M>::perimeter_count() const
{
    check_regenerate_perimeters();
    return m_perimeters.size();
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
const typename quetzal::brep::Surface<Traits, M>::perimeters_type& quetzal::brep::Surface<Traits, M>::perimeters() const
{
    check_regenerate_perimeters();
    return m_perimeters;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::perimeters_type& quetzal::brep::Surface<Traits, M>::perimeters()
{
    check_regenerate_perimeters();
    return m_perimeters;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
std::vector<quetzal::id_type> quetzal::brep::Surface<Traits, M>::find_seams(id_type idSurfacePartner)
{
    check_regenerate_perimeters();
    std::vector<id_type> ids;

    for (const auto& perimeter : m_perimeters)
    {
        auto idsFound = perimeter.find_seams(idSurfacePartner);
        ids.insert(end(ids), begin(idsFound), end(idsFound));
    }

    return ids;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
std::vector<quetzal::id_type> quetzal::brep::Surface<Traits, M>::find_seams(const std::string& nameSurfacePartner)
{
    assert(m_pmesh != nullptr);
    check_regenerate_perimeters();
    std::vector<id_type> ids;

    for (const auto& perimeter : m_perimeters)
    {
        auto idsFound = perimeter.find_seams(nameSurfacePartner);
        ids.insert(end(ids), begin(idsFound), end(idsFound));
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
    m_perimeters.clear();
    m_bRegeneratePerimeters = false;
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

    for (const auto& perimeter : m_perimeters)
    {
        if (perimeter.marked())
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

    for (const auto& perimeter : m_perimeters)
    {
        perimeter.reset();
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
    set_regenerate_perimeters();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::remove_face(id_type idFace)
{
    assert(m_face_ids.contains(idFace));
    m_face_ids.erase(idFace);
    set_regenerate_perimeters();
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
void quetzal::brep::Surface<Traits, M>::set_regenerate_perimeters(bool b)
{
    m_bRegeneratePerimeters = b;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::set_mesh(mesh_type& mesh)
{
    m_pmesh = &mesh;
    m_faces.set_source(mesh);

    for (auto& perimeter : m_perimeters)
    {
        perimeter.set_mesh(mesh);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::set_id(id_type id)
{
    m_id = id;
    m_faces.set_id(id);

    for (auto& perimeter : m_perimeters)
    {
        perimeter.set_surface_id(id);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::check_mesh(const mesh_type* const pmesh) const
{
    assert(m_pmesh == pmesh);
    assert(m_faces.check_source(pmesh));

    for (auto& perimeter : m_perimeters)
    {
        perimeter.check_mesh(pmesh);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::check_regenerate_perimeters()
{
    if (m_bRegeneratePerimeters && !m_bGeneratingPerimeters)
    {
        m_bGeneratingPerimeters = true;
        generate_perimeters();
        m_bRegeneratePerimeters = false;
        m_bGeneratingPerimeters = false;
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
quetzal::id_type quetzal::brep::Surface<Traits, M>::create_perimeter()
{
    id_type idPerimeter = m_perimeters.size();
    m_perimeters.emplace_back(*m_pmesh, m_id);
    return idPerimeter;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::generate_perimeters()
{
    reset();

    for (const auto& face : faces())
    {
        for (const auto& halfedge : face.halfedges())
        {
            if (halfedge.marked())
            {
                continue;
            }

            if (halfedge.border() || halfedge.partner_surface_id() != m_id)
            {
                generate_perimeter(halfedge.id());
            }
            else
            {
                halfedge.set_marked();
                halfedge.partner().set_marked();
            }
        }
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
void quetzal::brep::Surface<Traits, M>::generate_perimeter(id_type idHalfedge)
{
    assert(m_pmesh != nullptr);
    assert(m_pmesh->halfedge(idHalfedge).surface_id() != m_pmesh->halfedge(idHalfedge).partner_surface_id());

    id_type idHalfedgePrev = prev_surface_halfedge_id(*m_pmesh, idHalfedge);
    id_type idHalfedge0 = idHalfedge;
    while (idHalfedge != idHalfedge0 && idHalfedge != idHalfedgePrev)
    {
        idHalfedgePrev = idHalfedge;
        idHalfedge = next_surface_halfedge_id(*m_pmesh, idHalfedge);
    }

    id_type idPerimeter = create_perimeter();
    auto& perimeter = perimeters()[idPerimeter];

    idHalfedge0 = idHalfedge;
    id_type idSeam0 = nullid;
    id_type idSeamPrev = nullid;
    do
    {
        auto [idSeam, idHalfedgeNext] = generate_seam(nullid, idSeamPrev, idHalfedge, idPerimeter);

        if (idSeamPrev != nullid)
        {
            perimeter.seams()[idSeamPrev].set_next_id(idSeam);
            idSeam0 = idSeam;
        }

        idSeamPrev = idSeam;
        idHalfedge = idHalfedgeNext;
    } while (idHalfedge != idHalfedge0);

    perimeter.seams()[idSeam0].set_prev_id(idSeamPrev);
    perimeter.seams()[idSeamPrev].set_next_id(idSeam0);

    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename M>
std::array<quetzal::id_type, 2> quetzal::brep::Surface<Traits, M>::generate_seam(id_type idNext, id_type idPrev, id_type idHalfedge, id_type idPerimeter)
{
    assert(m_pmesh != nullptr);
    assert(m_pmesh->halfedge(idHalfedge).surface_seam());

    auto& perimeter = perimeters()[idPerimeter];
    id_type idSeam = perimeter.create_seam(idNext, idPrev, idHalfedge, idPerimeter);

    id_type idHalfedge0 = idHalfedge;

    do
    {
        m_pmesh->halfedge(idHalfedge).set_marked();
        idHalfedge = next_surface_halfedge_id(*m_pmesh, idHalfedge);
    } while (idHalfedge != idHalfedge0 && m_pmesh->halfedge(idHalfedge).surface_id() == m_id);

    return {idSeam, idHalfedge};
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
typename quetzal::brep::Surface<Traits, M>::faces_type::element_function_type quetzal::brep::Surface<Traits, M>::m_faces_element = [](mesh_type& mesh, id_type id, face_ids_type::iterator i) -> quetzal::brep::Surface<Traits, M>::face_type&
{
    assert(i != m_faces_end(mesh, id));
    auto& face = mesh.face(*i);
    assert(!face.deleted());
    return face;
};

//------------------------------------------------------------------------------
template<typename Traits, typename M>
typename quetzal::brep::Surface<Traits, M>::faces_type::const_element_function_type quetzal::brep::Surface<Traits, M>::m_faces_const_element = [](const mesh_type& mesh, id_type id, face_ids_type::iterator i) -> const quetzal::brep::Surface<Traits, M>::face_type&
{
    assert(i != m_faces_end(mesh, id));
    const auto& face = mesh.face(*i);
    assert(!face.deleted());
    return face;
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
