#if !defined(QUETZAL_MODEL_STL_IO_HPP)
#define QUETZAL_MODEL_STL_IO_HPP
//------------------------------------------------------------------------------
// model
// stl_io.hpp
//------------------------------------------------------------------------------

#include "quetzal/common/id.hpp"
#include "quetzal/stl/Reader.hpp"
#include "quetzal/stl/Writer.hpp"
#include "quetzal/stl/WriterBinary.hpp"
#include "quetzal/stl/WriterText.hpp"
#include "quetzal/stl/symbols.hpp"
#include <array>
#include <map>
#include <filesystem>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace quetzal::model
{

    template<typename M>
    void read_stl(M& mesh, const std::filesystem::path& pathname);

    // M can be Mesh or Submesh
    template<typename M>
    void write_stl(const M& m, const std::filesystem::path& pathname, const std::string& name = stl::ObjectNameDefault, bool bBinary = true);

} // namespace quetzal::model

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::read_stl(M& mesh, const std::filesystem::path& pathname)
{
    id_type idSubmesh = nullid;
    id_type idSurface = nullid;
    id_type idFace = nullid;
    size_t nVertices = 0;

    using segment_type = std::array<typename M::point_type, 2>;
    std::map<segment_type, id_type> segments;

    auto on_open = [&](M& mesh, const std::filesystem::path& pathname) -> void
    {
        mesh;
        pathname;

        assert(idSubmesh == nullid);
        assert(idSurface == nullid);
        assert(idFace == nullid);
        return;
    };

    auto on_object = [&](M& mesh, const std::string& name) -> void
    {
        assert(idSubmesh == nullid);
        assert(idSurface == nullid);
        assert(idFace == nullid);
        idSubmesh = mesh.get_submesh_id(name);
        return;
    };

    auto on_group = [&](M& mesh, const std::string& name) -> void
    {
        assert(idSubmesh != nullid);
        assert(idSurface == nullid);
        assert(idFace == nullid);
        idSurface = mesh.get_surface_id(idSubmesh, name);
        return;
    };

    auto on_face_open = [&](M& mesh) -> void
    {
        assert(idSubmesh != nullid);
        assert(idSurface != nullid);

        idFace = mesh.create_face(idSurface, mesh.halfedge_store_count());
        nVertices = 0;
        return;
    };

    auto on_face_normal = [&](M& mesh, const typename M::vector_type& normal) -> void
    {
        mesh.face(idFace).attributes().set_normal(normal);
        return;
    };

    auto on_face_vertex = [&](M& mesh, const typename M::point_type& position, const typename M::vector_type& normal) -> void
    {
        id_type nh = mesh.halfedge_store_count();
        typename M::vertex_attributes_type av;
        av.set_position(position);
        av.set_normal(normal);
        mesh.create_halfedge_vertex(0, nh + 1, nh - 1, idFace, av);
        ++nVertices;
        return;
    };

    auto on_face_close = [&](M& mesh) -> void
    {
        if (nVertices < 3)
        {
            return; // Throw away invalid faces with too few vertices
        }
        else if (nVertices > 3)
        {
            // For invalid faces with too many vertices, create non-standard polygonal face instead of error
            // log this case though ...
        }

        // Correct prev and next for first and last halfedges respectively
        id_type nh = mesh.halfedge_store_count();
        mesh.halfedge(nh - nVertices).set_prev_id(nh - 1);
        mesh.halfedge(nh - 1).set_next_id(nh - nVertices);

        // Add partner id to each halfedge in this new face
        typename M::face_type& face = mesh.face(idFace);
        for (typename M::halfedge_type& halfedge : face.halfedges())
        {
            id_type idHalfedge = halfedge.id();
            const typename M::point_type& position = halfedge.attributes().position();
            const typename M::point_type& positionNext = halfedge.next().attributes().position();
            segment_type segment = {positionNext, position};
            auto i = segments.find(segment);
            if (i == segments.end())
            {
                segments[{position, positionNext}] = idHalfedge;
            }
            else
            {
                halfedge.set_partner_id(i->second);
                mesh.halfedge(i->second).set_partner_id(idHalfedge);
                segments.erase(i);
            }
        }

        return;
    };

    stl::Reader<M> reader(on_open, on_object, on_group, on_face_open, on_face_normal, on_face_vertex, on_face_close);
    reader.read(mesh, pathname);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::write_stl(const M& m, const std::filesystem::path& pathname, const std::string& name, bool bBinary)
{
    using attributes_type = M::vertex_attributes_type;
    using value_type = attributes_type::value_type;
    using point_type = attributes_type::point_type;
    using vector_type = attributes_type::vector_type;

    std::unique_ptr<stl::Writer<value_type, point_type, vector_type>> pwriter;
    if (bBinary)
    {
        pwriter = std::make_unique<stl::WriterBinary<value_type, point_type, vector_type>>(pathname);
    }
    else
    {
        pwriter = std::make_unique<stl::WriterText<value_type, point_type, vector_type>>(pathname);
    }

    pwriter->set_object(name);

    for (const typename M::face_type& face : m.faces())
    {
        pwriter->set_face(face.attributes().normal());
        for (const auto& halfedge : face.halfedges())
        {
            pwriter->set_face_vertex(halfedge.attributes().position());
        }
    }

    pwriter->close();

    return;
}

#endif // QUETZAL_MODEL_STL_IO_HPP
