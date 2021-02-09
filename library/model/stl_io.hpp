#if !defined(QUETZAL_MODEL_STL_IO_HPP)
#define QUETZAL_MODEL_STL_IO_HPP
//------------------------------------------------------------------------------
// model
// stl_io.hpp
//------------------------------------------------------------------------------

// strict text read:
//   vectors have 3 components
//   loops have 3 points
//   facets have 1 loop
//   solid and endsolid names match
// may not be line oriented, so more general token parsing/consumption may technically be necessary

#include "common/Exception.hpp"
#include "common/id.hpp"
#include "math/Vector.hpp"
#include <array>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace quetzal
{

namespace model
{

    template<typename M>
    bool read_stl(M& mesh, const std::string& submeshname, const std::string& surfacename, const std::filesystem::path& pathname);

    template<typename M>
    bool read_stl_text(M& mesh, const std::string& submeshname, const std::string& surfacename, std::istream& is);

    template<typename M>
    bool read_stl_binary(M& mesh, const std::string& submeshname, const std::string& surfacename, std::istream& is);

    template<typename M>
    void write_stl_text(const M& m, const std::filesystem::path& pathname);

    template<typename M>
    void write_stl_binary(const M& m, const std::filesystem::path& pathname);

namespace stl_io_internal
{

    namespace Keyword
    {
        static const std::string Solid = "solid";
        static const std::string Facet = "facet";
        static const std::string Normal = "normal";
        static const std::string Outer = "outer";
        static const std::string Loop = "loop";
        static const std::string Vertex = "vertex";
        static const std::string EndLoop = "endloop";
        static const std::string EndFacet = "endfacet";
        static const std::string EndSolid = "endsolid";
    }

    //--------------------------------------------------------------------------
    template<typename V>
    V parse_vector(const std::vector<std::string>& tokens, size_t nStart)
    {
        using T = typename V::value_type;

        const size_t nComponents = 3;
        assert(V::dimension == nComponents);
        assert(tokens.size() == nStart + nComponents);

        V v =
        {
            quetzal::to_type<T>(tokens[nStart + 0]),
            quetzal::to_type<T>(tokens[nStart + 1]),
            quetzal::to_type<T>(tokens[nStart + 2])
        };

        return v;
    }

    //--------------------------------------------------------------------------
    float read_float(std::istream& is)
    {
        char c[4];
        is.read(c, 4);
        float f;
        reinterpret_cast<char*>(&f)[0] = c[0];
        reinterpret_cast<char*>(&f)[1] = c[1];
        reinterpret_cast<char*>(&f)[2] = c[2];
        reinterpret_cast<char*>(&f)[3] = c[3];
        return f;
    }

    //--------------------------------------------------------------------------
    uint32_t read_uint32(std::istream& is)
    {
        char c[4];
        is.read(c, 4);
        uint32_t i;
        reinterpret_cast<char*>(&i)[0] = c[0];
        reinterpret_cast<char*>(&i)[1] = c[1];
        reinterpret_cast<char*>(&i)[2] = c[2];
        reinterpret_cast<char*>(&i)[3] = c[3];
        return i;
    }

    //--------------------------------------------------------------------------
    uint16_t read_uint16(std::istream& is)
    {
        char c[2];
        is.read(c, 2);
        uint16_t i;
        reinterpret_cast<char*>(&i)[0] = c[0];
        reinterpret_cast<char*>(&i)[1] = c[1];
        return i;
    }

    //--------------------------------------------------------------------------
    template<typename V>
    V read_vector(std::istream& is)
    {
        assert(V::dimension == 3);
        typename V::value_type x = read_float(is);
        typename V::value_type y = read_float(is);
        typename V::value_type z = read_float(is);
        return {x, y, z};
    }

    //--------------------------------------------------------------------------
    void write_float(std::ostream& os, float f)
    {
        char c[4];
        c[0] = reinterpret_cast<char*>(&f)[0];
        c[1] = reinterpret_cast<char*>(&f)[1];
        c[2] = reinterpret_cast<char*>(&f)[2];
        c[3] = reinterpret_cast<char*>(&f)[3];
        os.write(c, 4);
        return;
    }

    //--------------------------------------------------------------------------
    void write_uint32(std::ostream& os, uint32_t i)
    {
        char c[4];
        c[0] = reinterpret_cast<char*>(&i)[0];
        c[1] = reinterpret_cast<char*>(&i)[1];
        c[2] = reinterpret_cast<char*>(&i)[2];
        c[3] = reinterpret_cast<char*>(&i)[3];
        os.write(c, 4);
        return;
    }

    //--------------------------------------------------------------------------
    void write_uint16(std::ostream& os, unsigned int i)
    {
        uint16_t ui16 = static_cast<uint16_t>(i);
        char c[2];
        c[0] = reinterpret_cast<char*>(&ui16)[0];
        c[1] = reinterpret_cast<char*>(&ui16)[1];
        os.write(c, 2);
        return;
    }

    //--------------------------------------------------------------------------
    template<typename V>
    void write_vector(std::ostream& os, const V& v)
    {
        assert(V::dimension == 3);
        write_float(os, v.x());
        write_float(os, v.y());
        write_float(os, v.z());
        return;
    }

    //--------------------------------------------------------------------------
    template<typename M>
    void add_face(M& mesh, id_type idSurface, const std::vector<typename M::point_type>& positions, const typename M::vector_type& normal, std::map<std::array<typename M::point_type, 2>, id_type>& segments)
    {
        using point_type = typename M::point_type;
        using segment_type = std::array<point_type, 2>;

        size_t nh = mesh.halfedge_store_count();

        id_type idFace = mesh.create_face(idSurface, nh, {normal});

        size_t nVertices = positions.size();
        for (size_t i = 0; i < nVertices; ++i)
        {
            typename M::vertex_attributes_type av;
            av.position() = positions[i];

            id_type idVertex = mesh.create_vertex(nh + i, av);
            mesh.create_halfedge(0, nh + i + 1, nh + i - 1, idVertex, idFace);
        }

        // Correct prev and next for first and last halfedges respectively
        mesh.halfedge(nh).set_prev_id(nh + nVertices - 1);
        mesh.halfedge(nh + nVertices - 1).set_next_id(nh);

        // Add partner id to each halfedge in this new face
        id_type ih = nh;
        for (typename M::halfedge_type& halfedge : mesh.face(idFace).halfedges())
        {
            const point_type& position = halfedge.attributes().position();
            const typename M::halfedge_type& heNext = halfedge.next();
            const point_type& positionNext = heNext.attributes().position();
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
    }

} // namespace stl_io_internal

} // namespace model

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::model::read_stl(M& mesh, const std::string& submeshname, const std::string& surfacename, const std::filesystem::path& pathname)
{
    std::ifstream ifs(pathname, std::ios_base::binary);
    if (!ifs)
    {
        return false;
    }

    char c[6];
    ifs.read(c, 6);
    ifs.seekg(0, std::ios_base::beg);
    if (strncmp(c, "solid ", 6) == 0)
    {
        return read_stl_text(mesh, submeshname, surfacename, ifs);
    }

    return read_stl_binary(mesh, submeshname, surfacename, ifs);
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::model::read_stl_text(M& mesh, const std::string& submeshname, const std::string& surfacename, std::istream& is)
{
    using point_type = typename M::point_type;
    using vector_type = typename M::vector_type;

    std::vector<point_type> positions;
    vector_type normal;

    std::map<std::array<point_type, 2>, id_type> segments;

    id_type idSubmesh = mesh.create_submesh(submeshname);
    id_type idSurface = mesh.create_surface(idSubmesh, surfacename);

    auto& submesh = mesh.submesh(idSubmesh);

    std::string keywordPrev;

    while (!is.eof())
    {
        std::string line;
        getline(is, line);

        auto tokens = tokenize(line);
        if (tokens.empty())
        {
            continue;
        }

        std::string keyword = tokens[0];

        if (keyword == stl_io_internal::Keyword::Solid)
        {
            if (!keywordPrev.empty())
            {
                return false;
            }

            if (tokens.size() > 2)
            {
                return false;
            }

            if (submesh.face_count() != 0)
            {
                return false;
            }

            if (tokens.size() > 1)
            {
                mesh.set_name(tokens[1]);
            }
        }
        else if (keyword == stl_io_internal::Keyword::Facet)
        {
            if (keywordPrev != stl_io_internal::Keyword::EndFacet && keywordPrev != stl_io_internal::Keyword::Solid)
            {
                return false;
            }

            if (tokens.size() != 5)
            {
                return false;
            }

            if (tokens[1] != stl_io_internal::Keyword::Normal)
            {
                return false;
            }

            if (!positions.empty())
            {
                return false;
            }

            normal = stl_io_internal::parse_vector<vector_type>(tokens, 2);
        }
        else if (keyword == stl_io_internal::Keyword::Outer)
        {
            if (keywordPrev != stl_io_internal::Keyword::Facet)
            {
                return false;
            }

            if (tokens.size() != 2)
            {
                return false;
            }

            if (tokens[1] != stl_io_internal::Keyword::Loop)
            {
                return false;
            }

            if (!positions.empty())
            {
                return false;
            }
        }
        else if (keyword == stl_io_internal::Keyword::Vertex)
        {
            if (keywordPrev != stl_io_internal::Keyword::Vertex && keywordPrev != stl_io_internal::Keyword::Outer)
            {
                return false;
            }

            if (tokens.size() != 4)
            {
                return false;
            }

            if (positions.size() == 3)
            {
                return false;
            }

            positions.emplace_back(stl_io_internal::parse_vector<point_type>(tokens, 1));
        }
        else if (keyword == stl_io_internal::Keyword::EndLoop)
        {
            if (keywordPrev != stl_io_internal::Keyword::Vertex)
            {
                return false;
            }

            if (tokens.size() != 1)
            {
                return false;
            }

            if (positions.size() < 3)
            {
                return false;
            }
        }
        else if (keyword == stl_io_internal::Keyword::EndFacet)
        {
            if (keywordPrev != stl_io_internal::Keyword::EndLoop)
            {
                return false;
            }

            stl_io_internal::add_face(mesh, idSurface, positions, normal, segments);
            positions.clear();
        }
        else if (keyword == stl_io_internal::Keyword::EndSolid)
        {
            if (keywordPrev != stl_io_internal::Keyword::EndFacet)
            {
                return false;
            }

            if (tokens.size() > 2)
            {
                return false;
            }

            if (tokens.size() == 1 && !mesh.name().empty() || tokens[1] != mesh.name())
            {
                return false;
            }
        }

        keywordPrev = keyword;
    }

    return segments.empty();
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::model::read_stl_binary(M& mesh, const std::string& submeshname, const std::string& surfacename, std::istream& is)
{
    using point_type = typename M::point_type;
    using vector_type = typename M::vector_type;

    std::vector<point_type> positions;
    vector_type normal;

    std::map<std::array<point_type, 2>, id_type> segments;

    id_type idSubmesh = mesh.create_submesh(submeshname);
    id_type idSurface = mesh.create_surface(idSubmesh, surfacename);

    is.ignore(80);
    size_t n = stl_io_internal::read_uint32(is);

    for (size_t i = 0; i < n; ++i)
    {
        if (is.eof())
        {
            return false;
        }

        normal = stl_io_internal::read_vector<vector_type>(is);
        positions.emplace_back(stl_io_internal::read_vector<point_type>(is));
        positions.emplace_back(stl_io_internal::read_vector<point_type>(is));
        positions.emplace_back(stl_io_internal::read_vector<point_type>(is));
        is.ignore(2); // uint16 attribute byte count ignored

        stl_io_internal::add_face(mesh, idSurface, positions, normal, segments);
        positions.clear();
    }

    return segments.empty();
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::write_stl_text(const M& m, const std::filesystem::path& pathname)
{
    std::ofstream ofs(pathname, std::ios_base::binary);
    if (!ofs)
    {
        std::ostringstream oss;
        oss << "Error opening output file " << pathname;
        throw Exception(oss.str(), __FILE__, __LINE__);
    }

    using point_type = typename M::traits_type::point_type;

    ofs << stl_io_internal::Keyword::Solid << " " << m.name() << std::endl;

    for (const auto& face : m.faces())
    {
        ofs << stl_io_internal::Keyword::Facet << " " << stl_io_internal::Keyword::Normal << " " << face.attributes().normal() << std::endl;
        ofs << "  " << stl_io_internal::Keyword::Outer << " " << stl_io_internal::Keyword::Loop << std::endl;

        for (const auto& halfedge : face.halfedges())
        {
            const point_type& position = halfedge.attributes().position();
            ofs << "    " << stl_io_internal::Keyword::Vertex << " " << position.x() << " " << position.y() << " " << position.z() << std::endl;
        }

        ofs << "  " << stl_io_internal::Keyword::EndLoop << std::endl;
        ofs << stl_io_internal::Keyword::EndFacet << std::endl;
    }

    ofs << stl_io_internal::Keyword::EndSolid << " " << m.name() << std::endl;

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::write_stl_binary(const M& m, const std::filesystem::path& pathname)
{
    std::ofstream ofs(pathname, std::ios_base::binary);
    if (!ofs)
    {
        std::ostringstream oss;
        oss << "Error opening output file " << pathname;
        throw Exception(oss.str(), __FILE__, __LINE__);
    }

    ofs.write("STL Binary - quetzal                                                            ", 80);
    stl_io_internal::write_uint32(ofs, m.face_count());

    for (const auto& face : m.faces())
    {
        stl_io_internal::write_vector(ofs, face.attributes().normal());

        for (const auto& halfedge : face.halfedges())
        {
            stl_io_internal::write_vector(ofs, halfedge.attributes().position());
        }

        stl_io_internal::write_uint16(ofs, 0);
    }

    return;
}

#endif // QUETZAL_MODEL_STL_IO_HPP
