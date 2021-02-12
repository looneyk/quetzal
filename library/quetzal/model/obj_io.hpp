#if !defined(QUETZAL_MODEL_OBJ_IO_HPP)
#define QUETZAL_MODEL_OBJ_IO_HPP
//------------------------------------------------------------------------------
// model
// obj_io.hpp
//------------------------------------------------------------------------------

#include "quetzal/common/id.hpp"
#include "quetzal/math/Vector.hpp"
#include "quetzal/wavefront_obj/Reader.hpp"
#include "quetzal/wavefront_obj/Writer.hpp"
#include "quetzal/wavefront_obj/symbols.hpp"
#include <array>
#include <map>
#include <filesystem>
#include <string>

namespace quetzal::model
{

    template<typename M>
    void read_obj(M& mesh, const std::filesystem::path& pathname);

    template<typename M>
    void write_obj(const M& mesh, const std::filesystem::path& pathname);

    // central location for these? ...
    const std::string MaterialsPropertyName = "materials";
    const std::string MaterialPropertyName = "material";

} // namespace quetzal::model

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::read_obj(M& mesh, const std::filesystem::path& pathname)
{
    id_type idSubmesh = nullid;
    id_type idSurface = nullid;
    id_type idFace = nullid;
    size_t nvFace = 0;
    std::string material;

    //--------------------------------------------------------------------------
    auto on_object = [&](M& mesh, const std::string& name) -> void
    {
        idSubmesh = mesh.contains_submesh(name) ? mesh.submesh(name).id() : mesh.create_submesh(name);
        idSurface = nullid;
        return;
    };

    //--------------------------------------------------------------------------
    auto on_group = [&](M& mesh, const std::string& name) -> void
    {
        if (idSubmesh == nullid)
        {
            idSubmesh = mesh.create_submesh(wavefront_obj::ObjectNameDefault);
        }

        if (!mesh.contains_surface(idSubmesh, name))
        {
            idSurface = mesh.create_surface(idSubmesh, name);
        }

        if (mesh.surface(idSurface).empty() && !material.empty())
        {
            mesh.surface(idSurface).set_property(MaterialPropertyName, material);
        }
        else if (mesh.surface(idSurface).property(MaterialPropertyName) != material)
        {
            // The case where an existing group is being specified in the context of a different material is not handled here ...
            // This needs to be deferred in case the discrepancy is corrected in a subsequent material specification ...
            assert(false);
        }

        return;
    };

    //--------------------------------------------------------------------------
    auto on_face_open = [&](M& mesh) -> void
    {
        if (idSurface == nullid)
        {
            if (idSubmesh == nullid)
            {
                idSubmesh = mesh.create_submesh(wavefront_obj::ObjectNameDefault);
            }

            idSurface = mesh.create_surface(idSubmesh, wavefront_obj::GroupNameDefault);
        }
        else
        {
            assert(idSubmesh != nullid);
        }

        idFace = mesh.create_face(idSurface, mesh.halfedge_store_count());
        nvFace = 0;
        return;
    };

    //--------------------------------------------------------------------------
    auto on_face_vertex = [&](M& mesh, const typename M::point_type& position, const typename M::vector_type& normal, const typename M::texcoord_type& texcoord) -> void
    {
        id_type nh = mesh.halfedge_store_count();
        typename M::vertex_attributes_type av = {position, normal, texcoord};
        id_type idVertex = mesh.create_vertex(nh, av);
        mesh.create_halfedge(0, nh + 1, nh - 1, idVertex, idFace);
        ++nvFace;
        return;
    };

    //--------------------------------------------------------------------------
    auto on_face_close = [&](M& mesh) -> void
    {
        using segment_type = std::array<typename M::point_type, 2>;
        std::map<segment_type, id_type> segments;

        // Correct prev and next for first and last halfedges respectively
        id_type nh = mesh.halfedge_store_count();
        mesh.halfedge(nh - nvFace).set_prev_id(nh - 1);
        mesh.halfedge(nh - 1).set_next_id(nh - nvFace);

        // Add partner id to each halfedge in this new face
        id_type ih = nh - nvFace;
        typename M::face_type& face = mesh.faces().back();
        for (typename M::halfedge_type& halfedge : face.halfedges())
        {
            const typename M::point_type& position = halfedge.attributes().position();
            const typename M::halfedge_type& heNext = halfedge.next();
            const typename M::point_type& positionNext = heNext.attributes().position();
            segment_type segment = {positionNext, position};
            auto i = segments.find(segment);
            if (i == segments.end())
            {
                segments[{position, positionNext}] = ih;
            }
            else
            {
                halfedge.set_partner_id(i->second);
                mesh.halfedge(i->second).set_partner_id(ih);
                segments.erase(i);
            }

            ++ih;
        }

        return;
    };

    //--------------------------------------------------------------------------
    auto on_materials = [&](M& mesh, const std::string& name) -> void
    {
        std::string prefix;
        if (mesh.contains_property(MaterialsPropertyName))
        {
            prefix = mesh.property(MaterialsPropertyName) + " ";
        }

        mesh.set_property(MaterialsPropertyName, prefix + name);
        return;
    };

    //--------------------------------------------------------------------------
    auto on_material = [&](M& mesh, const std::string& name) -> void
    {
        material = name;

        if (idSurface == nullid)
        {
            on_group(mesh, wavefront_obj::GroupNameDefault);
        }

        if (mesh.surface(idSurface).empty() && !material.empty())
        {
            mesh.surface(idSurface).set_property(MaterialPropertyName, material);
        }
        else if (mesh.surface(idSurface).property(MaterialPropertyName) != material)
        {
            // The case where an existing group is being specified in the context of a different material is not handled here ...
            // Could handle this here creating surface with name group_material, store group name in properties for subsequent use, ...
            assert(false);
        }

        return;
    };

    wavefront_obj::Reader<M> reader(on_object, on_group, on_face_open, on_face_vertex, on_face_close, on_materials, on_material);
    reader.read(pathname, mesh);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::write_obj(const M& mesh, const std::filesystem::path& pathname)
{
    wavefront_obj::Writer<typename M::value_type> writer(pathname);

    if (mesh.contains_property(MaterialsPropertyName))
    {
        writer.set_materials(mesh.property(MaterialsPropertyName));
    }

    for (const typename M::vertex_type& vertex : mesh.vertices())
    {
        const auto& attributes = vertex.attributes();
        writer.set_vertex(attributes.position(), attributes.normal(), attributes.texcoord());
    }

    auto write_face = [&](const typename M::face_type& face)
    {
        writer.set_face();
        for (const auto& halfedge : face.halfedges())
        {
            const auto& attributes = halfedge.vertex().attributes();
            writer.set_face_vertex(attributes.position(), attributes.normal(), attributes.texcoord());
        }
    };

    // Later, address the comment below ...
    // first faces with no surface or submesh ...
    // then faces with only a surface (loop through surfaces, write those without submesh) ...
    // remainder handled in submesh loop (first loop submesh faces writing those with no surface, then loop through surfaces) ...
    // flag here that tells whether these checks actually need to be made (all faces have submesh and surface) ...

    for (const typename M::face_type& face : mesh.faces())
    {
        if (face.surface_id() != nullid && face.submesh_id() != nullid)
        {
            continue;
        }

        // for faces in a surface that is not in a submesh, surface information is lost ...
        write_face(face);
    }

    // Write remaining faces by object and group
    for (const typename M::submesh_type& submesh : mesh.submeshes())
    {
        assert(!submesh.empty());
        writer.set_object(submesh.name());
        for (const auto& surface : submesh.surfaces())
        {
            assert(!surface.empty());
            writer.set_group(surface.name());
            if (surface.contains_property(MaterialPropertyName))
            {
                writer.set_material(surface.property(MaterialPropertyName));
            }

            for (const auto& face : surface.faces())
            {
                write_face(face);
            }
        }
    }

    return;
}

#endif // QUETZAL_MODEL_OBJ_IO_HPP
