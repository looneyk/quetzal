#if !defined(QUETZAL_GEOMETRY_ORIENTEDPOINT_HPP)
#define QUETZAL_GEOMETRY_ORIENTEDPOINT_HPP
//------------------------------------------------------------------------------
// geometry
// OrientedPoint.hpp
//------------------------------------------------------------------------------

#include "Orientation.hpp"
#include "Point.hpp"

namespace quetzal::geometry
{

    //--------------------------------------------------------------------------
    template<typename Traits>
    class OrientedPoint
    {
    public:

		static_assert(Traits::dimension >= 3);

        using value_type = Traits::value_type;
        using point_type = Point<Traits>;
        using orientation_type = Orientation<Traits>;

        OrientedPoint() = default;
        OrientedPoint(const point_type& position, const orientation_type& orientation);
        OrientedPoint(const OrientedPoint&) = default;
        ~OrientedPoint() = default;

        OrientedPoint& operator=(const OrientedPoint&) = default;

        point_type position() const;
        void set_position(const point_type& position);

        orientation_type orientation() const;
        void set_orientation(const orientation_type& orientation);

    private:

        point_type m_position;
        orientation_type m_orientation;
    };

} // namespace quetzal::geometry

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::OrientedPoint<Traits>::OrientedPoint(const point_type& position, const orientation_type& orientation) :
    m_position(position),
    m_orientation(orientation)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::OrientedPoint<Traits>::point_type quetzal::geometry::OrientedPoint<Traits>::position() const
{
    return m_position;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::OrientedPoint<Traits>::set_position(const point_type& position)
{
    m_position = position;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::OrientedPoint<Traits>::orientation_type quetzal::geometry::OrientedPoint<Traits>::orientation() const
{
    return m_orientation;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::OrientedPoint<Traits>::set_orientation(const orientation_type& orientation)
{
    m_orientation = orientation;
    return;
}

#endif // QUETZAL_GEOMETRY_ORIENTEDPOINT_HPP
