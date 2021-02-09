#if !defined(QUETZAL_GEOMETRY_RAY_HPP)
#define QUETZAL_GEOMETRY_RAY_HPP
//------------------------------------------------------------------------------
// geometry
// Ray.hpp
//------------------------------------------------------------------------------

#include "Point.hpp"
#include "math/Vector.hpp"
#include "math/floating_point.hpp"
#include "math/math_util.hpp"
#include <iostream>
#include <cassert>

namespace quetzal
{

namespace geometry
{

    // Client responsibility to ensure that the direction vector is unit length

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Ray
    {
    public:

        using value_type = typename Traits::value_type;
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

    private:

        point_type m_endpoint;
        vector_type m_direction;
    };

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, Ray<Traits> ray);

} // namespace geometry

} // namespace quetzal

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
std::ostream& quetzal::geometry::operator<<(std::ostream& os, Ray<Traits> ray)
{
    os << "[" << ray.endpoint() << "], [" << ray.direction() << "]";
    return os;
}

#endif // QUETZAL_GEOMETRY_RAY_HPP
