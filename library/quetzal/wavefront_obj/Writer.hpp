#if !defined(QUETZAL_WAVEFRONT_OBJ_WRITER_HPP)
#define QUETZAL_WAVEFRONT_OBJ_WRITER_HPP
//------------------------------------------------------------------------------
// Wavefront obj file i/o
// Writer.hpp
//------------------------------------------------------------------------------

#include "quetzal/common/Exception.hpp"
#include "quetzal/math/Vector.hpp"
#include "quetzal/math/VectorTraits.hpp"
#include "symbols.hpp"
#include <map>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>

namespace quetzal::wavefront_obj
{

    //--------------------------------------------------------------------------
    template<typename T, typename P = math::Vector<math::VectorTraits<T, 3>>, typename V = math::Vector<math::VectorTraits<T, 3>>, typename U = math::Vector<math::VectorTraits<T, 2>>>
    class Writer
    {
    public:

        using value_type = T;
        using point_type = P;
        using vector_type = V;
        using texcoord_type = U;

        Writer(const std::filesystem::path& pathname);
        Writer(const Writer&) = delete;
        ~Writer();

        Writer& operator=(const Writer&) = delete;

        void set_vertex(const point_type& position, const vector_type& normal, const texcoord_type& texcoord);
        void set_object(const std::string& name);
        void set_group(const std::string& name);
        void set_face();
        void set_face_vertex(const point_type& position, const vector_type& normal, const texcoord_type& texcoord);
        void set_materials(const std::string& name);
        void set_material(const std::string& name);

        void clear();

    private:

        using id_type = long;

        void write_vertices();
        void close_section();
        void close_face();

        std::ofstream m_os;

        std::vector<point_type> m_positions;
        std::vector<vector_type> m_normals;
        std::vector<texcoord_type> m_texcoords;

        std::map<point_type, id_type> m_unique_positions;
        std::map<vector_type, id_type> m_unique_normals;
        std::map<texcoord_type, id_type> m_unique_texcoords;

        bool m_bVertices;
        bool m_bFace;
    };

} // namespace quetzal::wavefront_obj

