#if !defined(QUETZAL_STL_READER_HPP)
#define QUETZAL_STL_READER_HPP
//------------------------------------------------------------------------------
// STL file i/o
// Reader.hpp
//------------------------------------------------------------------------------

#include "quetzal/common/Exception.hpp"
#include "quetzal/math/Vector.hpp"
#include "quetzal/math/VectorTraits.hpp"
#include "binary_util.hpp"
#include "symbols.hpp"
#include <array>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>

namespace quetzal::stl
{

    //--------------------------------------------------------------------------
    template<typename M>
    class Reader
    {
    public:

        using attributes_type = M::vertex_attributes_type;
        using value_type = attributes_type::value_type;
        using point_type = attributes_type::point_type;
        using vector_type = attributes_type::vector_type;

        using open_function_type = std::function<void(M&, const std::filesystem::path& pathname)>;
        using object_function_type = std::function<void(M&, const std::string&)>;
        using group_function_type = std::function<void(M&, const std::string&)>;
        using face_open_function_type = std::function<void(M&)>;
        using face_normal_function_type = std::function<void(M&, const vector_type&)>;
        using face_vertex_function_type = std::function<void(M&, const point_type&, const vector_type&)>;
        using face_close_function_type = std::function<void(M&)>;

        Reader(open_function_type on_open, object_function_type on_object, group_function_type on_group, face_open_function_type on_face_open, face_normal_function_type on_face_normal, face_vertex_function_type on_face_vertex, face_close_function_type on_face_close);
        Reader(const Reader&) = delete;
        ~Reader() = default;

        Reader& operator=(const Reader&) = delete;

        bool read(M& mesh, const std::filesystem::path& pathname);

    private:

        bool read_text(M& mesh);
        bool read_binary(M& mesh);

        void next();
        bool accept(const std::string& keyword);
        bool expect(const std::string& keyword);
        std::string extract_string();
        vector_type extract_vector();

        std::filesystem::path m_pathname;
        std::ifstream m_ifs;
        std::string m_token;
        size_t m_n;

        open_function_type m_on_open;
        object_function_type m_on_object;
        group_function_type m_on_group;
        face_open_function_type m_on_face_open;
        face_normal_function_type m_on_face_normal;
        face_vertex_function_type m_on_face_vertex;
        face_close_function_type m_on_face_close;
    };

} // namespace quetzal::stl

