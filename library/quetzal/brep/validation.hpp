#if !defined(QUETZAL_BREP_VALIDATION_HPP)
#define QUETZAL_BREP_VALIDATION_HPP
//------------------------------------------------------------------------------
// brep
// validation.hpp
//------------------------------------------------------------------------------

#include "mesh_geometry.hpp"
#include "mesh_util.hpp"
#include "quetzal/common/string_util.hpp"
#include "quetzal/geometry/Attributes.hpp"
#include "quetzal/math/floating_point.hpp"
#include "quetzal/math/math_util.hpp"
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

namespace quetzal::brep
{

    constexpr int v_ulp = math::ulpDefault;
//    constexpr int v_ulp = 1;
    template<typename M>
    constexpr typename M::value_type v_max = M::val(1000);

    template<typename M>
    bool check_halfedge(const M& mesh, const typename M::halfedge_type& halfedge, bool bBorderOK = true);

    template<typename M>
    bool check_halfedges(const M& mesh);

    template<typename M>
    bool check_vertex(const M& mesh, const typename M::vertex_type& vertex);

    template<typename M>
    bool check_vertices(const M& mesh);

    template<typename M>
    bool check_face(const M& mesh, const typename M::face_type& face);

    template<typename M>
    bool check_faces(const M& mesh);

    template<typename M>
    bool check_border_face(const M& mesh, id_type idHalfedge);

    template<typename M>
    bool check_surfaces(const M& mesh);

    template<typename M>
    bool check_submeshes(const M& mesh);

    template<typename M>
    bool check_triangular_face(const M& mesh, const typename M::face_type& face);

    template<typename M>
    bool check_triangulation(const M& mesh);

    template<typename M>
    bool check_manifold_edges(const M& mesh);

    template<typename M>
    bool check_adjacent_face_normals(const M& mesh);

    template<typename M>
    bool check_adjacent_face_normals(const M& mesh, const typename M::face_type& face);

    template<typename M>
    bool check_split_edge(const M& mesh, const typename M::halfedge_type& halfedgeOrig, const typename M::halfedge_type& partnerOrig, const typename M::halfedge_type& halfedge);

    template<typename M>
    bool check_split_face(const M& mesh, id_type idHalfedgeA, id_type idHalfedgeB, size_t nEdgesOrig);

    template<typename M>
    bool check_attributes(const M& mesh);

    template<typename M>
    void write_halfedge(const typename M::halfedge_type& halfedge, const std::string& label = "");

    template<typename M>
    void write_vertex(const typename M::vertex_type& vertex, const std::string& label = "");

    template<typename M>
    void write_face(const typename M::face_type& face, const std::string& label = "");

    template<typename M>
    void write_face_vertices(const M& mesh, id_type idHalfedge);

    template<typename M>
    void write_hole_vertices(const M& mesh, id_type idHalfedge);

    template<typename M>
    void write_surface(const typename M::surface_type& surface, const std::string& label = "");

    template<typename M>
    void write_submesh(const typename M::submesh_type& submesh, const std::string& label = "");

    template<typename M>
    void write_summary(const M& mesh);

    template<typename M>
    void validate(const M& mesh);

    template<typename M>
    void dump(const M& mesh);

    // Functions to validate mesh geometry
    // model/attributes_validation.hpp? ...

    // Calculates face normals and compares with each face vertex normal
    template<typename M>
    bool check_face_normals(const M& mesh);

    // Calculates face normal and compares with each face vertex normal
    template<typename M>
    bool check_face_normal(const M& mesh, const typename M::face_type& face);

    template<typename M>
    bool check_normal_consistency(const M& mesh, bool bCheckPosition = false);

    template<typename M>
    bool check_normal_consistency(const M& mesh, const typename M::face_type& face, bool bCheckPosition = false);

    template<typename M>
    bool check_surface_normal_consistency(const M& mesh);

    template<typename M>
    bool check_surface_normal_consistency(const M& mesh, const typename M::surface_type& surface);

    template<typename M>
    bool check_surface_normal_consistency(const M& mesh, const typename M::surface_type& surface, const typename M::vertex_type& vertex);

    template<typename M>
    bool check_face_angles(const M& mesh);

    template<typename M>
    bool check_face_angles(const M& mesh, const typename M::face_type& face);

    template<typename M>
    bool check_convex(const M& mesh);

    template<typename M>
    bool check_convex(const M& mesh, const typename M::face_type& face);

    template<typename M>
    bool check_outward_normals(const M& mesh);

    template<typename M>
    bool check_spherical(const M& mesh, typename M::value_type radius);

    template<typename M>
    std::map<typename M::value_type, size_t> count_edge_lengths(const M& mesh); // Returns count of each distinct length

} // namespace quetzal::brep

//--------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_halfedge(const M& mesh, const typename M::halfedge_type& halfedge, bool bBorderOK)
{
    bool bOK = true;
    if (halfedge.deleted())
    {
        return bOK;
    }

    std::ostringstream oss;

    if (mesh.halfedge(halfedge.id()).id() != halfedge.id())
    {
        oss << "  Bad halfedge id " << "mesh.halfedge(halfedge.id()).id() != halfedge.id() " << mesh.halfedge(halfedge.id()).id() << " " << halfedge.id() << std::endl;
        bOK = false;
    }

    if (halfedge.partner_id() == nullid)
    {
        oss << "  Border" << std::endl;
        if (!bBorderOK)
        {
            bOK = false;
        }
    }
    else if (!good(halfedge.partner_id(), mesh.halfedge_store()))
    {
        oss << "  Bad partner " << print(halfedge.partner_id(), mesh.halfedge_store()) << std::endl;
        bOK = false;
    }

    if (!good(halfedge.next_id(), mesh.halfedge_store()))
    {
        oss << "  Bad next " << print(halfedge.next_id(), mesh.halfedge_store()) << std::endl;
        bOK = false;
    }

    if (!good(halfedge.prev_id(), mesh.halfedge_store()))
    {
        oss << "  Bad prev " << print(halfedge.prev_id(), mesh.halfedge_store()) << std::endl;
        bOK = false;
    }

    if (!good(halfedge.vertex_id(), mesh.vertex_store()))
    {
        oss << "  Bad vertex " << print(halfedge.vertex_id(), mesh.vertex_store()) << std::endl;
        bOK = false;
    }

    if (!good(halfedge.face_id(), mesh.face_store()))
    {
        oss << "  Bad face " << print(halfedge.face_id(), mesh.face_store()) << std::endl;
        bOK = false;
    }

    if (good(halfedge.partner_id(), mesh.halfedge_store()))
    {
        const auto& partner = halfedge.partner();
        if (!(halfedge.partner_id() == partner.id()))
        {
            oss << "Bad: !(halfedge.partner_id() == partner.id()" << halfedge.partner_id() << " " << partner.id() << std::endl;
            bOK = false;
        }
        if (!(vector_eq(halfedge.attributes().position(), partner.next().attributes().position(), v_ulp)))
        {
            const auto& pos1 = halfedge.attributes().position();
            const auto& pos2 = partner.next().attributes().position();
            oss << "Bad: !(vector_eq(halfedge.attributes().position(), partner.next().attributes().position())" << " [" << pos1 << "] [" << pos2 << "]" << std::endl;
            bOK = false;
        }
        if (partner.prev().partner_id() != nullid
            && !vector_eq(halfedge.next().attributes().position(), partner.attributes().position(), v_ulp))
        {
            const auto& pos1 = halfedge.next().attributes().position();
            const auto& pos2 = partner.attributes().position();
            oss << "Bad: !(vector_eq(halfedge.next().attributes().position(), partner.attributes().position())" << " [" << pos1 << "] [" << pos2 << "]" << std::endl;
            bOK = false;
        }
        if (halfedge.prev().partner_id() != nullid
            && !(vector_eq(halfedge.attributes().position(), halfedge.prev().partner().attributes().position(), v_ulp)))
        {
            const auto& pos1 = halfedge.attributes().position();
            const auto& pos2 = halfedge.prev().partner().attributes().position();
            oss << "Bad: !(vector_eq(halfedge.attributes().position(), halfedge.prev().partner().attributes().position())" << " [" << pos1 << "] [" << pos2 << "]" << std::endl;
            bOK = false;
        }
        if (partner.prev().partner_id() != nullid
            && !(vector_eq(partner.attributes().position(), partner.prev().partner().attributes().position(), v_ulp)))
        {
            const auto& pos1 = partner.attributes().position();
            const auto& pos2 = partner.prev().partner().attributes().position();
            oss << "Bad: !(vector_eq(partner.attributes().position(), partner.prev().partner().attributes().position())" << " [" << pos1 << "] [" << pos2 << "]" << std::endl;
            bOK = false;
        }
    }

    if (!bOK)
    {
        std::cout << "Halfedge " << halfedge.id() << std::endl << oss.str();
    }

    return bOK;
}