//------------------------------------------------------------------------------
template<typename T, typename P, typename V, typename U>
quetzal::wavefront_obj::Writer<T, P, V, U>::Writer(const std::filesystem::path& pathname) :
    m_os(pathname, std::ios_base::binary),
    m_positions(),
    m_normals(),
    m_texcoords(),
    m_unique_positions(),
    m_unique_normals(),
    m_unique_texcoords(),
    m_bVertices(true),
    m_bFace(false)
{
    if (!m_os)
    {
        std::ostringstream oss;
        oss << "Error opening output file " << pathname;
        throw Exception(oss.str(), __FILE__, __LINE__);
    }
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V, typename U>
quetzal::wavefront_obj::Writer<T, P, V, U>::~Writer()
{
    close_face();
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V, typename U>
void quetzal::wavefront_obj::Writer<T, P, V, U>::set_vertex(const point_type& position, const vector_type& normal, const texcoord_type& texcoord)
{
    assert(m_bVertices);

    id_type idPosition = 0;
    const auto ip = m_unique_positions.find(position);
    if (ip != m_unique_positions.end())
    {
        idPosition = ip->second;
    }
    else
    {
        idPosition = static_cast<id_type>(m_positions.size());
        m_unique_positions.insert(std::make_pair(position, idPosition));
        m_positions.push_back(position);
    }

    id_type idNormal = 0;
    const auto in = m_unique_normals.find(normal);
    if (in != m_unique_normals.end())
    {
        idNormal = in->second;
    }
    else
    {
        idNormal = static_cast<id_type>(m_normals.size());
        m_unique_normals.insert(std::make_pair(normal, idNormal));
        m_normals.push_back(normal);
    }

    id_type idTexcoord = 0;
    const auto it = m_unique_texcoords.find(texcoord);
    if (it != m_unique_texcoords.end())
    {
        idTexcoord = it->second;
    }
    else
    {
        idTexcoord = static_cast<id_type>(m_texcoords.size());
        m_unique_texcoords.insert(std::make_pair(texcoord, idTexcoord));
        m_texcoords.push_back(texcoord);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V, typename U>
void quetzal::wavefront_obj::Writer<T, P, V, U>::set_object(const std::string& name)
{
    close_section();
    m_os << Keyword::Object << " " << name << std::endl;
    return;
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V, typename U>
void quetzal::wavefront_obj::Writer<T, P, V, U>::set_group(const std::string& name)
{
    close_section();
    m_os << Keyword::Group << " " << (name != GroupNameDefault ? name : "") << std::endl;
    return;
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V, typename U>
void quetzal::wavefront_obj::Writer<T, P, V, U>::set_face()
{
    close_section();
    m_os << Keyword::Face;
    m_bFace = true;
    return;
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V, typename U>
void quetzal::wavefront_obj::Writer<T, P, V, U>::set_face_vertex(const point_type& position, const vector_type& normal, const texcoord_type& texcoord)
{
    assert(m_bFace);

    const auto ip = m_unique_positions.find(position);
    assert(ip != m_unique_positions.end());
    m_os << " " << (ip->second + 1) << "/";

    if (!m_unique_texcoords.empty())
    {
        const auto it = m_unique_texcoords.find(texcoord);
        assert(it != m_unique_texcoords.end());
        m_os << (it->second + 1);
    }

    m_os << "/";

    if (!m_unique_normals.empty())
    {
        const auto in = m_unique_normals.find(normal);
        assert(in != m_unique_normals.end());
        m_os << (in->second + 1);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V, typename U>
void quetzal::wavefront_obj::Writer<T, P, V, U>::set_materials(const std::string& name)
{
    m_os << Keyword::Materials << " " << name << std::endl;
    return;
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V, typename U>
void quetzal::wavefront_obj::Writer<T, P, V, U>::set_material(const std::string& name)
{
    m_os << Keyword::Material << " " << name << std::endl;
    return;
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V, typename U>
void quetzal::wavefront_obj::Writer<T, P, V, U>::clear()
{
    m_positions.clear();
    m_normals.clear();
    m_texcoords.clear();
    m_unique_positions.clear();
    m_unique_normals.clear();
    m_unique_texcoords.clear();
    return;
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V, typename U>
void quetzal::wavefront_obj::Writer<T, P, V, U>::write_vertices()
{
    if (m_unique_normals.size() == 1 && m_normals[0].zero())
    {
        m_normals.clear();
        m_unique_normals.clear();
    }

    if (m_unique_texcoords.size() == 1)
    {
        m_texcoords.clear();
        m_unique_texcoords.clear();
    }

    m_os << "# Vertex Positions" << std::endl;
    for (const point_type& v : m_positions)
    {
        m_os << Keyword::Position << " " << v << std::endl;
    }

    if (!m_texcoords.empty())
    {
        m_os << std::endl;
        m_os << "# Texture Coordinates" << std::endl;
        for (const texcoord_type& v : m_texcoords)
        {
            m_os << Keyword::Texcoord << " " << v << std::endl;
        }
    }

    if (!m_normals.empty())
    {
        m_os << std::endl;
        m_os << "# Vertex Normals" << std::endl;
        for (const vector_type& v : m_normals)
        {
            m_os << Keyword::Normal << " " << v << std::endl;
        }
    }

    m_os << std::endl;
    m_os << "# Faces" << std::endl;

    return;
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V, typename U>
void quetzal::wavefront_obj::Writer<T, P, V, U>::close_section()
{
    if (m_bVertices)
    {
        write_vertices();
        m_bVertices = false;
    }
    else
    {
        close_face();
    }

    return;
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V, typename U>
void quetzal::wavefront_obj::Writer<T, P, V, U>::close_face()
{
    if (m_bFace)
    {
        m_os << std::endl;
        m_bFace = false;
    }

    return;
}

#endif // QUETZAL_WAVEFRONT_OBJ_WRITER_HPP