//------------------------------------------------------------------------------
template<typename M>
quetzal::stl::Reader<M>::Reader(open_function_type on_open, object_function_type on_object, group_function_type on_group, face_open_function_type on_face_open, face_normal_function_type on_face_normal, face_vertex_function_type on_face_vertex, face_close_function_type on_face_close) :
    m_pathname(),
    m_ifs(),
    m_token(),
    m_n(0),
    m_on_open(on_open),
    m_on_object(on_object),
    m_on_group(on_group),
    m_on_face_open(on_face_open),
    m_on_face_normal(on_face_normal),
    m_on_face_vertex(on_face_vertex),
    m_on_face_close(on_face_close)
{
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::stl::Reader<M>::read(M& mesh, const std::filesystem::path& pathname)
{
    m_pathname = pathname;

    m_ifs.open(m_pathname, std::ios_base::binary);
    if (!m_ifs)
    {
        std::ostringstream oss;
        oss << "Error opening input file " << m_pathname;
        throw Exception(oss.str(), __FILE__, __LINE__);
    }

    m_on_open(mesh, m_pathname);

    char discriminator[6];
    m_ifs.read(discriminator, 5);
    discriminator[5] = '\0';
    m_ifs.seekg(0);
    if (std::string(discriminator) == stl::Keyword::Solid)
    {
        return read_text(mesh);
    }

    return read_binary(mesh);
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::stl::Reader<M>::read_text(M& mesh)
{
    using point_type = M::point_type;
    using vector_type = M::vector_type;

    next();
    expect(Keyword::Solid);
    std::string name = extract_string();
    m_on_object(mesh, name);
    m_on_group(mesh, stl::GroupNameDefault);

    for (;;)
    {
        if (m_ifs.eof())
        {
            std::ostringstream oss;
            oss << "Error premature eof in file " << m_pathname << " line " << m_n;
            throw Exception(oss.str(), __FILE__, __LINE__);
        }

        if (accept(Keyword::EndSolid))
        {
            std::string token = extract_string();
//            m_on_close(token); // ...
            if (token != name)
            {
                std::ostringstream oss;
                oss << "Error endsolid name " << token << " does not match original name " << name << " in file " << m_pathname << " line " << m_n;
                throw Exception(oss.str(), __FILE__, __LINE__);
            }

            break;
        }

        expect(Keyword::Facet);
        m_on_face_open(mesh);

        expect(Keyword::Normal);
        vector_type normal = extract_vector();
        m_on_face_normal(mesh, normal);
        
        expect(Keyword::Outer);
        expect(Keyword::Loop);

        while (accept(Keyword::Vertex))
        {
            vector_type position = extract_vector();
            m_on_face_vertex(mesh, position, normal);
        }

        expect(Keyword::EndLoop);

        expect(Keyword::EndFacet);
        m_on_face_close(mesh);
    }

    return true;
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::stl::Reader<M>::read_binary(M& mesh)
{
    using point_type = M::point_type;
    using vector_type = M::vector_type;

    std::string name = m_pathname.stem().string();
    m_on_object(mesh, name);
    m_on_group(mesh, GroupNameDefault);

    m_ifs.ignore(80); // Skip header
    size_t nFaces = read_uint32(m_ifs);

    for (size_t i = 0; i < nFaces; ++i)
    {
        if (m_ifs.eof())
        {
            std::ostringstream oss;
            oss << "Error premature eof in file " << m_pathname << " line " << m_n;
            throw Exception(oss.str(), __FILE__, __LINE__);
        }

        m_on_face_open(mesh);

        vector_type normal = read_vector<vector_type>(m_ifs);
        m_on_face_normal(mesh, normal);

        m_on_face_vertex(mesh, read_vector<point_type>(m_ifs), normal);
        m_on_face_vertex(mesh, read_vector<point_type>(m_ifs), normal);
        m_on_face_vertex(mesh, read_vector<point_type>(m_ifs), normal);

        read_uint16(m_ifs); // uint16 attribute byte count ignored

        m_on_face_close(mesh);
    }

    return true;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::stl::Reader<M>::next()
{
    while (isspace(m_ifs.peek()))
    {
        int c = m_ifs.get();
        if (c == '\n')
        {
            ++m_n;
        }
    }

    if (m_ifs.eof())
    {
        std::ostringstream oss;
        oss << "Error premature eof in file " << m_pathname << " line " << m_n;
        throw Exception(oss.str(), __FILE__, __LINE__);
    }

    m_ifs >> m_token;
    return;
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::stl::Reader<M>::accept(const std::string& keyword)
{
    if (m_token == keyword)
    {
        next();
        return true;
    }

    return false;
}

//------------------------------------------------------------------------------
template<typename M>
bool quetzal::stl::Reader<M>::expect(const std::string& keyword)
{
    if (!accept(keyword))
    {
        std::ostringstream oss;
        oss << "Error invalid token " << m_token << ", " << keyword << " expected at line " << m_n;
        throw Exception(oss.str().c_str(), __FILE__, __LINE__);
    }

    return true;
}

//------------------------------------------------------------------------------
template<typename M>
std::string quetzal::stl::Reader<M>::extract_string()
{
    std::string token = m_token;
    next();
    return token;
}

//------------------------------------------------------------------------------
template<typename M>
quetzal::stl::Reader<M>::vector_type quetzal::stl::Reader<M>::extract_vector()
{
    using T = M::value_type;
    using V = M::vector_type;

    std::string sx = extract_string();
    std::string sy = extract_string();
    std::string sz = extract_string();

    V v =
    {
        quetzal::to_type<T>(sx),
        quetzal::to_type<T>(sy),
        quetzal::to_type<T>(sz)
    };

    return v;
}

#endif // QUETZAL_STL_READER_HPP
