#if !defined(QUETZAL_BREP_MESH_BOOLEAN_HPP)
#define QUETZAL_BREP_MESH_BOOLEAN_HPP
//------------------------------------------------------------------------------
// brep
// mesh_boolean.hpp
//------------------------------------------------------------------------------

#include "Mesh.hpp"
#include "Submesh.hpp"
#include "Halfedge.hpp"
#include "id.hpp"
#include "mesh_geometry.hpp"
#include "mesh_util.hpp"
#include "mesh_inversion.hpp"
#include "quetzal/geometry/intersect.hpp"
#include <algorithm>
#include <array>
#include <functional>
#include <iterator>
#include <unordered_map>
#include <vector>

#include <algorithm>
#include "validation.hpp"

namespace quetzal::brep
{

    // template parameters: intersections[idFaceA] = idHalfedgeB is the outward directed halfedge from faceA in the exterior direction
    using intersections_type = std::multimap<id_type, id_type>;

    // input: border halfedge ids in order idHalfedgeInteriorA, idHalfedgeExteriorA, idHalfedgeInteriorB, idHalfedgeExteriorB
    template<typename Traits>
    using connect_function_type = std::function<void (Mesh<Traits>&, id_type, id_type, id_type, id_type)>;

    // input: idSubmeshA, idSubmeshB
    template<typename Traits>
    using disjoint_function_type = std::function<void (Mesh<Traits>&, id_type, id_type)>;

    // input: idSubmeshA, idSubmeshB, bInteriorB
    template<typename Traits>
    using inclusion_function_type = std::function<void (Mesh<Traits>&, id_type, id_type, bool)>;

    // A || B
    template<typename Traits>
    id_type boolean_union(Mesh<Traits>& mesh, id_type idSubmeshA, id_type idSubmeshB, const std::string& name);

    // A && B
    template<typename Traits>
    id_type boolean_intersection(Mesh<Traits>& mesh, id_type idSubmeshA, id_type idSubmeshB, const std::string& name);

    // A && !B
    template<typename Traits>
    id_type boolean_difference(Mesh<Traits>& mesh, id_type idSubmeshA, id_type idSubmeshB, const std::string& name);

    // A && !B || !A && B
    template<typename Traits>
    id_type boolean_symmetric_difference(Mesh<Traits>& mesh, id_type idSubmeshA, id_type idSubmeshB, const std::string& name);

    // Create a new or use an existing submesh with name for the result
    // Surface names in the result will typically consist of some from each original submesh
    // Returns submesh id of the result
    template<typename Traits>
    id_type boolean_operation(Mesh<Traits>& mesh, id_type idSubmeshA, id_type idSubmeshB, const std::string& name, connect_function_type<Traits> connect, disjoint_function_type<Traits> disjoint, inclusion_function_type<Traits> inclusion);

    // Implementation functions

    template<typename Traits>
    std::vector<std::vector<quetzal::id_type>> ordered_intersections(Mesh<Traits>& mesh, id_type idSubmeshA, id_type idSubmeshB);

    // find intersections of submeshB edges with submeshA faces, and split edges
    template<typename Traits>
    void split_submesh_intersections(Mesh<Traits>& mesh, const Submesh<Traits>& submeshA, Submesh<Traits>& submeshB, intersections_type& intersections);

    template<typename Traits>
    void halfedge_intersection(Mesh<Traits>& mesh, const Submesh<Traits>& submesh, Halfedge<Traits>& halfedge, intersections_type& intersections);

    template<typename Traits>
    std::tuple<quetzal::id_type, typename Traits::point_type> face_intersection(const Submesh<Traits>& submesh, const Halfedge<Traits>& halfedge);

    template<typename Traits>
    void split_intersection(Mesh<Traits>& mesh, id_type idFaceA, id_type idHalfedgeB, typename Traits::point_type point, intersections_type& intersections);

    // intersections are consumed in the process of ordering them into sets
    template<typename Traits>
    std::vector<std::vector<quetzal::id_type>> order_intersections(const Mesh<Traits>& mesh, intersections_type& intersections);

    // idHalfedgeInterior, idHalfedgeExterior
    template<typename Traits>
    std::array<id_type, 2> split_submesh(Mesh<Traits>& mesh, id_type idSubmesh, const std::vector<id_type>& idHalfedges);

    template<typename Traits>
    void connect_border_sections(Mesh<Traits>& mesh, id_type idHalfedgeA, id_type idHalfedgeB);

