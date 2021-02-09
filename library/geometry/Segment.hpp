#if !defined(QUETZAL_GEOMETRY_SEGMENT_HPP)
#define QUETZAL_GEOMETRY_SEGMENT_HPP
//------------------------------------------------------------------------------
// geometry
// Segment.hpp
//------------------------------------------------------------------------------

#include "Point.hpp"
#include "math/Vector.hpp"
#include "math/floating_point.hpp"
#include "math/math_util.hpp"
#include <algorithm>
#include <iostream>
#include <cassert>

namespace quetzal
{

namespace geometry
{

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Segment
    {
    public:

        using value_type = typename Traits::value_type;
        using vector_type = math::Vector<Traits>;
        using point_type = Point<Traits>;
        using points_type = std::array<point_type, 2>;
        using size_type = typename Traits::size_type;

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
        vector_type direction() const; // vector connecting endpoints, not a unit vector

        point_type point(value_type t) const;

        bool contains(const point_type& point) const;

    private:

        points_type m_endpoints;
    };

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, Segment<Traits> segment);

} // namespace geometry

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Segment<Traits>::Segment(const points_type& points) :
    m_endpoints(points)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Segment<Traits>::Segment(const point_type& a, const point_type& b) :
    m_endpoints{a, b}
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
    return m_endpoints[n];
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Segment<Traits>::set_endpoint(size_type n, const point_type& point)
{
    assert(n <= 1);
    m_endpoints[n] = point;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Segment<Traits>::set_endpoints(const point_type& a, const point_type& b)
{
    m_endpoints = {a, b};
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Segment<Traits>::value_type quetzal::geometry::Segment<Traits>::length() const
{
    return direction().norm();
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Segment<Traits>::vector_type quetzal::geometry::Segment<Traits>::direction() const
{
    return m_endpoints[1] - m_endpoints[0];
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
bool quetzal::geometry::Segment<Traits>::contains(const point_type& point) const
{
    value_type t = dot(point - m_endpoints[0], direction()) / direction().norm_squared();
    return math::float_ge0(t) && math::float_le(t, Traits::val(1)) && vector_eq(point, this->point(t));
}

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, Segment<Traits> segment)
{
    os << "[" << segment.endpoint(0) << "], [" << segment.endpoint(1) << "]";
    return os;
}

#endif // QUETZAL_GEOMETRY_SEGMENT_HPP
