#if !defined(QUETZAL_GEOMETRY_POLYGON_HPP)
#define QUETZAL_GEOMETRY_POLYGON_HPP
//------------------------------------------------------------------------------
// geometry
// Polygon.hpp
//------------------------------------------------------------------------------

#include "Point.hpp"
#include "Points.hpp"
#include "Plane.hpp"
#include "triangle_util.hpp"
#include "math/Matrix.hpp"
#include "math/Vector.hpp"
#include <iostream>
#include <vector>
#include <cassert>

namespace quetzal
{

namespace geometry
{

    // Polygon is an ordered set of vertices
    // Each sequential pair of vertices defines an edge, with an implied edge between the last and first vertices
    // edge count == vertex count
    // Results of center, area, contains, normal, and plane are undefined if the polygon is not planar or vertices are not in CCW order

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Polygon
    {
    public:

        using value_type = typename Traits::value_type;
        using vector_type = math::Vector<Traits>;
        using point_type = Point<Traits>;
        using vertex_type = Point<Traits>;
        using vertices_type = Points<Traits>;
        using size_type = typename Traits::size_type;

        // this may be enough to disable plane ...
        // but requires the ugly plane_type<> ...
//        template<typename = std::enable_if_t<(Traits::dimension == 3)>>
        using plane_type = Plane<Traits>;

        Polygon() = default;
        explicit Polygon(size_type n);
        Polygon(std::initializer_list<vertex_type> points);
        Polygon(const Polygon&) = default;
        Polygon(Polygon&&) = default;
        virtual ~Polygon() = default;

        template<typename InputIterator>
        Polygon(InputIterator first, InputIterator last);

        Polygon& operator=(const Polygon&) = default;
        Polygon& operator=(Polygon&&) = default;

        const vertex_type& vertex(size_type n) const;
        vertex_type& vertex(size_type n);
        void set_vertex(size_type n, const point_type& point);

        const vertices_type& vertices() const;
        vertices_type& vertices();

        // These will be the same but are provided for clarity in different contexts
        size_t vertex_count() const noexcept;
        size_t edge_count() const noexcept;

        bool planar() const;
        bool contains(const point_type& point) const;

        point_type center() const;
        vector_type extent() const;
        std::array<point_type, 2> bounds() const;

        // Based on implied winding around positive z-axis
        template<typename = std::enable_if_t<(Traits::dimension == 2)>>
        value_type area() const;

        template<typename = std::enable_if_t<(Traits::dimension == 3)>>
        value_type area(const vector_type& normal) const;

        // Based on implied winding around positive z-axis
        template<typename = std::enable_if_t<(Traits::dimension == 2)>>
        value_type signed_area() const;

        template<typename = std::enable_if_t<(Traits::dimension == 3)>>
        value_type signed_area(const vector_type& normal) const;

        // Assumes CCW winding order
        template<typename = std::enable_if_t<(Traits::dimension == 3)>>
        vector_type normal() const;

        template<typename = std::enable_if_t<(Traits::dimension == 3)>>
        plane_type plane() const;

        virtual void clear();

    private:

        vertices_type m_vertices;
    };

    template<typename Traits>
    Polygon<Traits> lerp(const Polygon<Traits>& a, const Polygon<Traits>& b, typename Traits::value_type t);

    template<typename Traits>
    Polygon<Traits>& operator*=(Polygon<Traits>& lhs, const math::Matrix<typename Traits::value_type, Traits::dimension, Traits::dimension>& rhs);

    template<typename Traits>
    Polygon<Traits>& operator*=(Polygon<Traits>& lhs, const math::Matrix<typename Traits::value_type, Traits::dimension + 1, Traits::dimension + 1>& rhs);

    template<typename Traits>
    Polygon<Traits> operator*(const math::Matrix<typename Traits::value_type, Traits::dimension, Traits::dimension>& lhs, const Polygon<Traits>& rhs);

    template<typename Traits>
    Polygon<Traits> operator*(const Polygon<Traits>& lhs, const math::Matrix<typename Traits::value_type, Traits::dimension, Traits::dimension>& rhs);

    template<typename Traits>
    Polygon<typename Traits::reduced_type> operator*(const math::Matrix<typename Traits::value_type, Traits::dimension, Traits::dimension>& lhs, const Polygon<typename Traits::reduced_type>& rhs);

    template<typename Traits>
    Polygon<Traits> operator*(const Polygon<Traits>& lhs, const math::Matrix<typename Traits::value_type, Traits::dimension + 1, Traits::dimension + 1>& rhs);

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, const Polygon<Traits>& polygon);

} // namespace geometry

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polygon<Traits>::Polygon(size_type n) :
    m_vertices(n)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polygon<Traits>::Polygon(std::initializer_list<vertex_type> points) :
    m_vertices(points)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
