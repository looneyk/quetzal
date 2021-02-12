#if !defined(QUETZAL_GEOMETRY_POLYLINE_HPP)
#define QUETZAL_GEOMETRY_POLYLINE_HPP
//------------------------------------------------------------------------------
// geometry
// Polyline.hpp
//------------------------------------------------------------------------------

#include "Point.hpp"
#include "Points.hpp"
#include "points_util.hpp"
#include "quetzal/geometry/Segment.hpp"
#include "quetzal/geometry/distance.hpp" // ...
#include <filesystem>
#include <iostream>
#include <vector>

namespace quetzal
{

namespace geometry
{

    // Polyline is an ordered set of points
    // Each sequential pair of points defines an edge
    // edge count == vertex count - 1

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Polyline
    {
    public:

        using value_type = typename Traits::value_type;
        using vector_type = math::Vector<Traits>;
        using point_type = Point<Traits>;
        using vertex_type = Point<Traits>;
        using vertices_type = Points<Traits>;
        using size_type = typename Traits::size_type;

        using reference = typename vertices_type::reference;
        using const_reference = typename vertices_type::const_reference;
        using iterator = typename vertices_type::iterator;
        using const_iterator = typename vertices_type::const_iterator;

        Polyline() = default;
        explicit Polyline(size_type n);
        Polyline(std::initializer_list<vertex_type> points);
        Polyline(const Polyline&) = default;
        Polyline(Polyline&&) = default;
        ~Polyline() = default;

        Polyline& operator=(const Polyline&) = default;
        Polyline& operator=(Polyline&&) = default;

        const vertex_type& vertex(size_type n) const;
        vertex_type& vertex(size_type n);
        void set_vertex(size_type n, const point_type& point);

        // functions to add/insert points, or just remove iterators/accessors, ...
        const vertices_type& vertices() const;
        vertices_type& vertices();

        iterator begin() noexcept;
        const_iterator begin() const noexcept;
        iterator end() noexcept;
        const_iterator end() const noexcept;

        size_t vertex_count() const noexcept;
        size_t edge_count() const noexcept;

        typename Traits::value_type length() const;
        typename Traits::value_type length(size_t i0, size_t i1) const;

        void clear();

        void load(const std::filesystem::path& filepath); // Replace the current set of points
        void append(const std::filesystem::path& filepath);

    private:

        vertices_type m_vertices;
    };

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, const Polyline<Traits>& polyline);

} // namespace geometry

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polyline<Traits>::Polyline(size_type n) :
    m_vertices(n)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polyline<Traits>::Polyline(std::initializer_list<vertex_type> points) :
    m_vertices(points)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
typename const quetzal::geometry::Polyline<Traits>::vertex_type& quetzal::geometry::Polyline<Traits>::vertex(size_type n) const
{
    assert(n < m_vertices.size());
    return m_vertices[n];
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Polyline<Traits>::vertex_type& quetzal::geometry::Polyline<Traits>::vertex(size_type n)
{
    assert(n < m_vertices.size());
    return m_vertices[n];
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Polyline<Traits>::set_vertex(size_type n, const point_type& point)
{
    assert(n < m_vertices.size());
    m_vertices[n] = point;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Polyline<Traits>::vertices_type& quetzal::geometry::Polyline<Traits>::vertices()
{
    return m_vertices;
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::geometry::Polyline<Traits>::vertices_type& quetzal::geometry::Polyline<Traits>::vertices() const
{
    return m_vertices;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Polyline<Traits>::iterator quetzal::geometry::Polyline<Traits>::begin() noexcept
{
    return m_vertices.begin();
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Polyline<Traits>::const_iterator quetzal::geometry::Polyline<Traits>::begin() const noexcept
{
    return m_vertices.begin();
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Polyline<Traits>::iterator quetzal::geometry::Polyline<Traits>::end() noexcept
{
    return m_vertices.end();
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Polyline<Traits>::const_iterator quetzal::geometry::Polyline<Traits>::end() const noexcept
{
    return m_vertices.end();
}

//------------------------------------------------------------------------------
template<typename Traits>
size_t quetzal::geometry::Polyline<Traits>::vertex_count() const noexcept
{
    return m_vertices.size();
}

//------------------------------------------------------------------------------
template<typename Traits>
size_t quetzal::geometry::Polyline<Traits>::edge_count() const noexcept
{
    return m_vertices.size() - 1;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::geometry::Polyline<Traits>::length() const
{
    typename Traits::value_type d = Traits::val(0);

    size_t n = m_vertices.size();
    for (size_t i = 1; i < n; ++i)
    {
        d += geometry::distance(m_vertices[i - 1], m_vertices[i]);
    }

    return d;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::geometry::Polyline<Traits>::length(size_t i0, size_t i1) const
{
    assert(i1 < vertex_count());
    assert(i0 < i1);

    typename Traits::value_type d = Traits::val(0);
    for (size_t i = i0 + 1; i <= i1; ++i)
    {
        d += geometry::distance(m_vertices[i - 1], m_vertices[i]);
    }

    return d;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Polyline<Traits>::clear()
{
    m_vertices.clear();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Polyline<Traits>::load(const std::filesystem::path& filepath)
{
    m_vertices.clear();
    load_points(filepath, m_vertices);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Polyline<Traits>::append(const std::filesystem::path& filepath)
{
    load_points(filepath, m_vertices);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, const Polyline<Traits>& polyline)
{
    os << polyline.vertices();
    return os;
}

#endif // QUETZAL_GEOMETRY_POLYLINE_HPP
