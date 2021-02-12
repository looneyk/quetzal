#if !defined(QUETZAL_GEOMETRY_PLANE_HPP)
#define QUETZAL_GEOMETRY_PLANE_HPP
//------------------------------------------------------------------------------
// geometry
// Plane.hpp
//------------------------------------------------------------------------------

#include "Point.hpp"
#include "quetzal/math/Vector.hpp"
#include <iostream>
#include <cassert>

namespace quetzal
{

namespace geometry
{

    // Client responsibility to ensure that the normal vector is unit length

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Plane
    {
    public:

		static_assert(Traits::dimension >= 3);

        using vector_type = math::Vector<Traits>;
        using point_type = Point<Traits>;

        Plane() = default;
        Plane(const point_type& point, const vector_type& normal);
        Plane(const Plane&) = default;
        Plane(Plane&&) = default;
        ~Plane() = default;

        Plane& operator=(const Plane&) = default;
        Plane& operator=(Plane&&) = default;

        point_type point() const;
        void set_point(const point_type& point);

        vector_type normal() const;
        void set_normal(const vector_type& normal);

        // Returns -1, 0, 1: interior, contains, exterior
        // Exterior is defined by the direction of the normal
        int region(const point_type& point) const;

        bool contains(const point_type& point) const;
        bool interior(const point_type& point) const;
        bool exterior(const point_type& point) const;

    private:

        point_type m_point;
        vector_type m_normal;
    };

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, Plane<Traits> plane);

} // namespace geometry

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Plane<Traits>::Plane(const point_type& point, const vector_type& normal) :
    m_point(point),
    m_normal(normal)
{
    assert(vector_unit(normal));
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Plane<Traits>::point_type quetzal::geometry::Plane<Traits>::point() const
{
    return m_point;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Plane<Traits>::set_point(const point_type& point)
{
    m_point = point;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Plane<Traits>::vector_type quetzal::geometry::Plane<Traits>::normal() const
{
    return m_normal;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Plane<Traits>::set_normal(const vector_type& normal)
{
    assert(vector_unit(normal));
    m_normal = normal;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
int quetzal::geometry::Plane<Traits>::region(const point_type& point) const
{
    typename Traits::value_type d = dot(m_normal, (point - m_point));
    return math::float_eq0(d) ? 0 : (d < Traits::val(0) ? -1 : 1);
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Plane<Traits>::contains(const point_type& point) const
{
    return region(point) == 0;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Plane<Traits>::interior(const point_type& point) const
{
    return region(point) < 0;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Plane<Traits>::exterior(const point_type& point) const
{
    return region(point) > 0;
}

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, Plane<Traits> plane)
{
    os << "[" << plane.point() << "], [" << plane.normal() << "]";
    return os;
}

#endif // QUETZAL_GEOMETRY_PLANE_HPP