//--------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_halfedges(const M& mesh)
{
    bool bOK = true;

    std::cout << "Checking halfedges: " << mesh.halfedge_count() << "/" << mesh.halfedge_store_count() << " halfedges" << std::endl;
    for (const auto& halfedge : mesh.halfedges())
    {
        bOK = check_halfedge<M>(mesh, halfedge) && bOK;
    }

    if (bOK)
    {
        std::cout << "OK" << std::endl;
    }

    return bOK;
}

//--------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_vertex(const M& mesh, const typename M::vertex_type& vertex)
{
    bool bOK = true;
    if (vertex.deleted())
    {
        return bOK;
    }

    typename M::value_type coordMax = v_max<M>;
    const auto position = vertex.attributes().position();
    if (std::abs(position.x()) > coordMax || std::abs(position.y()) > coordMax || std::abs(position.z()) > coordMax)
    {
        std::cout << "Bad: coordinate > " << coordMax << ": " << vertex.id() << position << std::endl;
        bOK = false;
    }

    std::string errors;
    bOK = vertex.attributes().validate(errors);
    if (!bOK)
    {
        std::cout << "Bad: vertex " << vertex.id() << " attributes: " << errors << std::endl;
        bOK = false;
    }

    if (mesh.vertex(vertex.id()).id() != vertex.id())
    {
        std::cout << "  Bad vertex id " << "mesh.vertex(vertex.id()).id() != vertex.id() " << mesh.vertex(vertex.id()).id() << " " << vertex.id() << std::endl;
        bOK = false;
    }

    if (vertex.halfedge().border())
    {
        std::cout << "  Border halfedge, vertex " << vertex.id() << std::endl;
        return bOK;
    }

    if (!good(vertex.halfedge_id(), mesh.halfedge_store()))
    {
        std::cout << "  Bad halfedge " << print(vertex.halfedge_id(), mesh.halfedge_store()) << std::endl;
        bOK = false;
    }

    if (vertex.halfedges().size() != vertex.halfedge_count())
    {
        std::cout << "Bad: vertex.halfedges().size() != vertex.halfedge_count() " << std::setw(4) << vertex.halfedges().size() << " " << vertex.halfedge_count() << std::endl;
        bOK = false;
    }

    size_t n = 0;
    std::set<id_type> ids;
    for (const auto& halfedge : vertex.halfedges())
    {
        if (ids.contains(halfedge.id()))
        {
            std::cout << "Bad: halfedge iterator: duplicate id at n " << n << ", v " << vertex.id() << ": h " << halfedge.id() << std::endl;
            bOK = false;
            break;
        }

        ids.insert(halfedge.id());

        if (halfedge.partner_id() == nullid)
        {
            std::cout << "Warn: halfedge iterator: halfedge.partner_id() == nullid" << " v " << vertex.id() << " h " << halfedge.id() << std::endl;
            bOK = false;
            break;
        }

        const auto& attributes = halfedge.attributes();
        if (!vector_eq(attributes.position(), position))
        {
            std::cout << "Bad: !vector_eq(attributes.position(), position) : " << vertex.id() << " [" << attributes.position() << "] [" << position << "]" << std::endl;
            bOK = false;
        }
/* work on this ...
        else if (attributes.position() != position)
        {
            std::cout << "Warn: attributes.position() != position : " << vertex.id() << " [" << attributes.position() << "] [" << position << "]" << std::endl;
            bOK = false;
        }
*/
        if (!vector_eq(halfedge.attributes().position(), halfedge.partner().next().attributes().position(), v_ulp))
        {
            std::cout << "Bad: !vector_eq(halfedge.attributes().position(), halfedge.partner().next().attributes().position())" << "  " << halfedge.id() << " [" << halfedge.attributes().position() << "] [" << halfedge.partner().next().attributes().position() << "]" << std::endl;
            bOK = false;
        }

        if (halfedge.prev().partner_id() == nullid)
        {
            std::cout << "Warn: halfedge iterator: halfedge.prev().partner_id() == nullid" << " v " << vertex.id() << " h " << halfedge.id() << std::endl;
            bOK = false;
            break;
        }

        if (!vector_eq(halfedge.attributes().position(), halfedge.prev().partner().attributes().position(), v_ulp))
        {
            std::cout << "Bad: !vector_eq(halfedge.attributes().position(), halfedge.prev().partner().attributes().position())" << "  " << halfedge.id() << " [" << halfedge.attributes().position() << "] [" << halfedge.prev().partner().attributes().position() << "]" << std::endl;
            bOK = false;
        }

        ++n;
    }

    return bOK;
}

//--------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_vertices(const M& mesh)
{
    bool bOK = true;

    std::cout << "Checking vertices: " << mesh.vertex_count() << "/" << mesh.vertex_store_count() << " vertices" << std::endl;
    for (const auto& vertex : mesh.vertices())
    {
        bOK = check_vertex(mesh, vertex) && bOK;
    }

    if (bOK)
    {
        std::cout << "OK" << std::endl;
    }

    return bOK;
}

