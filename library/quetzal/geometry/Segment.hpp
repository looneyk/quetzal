#if !defined(QUETZAL_GEOMETRY_SEGMENT_HPP)
#define QUETZAL_GEOMETRY_SEGMENT_HPP
//------------------------------------------------------------------------------
// geometry
// Segment.hpp
//------------------------------------------------------------------------------

#include "Partition.hpp"
#include "Point.hpp"
#include "quetzal/math/Vector.hpp"
#include "quetzal/math/floating_point.hpp"
#include "quetzal/math/math_util.hpp"
#include <algorithm>
#include <iostream>
#include <cassert>

namespace quetzal::geometry
{

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Segment : public Partition<Traits>
    {
    public:

        using traits_type = Traits;
        using size_type = Traits::size_type;
        using value_type = Traits::value_type;
        using vector_type = math::Vector<Traits>;
        using point_type = Point<Traits>;
        using points_type = std::array<point_type, 2>;

        Segment() = default;
        Segment(const points_type& points);
        Segment(const point_type& a, const point_type& b);
        Segment(const Segment&) = default;
        Segment(Segment&&) = default;
        ~Segment() = default;

        Segment& operator=(const Segment&) = default;
        Segment& operator=(Segment&&) = default;

        const point_type& endpoint(size_type n) const;
        point_type& endpoint(size_type n);
        void set_endpoint(size_type n, const point_type& point);
        void set_endpoints(const point_type& a, const point_type& b);

        value_type length() const;
        vector_type vector() const; // vector connecting endpoints
        vector_type direction() const; // unit direction vector

        point_type point(value_type t) const;

        int compare(const point_type& point) const override;

        Point<Traits> projection(const Point<Traits>& point) const;
        value_type projection_parameter(const Point<Traits>& point) const;

    private:

        points_type m_endpoints;

        // Cached direction vector calculated only when needed
        mutable bool m_bDirection;
        mutable vector_type m_direction;
    };

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, Segment<Traits> segment);

} // namespace quetzal::geometry

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Segment<Traits>::Segment(const points_type& points) :
    Partition<Traits>(),
    m_endpoints(points),
    m_bDirection(false),
    m_direction()
{
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Segment<Traits>::Segment(const point_type& a, const point_type& b) :
    m_endpoints{a, b},
    m_bDirection(false),
    m_direction()
{
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::geometry::Segment<Traits>::point_type& quetzal::geometry::Segment<Traits>::endpoint(size_type n) const
{
    assert(n <= 1);
    return m_endpoints[n];
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Segment<Traits>::point_type& quetzal::geometry::Segment<Traits>::endpoint(size_type n)
{
    assert(n <= 1);
    m_bDirection = false;
    return m_endpoints[n];
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Segment<Traits>::set_endpoint(size_type n, const point_type& point)
{
    assert(n <= 1);
    m_bDirection = false;
    m_endpoints[n] = point;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Segment<Traits>::set_endpoints(const point_type& a, const point_type& b)
{
    m_bDirection = false;
    m_endpoints = {a, b};
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Segment<Traits>::value_type quetzal::geometry::Segment<Traits>::length() const
{
    return vector().norm();
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Segment<Traits>::vector_type quetzal::geometry::Segment<Traits>::vector() const
{
    return m_endpoints[1] - m_endpoints[0];
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Segment<Traits>::vector_type quetzal::geometry::Segment<Traits>::direction() const
{
    if (!m_bDirection)
    {
        m_direction = normalize(m_endpoints[1] - m_endpoints[0]);
        m_bDirection = true;
    }
     
    return m_direction;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Segment<Traits>::point_type quetzal::geometry::Segment<Traits>::point(value_type t) const
{
    assert(math::float_clamped01(t));
    return math::lerp(m_endpoints[0], m_endpoints[1], t);
}

//------------------------------------------------------------------------------
template<typename Traits>
int quetzal::geometry::Segment<Traits>::compare(const point_type& point) const
{
    if (vector_eq(point, m_endpoints[0]) || vector_eq(point, m_endpoints[1]))
    {
        return 0;
    }

    value_type t = projection_parameter(point);
    return math::float_clamped01(t) && vector_eq(point, this->point(t)) ? -1 : 1;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Segment<Traits>::point_type quetzal::geometry::Segment<Traits>::projection(const Point<Traits>& point) const
{
    return this->point(projection_parameter(point));
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Segment<Traits>::value_type quetzal::geometry::Segment<Traits>::projection_parameter(const Point<Traits>& point) const
{
    return math::dot(point - m_endpoints[0], vector()) / vector().norm_squared();
}

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, Segment<Traits> segment)
{
    os << "[" << segment.endpoint(0) << "], [" << segment.endpoint(1) << "]";
    return os;
}

#endif // QUETZAL_GEOMETRY_SEGMENT_HPP
