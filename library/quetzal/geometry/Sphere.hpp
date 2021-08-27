#if !defined(QUETZAL_GEOMETRY_SPHERE_HPP)
#define QUETZAL_GEOMETRY_SPHERE_HPP
//------------------------------------------------------------------------------
// geometry
// Sphere.hpp
//------------------------------------------------------------------------------

#include "Point.hpp"
#include <iostream>

namespace quetzal::geometry
{

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Sphere
    {
    public:

        using value_type = Traits::value_type;
        using point_type = Point<Traits>;

        Sphere() = default;
        Sphere(const point_type& center, const value_type& radius);
        Sphere(const Sphere&) = default;
        ~Sphere() = default;

        Sphere& operator=(const Sphere&) = default;

        point_type center() const;
        void set_center(const point_type& center);

        value_type radius() const;
        void set_radius(const value_type& radius);

        point_type point(value_type azimuth, value_type elevation) const;

        // Closed interval test
        bool contains(const point_type& point);

    private:

        point_type m_center;
        value_type m_radius;
    };

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, const Sphere<Traits>& sphere);

} // namespace quetzal::geometry

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Sphere<Traits>::Sphere(const point_type& center, const value_type& radius) :
    m_center(center),
    m_radius(radius)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Sphere<Traits>::point_type quetzal::geometry::Sphere<Traits>::center() const
{
    return m_center;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Sphere<Traits>::set_center(const point_type& center)
{
    m_center = center;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Sphere<Traits>::value_type quetzal::geometry::Sphere<Traits>::radius() const
{
    return m_radius;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Sphere<Traits>::set_radius(const value_type& radius)
{
    m_radius = radius;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Sphere<Traits>::point_type quetzal::geometry::Sphere<Traits>::point(value_type azimuth, value_type elevation) const
{
    return {m_radius * cos(elevation) * cos(azimuth), m_radius * cos(elevation) * sin(azimuth), m_radius * sin(elevation)};
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Sphere<Traits>::contains(const point_type& point)
{
    return float_le((point - m_center).norm_squared(), m_radius * m_radius);
}

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, const Sphere<Traits>& sphere)
{
    os << "[" << sphere.center() << "], " << sphere.radius();
    return os;
}

#endif // QUETZAL_GEOMETRY_SPHERE_HPP
