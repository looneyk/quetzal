#if !defined(QUETZAL_STL_WRITERTEXT_HPP)
#define QUETZAL_STL_WRITERTEXT_HPP
//------------------------------------------------------------------------------
// STL file i/o
// WriterText.hpp
//------------------------------------------------------------------------------

#include "quetzal/common/Exception.hpp"
#include "quetzal/math/Vector.hpp"
#include "quetzal/math/VectorTraits.hpp"
#include "Writer.hpp"
#include "symbols.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>

namespace quetzal::stl
{

    //--------------------------------------------------------------------------
    template<typename T, typename P = math::Vector<math::VectorTraits<T, 3>>, typename V = math::Vector<math::VectorTraits<T, 3>>>
    class WriterText : public Writer<T, P, V>
    {
    public:

        using value_type = T;
        using point_type = P;
        using vector_type = V;

        WriterText(const std::filesystem::path& pathname);
        WriterText(const WriterText&) = delete;
        WriterText(WriterText&&) = delete;
        ~WriterText();

        WriterText& operator=(const WriterText&) = delete;
        WriterText& operator=(WriterText&&) = delete;

        void set_object(const std::string& name) override;
        void set_face(const vector_type& normal) override;
        void set_face_vertex(const point_type& position) override;
        void close_face() override;
        void close() override;

    private:

        std::ofstream m_ofs;
        std::string m_name;
        bool m_bObject;
        bool m_bFace;
        bool m_bClosed;
    };

} // namespace quetzal::stl

//------------------------------------------------------------------------------
template<typename T, typename P, typename V>
quetzal::stl::WriterText<T, P, V>::WriterText(const std::filesystem::path& pathname) :
    Writer<T, P, V>(),
    m_ofs(pathname, std::ios_base::binary),
    m_bObject(false),
    m_bFace(false),
    m_bClosed(false)
{
    if (!m_ofs)
    {
        std::ostringstream oss;
        oss << "Error opening output file " << pathname;
        throw Exception(oss.str(), __FILE__, __LINE__);
    }
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V>
quetzal::stl::WriterText<T, P, V>::~WriterText()
{
    close();
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V>
void quetzal::stl::WriterText<T, P, V>::set_object(const std::string& name)
{
    assert(!m_bObject);
    assert(!m_bFace);
    m_name = name;
    m_ofs << Keyword::Solid << " " << name << std::endl;

    m_bObject = true;
    return;
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V>
void quetzal::stl::WriterText<T, P, V>::set_face(const vector_type& normal)
{
    assert(m_bObject);
    close_face();
    m_ofs << "  " << Keyword::Facet << " " << Keyword::Normal << " " << normal << std::endl;
    m_ofs << "    " << Keyword::Outer << " " << Keyword::Loop << std::endl;
    m_bFace = true;
    return;
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V>
void quetzal::stl::WriterText<T, P, V>::set_face_vertex(const point_type& position)
{
    assert(m_bFace);
    m_ofs << "      " << Keyword::Vertex << " " << position << std::endl;
    return;
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V>
void quetzal::stl::WriterText<T, P, V>::close_face()
{
    if (m_bFace)
    {
        m_ofs << "    " << Keyword::EndLoop << std::endl;
        m_ofs << "  " << Keyword::EndFacet << std::endl;
        m_bFace = false;
    }

    return;
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V>
void quetzal::stl::WriterText<T, P, V>::close()
{
    if (!m_bClosed)
    {
        close_face();
        m_ofs << Keyword::EndSolid << " " << m_name << std::endl;
        m_ofs.close();
        m_bClosed = true;
    }

    return;
}

#endif // QUETZAL_STL_WRITERTEXT_HPP
