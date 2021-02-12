#if !defined(QUETZAL_GEOMETRY_BOX_HPP)
#define QUETZAL_GEOMETRY_BOX_HPP
//------------------------------------------------------------------------------
// geometry
// Box.hpp
//------------------------------------------------------------------------------

#include "Point.hpp"
#include "quetzal/math/floating_point.hpp"
#include <iostream>

namespace quetzal
{

namespace geometry
{

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Box
    {
    public:

        using value_type = typename Traits::value_type;
        using point_type = Point<Traits>;

        Box() = default;
        Box(const point_type& pointLower, const point_type& pointUpper);
        Box(const Box&) = default;
        ~Box() = default;

        Box& operator=(const Box&) = default;

        point_type lower() const;
        point_type upper() const;

        void set_lower(const point_type& point) const;
        void set_upper(const point_type& point) const;
        void set_bounds(const point_type& pointLower, const point_type& pointUpper);

        // Closed interval test
        bool contains(const point_type& point);

    private:

        void order();

        point_type m_pointLower;
        point_type m_pointUpper;
    };

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, const Box<Traits>& box);

} // namespace geometry

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Box<Traits>::Box(const point_type& pointLower, const point_type& pointUpper) :
    m_pointLower(pointLower),
    m_pointUpper(pointUpper)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Box<Traits>::point_type quetzal::geometry::Box<Traits>::lower() const
{
    return m_pointLower;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Box<Traits>::point_type quetzal::geometry::Box<Traits>::upper() const
{
    return m_pointUpper;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Box<Traits>::set_lower(const point_type& point)
{
    m_pointLower = point;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Box<Traits>::set_upper(const point_type& point)
{
    m_pointUpper = point;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Box<Traits>::set_bounds(const point_type& pointLower, const point_type& pointUpper)
{
    m_pointLower = pointLower;
    m_pointUpper = pointUpper;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Box<Traits>::contains(const point_type& point)
{
    order();

    for (size_t i = 0; i < Traits::dimension; ++i)
    {
        if constexpr (std::is_floating_point_v<value_type>)
        {
            if (float_lt(point[i], m_pointLower[i]) || float_gt(point[i], m_pointUpper[i]))
            {
                return false;
            }
        }
        else
        {
            if (point[i] < m_pointLower[i]) || point[i] > m_pointUpper[i])
            {
                return false;
            }
        }
    }

    return true;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::Box<Traits>::order()
{
    for (size_t i = 0; i < Traits::dimension; ++i)
    {
        if (m_pointLower[i] > m_pointUpper[i])
        {
            std::swap(m_pointLower[i], m_pointUpper[i]);
        }
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, const Box<Traits>& box)
{
    os << "[" << box.lower() << "] .. [" << box.upper() << "]";
    return os;
}

#endif // QUETZAL_GEOMETRY_BOX_HPP
