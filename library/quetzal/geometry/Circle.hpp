#if !defined(QUETZAL_GEOMETRY_CIRCLE_HPP)
#define QUETZAL_GEOMETRY_CIRCLE_HPP
//------------------------------------------------------------------------------
// geometry
// Circle.hpp
//------------------------------------------------------------------------------

#include "Point.hpp"
#include <iostream>

namespace quetzal
{

namespace geometry
{

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Circle
    {
    public:

        using value_type = typename Traits::value_type;
        using point_type = Point<Traits>;

        Circle() = default;
        Circle(const point_type& point, const value_type& radius);
        Circle(const Circle&) = default;
        ~Circle() = default;

        Circle& operator=(const Circle&) = default;

        point_type point() const;
        void set_point(const point_type& point);

        value_type radius() const;
        void set_radius(const value_type& radius);

        point_type point(value_type r, value_type phi) const;

        // Closed interval test
        bool contains(const point_type& point);

    private:

        point_type m_point;
        value_type m_radius;
    };

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, const Circle<Traits>& circle);

} // namespace geometry

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Circle<Traits>::Circle(const point_type& point, const value_type& radius) :
    m_point(point),
    m_radius(radius)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Circle<Traits>::point_type quetzal::geometry::Circle<Traits>::point() const
{
    return m_point;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Circle<Traits>::set_point(const point_type& point)
{
    m_point = point;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Circle<Traits>::value_type quetzal::geometry::Circle<Traits>::radius() const
{
    return m_radius;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Circle<Traits>::set_radius(const value_type& radius)
{
    m_radius = radius;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Circle<Traits>::point_type quetzal::geometry::Circle<Traits>::point(value_type r, value_type phi) const
{
    assert(float_le(abs(r), m_radius));

    if constexpr (Traits::dimension == 3)
    {
        return {r * cos(phi), r * sin(phi), value_type(0)};
    }
    else if constexpr (Traits::dimension == 2)
    {
        return {r * cos(phi), r * sin(phi)};
    }
    else
    {
        static_assert(false, "Dimension out of range.");
    }
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Circle<Traits>::contains(const point_type& point)
{
    return float_le((point - m_point).norm_squared(), m_radius * m_radius);
}

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, const Circle<Traits>& circle)
{
    os << "[" << circle.point() << "], " << circle.radius();
    return os;
}

#endif // QUETZAL_GEOMETRY_CIRCLE_HPP
