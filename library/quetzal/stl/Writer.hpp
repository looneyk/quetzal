#if !defined(QUETZAL_STL_WRITER_HPP)
#define QUETZAL_STL_WRITER_HPP
//------------------------------------------------------------------------------
// STL file i/o
// Writer.hpp
//------------------------------------------------------------------------------

#include <string>

namespace quetzal::stl
{

    //--------------------------------------------------------------------------
    template<typename T, typename P, typename V>
    class Writer
    {
    public:

        using value_type = T;
        using point_type = P;
        using vector_type = V;

        Writer() = default;
        Writer(const Writer&) = delete;
        Writer(Writer&&) = delete;
        virtual ~Writer() = default;

        Writer& operator=(const Writer&) = delete;
        Writer& operator=(Writer&&) = delete;

        virtual void set_object(const std::string& name) = 0;
        virtual void set_face(const vector_type& normal) = 0;
        virtual void set_face_vertex(const point_type& position) = 0;

        // These will generally be called automatically at tye appropriate time
        virtual void close_face() = 0;
        virtual void close() = 0;
    };

} // namespace quetzal::stl

#endif // QUETZAL_STL_WRITER_HPP