//--------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_face(const M& mesh, const typename M::face_type& face)
{
    bool bOK = true;
    if (face.deleted())
    {
        return bOK;
    }

    std::map<id_type, size_t> counts;
    size_t n = 0;

    typename M::vector_type normal = face.attributes().normal();
    if (math::float_ne(normal.norm(), M::val(1)))
    {
        std::cout << "Bad: |normal| != 1:" << "\t" << face.id() << "\t" << normal << std::endl;
        bOK = false;
    }

    if (!good(face.halfedge_id(), mesh.halfedge_store()))
    {
        std::cout << "  Bad halfedge " << print(face.halfedge_id(), mesh.halfedge_store()) << std::endl;
        bOK = false;
    }

    if (mesh.face(face.id()).id() != face.id())
    {
        std::cout << "  Bad face id " << "mesh.face(face.id()).id() != face.id() " << mesh.face(face.id()).id() << " " << face.id() << std::endl;
        bOK = false;
    }

    typename M::point_type positionPrev = face.halfedge().prev().attributes().position();

    for (const auto& halfedge : face.halfedges())
    {
        if (halfedge.face_id() == face.id())
        {
            ++n;
        }

        if (halfedge.face_id() != face.id())
        {
            std::cout << "Bad: halfedge.face_id() != face.id()" << "\t" << halfedge.face_id() << "\t" << face.id() << std::endl;
            bOK = false;
        }

        if (dot(halfedge.attributes().normal(), face.attributes().normal()) <= 0)
        {
            std::cout << "Bad: dot(halfedge.attributes().normal(), face.attributes().normal()) <= 0" << " f " << face.id() << " h " << halfedge.id() << " [" << halfedge.attributes().normal() << "] [" << face.attributes().normal() << "]" << std::endl;
            bOK = false;
        }

        if constexpr (M::vertex_attributes_type::contains(geometry::AttributesFlags::Texcoord0))
        {
            typename M::vector_type texcoord = {halfedge.attributes().texcoord().x(), M::val(1) - halfedge.attributes().texcoord().y(), M::val(0)};
            typename M::vector_type texcoordNext = {halfedge.next().attributes().texcoord().x(), M::val(1) - halfedge.next().attributes().texcoord().y(), M::val(0)};
            typename M::vector_type texcoordPrev = {halfedge.prev().attributes().texcoord().x(), M::val(1) - halfedge.prev().attributes().texcoord().y(), M::val(0)};
            typename M::vector_type texture_direction = cross(texcoordNext - texcoord, texcoordPrev - texcoord);
            typename M::value_type projection = dot(texture_direction, {M::val(0), M::val(0), M::val(1)});

            if (convex(halfedge))
            {
                if (math::float_le0(projection))
                {
                    std::cout << "Bad: convex dot(texture_direction, {0, 0, 1}) <= 0" << " f " << halfedge.face_id() << " h " << halfedge.id() << " [" << texture_direction << "]: [" << halfedge.attributes().position() << "] [" << halfedge.attributes().texcoord() << "]" << std::endl;
                    std::cout << "[" << texcoord << "] [" << texcoordNext << "] [" << texcoordPrev << "] " << std::endl;
                    bOK = false;
                }
            }
            else if (reflex(halfedge))
            {
                if (math::float_ge0(projection))
                {
                    std::cout << "Bad: reflex dot(texture_direction, {0, 0, 1}) >= 0" << " f " << halfedge.face_id() << " h " << halfedge.id() << " [" << texture_direction << "]: [" << halfedge.attributes().position() << "] [" << halfedge.attributes().texcoord() << "]" << std::endl;
                    bOK = false;
                }
            }
        }

        typename M::point_type position = halfedge.attributes().position();
        if (vector_eq(position, positionPrev))
        {
            std::cout << "Bad: duplicated vertex positions: " << halfedge.prev_id() << " [" << positionPrev << "] " << halfedge.id() << " [" << position << "]" << std::endl;
            bOK = false;
        }

        positionPrev = position;
        ++counts[halfedge.vertex_id()];
    }

    if (n != face.halfedge_count())
    {
        std::cout << "    Bad halfedge face ids, " << n << " / " << face.halfedge_count() << " face id " << face.id() << " references found." << std::endl;
    }

    if (counts.size() < face.halfedge_count())
    {
        std::cout << "Bad: counts.size() < face.halfedge_count()" << std::endl;
        bOK = false;
    }

    if (face.surface_id() == nullid)
    {
        std::cout << "Warn: face " << face.id() << " surface null" << std::endl;
    }
    else if (face.surface_id() >= mesh.surface_store_count())
    {
        std::cout << "Bad: face " << face.id() << " surface out of range:  " << face.surface_id() << "/" << mesh.surface_store_count() << std::endl;
        bOK = false;
    }
    else if (!face.surface().contains_face(face.id()))
    {
        std::cout << "Bad: face " << face.id() << " not included in surface " << face.surface_id() << " " << face.surface().name()<< std::endl;
        bOK = false;
    }
    else if (face.submesh_id() !=  face.surface().submesh_id())
    {
        std::cout << "Bad: face " << face.id() << " submesh " << face.submesh_id() << " does not match face surface " << face.surface_id() << " submesh " << face.surface().submesh_id()<< std::endl;
        bOK = false;
    }

    if (face.submesh_id() == nullid)
    {
        std::cout << "Warn: face " << face.id() << " submesh null" << std::endl;
    }
    else if (face.submesh_id() >= mesh.submesh_store_count())
    {
        std::cout << "Bad: face " << face.id() << " submesh out of range:  " << face.submesh_id() << "/" << mesh.submesh_store_count() << std::endl;
        bOK = false;
    }
    else if (!face.submesh().contains_face(face.id()))
    {
        std::cout << "Bad: face " << face.id() << " not included in submesh " << face.submesh_id() << " " << face.submesh().name() << std::endl;
        bOK = false;
    }

    return bOK;
}

//--------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_faces(const M& mesh)
{
    bool bOK = true;

    std::cout << "Checking faces: " << mesh.face_count() << "/" << mesh.face_store_count() << " faces" << std::endl;
    for (const auto& face : mesh.faces())
    {
        bOK = check_face(mesh, face) && bOK;
    }

    if (bOK)
    {
        std::cout << "OK" << std::endl;
    }

    return bOK;
}

//--------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_border_face(const M& mesh, id_type idHalfedge)
{
    bool bOK = true;

    id_type idPrev = prev_border_halfedge(mesh, idHalfedge);
    typename M::point_type positionPrev = mesh.halfedge(idPrev).attributes().position();

    id_type id = idHalfedge;
    do
    {
        const auto& halfedge = mesh.halfedge(id);
        typename M::point_type position = halfedge.attributes().position();
        if (vector_eq(position, positionPrev))
        {
            std::cout << "Bad: duplicated vertex positions: " << idPrev << " [" << positionPrev << "] " << halfedge.id() << " [" << position << "]" << std::endl;
            bOK = false;
        }

        idPrev = id;
        positionPrev = position;

        id = next_border_halfedge(mesh, id);
    } while (id != idHalfedge);

    return bOK;
}

//--------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_surfaces(const M& mesh)
{
    bool bOK = true;

    if (mesh.surface_count() != mesh.surface_index_count())
    {
        std::cout << "Bad: mesh.surface_count() != mesh.surface_index_count() " << mesh.surface_count() << " " << mesh.surface_index_count() << std::endl;
        bOK = false;
    }

    if (mesh.surface_count() == 0)
    {
        return bOK;
    }

    std::map<id_type, size_t> face_counts;

    std::cout << "Checking surfaces: " << mesh.surface_count() << "/" << mesh.surface_store_count() << " surfaces" << std::endl;
    for (const auto& surface : mesh.surfaces())
    {
        if (surface.empty())
        {
            std::cout << "Bad: surface.empty() " << surface.id() << std::endl;
            bOK = false;
        }

        if (surface.name().empty())
        {
            std::cout << "Warn: surface.name().empty() " << surface.id() << std::endl;
        }

        if (surface.submesh_id() == nullid)
        {
            std::cout << "Warn: surface " << surface.id() << " submesh null" << std::endl;
        }
        else if (surface.submesh_id() > mesh.submesh_store_count())
        {
            std::cout << "Bad: surface " << surface.id() << " submesh out of range:  " << surface.submesh_id() << "/" << mesh.submesh_store_count() << std::endl;
            bOK = false;
        }
        else if (!surface.submesh().contains_surface(surface.id()))
        {
            std::cout << "Bad: surface " << surface.id() << " not included in submesh " << surface.submesh_id() << " " << surface.submesh().name() << std::endl;
            bOK = false;
        }

        for (id_type idFace : surface.face_ids())
        {
            ++face_counts[idFace];

            if (mesh.face(idFace).deleted())
            {
                std::cout << "Bad: mesh.face(idFace).deleted(): face: " << idFace << " in surface " << surface.id() << std::endl;
                bOK = false;
            }

            if (mesh.face(idFace).surface_id() != surface.id())
            {
                std::cout << "Bad: mesh.face(idFace).surface_id() != surface.id(): face: " << idFace << " " << mesh.face(idFace).surface_id() << " " << surface.id() << std::endl;
                bOK = false;
            }
        }
    }

    for (const auto& face_count : face_counts)
    {
        if (face_count.second > 1)
        {
            std::cout << "Bad: face " << face_count.first << " included in " << face_count.second << " surfaces" << std::endl;
            bOK = false;
        }
    }

    std::map<id_type, size_t> surface_face_counts;

    for (const auto& face : mesh.faces())
    {
        if (face.surface_id() == nullid)
        {
            size_t n = 0;
            for (const auto& surface : mesh.surfaces())
            {
                for (id_type idFace : surface.face_ids())
                {
                    if (idFace == face.id())
                    {
                        ++n;
                    }
                }
            }

            if (n > 0)
            {
                std::cout << "Bad: face with null surface occurs " << n << " times in surfaces" << std::endl;
                bOK = false;
            }

            continue;
        }

        ++surface_face_counts[face.surface_id()];

        size_t n = 0;
        for (id_type idFace : face.surface().face_ids())
        {
            if (idFace == face.id())
            {
                ++n;
            }
        }

        if (n != 1)
        {
            std::cout << "Bad: face " << face.id() << " occurs " << n << "/1 times in surface " << face.surface_id() << " " << face.surface().name() << std::endl;
            bOK = false;
        }
    }

    if (surface_face_counts.size() != mesh.surface_count())
    {
        std::cout << "Bad: surface_face_counts.size() != mesh.surface_count() " << surface_face_counts.size() << " " << mesh.surface_count() << std::endl;
        bOK = false;
    }

    for (const auto& surface_face_count : surface_face_counts)
    {
        if (mesh.surface(surface_face_count.first).face_count() != surface_face_count.second)
        {
            std::cout << "Bad: mesh.surface(surface_face_count.first).face_count() != surface_face_count.second for surface " << surface_face_count.first << ", " << mesh.surface(surface_face_count.first).face_count() << " " << surface_face_count.second << std::endl;
            bOK = false;
        }
    }

    for (const auto& s : mesh.surfaces())
    {
        if (s.face_count() != surface_face_counts[s.id()])
        {
            std::cout << "Bad: s.face_count() != surface_face_counts[s.id()] for surface " << s.id() << ", " << s.face_count() << " " << surface_face_counts[s.id()] << std::endl;
            bOK = false;
        }
    }

    if (face_counts.size() != mesh.face_count())
    {
        std::cout << "Bad: surface face counts: Only " << face_counts.size() << " out of " << mesh.face_count() << " faces are associated with surfaces" << std::endl;
        bOK = false;
    }

    if (bOK)
    {
        std::cout << "OK" << std::endl;
    }

    return bOK;
}

