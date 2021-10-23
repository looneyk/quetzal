#if !defined(QUETZAL_GEOMETRY_POLYGON_HPP)
#define QUETZAL_GEOMETRY_POLYGON_HPP
//------------------------------------------------------------------------------
// geometry
// Polygon.hpp
//------------------------------------------------------------------------------

#include "AxisAlignedBoundingBox.hpp"
#include "Partition.hpp"
#include "Point.hpp"
#include "Plane.hpp"
#include "Segment.hpp"
#include "triangle_util.hpp"
#include "quetzal/math/DimensionReducer.hpp"
#include "quetzal/math/Matrix.hpp"
#include "quetzal/math/Vector.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <cassert>

namespace quetzal::geometry
{

    // Polygon is an ordered set of vertices
    // Each sequential pair of vertices defines an edge, with an implied edge between the last and first vertices
    // edge count == vertex count
    // Results of centroid, area, normal, and plane are only meaningful if the polygon is planar and the vertices are in CCW order

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Polygon : public Partition<Traits>
    {
    public:

        using traits_type = Traits;
        using size_type = Traits::size_type;
        using value_type = Traits::value_type;
        using vector_type = math::Vector<Traits>;
        using point_type = Point<Traits>;
        using vertex_type = Point<Traits>;
        using vertices_type = std::vector<vertex_type>;
        using edge_type = Segment<Traits>;
        using matrix_type = math::Matrix<value_type, Traits::dimension + 1, Traits::dimension + 1>;

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

        void transform(const matrix_type& matrix);

        // Returns -1, 0, 1: interior, boundary, exterior
        // Exterior is defined by the winding order
        int compare(const point_type& point) const override;

        virtual void clear();

        // Point is in the region defined by the polygon
        // Results are indeterminate if the point is on an edge
        // integrate these into compare ...
        bool contains_(const point_type& point) const requires (Traits::dimension == 2);
        bool contains_(const point_type& point) const requires (Traits::dimension == 3);

    private:

        vertices_type m_vertices;
    };

    template<typename Traits>
    Polygon<Traits> lerp(const Polygon<Traits>& a, const Polygon<Traits>& b, typename Traits::value_type t);

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, const Polygon<Traits>& polygon);

} // namespace quetzal::geometry

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polygon<Traits>::Polygon(size_type n) :
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
void quetzal::geometry::Polygon<Traits>::transform(const matrix_type& matrix)
{
    for (auto& vertex : m_vertices)
    {
        vertex *= matrix;
    }

    return;
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

    return contains_(point) ? -1 : 1;
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
bool quetzal::geometry::Polygon<Traits>::contains_(const point_type& point) const requires (Traits::dimension == 2)
{
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
bool quetzal::geometry::Polygon<Traits>::contains_(const point_type& point) const requires (Traits::dimension == 3)
{
    if (!planar() || !plane().contains(point))
    {
        return false;
    }

	math::DimensionReducer<Traits> dr(normal());

    Polygon<typename Traits::reduced_traits> polygon;
    std::transform(m_vertices.begin(), m_vertices.end(), std::back_inserter(polygon.vertices()), [&](const math::Vector<Traits>& v) -> math::Vector<Traits::reduced_traits> { return dr.reduce(v); });

    return polygon.contains_(dr.reduce(point));
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
std::ostream& quetzal::geometry::operator<<(std::ostream& os, const Polygon<Traits>& polygon)
{
    os << polygon.vertices();
    return os;
}

#endif // QUETZAL_GEOMETRY_POLYGON_HPP