template<typename InputIterator>
quetzal::geometry::Polygon<Traits>::Polygon(InputIterator first, InputIterator last) :
    m_vertices()
{
    for (auto i = first; i != last; ++i)
    {
        m_vertices.push_back(*i);
    }
}

//------------------------------------------------------------------------------
template<typename Traits>
typename const quetzal::geometry::Polygon<Traits>::vertex_type& quetzal::geometry::Polygon<Traits>::vertex(size_type n) const
{
    assert(n < m_vertices.size());
    return m_vertices[n];
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Polygon<Traits>::vertex_type& quetzal::geometry::Polygon<Traits>::vertex(size_type n)
{
    assert(n < m_vertices.size());
    return m_vertices[n];
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Polygon<Traits>::set_vertex(size_type n, const point_type& point)
{
    assert(n < m_vertices.size());
    m_vertices[n] = point;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Polygon<Traits>::vertices_type& quetzal::geometry::Polygon<Traits>::vertices()
{
    return m_vertices;
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::geometry::Polygon<Traits>::vertices_type& quetzal::geometry::Polygon<Traits>::vertices() const
{
    return m_vertices;
}

//------------------------------------------------------------------------------
template<typename Traits>
size_t quetzal::geometry::Polygon<Traits>::vertex_count() const noexcept
{
    return m_vertices.size();
}

//------------------------------------------------------------------------------
template<typename Traits>
size_t quetzal::geometry::Polygon<Traits>::edge_count() const noexcept
{
    return m_vertices.size();
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Polygon<Traits>::planar() const
{
    if constexpr (Traits::dimension >= 3)
    {
        if (m_vertices.size() <= 3)
        {
            return true;
        }

        Plane<Traits> plane = this->plane();
        for (const auto& point : m_vertices)
        {
            if (!plane.contains(point))
            {
                return false;
            }
        }
    }

    return true;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Polygon<Traits>::contains(const point_type& point) const
{
    point; // ...
    assert(false); // ...
    return false; // ...
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Polygon<Traits>::point_type quetzal::geometry::Polygon<Traits>::center() const
{
    point_type position;
    for (const auto& point : m_vertices)
    {
        position += point;
    }

    return position / static_cast<typename Traits::value_type>(m_vertices.size());
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Polygon<Traits>::vector_type quetzal::geometry::Polygon<Traits>::extent() const
{
    auto [boundsMin, boundsMax] = bounds();
    return boundsMax - boundsMin;
}

//------------------------------------------------------------------------------
template<typename Traits>
std::array<typename quetzal::geometry::Polygon<Traits>::point_type, 2> quetzal::geometry::Polygon<Traits>::bounds() const
{
    point_type boundsMin = m_vertices[0];
    point_type boundsMax = m_vertices[0];

    for (const auto& point : m_vertices)
    {
        if (point.x() < boundsMin.x()) boundsMin.set_x(point.x());
        if (point.y() < boundsMin.y()) boundsMin.set_y(point.y());
        if (point.z() < boundsMin.z()) boundsMin.set_z(point.z());
        if (point.x() > boundsMax.x()) boundsMax.set_x(point.x());
        if (point.y() > boundsMax.y()) boundsMax.set_y(point.y());
        if (point.z() > boundsMax.z()) boundsMax.set_z(point.z());
    }

    return {boundsMin, boundsMax};
}

//------------------------------------------------------------------------------
template<typename Traits>
template<typename>
typename Traits::value_type quetzal::geometry::Polygon<Traits>::area() const
{
    return std::abs(signed_area());
}

//------------------------------------------------------------------------------
template<typename Traits>
template<typename>
typename Traits::value_type quetzal::geometry::Polygon<Traits>::area(const vector_type& normal) const
{
    return std::abs(signed_area(normal));
}

//------------------------------------------------------------------------------
template<typename Traits>
template<typename>
typename Traits::value_type quetzal::geometry::Polygon<Traits>::signed_area() const
{
    typename Traits::value_type area = 0;

    size_t n = m_vertices.size();
    for (size_t i = 1; i < n - 1; ++i)
    {
        area += triangle_area(m_vertices[0], m_vertices[i], m_vertices[i + 1]);
    }

    return area;
}

//------------------------------------------------------------------------------
template<typename Traits>
template<typename>
typename Traits::value_type quetzal::geometry::Polygon<Traits>::signed_area(const vector_type& normal) const
{
    typename Traits::value_type area = 0;

    size_t n = m_vertices.size();
    for (size_t i = 1; i < n - 1; ++i)
    {
        area += triangle_area(m_vertices[0], m_vertices[i], m_vertices[i + 1], normal);
    }

    return area;
}

//------------------------------------------------------------------------------
template<typename Traits>
template<typename>
typename quetzal::geometry::Polygon<Traits>::vector_type quetzal::geometry::Polygon<Traits>::normal() const
{
    size_t n = m_vertices.size();
    for (size_t i = 0; i < n; ++i)
    {
        size_t i0 = i;
        size_t i1 = (i + 1) % n;
        size_t i2 = (i + 2) % n;
        if (colinear(m_vertices[i0], m_vertices[i1], m_vertices[i2]))
        {
            continue;
        }

        return normalize(cross(m_vertices[i2] - m_vertices[i1], m_vertices[i0] - m_vertices[i1]));
    }

    return Polygon<Traits>::vector_type(); // Degenerate case, all polygon vertices fall on a single line
}

//------------------------------------------------------------------------------
template<typename Traits>
template<typename>
typename quetzal::geometry::Polygon<Traits>::plane_type quetzal::geometry::Polygon<Traits>::plane() const
{
    return plane_type(m_vertices[0], normal());
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Polygon<Traits>::clear()
{
    m_vertices.clear();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polygon<Traits> quetzal::geometry::lerp(const Polygon<Traits>& a, const Polygon<Traits>& b, typename Traits::value_type t)
{
    assert(a.vertex_count() == b.vertex_count());

    size_t n = a.vertex_count();
    Polygon<Traits> polygon(n);

    for (size_t i = 0; i < n; ++i)
    {
        polygon.vertex(i) = lerp(a.vertex(i), b.vertex(i), t);
    }

    return polygon;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polygon<Traits>& quetzal::geometry::operator*=(Polygon<Traits>& lhs, const math::Matrix<typename Traits::value_type, Traits::dimension, Traits::dimension>& rhs)
{
    for (size_t i = 0; i < lhs.vertex_count(); ++i)
    {
        lhs.vertex(i) *= rhs;
    }

    return lhs;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polygon<Traits>& quetzal::geometry::operator*=(Polygon<Traits>& lhs, const math::Matrix<typename Traits::value_type, Traits::dimension + 1, Traits::dimension + 1>& rhs)
{
    for (size_t i = 0; i < lhs.vertex_count(); ++i)
    {
        lhs.vertex(i) *= rhs;
    }

    return lhs;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polygon<Traits> quetzal::geometry::operator*(const math::Matrix<typename Traits::value_type, Traits::dimension, Traits::dimension>& lhs, const Polygon<Traits>& rhs)
{
    Polygon<Traits> polygon(rhs.vertex_count());

    for (size_t i = 0; i < rhs.vertex_count(); ++i)
    {
        polygon.vertex(i) = lhs * rhs.vertex(i);
    }

    return polygon;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polygon<Traits> quetzal::geometry::operator*(const Polygon<Traits>& lhs, const math::Matrix<typename Traits::value_type, Traits::dimension, Traits::dimension>& rhs)
{
    Polygon<Traits> polygon(lhs.vertex_count());

    for (size_t i = 0; i < lhs.vertex_count(); ++i)
    {
        polygon.vertex(i) = lhs.vertex(i) * rhs;
    }

    return polygon;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polygon<typename Traits::reduced_type> quetzal::geometry::operator*(const math::Matrix<typename Traits::value_type, Traits::dimension, Traits::dimension>& lhs, const Polygon<typename Traits::reduced_type>& rhs)
{
    Polygon<typename Traits::reduced_type> polygon(rhs.vertex_count());

    for (size_t i = 0; i < rhs.vertex_count(); ++i)
    {
        polygon.vertex(i) = lhs * rhs.vertex(i);
    }

    return polygon;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polygon<Traits> quetzal::geometry::operator*(const Polygon<Traits>& lhs, const math::Matrix<typename Traits::value_type, Traits::dimension + 1, Traits::dimension + 1>& rhs)
{
    Polygon<Traits> polygon(lhs.vertex_count());

    for (size_t i = 0; i < lhs.vertex_count(); ++i)
    {
        polygon.vertex(i) = lhs.vertex(i) * rhs;
    }

    return polygon;
}

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, const Polygon<Traits>& polygon)
{
    os << polygon.vertices();
    return os;
}

#endif // QUETZAL_GEOMETRY_POLYGON_HPP
