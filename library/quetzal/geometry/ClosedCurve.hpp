#if !defined(QUETZAL_GEOMETRY_CLOSEDCURVE_HPP)
#define QUETZAL_GEOMETRY_CLOSEDCURVE_HPP
//------------------------------------------------------------------------------
// geometry
// Polygon.hpp
//------------------------------------------------------------------------------

#include "AxisAlignedBoundingBox.hpp"
#include "Partition.hpp"
#include "Point.hpp"
#include "Points.hpp"
#include "Polyline.hpp"
#include "Plane.hpp"
#include "Segment.hpp"
#include "triangle_util.hpp"
#include "quetzal/math/Matrix.hpp"
#include "quetzal/math/Vector.hpp"
#include <iostream>
#include <vector>
#include <cassert>

namespace quetzal::geometry
{

    // ClosedCurve is a polygon with section information

    // Polygon is an ordered set of vertices
    // Each sequential pair of vertices defines an edge, with an implied edge between the last and first vertices
    // edge count == vertex count
    // Sections are a set of sequential polygon edges that are taken together as a unit, and cover the polygon vertices
    // Results of centroid, area, normal, and plane are only meaningful if the polygon is planar and the vertices are in CCW order

    //--------------------------------------------------------------------------
    template<typename Traits> : public Partition<Traits>
    class Polygon
    {
    public:

        using traits_type = Traits;
        using size_type = Traits::size_type;
        using value_type = Traits::value_type;
        using vector_type = math::Vector<Traits>;
        using point_type = Point<Traits>;
        using vertex_type = Point<Traits>;
        using vertices_type = Points<Traits>;
        using edge_type = Segment<Traits>;
        using section_type = Polyline<Traits>;

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

        const vertices_type& vertices() const;
        vertices_type& vertices();

        size_t vertex_count() const noexcept;
        const vertex_type& vertex(size_type n) const;
        vertex_type& vertex(size_type n);
        void set_vertex(size_type n, const point_type& point);

        size_t edge_count() const noexcept;
        edge_type edge(size_type n) const;

        size_t section_count() const noexcept;
        section_type section(size_type n) const;
        size_t section_vertex_id(size_type n) const; // First vertex in section n

        bool planar() const;

        point_type centroid() const;
        point_type vertex_mean() const; // The geometric mean of all vertices
        AxisAlignedBoundingBox<Traits> bounds() const;

        value_type area() const requires (Traits::dimension == 2);
        value_type area(const vector_type& normal) const requires (Traits::dimension == 3);

        // Based on expressed or implied CCW winding order
        value_type signed_area() const requires (Traits::dimension == 2);
        value_type signed_area(const vector_type& normal) const requires (Traits::dimension == 3);

        // Assumes CCW winding order
        vector_type normal() const requires (Traits::dimension == 3);
        Plane<Traits> plane() const requires (Traits::dimension == 3);

        // Returns -1, 0, 1: interior, boundary, exterior
        // Exterior is defined by the winding order
        int compare(const point_type& point) const override;

        virtual void clear();

    private:

        vertices_type m_vertices;
        std::vector<size_type> m_sections;
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

} // namespace quetzal::geometry

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polygon<Traits>::Polygon(size_type n) :
    Partition<Traits>(),
    m_vertices(n)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polygon<Traits>::Polygon(std::initializer_list<vertex_type> points) :
    Partition<Traits>(),
    m_vertices(points)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
template<typename InputIterator>
quetzal::geometry::Polygon<Traits>::Polygon(InputIterator first, InputIterator last) :
    Partition<Traits>(),
    m_vertices()
{
    for (auto i = first; i != last; ++i)
    {
        m_vertices.push_back(*i);
    }
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
size_t quetzal::geometry::Polygon<Traits>::edge_count() const noexcept
{
    return m_vertices.size();
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Polygon<Traits>::edge_type quetzal::geometry::Polygon<Traits>::edge(size_type n) const
{
    assert(n < m_vertices.size());
    return {m_vertices[n], m_vertices[(n + 1) % m_vertices.size()]};
}

//------------------------------------------------------------------------------
template<typename Traits>
size_t quetzal::geometry::Polygon<Traits>::section_count() const noexcept
{
    return m_sections.size();
}

//------------------------------------------------------------------------------
template<typename Traits>
typename const quetzal::geometry::Polygon<Traits>::section_type quetzal::geometry::Polygon<Traits>::section(size_type n) const
{
    assert(n < m_sections.size());

    section_type section;

    size_type nNext = (n < m_sections.size() - 1 ? n + 1 : 0);
    size_type iStart = m_sections[n];
    size_type iEnd = m_sections[nNext];

    size_type iEnd1 = (iEnd < iStart ? iEnd : m_sections.size());
    for (size_type i = iStart; i < iEnd1; ++i)
    {
        section.push_back(m_vertices[i]);
    }

    if (iEnd < iStart)
    {
        for (size_type i = 0; i < iEnd; ++i)
        {
            section.push_back(m_vertices[i]);
        }
    }

    return section;
}

//------------------------------------------------------------------------------
template<typename Traits>
size_t quetzal::geometry::Polygon<Traits>::section_vertex_id(size_type n) const
{
    assert(n < m_sections.size());
    return m_sections[n];
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
typename quetzal::geometry::Polygon<Traits>::point_type quetzal::geometry::Polygon<Traits>::centroid() const
{
    if (m_vertices.size() == 3)
    {
        return vertex_mean();
    }

    typename Traits::value_type areaTotal = 0;
    quetzal::geometry::Polygon<Traits>::point_type weightedTriangleCentroidTotal;

    size_t n = m_vertices.size();
    for (size_t i = 1; i < n - 1; ++i)
    {
        typename Traits::value_type area = triangle_area(m_vertices[0], m_vertices[i], m_vertices[i + 1]);
        areaTotal += area;
        weightedTriangleCentroidTotal += area * triangle_centroid(m_vertices[0], m_vertices[i], m_vertices[i + 1]);
    }

    return weightedTriangleCentroidTotal / areaTotal;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Polygon<Traits>::point_type quetzal::geometry::Polygon<Traits>::vertex_mean() const
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
quetzal::geometry::AxisAlignedBoundingBox<Traits> quetzal::geometry::Polygon<Traits>::bounds() const
{
    return AxisAlignedBoundingBox(m_vertices);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::geometry::Polygon<Traits>::area() const requires (Traits::dimension == 2)
{
    return std::abs(signed_area());
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::geometry::Polygon<Traits>::area(const vector_type& normal) const requires (Traits::dimension == 3)
{
    return std::abs(signed_area(normal));
}

//------------------------------------------------------------------------------
template<typename Traits>
typename Traits::value_type quetzal::geometry::Polygon<Traits>::signed_area() const requires (Traits::dimension == 2)
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
typename Traits::value_type quetzal::geometry::Polygon<Traits>::signed_area(const vector_type& normal) const requires (Traits::dimension == 3)
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
typename quetzal::geometry::Polygon<Traits>::vector_type quetzal::geometry::Polygon<Traits>::normal() const requires (Traits::dimension == 3)
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
quetzal::geometry::Plane<Traits> quetzal::geometry::Polygon<Traits>::plane() const requires (Traits::dimension == 3)
{
    return Plane<Traits>(m_vertices[0], normal());
}

//------------------------------------------------------------------------------
template<typename Traits>
int quetzal::geometry::Polygon<Traits>::compare(const point_type& point) const
{
    for (size_t i = 0; i < edge_count(); ++i)
    {
        Segment<Traits> segment(m_vertices[i], m_vertices[(i + 1) % vertex_count()]);
        if (segment.contains(point))
        {
            return 0;
        }
    }

    return contains(point) ? -1 : 1;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Polygon<Traits>::contains(const point_type& point) const requires (Traits::dimension == 2)
{
    // integrate these into compare ...

    // Return >0: left, ==0: on, <0: right for p2 relative to the line through p0 and p1
    auto left = [](point_type p0, point_type p1, point_type p2) -> int
    {
        auto t = (p1.x() - p0.x()) * (p2.y() - p0.y()) - (p2.x() -  p0.x()) * (p1.y() - p0.y());
        if (math::float_eq0(t))
            return 0;
        return t < 0 ? -1 : 1;
    };

    auto vertices = m_vertices;
    vertices.push_back(m_vertices[0]);

    int n = 0; // the  winding number counter

    for (int i = 0; i < edge_count(); ++i)
    {
        if (math::float_le(vertices[i].y(), point.y()))
        {
            if (math::float_gt(vertices[i + 1].y(), point.y())) // upward crossing
                 if (left(vertices[i], vertices[i + 1], point) > 0)
                     ++n; // valid up intersect
        }
        else
        {
            if (math::float_le(vertices[i + 1].y(), point.y())) // downward crossing
                 if (left(vertices[i], vertices[i + 1], point) < 0)
                     --n; // valid down intersect
        }
    }

    return n != 0;
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
    os << polygon.sections();
    return os;
}

#endif // QUETZAL_GEOMETRY_CLOSEDCURVE_HPP
