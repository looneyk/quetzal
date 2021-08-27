#if !defined(QUETZAL_GEOMETRY_RAY_HPP)
#define QUETZAL_GEOMETRY_RAY_HPP
//------------------------------------------------------------------------------
// geometry
// Ray.hpp
//------------------------------------------------------------------------------

#include "Point.hpp"
#include "quetzal/math/Vector.hpp"
#include "quetzal/math/floating_point.hpp"
#include "quetzal/math/math_util.hpp"
#include <iostream>
#include <cassert>

namespace quetzal::geometry
{

    // Client responsibility to ensure that the direction vector is unit length

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Ray
    {
    public:

        using traits_type = Traits;
        using value_type = Traits::value_type;
        using vector_type = math::Vector<Traits>;
        using point_type = Point<Traits>;

        Ray() = default;
        Ray(const point_type& point, const vector_type& direction);
        Ray(const Ray&) = default;
        Ray(Ray&&) = default;
        ~Ray() = default;

        Ray& operator=(const Ray&) = default;
        Ray& operator=(Ray&&) = default;

        const point_type& endpoint() const;
        point_type& endpoint();
        void set_endpoint(const point_type& point);

        vector_type direction() const;
        void set_direction(const vector_type& direction);

        point_type point(value_type t) const;

        bool contains(const point_type& point) const;

        Point<Traits> projection(const Point<Traits>& point) const;
        value_type projection_parameter(const Point<Traits>& point) const;

    private:

        point_type m_endpoint;
        vector_type m_direction;
    };

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, Ray<Traits> ray);

} // namespace quetzal::geometry

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Ray<Traits>::Ray(const point_type& point, const vector_type& direction) :
    m_endpoint(point),
    m_direction(direction)
{
    assert(vector_unit(direction));
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::geometry::Ray<Traits>::point_type& quetzal::geometry::Ray<Traits>::endpoint() const
{
    return m_endpoint;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Ray<Traits>::point_type& quetzal::geometry::Ray<Traits>::endpoint()
{
    return m_endpoint;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Ray<Traits>::set_endpoint(const point_type& point)
{
    m_endpoint = point;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Ray<Traits>::vector_type quetzal::geometry::Ray<Traits>::direction() const
{
    return m_direction;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Ray<Traits>::set_direction(const vector_type& direction)
{
    assert(vector_unit(direction));

    m_direction = direction;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Ray<Traits>::point_type quetzal::geometry::Ray<Traits>::point(value_type t) const
{
    assert(math::float_ge0(t));
    return m_endpoint + math::clamp0(t) * m_direction;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Ray<Traits>::contains(const point_type& point) const
{
    value_type t = dot(point - m_endpoint, m_direction);
    return math::float_ge0(t) && vector_eq(point, this->point(t));
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Ray<Traits>::point_type quetzal::geometry::Ray<Traits>::projection(const Point<Traits>& point) const
{
    return this->point(projection_parameter(point));
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Ray<Traits>::value_type quetzal::geometry::Ray<Traits>::projection_parameter(const Point<Traits>& point) const
{
    return math::dot(point - m_endpoint, m_direction);
}

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, Ray<Traits> ray)
{
    os << "[" << ray.endpoint() << "], [" << ray.direction() << "]";
    return os;
}

#endif // QUETZAL_GEOMETRY_RAY_HPP