    // bReset parameter is used to trigger reset on first, non-recursive call; not a user parameter
    template<typename Traits>
    void delete_border_section(Mesh<Traits>& mesh, id_type idHalfedge, bool bReset = true);

    template<typename Traits, typename InputIterator>
    std::vector<id_type> order_face_halfedges(const Mesh<Traits>& mesh, InputIterator first, InputIterator last);

    template<typename Traits>
    bool submesh_contains(const Mesh<Traits>& mesh, const Submesh<Traits>& submeshA, const Submesh<Traits>& submeshB);

    // debug functions ...

    template<typename Traits>
    void rename_submesh_border_section(Mesh<Traits>& mesh, id_type idHalfedge, const std::string& name, bool bReset = true);

    template<typename Traits>
    void dump_border_section(Mesh<Traits>& mesh, id_type idHalfedge, bool bReset = true);

    template<typename Traits>
    void dump_border(Mesh<Traits>& mesh, id_type idHalfedge);

} // namespace quetzal::brep

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::id_type quetzal::brep::boolean_union(Mesh<Traits>& mesh, id_type idSubmeshA, id_type idSubmeshB, const std::string& name)
{
    connect_function_type<Traits> connect = [](Mesh<Traits>& mesh, id_type idHalfedgeInteriorA, id_type idHalfedgeExteriorA, id_type idHalfedgeInteriorB, id_type idHalfedgeExteriorB) -> void
    {
        connect_border_sections(mesh, idHalfedgeExteriorA, idHalfedgeExteriorB);
        delete_border_section(mesh, idHalfedgeInteriorA);
        delete_border_section(mesh, idHalfedgeInteriorB);
        return;
    };

    disjoint_function_type<Traits> disjoint = [](Mesh<Traits>&, id_type, id_type) -> void
    {
        return;
    };

    inclusion_function_type<Traits> inclusion = [](Mesh<Traits>& mesh, id_type idSubmeshA, id_type idSubmeshB, bool bInteriorB) -> void
    {
        mesh.delete_submesh(bInteriorB ? idSubmeshB : idSubmeshA);
        return;
    };

    return boolean_operation(mesh, idSubmeshA, idSubmeshB, name, connect, disjoint, inclusion);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::id_type quetzal::brep::boolean_intersection(Mesh<Traits>& mesh, id_type idSubmeshA, id_type idSubmeshB, const std::string& name)
{
    connect_function_type<Traits> connect = [](Mesh<Traits>& mesh, id_type idHalfedgeInteriorA, id_type idHalfedgeExteriorA, id_type idHalfedgeInteriorB, id_type idHalfedgeExteriorB) -> void
    {
        connect_border_sections(mesh, idHalfedgeInteriorA, idHalfedgeInteriorB);
        delete_border_section(mesh, idHalfedgeExteriorA);
        delete_border_section(mesh, idHalfedgeExteriorB);
        return;
    };

    disjoint_function_type<Traits> disjoint = [](Mesh<Traits>& mesh, id_type idSubmeshA, id_type idSubmeshB) -> void
    {
        mesh.delete_submesh(idSubmeshA);
        mesh.delete_submesh(idSubmeshB);
        return;
    };

    inclusion_function_type<Traits> inclusion = [](Mesh<Traits>& mesh, id_type idSubmeshA, id_type idSubmeshB, bool bInteriorB) -> void
    {
        mesh.delete_submesh(bInteriorB ? idSubmeshA : idSubmeshB);
        return;
    };

    return boolean_operation(mesh, idSubmeshA, idSubmeshB, name, connect, disjoint, inclusion);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::id_type quetzal::brep::boolean_difference(Mesh<Traits>& mesh, id_type idSubmeshA, id_type idSubmeshB, const std::string& name)
{
    connect_function_type<Traits> connect = [](Mesh<Traits>& mesh, id_type idHalfedgeInteriorA, id_type idHalfedgeExteriorA, id_type idHalfedgeInteriorB, id_type idHalfedgeExteriorB) -> void
    {
        invert_border_section(mesh, idHalfedgeInteriorB);
        connect_border_sections(mesh, idHalfedgeExteriorA, idHalfedgeInteriorB);
        delete_border_section(mesh, idHalfedgeInteriorA);
        delete_border_section(mesh, idHalfedgeExteriorB);
        return;
    };

    disjoint_function_type<Traits> disjoint = [](Mesh<Traits>& mesh, id_type, id_type idSubmeshB) -> void
    {
        mesh.delete_submesh(idSubmeshB);
        return;
    };

    inclusion_function_type<Traits> inclusion = [](Mesh<Traits>& mesh, id_type idSubmeshA, id_type idSubmeshB, bool bInteriorB) -> void
    {
        if (bInteriorB)
        {
            invert_submesh(mesh, idSubmeshB);
            return;
        }

        mesh.delete_submesh(idSubmeshA);
        mesh.delete_submesh(idSubmeshB);
        return;
    };

    return boolean_operation(mesh, idSubmeshA, idSubmeshB, name, connect, disjoint, inclusion);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::id_type quetzal::brep::boolean_symmetric_difference(Mesh<Traits>& mesh, id_type idSubmeshA, id_type idSubmeshB, const std::string& name)
{
    connect_function_type<Traits> connect = [](Mesh<Traits>& mesh, id_type idHalfedgeInteriorA, id_type idHalfedgeExteriorA, id_type idHalfedgeInteriorB, id_type idHalfedgeExteriorB) -> void
    {
        invert_border_section(mesh, idHalfedgeInteriorA);
        invert_border_section(mesh, idHalfedgeInteriorB);
//model::transform_border_section_positions(mesh, idHalfedgeExteriorB, math::translation(10.0f, 10.0f, 10.0f)); // ...
//model::transform_border_section_positions(mesh, idHalfedgeInteriorA, math::translation(10.0f, 10.0f, 10.0f)); // ...
        connect_border_sections(mesh, idHalfedgeExteriorA, idHalfedgeInteriorB);
        connect_border_sections(mesh, idHalfedgeExteriorB, idHalfedgeInteriorA);
        return;
    };

    disjoint_function_type<Traits> disjoint = [](Mesh<Traits>&, id_type, id_type) -> void
    {
        return;
    };

    inclusion_function_type<Traits> inclusion = [](Mesh<Traits>& mesh, id_type idSubmeshA, id_type idSubmeshB, bool bInteriorB) -> void
    {
        invert_submesh(mesh, bInteriorB ? idSubmeshB : idSubmeshA);
        return;
    };

    return boolean_operation(mesh, idSubmeshA, idSubmeshB, name, connect, disjoint, inclusion);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::id_type quetzal::brep::boolean_operation(Mesh<Traits>& mesh, id_type idSubmeshA, id_type idSubmeshB, const std::string& name, connect_function_type<Traits> connect, disjoint_function_type<Traits> disjoint, inclusion_function_type<Traits> inclusion)
{
    id_type idSubmesh = mesh.submesh_id(name);

    std::string prefix;
    auto renamer = [&](const std::string& name) -> std::string { return prefix + name; };

    prefix = mesh.submesh(idSubmeshA).name() + "_";
    mesh.rename_submesh_surfaces(idSubmeshA, renamer);

    prefix = mesh.submesh(idSubmeshB).name() + "_";
    mesh.rename_submesh_surfaces(idSubmeshB, renamer);

    std::vector<std::vector<id_type>> idSets = ordered_intersections(mesh, idSubmeshA, idSubmeshB);
    if (idSets.empty())
    {
        // No intersection, so only a single vertex position needs to be checked

        bool bInteriorA = surface_contains(mesh.submesh(idSubmeshB), mesh.submesh(idSubmeshA).faces().front().halfedge().attributes().position());
        bool bInteriorB = surface_contains(mesh.submesh(idSubmeshA), mesh.submesh(idSubmeshB).faces().front().halfedge().attributes().position());

        if (bInteriorA || bInteriorB)
        {
            inclusion(mesh, idSubmeshA, idSubmeshB, bInteriorB);
        }

        disjoint(mesh, idSubmeshA, idSubmeshB);
    }
    else
    {
        for (auto& idsOrdered : idSets)
        {
std::cout << "intersection set" << std::endl;
for (id_type id : idsOrdered)
{
    std::cout << "\t" << id;
}
std::cout << std::endl;

std::cout << "ordered_intersections" << std::endl;
//validate(mesh);
for (id_type id : idsOrdered)
{
write_halfedge<Mesh<Traits>>(mesh.halfedge(id), "h ");
write_halfedge<Mesh<Traits>>(mesh.halfedge(id).partner(), "p ");
write_face<Mesh<Traits>>(mesh.halfedge(id).face(), "f ");
}

std::cout << "split_submesh a" << std::endl;
            auto [idHalfedgeInteriorA, idHalfedgeExteriorA] = split_submesh(mesh, idSubmeshA, idsOrdered);
//validate(mesh);
std::cout << "split_submesh b" << std::endl;

            std::reverse(idsOrdered.begin(), idsOrdered.end());
            auto [idHalfedgeInteriorB, idHalfedgeExteriorB] = split_submesh(mesh, idSubmeshB, idsOrdered);
//validate(mesh);

std::cout << "interior a" << std::endl;
dump_border(mesh, idHalfedgeInteriorA);
dump_border_section(mesh, idHalfedgeInteriorA);
std::cout << "exterior a" << std::endl;
dump_border(mesh, idHalfedgeExteriorA);
dump_border_section(mesh, idHalfedgeExteriorA);
std::cout << "interior b" << std::endl;
dump_border(mesh, idHalfedgeInteriorB);
dump_border_section(mesh, idHalfedgeInteriorB);
std::cout << "exterior b" << std::endl;
dump_border(mesh, idHalfedgeExteriorB);
dump_border_section(mesh, idHalfedgeExteriorB);

            connect(mesh, idHalfedgeInteriorA, idHalfedgeExteriorA, idHalfedgeInteriorB, idHalfedgeExteriorB);
        }
    }

    if (idSubmeshA != idSubmesh)
    {
        mesh.rename_submesh(idSubmeshA, name);
    }

    if (idSubmeshB != idSubmesh)
    {
        mesh.rename_submesh(idSubmeshB, name);
    }

    return idSubmesh != nullid ? idSubmesh : mesh.submesh_id(name);
}

//------------------------------------------------------------------------------
template<typename Traits>
std::vector<std::vector<quetzal::id_type>> quetzal::brep::ordered_intersections(Mesh<Traits>& mesh, id_type idSubmeshA, id_type idSubmeshB)
{
    Submesh<Traits> submeshA = mesh.submesh(idSubmeshA);
    Submesh<Traits> submeshB = mesh.submesh(idSubmeshB);

    intersections_type intersections;
    split_submesh_intersections(mesh, submeshA, submeshB, intersections);
    split_submesh_intersections(mesh, submeshB, submeshA, intersections);
write_summary(mesh);
for (const auto& i : intersections)
{
    std::cout << "face " << i.first << ": " << i.second << " | " << mesh.halfedge(i.second).partner_id() << " : " << mesh.halfedge(i.second).face_id() << std::endl;
}

    return order_intersections(mesh, intersections);
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::split_submesh_intersections(Mesh<Traits>& mesh, const Submesh<Traits>& submeshA, Submesh<Traits>& submeshB, intersections_type& intersections)
{
//size_t i = 0;
    for (auto& faceB : submeshB.faces())
    {
        for (auto& halfedgeB : faceB.halfedges())
        {            
            if (halfedgeB.checked())
            {
                continue;
            }

//if (i == 2)
//{
//    int j = 0;
//}
            halfedge_intersection(mesh, submeshA, halfedgeB, intersections);
//assert(intersections.empty());
//++i;
        }
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::halfedge_intersection(Mesh<Traits>& mesh, const Submesh<Traits>& submesh, Halfedge<Traits>& halfedge, intersections_type& intersections)
{
    auto [idFace, point] = face_intersection(submesh, halfedge);
    if (idFace == nullid)
    {
        halfedge.set_checked();
        halfedge.partner().set_checked();
        return;
    }

    // check here for intersection at endpoints and handle differently ...
    // this is a start, but also need to handle face - face intersections at only one point ...
    if (vector_eq(point, halfedge.attributes().position()))
    {
        bool bExterior = to_halfspace(mesh.face(idFace)).exterior(halfedge.next().attributes().position());
        if (bExterior)
        {
            intersections.emplace(idFace, halfedge.id());
        }

        return;
    }
    else if (vector_eq(point, halfedge.next().attributes().position()))
    {
        bool bExterior = to_halfspace(mesh.face(idFace)).exterior(halfedge.attributes().position());
        if (bExterior)
        {
            intersections.emplace(idFace, halfedge.partner_id());
        }

        return;
    }

    id_type idHalfedge = halfedge.id();
    split_intersection(mesh, idFace, idHalfedge, point, intersections);

    halfedge_intersection(mesh, submesh, mesh.halfedge(idHalfedge).next(), intersections);
    halfedge_intersection(mesh, submesh, mesh.halfedge(idHalfedge), intersections);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
std::tuple<quetzal::id_type, typename Traits::point_type> quetzal::brep::face_intersection(const Submesh<Traits>& submesh, const Halfedge<Traits>& halfedge)
{
    geometry::Segment<typename Traits::vector_traits> segment = to_segment(halfedge);

    for (const auto& face : submesh.faces())
    {
//if (idFace == 74)
//{
//    int j = 0;
//}

        geometry::Polygon<typename Traits::vector_traits> polygon = to_polygon(face);
        geometry::Intersection<typename Traits::vector_traits> intersection = geometry::intersection(segment, polygon);

        if (intersection.locus() == geometry::Locus::Point)
        {
            return {face.id(), intersection.point()}; // return intersection? ...
        }
        else if (intersection.locus() == geometry::Locus::Segment)
        {
            // ...
std::cout << "*** segment!" << std::endl;
break;
        }
        else if (intersection.locus() != geometry::Locus::Empty)
std::cout << "*** other! " << intersection << std::endl;
    }

    return {nullid, {}};
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::split_intersection(Mesh<Traits>& mesh, id_type idFaceA, id_type idHalfedgeB, typename Traits::point_type point, intersections_type& intersections)
{
    // if halfedgeB.attributes().position() == point, don't split, just use halfedgeB and prev ...
    // if halfedgeB.next().attributes().position() == point, don't split, just use halfedgeB and next ...
    // may need halfedge and next/prev both before and after point ...

    split_edge(mesh, idHalfedgeB, point);

    // Add intersections for exterior halfedges directed away from face
    const auto& halfedgeB = mesh.halfedge(idHalfedgeB);
    bool bExterior = to_halfspace(mesh.face(idFaceA)).exterior(halfedgeB.attributes().position());
    intersections.emplace(idFaceA, bExterior ? halfedgeB.partner_id() : halfedgeB.next_id());
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
std::vector<std::vector<quetzal::id_type>> quetzal::brep::order_intersections(const Mesh<Traits>& mesh, intersections_type& intersections)
{
    if (intersections.empty())
    {
        return {};
    }

    // handle case where there is no face transition ...

    std::vector<std::vector<id_type>> idSets;

    while (!intersections.empty())
    {
        std::vector<id_type> idsOrdered;

        // Start at a face transition
        id_type idFace0 = nullid;
        id_type idHalfedge = nullid;
        for (const auto& intersection : intersections)
        {
            if (idFace0 != nullid && intersection.first != idFace0)
            {
                assert(idHalfedge != nullid);
                break;
            }

            idFace0 = intersection.first;
            idHalfedge = intersection.second;
        }

        bool bReverse = false;
std::cout << "bReverse: " << std::boolalpha << bReverse << std::endl;

        id_type idSubmesh = mesh.halfedge(idHalfedge).face().submesh_id();

        for (id_type idFace = idFace0; intersections.contains(idFace); )
        {
            // Handle multiple intersections in the same face
            auto ii = intersections.equal_range(idFace);
            assert(std::distance(ii.first, ii.second) > 0);

            std::vector<id_type> idsOrderedFace = order_face_halfedges(mesh, ii.first, ii.second);
            if (bReverse)
            {
                idsOrdered.insert(idsOrdered.end(), idsOrderedFace.rbegin(), idsOrderedFace.rend());
            }
            else
            {
                idsOrdered.insert(idsOrdered.end(), idsOrderedFace.begin(), idsOrderedFace.end());
            }

            idHalfedge = idsOrdered.back();

            if (mesh.halfedge(idHalfedge).face().submesh_id() != idSubmesh)
            {
                bReverse = !bReverse;
                idSubmesh = mesh.halfedge(idHalfedge).face().submesh_id();
            }

            if (bReverse)
            {
                idFace = mesh.halfedge(idHalfedge).partner().face_id();
            }
            else
            {
                idFace = mesh.halfedge(idHalfedge).face_id();
            }

            intersections.erase(ii.first, ii.second);
        }

        idSets.push_back(idsOrdered);
    }

    return idSets;
}

//------------------------------------------------------------------------------
template<typename Traits>
std::array<quetzal::id_type, 2> quetzal::brep::split_submesh(Mesh<Traits>& mesh, id_type idSubmesh, const std::vector<id_type>& idHalfedges)
{
    assert(!idHalfedges.empty());
    size_t n = idHalfedges.size();

    size_t i0 = 0;
    while (mesh.halfedge(idHalfedges[i0]).face().submesh_id() != idSubmesh)
    {
        ++i0;
    }

    std::function<size_t(size_t)> next = [n](size_t i) -> size_t { return (i + 1) % n; };

    id_type idHalfedgeInterior = nullid;
    id_type idHalfedgeExterior = nullid;

    id_type i = i0;
    do
    {
        id_type iNext = i + 1;
        while (mesh.halfedge(idHalfedges[iNext % n]).face().submesh_id() != idSubmesh)
        {
            ++iNext;
        }
assert(iNext == i + 2);

        id_type idHalfedge0 = idHalfedges[i];
        id_type idHalfedge1 = idHalfedges[iNext % n];
std::cout << "halfedges: " << i << ": " << idHalfedge0 << ", " << iNext << ": " <<  idHalfedge1 << std::endl;
        id_type idHalfedgeSplit = nullid;

assert(mesh.halfedge(idHalfedge0).partner().next().face_id() == mesh.halfedge(idHalfedge1).face_id());
size_t nn = mesh.halfedge(idHalfedge1).face().halfedge_count();
            id_type idHalfedgeInner = mesh.halfedge(idHalfedge0).partner().next_id();
            split_face(mesh, idHalfedgeInner, idHalfedge1);
            idHalfedgeSplit = mesh.halfedge(idHalfedge1).prev_id();
assert(mesh.halfedge(idHalfedgeSplit).prev().partner_id() == idHalfedge0);
check_split_face(mesh, idHalfedgeInner, idHalfedge1, nn);

// why not? ...
// should be:
//assert(mesh.halfedge(idHalfedge0).face_id() == mesh.halfedge(idHalfedge1).partner().next().face_id());
//size_t nn = mesh.halfedge(idHalfedge0).face().halfedge_count();
//        id_type idHalfedgeInner = mesh.halfedge(idHalfedge1).partner().next_id();
//        split_face(mesh, idHalfedge0, idHalfedgeInner);
//        idHalfedgeSplit = mesh.halfedge(idHalfedge0).prev_id();
//assert(mesh.halfedge(idHalfedgeSplit).prev().partner_id() == idHalfedge1);
//check_split_face(mesh, idHalfedge0, idHalfedgeInner, nn);

        // handle the k == iNext case ...
        // make sure that multi-point split works, that idHalfedge0 is adjusted ...
        for (size_t k = i + 1; k < iNext; ++k)
        {
            split_edge(mesh, idHalfedgeSplit, mesh.halfedge(idHalfedges[k % n]).attributes().position());

            if (idHalfedgeInterior == nullid)
            {
                idHalfedgeExterior = idHalfedgeSplit;
                idHalfedgeInterior = mesh.halfedge(idHalfedgeExterior).partner_id();
            }

std::cout << mesh.halfedge(idHalfedgeSplit).id() << " | " << mesh.halfedge(idHalfedgeSplit).partner_id() << std::endl;
std::cout << mesh.halfedge(idHalfedgeSplit).next_id() << " | " << mesh.halfedge(idHalfedgeSplit).next().partner_id() << std::endl;

            // also do this for edges that are already along the border ...
            // do this at each split above ...
            mesh.halfedge(idHalfedgeSplit).next().partner().set_border();
            mesh.halfedge(idHalfedgeSplit).next().set_border();
            mesh.halfedge(idHalfedgeSplit).partner().set_border();
            mesh.halfedge(idHalfedgeSplit).set_border();
        }

        i = iNext;
    }
    while (i != i0 + n);

    return {idHalfedgeInterior, idHalfedgeExterior};
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::connect_border_sections(Mesh<Traits>& mesh, id_type idHalfedgeA, id_type idHalfedgeB)
{
    id_type idA = idHalfedgeA;
    id_type idB = idHalfedgeB;

    while (!vector_eq(mesh.halfedge(idB).next().attributes().position(), mesh.halfedge(idA).attributes().position()))
    {
        idB = next_border_halfedge(mesh, idB);
    }

    for (;;)
    {
        id_type idNextA = next_border_halfedge(mesh, idA);
        id_type idNextB = prev_border_halfedge(mesh, idB);
        if (idNextA == nullid || idNextB == nullid)
        {
            assert(idNextA == nullid && idNextB == nullid);
            return;
        }

        mesh.halfedge(idA).set_partner_id(idB);
        mesh.halfedge(idB).set_partner_id(idA);

        idA = idNextA;
        idB = idNextB;
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::delete_border_section(Mesh<Traits>& mesh, id_type idHalfedge, bool bReset)
{
    if (bReset)
    {
        mesh.reset();
    }

    auto& face = mesh.halfedge(idHalfedge).face();
    if (face.marked())
    {
        return;
    }

    face.set_marked();

    for (auto& halfedge : face.halfedges())
    {
        if (!halfedge.border())
        {
            delete_border_section(mesh, halfedge.partner_id(), false);
        }
    }

    mesh.delete_face(face.id());
    return;
}

//------------------------------------------------------------------------------
template<typename Traits, typename InputIterator>
std::vector<quetzal::id_type> quetzal::brep::order_face_halfedges(const Mesh<Traits>& mesh, InputIterator first, InputIterator last)
{
    std::vector<id_type> idsOrdered;

    // this works now since there is only 1 per face ...
    std::unordered_map<id_type, id_type> far;
    for (auto i = first; i != last; ++i)
    {
        far[mesh.halfedge(i->second).partner().face_id()] = i->second;
    }

    std::unordered_map<id_type, id_type> near;
    for (auto i = first; i != last; ++i)
    {
        near[mesh.halfedge(i->second).face_id()] = i->second;
    }

//        id_type idHalfedge = first->second;
id_type idHalfedge = nullid;
    for (auto i = first; i != last; ++i)
    {
        if (!near.contains(mesh.halfedge(i->second).partner().face_id()))
        {
            idHalfedge = i->second;
            break;
        }
    }

    // infinite loop here in case where there are no submesh transitions ...
    idsOrdered.push_back(idHalfedge);
    while (far.contains(mesh.halfedge(idHalfedge).face_id()))
    {
        idHalfedge = far[mesh.halfedge(idHalfedge).face_id()];
        idsOrdered.push_back(idHalfedge);
    }

    return idsOrdered;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::brep::submesh_contains(const Mesh<Traits>& mesh, const Submesh<Traits>& submeshA, const Submesh<Traits>& submeshB)
{
    submeshB.reset();

    for (const auto& face : submeshB.faces())
    {
        for (const auto& halfedge : face.halfedges())
        {
            if (halfedge.vertex().marked())
            {
                continue;
            }

            if (!submesh_contains(submeshA, halfedge.attributes().position()))
            {
                return false;
            }

            for (const auto& h : halfedge.vertex().halfedges())
            {
                h.face().set_marked();
            }

            break;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::rename_submesh_border_section(Mesh<Traits>& mesh, id_type idHalfedge, const std::string& name, bool bReset)
{
    if (bReset)
    {
        mesh.reset();
    }

    id_type idFace = mesh.halfedge(idHalfedge).face_id();
    id_type idSubmesh = mesh.submesh_id(name);
    mesh.face(idFace).set_submesh_id(idSubmesh);
    mesh.face(idFace).set_marked();

    for (const auto& halfedge : mesh.face(idFace).halfedges())
    {
        if (!halfedge.border())
        {
            if (!halfedge.partner().face().marked())
            {
                rename_submesh_border_section(mesh, halfedge.partner_id(), name, false);
            }
        }
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::dump_border_section(Mesh<Traits>& mesh, id_type idHalfedge, bool bReset)
{
    if (bReset)
    {
        mesh.reset();
    }

    auto& face = mesh.halfedge(idHalfedge).face();
    write_face<Mesh<Traits>>(face);
    face.set_marked();

    for (const auto& halfedge : face.halfedges())
    {
        if (!halfedge.border())
        {
            if (!halfedge.partner().face().marked())
            {
                dump_border_section(mesh, halfedge.partner_id(), false);
            }
        }
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::brep::dump_border(Mesh<Traits>& mesh, id_type idHalfedge)
{
    assert(mesh.halfedge(idHalfedge).border());

    std::cout << "border " << idHalfedge << std::endl;

    id_type id = idHalfedge;
    do
    {
        write_halfedge<Mesh<Traits>>(mesh.halfedge(id));
        id = next_border_halfedge(mesh, id);
    }
    while (id != idHalfedge);

    return;
}

#endif // QUETZAL_BREP_MESH_BOOLEAN_HPP