//--------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_submeshes(const M& mesh)
{
    bool bOK = true;

    if (mesh.submesh_count() != mesh.submesh_index_count())
    {
        std::cout << "Bad: mesh.submesh_count() != mesh.submesh_index_count() " << mesh.submesh_count() << " " << mesh.submesh_index_count() << std::endl;
        bOK = false;
    }

    if (mesh.submesh_count() == 0)
    {
        return bOK;
    }

    std::map<id_type, size_t> face_counts;
    std::map<id_type, size_t> surface_counts;

    std::cout << "Checking submeshes: " << mesh.submesh_count() << "/" << mesh.submesh_store_count() << " submeshes" << std::endl;
    for (const auto& submesh : mesh.submeshes())
    {
        if (submesh.empty())
        {
            std::cout << "Bad: submesh.empty() " << submesh.id() << std::endl;
            bOK = false;
        }

        if (submesh.name().empty())
        {
            std::cout << "Warn: submesh.name().empty() " << submesh.id() << std::endl;
        }

        for (id_type idFace : submesh.face_ids())
        {
            ++face_counts[idFace];

            if (mesh.face(idFace).deleted())
            {
                std::cout << "Bad: mesh.face(idFace).deleted(): face: " << idFace << " in submesh " << submesh.id() << std::endl;
                bOK = false;
            }

            if (mesh.face(idFace).submesh_id() != submesh.id())
            {
                std::cout << "Bad: mesh.face(idFace).submesh_id() != submesh.id() " << mesh.face(idFace).submesh_id() << " " << submesh.id() << std::endl;
                bOK = false;
            }
        }

        for (id_type idSurface : submesh.surface_ids())
        {
            ++surface_counts[idSurface];

            if (mesh.surface(idSurface).deleted())
            {
                std::cout << "Bad: mesh.surface(idSurface).deleted(): surface: " << idSurface << " in submesh " << submesh.id() << std::endl;
                bOK = false;
            }

            if (mesh.surface(idSurface).submesh_id() != submesh.id())
            {
                std::cout << "Bad: mesh.surface(idSurface).submesh_id() != submesh.id() " << mesh.surface(idSurface).submesh_id() << " " << submesh.id() << std::endl;
                bOK = false;
            }
        }
    }

    for (const auto& face_count : face_counts)
    {
        if (face_count.second > 1)
        {
            std::cout << "Bad: face " << face_count.first << " occurs " << face_count.second << "/1 times in submeshes" << std::endl;
            bOK = false;
        }
    }

    for (const auto& surface_count : surface_counts)
    {
        if (surface_count.second > 1)
        {
            std::cout << "Bad: surface " << surface_count.first << " occurs " << surface_count.second << "/1 times in submeshes" << std::endl;
            bOK = false;
        }
    }

    for (const auto& face : mesh.faces())
    {
        if (face.submesh_id() == nullid)
        {
            size_t n = 0;
            for (const auto& submesh : mesh.submeshes())
            {
                for (id_type idFace : submesh.face_ids())
                {
                    if (idFace == face.id())
                    {
                        ++n;
                    }
                }
            }

            if (n > 0)
            {
                std::cout << "Bad: face with null submesh occurs " << n << " times in submeshes" << std::endl;
                bOK = false;
            }

            continue;
        }

        size_t n = 0;
        for (id_type idFace : face.submesh().face_ids())
        {
            if (idFace == face.id())
            {
                ++n;
            }
        }

        if (n != 1)
        {
            std::cout << "Bad: face " << face.id() << " occurs " << n << "/1 times in submesh " << face.submesh_id() << " " << face.submesh().name() << std::endl;
            bOK = false;
        }
    }

    // same for surfaces ...

    if (face_counts.size() != mesh.face_count())
    {
        std::cout << "Bad: submesh face counts: " << face_counts.size() << " out of " << mesh.face_count() << " faces are associated with submeshes" << std::endl;
        bOK = false;
    }

    if (bOK)
    {
        std::cout << "OK" << std::endl;
    }

    return bOK;
}

//--------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_triangular_face(const M& mesh, const typename M::face_type& face)
{
    bool bOK = true;
    if (face.deleted())
    {
        return bOK;
    }

    if (face.halfedge_count() != 3)
    {
        std::cout << "Bad: face.halfedge_count() != 3" << std::endl;
        bOK = false;
    }

    if (!convex(face.halfedge()))
    {
        std::cout << "Bad: !convex(face.halfedge())" << std::endl;
        bOK = false;
    }

    return bOK;
}

//--------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_triangulation(const M& mesh)
{
    bool bOK = true;

    std::cout << "Checking triangulation: " << mesh.face_count() << " faces" << std::endl;
    for (const auto& face : mesh.faces())
    {
        bOK = check_triangular_face<M>(mesh, face) && bOK;
    }

    if (bOK)
    {
        std::cout << "OK" << std::endl;
    }

    return bOK;
}

//--------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_manifold_edges(const M& mesh)
{
    bool bOK = true;

    std::map<id_type, size_t> face_counts;
    std::map<id_type, size_t> partner_face_counts;

    std::cout << "Checking manifold edges: " << mesh.halfedge_count() << " halfedges" << std::endl;
    for (const auto& face : mesh.faces())
    {
        for (const auto& halfedge : face.halfedges())
        {
            face_counts[halfedge.id()]++;
            partner_face_counts[halfedge.partner_id()]++;
        }
    }

    for (const auto& i : face_counts)
    {
        if (i.second > 1)
        {
            std::cout << "Bad: halfedge " << i.first << " connected to " << i.second << " faces" << std::endl;
            bOK = false;
        }
    }

    for (const auto& i : partner_face_counts)
    {
        if (i.second > 1)
        {
            std::cout << "Bad: halfedge partner " << i.first << " connected to " << i.second << " faces" << std::endl;
            bOK = false;
        }
    }

    bOK = check_adjacent_face_normals(mesh) && bOK; // ...

    if (bOK)
    {
        std::cout << "OK" << std::endl;
    }

    return bOK;
}

//--------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_adjacent_face_normals(const M& mesh)
{
    bool bOK = true;

    std::cout << "Checking adjacent face normals: " << mesh.face_count() << " faces" << std::endl;
    for (const auto& face : mesh.faces())
    {
        bOK = check_adjacent_face_normals(mesh, face) && bOK;
    }

    if (bOK)
    {
        std::cout << "OK" << std::endl;
    }

    return bOK;
}

