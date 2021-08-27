#if !defined(QUETZAL_STL_WRITERBINARY_HPP)
#define QUETZAL_STL_WRITERBINARY_HPP
//------------------------------------------------------------------------------
// STL file i/o
// WriterBinary.hpp
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
    class WriterBinary : public Writer<T, P, V>
    {
    public:

        using value_type = T;
        using point_type = P;
        using vector_type = V;

        WriterBinary(const std::filesystem::path& pathname);
        WriterBinary(const WriterBinary&) = delete;
        WriterBinary(WriterBinary&&) = delete;
        ~WriterBinary();

        WriterBinary& operator=(const WriterBinary&) = delete;
        WriterBinary& operator=(WriterBinary&&) = delete;

        void set_object(const std::string& name) override;
        void set_face(const vector_type& normal) override;
        void set_face_vertex(const point_type& position) override;
        void close_face() override;
        void close() override;

    private:

        std::ofstream m_ofs;
        bool m_bObject;
        bool m_bFace;
        bool m_bClosed;
        size_t m_nFaces;
    };

} // namespace quetzal::stl

//------------------------------------------------------------------------------
template<typename T, typename P, typename V>
quetzal::stl::WriterBinary<T, P, V>::WriterBinary(const std::filesystem::path& pathname) :
    Writer<T, P, V>(),
    m_ofs(pathname, std::ios_base::binary),
    m_bObject(false),
    m_bFace(false),
    m_bClosed(false),
    m_nFaces(0)
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
quetzal::stl::WriterBinary<T, P, V>::~WriterBinary()
{
    close();
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V>
void quetzal::stl::WriterBinary<T, P, V>::set_object(const std::string& name)
{
    assert(!m_bObject);
    assert(!m_bFace);

    std::string header = "STL Binary: " + name;
    header.resize(80, ' ');
    header[79] = '\n';
    m_ofs.write(header.c_str(), 80);
    write_uint32(m_ofs, 0); // Placeholder value for now, given actual value at end

    m_bObject = true;
    return;
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V>
void quetzal::stl::WriterBinary<T, P, V>::set_face(const vector_type& normal)
{
    assert(m_bObject);
    close_face();
    write_vector(m_ofs, normal);
    ++m_nFaces;
    m_bFace = true;
    return;
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V>
void quetzal::stl::WriterBinary<T, P, V>::set_face_vertex(const point_type& position)
{
    assert(m_bFace);
    write_vector(m_ofs, position);
    return;
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V>
void quetzal::stl::WriterBinary<T, P, V>::close_face()
{
    if (m_bFace)
    {
        write_uint16(m_ofs, 0);
        m_bFace = false;
    }

    return;
}

//------------------------------------------------------------------------------
template<typename T, typename P, typename V>
void quetzal::stl::WriterBinary<T, P, V>::close()
{
    if (!m_bClosed)
    {
        close_face();
        m_ofs.seekp(80);
        write_uint32(m_ofs, m_nFaces);
        m_ofs.close();
        m_bClosed = true;
    }

    return;
}

#endif // QUETZAL_STL_WRITERBINARY_HPP
