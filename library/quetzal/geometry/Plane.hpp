#if !defined(QUETZAL_GEOMETRY_PLANE_HPP)
#define QUETZAL_GEOMETRY_PLANE_HPP
//------------------------------------------------------------------------------
// geometry
// Plane.hpp
//------------------------------------------------------------------------------

#include "Partition.hpp"
#include "Point.hpp"
#include "quetzal/math/Vector.hpp"
#include <iostream>
#include <cassert>

namespace quetzal::geometry
{

    // Client responsibility to ensure that the normal vector is unit length

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Plane : public Partition<Traits>
    {
    public:

        static_assert(Traits::dimension >= 3); // can this be done above at class level? usage in Polygon ...

        using traits_type = Traits;
        using vector_type = math::Vector<Traits>;
        using point_type = Point<Traits>;

        Plane() = default;
        Plane(const point_type& point, const vector_type& normal) requires (Traits::dimension >= 3);
        Plane(const Plane&) = default;
        Plane(Plane&&) = default;
        ~Plane() = default;

        Plane& operator=(const Plane&) = default;
        Plane& operator=(Plane&&) = default;

        point_type point() const requires (Traits::dimension >= 3);
        void set_point(const point_type& point) requires (Traits::dimension >= 3);

        vector_type normal() const requires (Traits::dimension >= 3);
        void set_normal(const vector_type& normal) requires (Traits::dimension >= 3);

        // Returns -1, 0, 1: interior, boundary, exterior
        // Exterior is defined by the direction of the normal
        int compare(const point_type& point) const override;

    private:

        point_type m_point;
        vector_type m_normal;
    };

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, Plane<Traits> plane);

} // namespace quetzal::geometry

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Plane<Traits>::Plane(const point_type& point, const vector_type& normal) requires (Traits::dimension >= 3) :
    Partition<Traits>(),
    m_point(point),
    m_normal(normal)
{
    assert(vector_unit(normal));
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Plane<Traits>::point_type quetzal::geometry::Plane<Traits>::point() const requires (Traits::dimension >= 3)
{
    return m_point;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Plane<Traits>::set_point(const point_type& point) requires (Traits::dimension >= 3)
{
    m_point = point;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Plane<Traits>::vector_type quetzal::geometry::Plane<Traits>::normal() const requires (Traits::dimension >= 3)
{
    return m_normal;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Plane<Traits>::set_normal(const vector_type& normal) requires (Traits::dimension >= 3)
{
    assert(vector_unit(normal));
    m_normal = normal;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
int quetzal::geometry::Plane<Traits>::compare(const point_type& point) const
{
    if constexpr(Traits::dimension == 2)
    {
        return -1;
    }
    else if constexpr(Traits::dimension >= 3)
    {
        auto d = dot(m_normal, (point - m_point));
        return math::float_eq0(d) ? -1 : 1;
    }
}

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, Plane<Traits> plane)
{
    os << "[" << plane.point() << "], [" << plane.normal() << "]";
    return os;
}

#endif // QUETZAL_GEOMETRY_PLANE_HPP