//--------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_adjacent_face_normals(const M& mesh, const typename M::face_type& face)
{
    bool bOK = true;
    if (face.deleted())
    {
        return bOK;
    }

    for (const auto& halfedge : face.halfedges())
    {
        const auto& faceA = halfedge.face();
        const auto& faceB = halfedge.partner().face();

        if (dot(faceA.attributes().normal(), faceB.attributes().normal()) < 0)
        {
            std::cout << "Bad?: adjacent face normals " << (faceA.surface_id() != nullid ? face.surface().name() : "-") << " " << faceA.id() << "/" << halfedge.id() << " [" << faceA.attributes().normal() << "] and " << (faceB.surface_id() != nullid ? face.surface().name() : "-") << " " << faceB.id() << "/" << halfedge.partner_id() << " [" << faceB.attributes().normal() << "]" << std::endl;
            bOK = false;
        }
    }

    return bOK;
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_split_edge(const M& mesh, const typename M::halfedge_type& halfedgeOrig, const typename M::halfedge_type& partnerOrig, const typename M::halfedge_type& halfedge)
{
    const typename M::halfedge_type& partner = mesh.halfedge(halfedge.partner_id());
    const typename M::halfedge_type& next = mesh.halfedge(halfedge.next_id());
    const typename M::halfedge_type& next_partner = mesh.halfedge(halfedge.next_id()).partner();

    bool bOK = true;

    std::cout << "Checking split_edge: " << halfedgeOrig.id() << " " << partnerOrig.id() << " " << halfedge.id() << std::endl;

    // Original
    if (halfedgeOrig.partner_id() != partnerOrig.id())
    {
        std::cout << "Bad: halfedgeOrig.partner_id() != partnerOrig.id()" << std::endl;
        bOK = false;
    }
    if (partnerOrig.partner_id() != halfedgeOrig.id())
    {
        std::cout << "Bad: partnerOrig.partner_id() != halfedgeOrig.id()" << std::endl;
        bOK = false;
    }

    // Halfedge
    if (halfedge.id() != halfedgeOrig.id())
    {
        std::cout << "Bad: halfedge.id() != halfedgeOrig.id()" << std::endl;
        bOK = false;
    }
    if (halfedge.partner_id() != partner.id())
    {
        std::cout << "Bad: halfedge.partner_id() != partner.id()" << std::endl;
        bOK = false;
    }
    if (halfedge.next_id() != next_partner.partner_id())
    {
        std::cout << "Bad: halfedge.next_id() != next_partner.partner_id()" << std::endl;
        bOK = false;
    }
    if (halfedge.prev_id() != halfedgeOrig.prev_id())
    {
        std::cout << "Bad: halfedge.prev_id() != halfedgeOrig.prev_id()" << std::endl;
        bOK = false;
    }
    if (halfedge.vertex_id() != halfedgeOrig.vertex_id())
    {
        std::cout << "Bad: halfedge.vertex_id() != halfedgeOrig.vertex_id()" << std::endl;
        bOK = false;
    }
    if (halfedge.face_id() != halfedgeOrig.face_id())
    {
        std::cout << "Bad: halfedge.face_id() != halfedgeOrig.face_id()" << std::endl;
        bOK = false;
    }

    // Partner
    if (partner.partner_id() != halfedgeOrig.id())
    {
        std::cout << "Bad: partner.partner_id() != halfedgeOrig.id()" << std::endl;
        bOK = false;
    }
    if (partner.next_id() != partnerOrig.next_id())
    {
        std::cout << "Bad: partner.next_id() != partnerOrig.next_id()" << std::endl;
        bOK = false;
    }
    if (partner.prev_id() != partnerOrig.id())
    {
        std::cout << "Bad: partner.prev_id() != partnerOrig.id()" << std::endl;
        bOK = false;
    }
    if (partner.face_id() != partnerOrig.face_id())
    {
        std::cout << "Bad: partner.face_id() != partnerOrig.face_id()" << std::endl;
        bOK = false;
    }

    // Next
    if (next.partner_id() != partnerOrig.id())
    {
        std::cout << "Bad: next.partner_id() != partnerOrig.id()" << std::endl;
        bOK = false;
    }
    if (next.next_id() != halfedgeOrig.next_id())
    {
        std::cout << "Bad: next.next_id() != halfedgeOrig.next_id()" << std::endl;
        bOK = false;
    }
    if (next.prev_id() != halfedgeOrig.id())
    {
        std::cout << "Bad: next.prev_id() != halfedgeOrig.id()" << std::endl;
        bOK = false;
    }
    if (next.face_id() != halfedgeOrig.face_id())
    {
        std::cout << "Bad: next.face_id() != halfedgeOrig.face_id()" << std::endl;
        bOK = false;
    }

    // Partner, Next
    if (!vector_eq(next.attributes().position(), partner.attributes().position(), v_ulp))
    {
        std::cout << "Bad: !vector_eq(next.attributes().position(), partner.attributes().position()" << std::endl;
        bOK = false;
    }

    // Next Partner
    if (next_partner.id() != partnerOrig.id())
    {
        std::cout << "Bad: next_partner.id() != partnerOrig.id()" << std::endl;
        bOK = false;
    }
    if (next_partner.partner_id() != next.id())
    {
        std::cout << "Bad: next_partner.partner_id() != next.id()" << std::endl;
        bOK = false;
    }
    if (next_partner.next_id() != partner.id())
    {
        std::cout << "Bad: next_partner.next_id() != partner.id()" << std::endl;
        bOK = false;
    }
    if (next_partner.prev_id() != partnerOrig.prev_id())
    {
        std::cout << "Bad: next_partner.prev_id() != partnerOrig.prev_id()" << std::endl;
        bOK = false;
    }
    if (next_partner.vertex_id() != partnerOrig.vertex_id())
    {
        std::cout << "Bad: next_partner.vertex_id() != partnerOrig.vertex_id()" << std::endl;
        bOK = false;
    }
    if (next_partner.face_id() != partnerOrig.face_id())
    {
        std::cout << "Bad: next_partner.face_id() != partnerOrig.face_id()" << std::endl;
        bOK = false;
    }

    // External
    if (halfedge.prev().next_id() != halfedge.id())
    {
        std::cout << "Bad: halfedge.prev().next_id() != halfedge.id()" << std::endl;
        bOK = false;
    }
    if (partner.next().prev_id() != partner.id())
    {
        std::cout << "Bad: partner.next().prev_id() != partner.id()" << std::endl;
        bOK = false;
    }
    if (next.next().prev_id() != next.id())
    {
        std::cout << "Bad: next.next().prev_id() != next.id()" << std::endl;
        bOK = false;
    }
    if (next_partner.prev().next_id() != next_partner.id())
    {
        std::cout << "Bad: next_partner.prev().next_id() != next_partner.id()" << std::endl;
        bOK = false;
    }

    if (bOK)
    {
        std::cout << "OK" << std::endl;
    }
    else
    {
        write_halfedge<M>(halfedgeOrig, "halfedgeOrig");
        write_halfedge<M>(partnerOrig, "partnerOrig");
        write_halfedge<M>(halfedge, "halfedge");
        write_halfedge<M>(partner, "partner");
        write_halfedge<M>(next, "next");
        write_halfedge<M>(next_partner, "next_partner");
    }

    return bOK;
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_split_face(const M& mesh, id_type idHalfedgeA, id_type idHalfedgeB, size_t nEdgesOrig)
{
    const typename M::face_type& faceA = mesh.halfedge(idHalfedgeA).face();
    const typename M::face_type& faceB = mesh.halfedge(idHalfedgeB).face();
    const typename M::halfedge_type& halfedgeA = mesh.halfedge(idHalfedgeA).prev();
    const typename M::halfedge_type& halfedgeB = mesh.halfedge(idHalfedgeB).prev();

    bool bOK = true;

    std::cout << "Checking split_face: " << idHalfedgeA << " " << idHalfedgeB << " " << nEdgesOrig << std::endl;

    if (faceA.halfedge_count() + faceB.halfedge_count() != nEdgesOrig + 2)
    {
        std::cout << "Bad: faceA.halfedge_count() + faceB.halfedge_count() != nEdgesOrig + 2";
        std::cout << ": " << faceA.halfedge_count() << " + " << faceB.halfedge_count() << " != " << nEdgesOrig << " + 2" << std::endl;
        bOK = false;
    }

    if (faceA.id() == faceB.id())
    {
        std::cout << "Bad: faceA.id() == faceB.id()" << std::endl;
        bOK = false;
    }

    if (faceA.halfedge().face_id() != faceA.id())
    {
        std::cout << "Bad: faceA.halfedge().face_id != faceA.id()" << std::endl;
        bOK = false;
    }

    if (faceB.halfedge().face_id() != faceB.id())
    {
        std::cout << "Bad: faceB.halfedge().face_id != faceB.id()" << std::endl;
        bOK = false;
    }

    if (halfedgeA.next_id() != idHalfedgeA)
    {
        std::cout << "Bad: halfedgeA.next_id != idHalfedgeA" << std::endl;
        bOK = false;
    }

    if (halfedgeB.next_id() != idHalfedgeB)
    {
        std::cout << "Bad: halfedgeB.next_id != idHalfedgeB" << std::endl;
        bOK = false;
    }

    if (halfedgeA.partner_id() != halfedgeB.id())
    {
        std::cout << "Bad: halfedgeA.partner_id() != halfedgeB.id()" << std::endl;
        bOK = false;
    }

    if (halfedgeB.partner_id() != halfedgeA.id())
    {
        std::cout << "Bad: halfedgeB.partner_id() != halfedgeA.id()" << std::endl;
        bOK = false;
    }

    if (halfedgeA.attributes().position() != halfedgeB.next().attributes().position())
    {
        std::cout << "Bad: halfedgeA.attributes().position != halfedgeB.next().attributes().position()" << std::endl;
        bOK = false;
    }

    if (halfedgeB.attributes().position() != halfedgeA.next().attributes().position())
    {
        std::cout << "Bad: halfedgeB.attributes().position != halfedgeA.next().attributes().position()" << std::endl;
        bOK = false;
    }

    for (const auto& halfedge : faceA.halfedges())
    {
        if (halfedge.face_id() != faceA.id())
        {
            std::cout << "Bad: halfedge.face_id() != faceA.id()" << std::endl;
            bOK = false;
        }
    }

    for (const auto& halfedge : faceB.halfedges())
    {
        if (halfedge.face_id() != faceB.id())
        {
            std::cout << "Bad: halfedge.face_id() != faceB.id()" << std::endl;
            bOK = false;
        }
    }

    if (bOK)
    {
        std::cout << "OK" << std::endl;
    }
    else
    {
        write_face<M>(faceA, "faceA halfedges");
        for (const auto& halfedge : faceA.halfedges())
        {
            write_halfedge<M>(halfedge, "    ");
        }
        write_face<M>(faceB, "faceB halfedges");
        for (const auto& halfedge : faceB.halfedges())
        {
            write_halfedge<M>(halfedge, "    ");
        }
        write_halfedge<M>(halfedgeA, "halfedgeA");
        write_halfedge<M>(halfedgeB, "halfedgeB");
    }

    return bOK;
}

//--------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_attributes(const M& mesh)
{
    std::string error;
    bool bOK = true;

    for (const auto& vertex : mesh.vertices())
    {
        if (!vertex.attributes().validate(error))
        {
            std::cout << "Bad: vertex.attributes().validate() : " << vertex.id() << " " << error << std::endl;
            bOK = false;
        }
    }

    for (const auto& face : mesh.faces())
    {
        if (!face.attributes().validate(error))
        {
            std::cout << "Bad: face.attributes().validate() : " << face.id() << " " << error << std::endl;
            bOK = false;
        }
    }

    return bOK;
}

//--------------------------------------------------------------------------
template<typename M>
void quetzal::brep::write_halfedge(const typename M::halfedge_type& halfedge, const std::string& label)
{
    std::cout << std::setw(16) << std::left << label;
    std::cout << " " << static_cast<const Flags&>(halfedge);
    std::cout << " " << std::right << std::setw(5) << halfedge.id();
    if (!halfedge.deleted())
    {
        if (halfedge.partner_id() != nullid)
        {
            std::cout << " " << std::setw(5) << halfedge.partner_id();
        }
        else
        {
            std::cout << " border";
        }
        std::cout << " " << std::setw(5) << halfedge.next_id();
        std::cout << " " << std::setw(5) << halfedge.prev_id();
        std::cout << " " << std::setw(5) << halfedge.vertex_id();
        std::cout << " " << std::setw(5) << halfedge.face_id();
//        std::cout << " [" << halfedge.attributes().position() << "]" << " (" << math::cartesian_to_spherical(halfedge.attributes().position()) << ")";
        std::cout << " [" << halfedge.attributes().position() << "]";
    }
    std::cout << std::endl;
    return;
}

//--------------------------------------------------------------------------
template<typename M>
void quetzal::brep::write_vertex(const typename M::vertex_type& vertex, const std::string& label)
{
    std::cout << std::setw(16) << std::left << label;
    std::cout << " " << static_cast<const Flags&>(vertex);
    std::cout << " " << std::right << std::setw(5) << vertex.id();
    if (!vertex.deleted())
    {
        std::cout << " " << std::setw(5) << vertex.halfedge_id();
        if (vertex.halfedge().border())
        {
            std::cout << " border halfedge";
        }
        else
        {
            std::cout << " " << std::setw(3) << vertex.halfedge_count() << " halfedges";
        }
//        std::cout << " [" << vertex.attributes().position() << "]" << " (" << math::cartesian_to_spherical(vertex.attributes().position()) << ")";
//        std::cout << " [" << vertex.attributes().normal() << "]" << " (" << math::cartesian_to_spherical(vertex.attributes().normal()) << ")";
        std::cout << " " << vertex.attributes();
    }
    std::cout << std::endl;
    return;
}

//--------------------------------------------------------------------------
template<typename M>
void quetzal::brep::write_face(const typename M::face_type& face, const std::string& label)
{
    std::cout << std::setw(16) << std::left << label;
    std::cout << " " << static_cast<const Flags&>(face);
    std::cout << " " << std::right << std::setw(5) << face.id();
    if (!face.deleted())
    {
        std::cout << " " << std::setw(5) << face.halfedge_id();
        std::cout << " " << std::setw(3) << face.halfedge_count() << " edges";
        std::cout << " " << std::setw(3) << face.hole_count() << " holes";

        std::cout << ", s ";
        if (face.surface_id() != nullid)
        {
            std::cout << face.surface_id() << " " << face.surface().name();
        }
        else
        {
            std::cout << "nullid";
        }

        std::cout << ", o ";
        if (face.submesh_id() != nullid)
        {
            std::cout << face.submesh_id() << " " << face.submesh().name();
        }
        else
        {
            std::cout << "nullid";
        }

//        std::cout << " [" << face.attributes().normal() << "]" << " (" << math::cartesian_to_spherical(face.attributes().normal()) << ")";
        std::cout << " " << face.attributes();
    }
    std::cout << std::endl;
    return;
}

//--------------------------------------------------------------------------
template<typename M>
void quetzal::brep::write_face_vertices(const M& mesh, id_type idHalfedge)
{
    const auto& face = mesh.halfedge(idHalfedge).face();
    assert(!face.deleted());
    check_face(mesh, face);

    const auto normal = face.attributes().normal();

    std::cout << "Face " << "\t" << face.id() << " of " << mesh.face_store_count() << "\t" << face.halfedge_count() << " edges, surface ";
    if (face.surface_id() != nullid)
    {
        std::cout << face.surface_id() << " " << face.surface().name();
    }
    else
    {
        std::cout << "nullid";
    }
    std::cout << " [" << normal << "]" << " (" << math::cartesian_to_spherical(normal) << ")";
    std::cout << "\t" << face.halfedge_count() << " halfedges";
    std::cout << std::endl;

    size_t n = 0;
    for (const auto& halfedge : face.halfedges())
    {
        check_halfedge(mesh, halfedge);
        id_type id = halfedge.id();
        const auto& position = halfedge.attributes().position();
        const auto& positionNext = halfedge.next().attributes().position();
        const auto& positionPrev = halfedge.prev().attributes().position();
        typename M::value_type angle = angle_unit(positionNext - position, positionPrev - position, normal);
        bool bConvex = convex_triad(position, positionNext, positionPrev, normal);
        bool bColinear = colinear(position, positionNext, positionPrev);
        bool bReflex = reflex_triad(position, positionNext, positionPrev, normal);
        bool bEarVertex = ear(halfedge, mesh);
        std::cout << "\t" << n++ << "\t" << id << "\t" << halfedge.partner_id() << "\t" << halfedge.next_id() << "\t" << halfedge.prev_id() << "\t" << halfedge.face_id() << "\t" << angle;
        std::cout << "\t" << vertex_orientation(mesh, id);
        std::cout << "\t" << (bConvex ? "convex" : "      ");
        std::cout << "  " << (bColinear ? "colinear" : "        ");
        std::cout << "  " << (bReflex ? "reflex" : "      ");
        std::cout << "\t" << (bEarVertex ? "ear" : " - ");
        std::cout << std::endl;
    }

    return;
}

//--------------------------------------------------------------------------
template<typename M>
void quetzal::brep::write_hole_vertices(const M& mesh, id_type idHalfedge)
{
    const auto& face = mesh.halfedge(idHalfedge).face();
    assert(!face.deleted());

    const auto normal = face.attributes().normal();

    std::cout << "Hole " << idHalfedge << ", face " << face.id();
    std::cout << std::endl;

    size_t n = 0;
    id_type id = idHalfedge;
    do
    {
        id_type idHalfedgeNext = mesh.halfedge(id).next_id();
        
        const auto& halfedge = mesh.halfedge(id);
        check_halfedge(mesh, halfedge);
        const auto& position = halfedge.attributes().position();
        const auto& positionNext = halfedge.next().attributes().position();
        const auto& positionPrev = halfedge.prev().attributes().position();
        typename M::value_type angle = angle_unit(positionNext - position, positionPrev - position, normal);
        bool bConvex = convex_triad(position, positionNext, positionPrev, normal);
        bool bColinear = colinear(position, positionNext, positionPrev);
        bool bReflex = reflex_triad(position, positionNext, positionPrev, normal);
        bool bEarVertex = ear(halfedge, mesh);
        std::cout << "\t" << n++ << "\t" << id << "\t" << halfedge.partner_id() << "\t" << halfedge.next_id() << "\t" << halfedge.prev_id() << "\t" << halfedge.face_id() << "\t" << angle;
        std::cout << "\t" << vertex_orientation(mesh, id);
        std::cout << "\t" << (bConvex ? "convex" : "      ");
        std::cout << "  " << (bColinear ? "colinear" : "        ");
        std::cout << "  " << (bReflex ? "reflex" : "      ");
        std::cout << "\t" << (bEarVertex ? "ear" : " - ");
        std::cout << std::endl;

        id = idHalfedgeNext;
    } while (id != idHalfedge);

    return;
}

//--------------------------------------------------------------------------
template<typename M>
void quetzal::brep::write_surface(const typename M::surface_type& surface, const std::string& label)
{
    std::cout << std::setw(16) << std::left << label;
    std::cout << " " << static_cast<const Flags&>(surface);
    std::cout << " " << std::right << std::setw(5) << surface.id();
    if (!surface.deleted())
    {
        std::cout << " " << surface.name();

        std::cout << ", o ";
        if (surface.submesh_id() != nullid)
        {
            std::cout << surface.submesh_id() << " " << surface.submesh().name();
        }
        else
        {
            std::cout << "nullid";
        }

        std::cout << ", " << surface.face_count() << " faces";
        for (id_type idFace : surface.face_ids())
        {
            std::cout << " " << idFace;
        }
    }
    std::cout << std::endl;

    return;
}

//--------------------------------------------------------------------------
template<typename M>
void quetzal::brep::write_submesh(const typename M::submesh_type& submesh, const std::string& label)
{
    std::cout << std::setw(16) << std::left << label;
    std::cout << " " << static_cast<const Flags&>(submesh);
    std::cout << " " << std::right << std::setw(5) << submesh.id();
    if (!submesh.deleted())
    {
        std::cout << " " << submesh.name();
        std::cout << "\n\t" << submesh.face_count() << " faces";
        for (id_type idFace : submesh.face_ids())
        {
            std::cout << " " << idFace;
        }
        std::cout << "\n\t" << submesh.surface_count() << " surfaces";
        for (id_type idSurface : submesh.surface_ids())
        {
            std::cout << " " << idSurface;
        }
    }
    std::cout << std::endl;

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::write_summary(const M& mesh)
{
    std::cout << "h " << mesh.halfedge_count() << "/" << mesh.halfedge_store_count() << "; ";
    std::cout << "v " << mesh.vertex_count() << "/" << mesh.vertex_store_count() << "; ";
    std::cout << "f " << mesh.face_count() << "/" << mesh.face_store_count() << "; ";
    std::cout << "s " << mesh.surface_count() << "/" << mesh.surface_store_count() << "; ";
    std::cout << "o " << mesh.submesh_count() << "/" << mesh.submesh_store_count();
    std::cout << std::endl;
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::validate(const M& mesh)
{
    check_halfedges(mesh);
    check_vertices(mesh);
    check_faces(mesh);
    check_surfaces(mesh);
    check_submeshes(mesh);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::brep::dump(const M& mesh)
{
    size_t n = 0;
    for (const auto& halfedge : mesh.halfedge_store())
    {
        write_halfedge<M>(halfedge, "Halfedge[" + to_string(n++) + "]");
        if (halfedge.deleted())
        {
            continue;
        }

        check_halfedge(mesh, halfedge);
    }

    n = 0;
    for (const auto& vertex : mesh.vertex_store())
    {
        write_vertex<M>(vertex, "Vertex[" + to_string(n++) + "]");
        if (vertex.deleted())
        {
            continue;
        }

        const auto& position = vertex.attributes().position();

        assert(vertex.halfedges().size() == vertex.halfedge_count());
        for (const auto& halfedge : vertex.halfedges())
        {
            std::cout << "    halfedge " << std::right << std::setw(5) << halfedge.id();
            std::cout << ", v " << std::setw(5) << halfedge.vertex_id();
            std::cout << ", vh " << std::setw(5) << halfedge.vertex().halfedge_id();
            std::cout << " [" << halfedge.attributes().position() << "] " << halfedge.attributes().position().norm() << " [" << halfedge.attributes().normal() << "]";
            std::cout << " " << (vector_eq(halfedge.attributes().position(), position, v_ulp) ? "OK" : "Bad");
            std::cout << std::endl;

    		if (halfedge.border())
    		{
    			break;
    		}
        }

        check_vertex(mesh, vertex);
    }

    n = 0;
    for (const auto& face : mesh.face_store())
    {
        write_face<M>(face, "Face[" + to_string(n++) + "]");
        if (face.deleted())
        {
            continue;
        }

        for (const auto& halfedge : face.halfedges())
        {
            std::cout << "    halfedge " << std::right << std::setw(5) << halfedge.id();
            std::cout << ", v " << std::setw(5) << halfedge.vertex_id();
            std::cout << ", f " << std::setw(5) << halfedge.face_id();
            std::cout << " [" << halfedge.attributes().position() << "] " << halfedge.attributes().position().norm() << " [" << halfedge.attributes().normal() << "]";
            std::cout << std::endl;
        }

        check_face(mesh, face);
    }

    n = 0;
    for (const auto& surface : mesh.surface_store())
    {
        write_surface<M>(surface, "Surface[" + to_string(n++) + "]");
        if (surface.deleted())
        {
            continue;
        }
    }

    std::cout << "Surface index" << std::endl;
    for (auto i : mesh.surface_index())
    {
        std::cout << "\t" << i.second << "\t" << i.first << std::endl;
    }

    check_surfaces(mesh);

    n = 0;
    for (const auto& submesh : mesh.submesh_store())
    {
        write_submesh<M>(submesh, "Submesh[" + to_string(n++) + "]");
        if (submesh.deleted())
        {
            continue;
        }
    }

    std::cout << "Submesh index" << std::endl;
    for (auto i : mesh.submesh_index())
    {
        std::cout << "\t" << i.second << "\t" << i.first << std::endl;
    }

    check_submeshes(mesh);

    return;
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_face_normals(const M& mesh)
{
    bool bOK = true;

    std::cout << "Checking face normals: " << mesh.face_count() << " faces" << std::endl;
    for (const auto& face : mesh.faces())
    {
        bOK = check_face_normal(mesh, face) && bOK;
    }

    if (bOK)
    {
        std::cout << "OK" << std::endl;
    }

    return bOK;
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_face_normal(const M& mesh, const typename M::face_type& face)
{
    bool bOK = true;
    if (face.deleted())
    {
        return bOK;
    }

    typename M::vector_type normal = face.attributes().normal();

    for (const auto& halfedge : face.halfedges())
    {
        if (!vector_eq(halfedge.attributes().normal(), normal, v_ulp))
        {
            std::cout << "Bad: !vector_eq(halfedge.attributes().normal(), normal) : " << face.id() << " " << halfedge.id() << "  " << halfedge.attributes().normal() << "  " << normal << std::endl;
            bOK = false;
        }
    }

    return bOK;
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_normal_consistency(const M& mesh, bool bCheckPosition)
{
    bool bOK = true;

    std::cout << "Checking normal consistency: " << mesh.face_count() << " faces" << std::endl;
    for (const auto& face : mesh.faces())
    {
        bOK = check_normal_consistency(mesh, face, bCheckPosition) && bOK;
    }

    if (bOK)
    {
        std::cout << "OK" << std::endl;
    }

    return bOK;
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_normal_consistency(const M& mesh, const typename M::face_type& face, bool bCheckPosition)
{
    mesh;

    bool bOK = true;
    if (face.deleted())
    {
        return bOK;
    }

    const auto& normal = face.attributes().normal();

    for (const auto& halfedge : face.halfedges())
    {
        const auto& attributes = halfedge.attributes();

        if (dot(attributes.normal(), normal) < 0)
        {
            std::cout << "Bad: dot(attributes.normal(), normal) < 0 : " << face.id() << " " << halfedge.id() << " [" << attributes.normal() << "] [" << normal << "]" << std::endl;
            bOK = false;
        }

        if (bCheckPosition)
        {
            if (dot(attributes.normal(), attributes.position()) < 0)
            {
                std::cout << "Bad: dot(attributes.normal(), attributes.position()) < 0 : " << face.id() << " " << halfedge.id() << " [" << attributes.normal() << "] [" << attributes.position() << "]" << std::endl;
                bOK = false;
            }
        }
    }

    return bOK;
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_surface_normal_consistency(const M& mesh)
{
    bool bOK = true;

    const_cast<M&>(mesh).reset();

    std::cout << "Checking surface normal consistency: " << mesh.surface_count() << " surfaces" << std::endl;
    for (const auto& surface : mesh.surfaces())
    {
        bOK = check_surface_normal_consistency(mesh, surface) && bOK;
    }

    if (bOK)
    {
        std::cout << "OK" << std::endl;
    }

    return bOK;
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_surface_normal_consistency(const M& mesh, const typename M::surface_type& surface)
{
    bool bOK = true;
    if (surface.deleted())
    {
        return bOK;
    }

    std::cout << surface.name() << std::endl;
    for (id_type idFace : surface.face_ids())
    {
        const auto& face = mesh.face(idFace);
        for (const auto& halfedge : face.halfedges())
        {
            const auto& vertex = halfedge.vertex();
            bOK = check_surface_normal_consistency(mesh, surface, vertex) && bOK;
        }
    }

    if (bOK)
    {
        std::cout << "OK" << std::endl;
    }

    return bOK;
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_surface_normal_consistency(const M& mesh, const typename M::surface_type& surface, const typename M::vertex_type& vertex)
{
    bool bOK = true;
    if (vertex.deleted())
    {
        return bOK;
    }

mesh; // ...
//    const auto& normal = vertex.halfedge().face().attributes().normal();
// check that surface/face, face/vertex normals agree too ...

    const auto& attributesNominal = vertex.halfedge().attributes();
    for (const auto& halfedge : vertex.halfedges())
    {
        if (halfedge.face().surface_id() == surface.id() && !halfedge.vertex().marked())
        {
            const auto& attributes = halfedge.attributes();

            if (!vector_eq(attributes.normal(), attributesNominal.normal()))
            {
                std::cout << "Bad: !vector_eq(attributes.normal(), attributesNominal.normal()) : " << vertex.id() << " [" << attributes.normal() << "] [" << attributesNominal.normal() << "]" << std::endl;
                bOK = false;
            }
            else if (attributes.normal() != attributesNominal.normal())
            {
                std::cout << "Warn: attributes.normal() != attributesNominal.normal() : " << vertex.id() << " [" << attributes.normal() << "] [" << attributesNominal.normal() << "]" << std::endl;
                bOK = false;
            }

            const_cast<typename M::halfedge_type&>(halfedge).vertex().set_marked();
        }
    }

    return bOK;
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_face_angles(const M& mesh)
{
    bool bOK = true;

    std::cout << "Checking face angles: " << mesh.face_count() << " faces" << std::endl;
    for (const auto& face : mesh.faces())
    {
        bOK = check_face_angles(mesh, face) && bOK;
    }

    if (bOK)
    {
        std::cout << "OK" << std::endl;
    }

    return bOK;
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_face_angles(const M& mesh, const typename M::face_type& face)
{
    bool bOK = true;
    if (face.deleted())
    {
        return bOK;
    }

    using T = M::value_type;

    size_t n = face.halfedge_count();
    T sum = 0;

    for (const auto& halfedge : face.halfedges())
    {
        const auto& av = halfedge.attributes();

        T a = angle(halfedge.next().attributes().position() - av.position(), halfedge.prev().attributes().position() - av.position());
        bool bColinear = colinear(halfedge);
        bool bConvex = convex(halfedge);
        bool bReflex = reflex(halfedge);
        if (bReflex)
        {
            a = math::PiTwo<T> - a;
        }

        if (bColinear && bConvex)
        {
            std::cout << "Bad: bColinear && bConvex : " << face.id() << " " << halfedge.id() << std::endl;
            bOK = false;
        }

        if (bColinear && bReflex)
        {
            std::cout << "Bad: bColinear && bReflex : " << face.id() << " " << halfedge.id() << std::endl;
            bOK = false;
        }

        if (bConvex && bReflex)
        {
            std::cout << "Bad: bConvex && bReflex : " << face.id() << " " << halfedge.id() << std::endl;
            bOK = false;
        }

        if (!bConvex && !bColinear && !bReflex)
        {
            std::cout << "Bad: !bConvex && !bColinear && !bReflex : " << face.id() << " " << halfedge.id() << std::endl;
            bOK = false;
        }

        sum += a;
    }

    if (math::float_ne(sum, math::Pi<T> * (n - 2)))
    {
        std::cout << "Bad: sum != math::Pi<T> * (n - 2) : " << face.id() << " " << sum << " " << (math::Pi<T> * (n - 2)) << std::endl;
        bOK = false;
    }

    return bOK;
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_convex(const M& mesh)
{
    bool bOK = true;

    std::cout << "Checking convex: " << mesh.face_count() << " faces" << std::endl;
    for (const auto& face : mesh.faces())
    {
        bOK = check_convex(mesh, face) && bOK;
    }

    if (bOK)
    {
        std::cout << "OK" << std::endl;
    }

    return bOK;
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_convex(const M& mesh, const typename M::face_type& face)
{
    bool bOK = true;
    if (face.deleted())
    {
        return bOK;
    }

    // convex_face not used so that non-convex vertex can be identified
    for (const auto& halfedge : face.halfedges())
    {
        if (!convex(halfedge))
        {
            std::cout << "Bad: !convex(halfedge) : " << face.id() << " " << halfedge.id() << std::endl;
            bOK = false;
        }
    }

    return bOK;
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_outward_normals(const M& mesh)
{
    bool bOK = true;

    for (const auto& vertex : mesh.vertices())
    {
        if (dot(vertex.attributes().position(), vertex.attributes().normal()) <= 0)
        {
            std::cout << "Bad: dot(vertex.attributes().position(), vertex.attributes().normal()) <= 0 : " << vertex.id() << "  " << vertex.attributes().position() << "  " << vertex.attributes().normal() << std::endl;
            bOK = false;
        }
    }

    return bOK;
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::brep::check_spherical(const M& mesh, typename M::value_type radius)
{
    bool bOK = true;

    for (const auto& vertex : mesh.vertices())
    {
        if (math::float_ne(dot(vertex.attributes().position(), vertex.attributes().position()), radius * radius))
        {
            std::cout << "Bad: dot(vertex.attributes().position(), vertex.attributes().position()) != radius * radius : " << vertex.id() << "  " << vertex.attributes().position() << "  " << radius << std::endl;
            bOK = false;
        }
    }

    return bOK;
}

//------------------------------------------------------------------------------
template<typename M>
std::map<typename M::value_type, size_t> quetzal::brep::count_edge_lengths(const M& mesh)
{
    using value_type = M::value_type;
    using point_type = M::point_type;

    std::map<value_type, size_t> m;

    for (const auto& face : mesh.faces())
    {
        typename M::halfedge_type& halfedge = face.halfedge();

        const point_type& pos0 = halfedge.attributes().position();
        const point_type& pos1 = halfedge.next().attributes().position();
        const point_type& pos2 = halfedge.prev().attributes().position();

        value_type length = norm(pos0 - pos1);
        m[length]++;
//        std::cout << (i + 0) << '\t' << (i + 1) << '\t' << length << std::endl;

        length = norm(pos1 - pos2);
        m[length]++;
//        std::cout << (i + 1) << '\t' << (i + 2) << '\t' << length << std::endl;

        length = norm(pos2 - pos0);
        m[length]++;
//        std::cout << (i + 2) << '\t' << (i + 0) << '\t' << length << std::endl;
    }

    return m;
}

#endif // QUETZAL_BREP_VALIDATION_HPP
