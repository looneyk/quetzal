#if !defined(QUETZAL_GEOMETRY_BOUNDINGSPHERE_HPP)
#define QUETZAL_GEOMETRY_BOUNDINGSPHERE_HPP
//------------------------------------------------------------------------------
// geometry
// BoundingSphere.hpp
//------------------------------------------------------------------------------

#include "BoundingVolume.hpp"
#include "quetzal/math/Vector.hpp"
#include "quetzal/math/floating_point.hpp"
#include <iostream>
#include <limits>

namespace quetzal::geometry
{

    //--------------------------------------------------------------------------
    template<typename Traits>
    class BoundingSphere : public BoundingVolume<Traits>
    {
    public:

        using typename BoundingVolume<Traits>::value_type;
        using typename BoundingVolume<Traits>::point_type;
        using vector_type = math::Vector<Traits>;

        BoundingSphere();
        BoundingSphere(const point_type& center, value_type radius);
        BoundingSphere(std::span<point_type> points);
        BoundingSphere(const BoundingSphere&) = default;
        BoundingSphere(BoundingSphere&&) = default;
        ~BoundingSphere() = default;

        BoundingSphere& operator=(const BoundingSphere&) = default;
        BoundingSphere& operator=(BoundingSphere&&) = default;

        point_type center() const;
        value_type radius() const;

        void set_center(const point_type& center);
        void set_radius(value_type radius);
        void set_bounds(const point_type& center, value_type radius);

        void clear() override;
        void insert(const point_type& point) override;
        void insert(std::span<point_type> points) override;

        // Returns -1, 0, 1: interior, boundary, exterior
        int compare(const point_type& point) const override;

        void print(std::ostream& os) const override;

    private:

        point_type m_center;
        value_type m_radius;
    };

} // namespace quetzal::geometry

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::BoundingSphere<Traits>::BoundingSphere() :
    BoundingSphere(point_type(), -std::numeric_limits<value_type>::max())
{
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::BoundingSphere<Traits>::BoundingSphere(const point_type& center, value_type radius) :
    m_center(center),
    m_radius(radius)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::BoundingSphere<Traits>::BoundingSphere(std::span<point_type> points) :
    BoundingSphere()
{
    insert(points);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::BoundingSphere<Traits>::point_type quetzal::geometry::BoundingSphere<Traits>::center() const
{
    return m_center;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::BoundingSphere<Traits>::value_type quetzal::geometry::BoundingSphere<Traits>::radius() const
{
    return m_radius;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::BoundingSphere<Traits>::set_center(const point_type& center)
{
    m_center = center;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::BoundingSphere<Traits>::set_radius(value_type radius)
{
    m_radius = radius;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::BoundingSphere<Traits>::set_bounds(const point_type& center, value_type radius)
{
    m_center = center;
    m_radius = radius;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::BoundingSphere<Traits>::clear()
{
    m_center.clear();
    m_radius = -std::numeric_limits<value_type>::max();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::BoundingSphere<Traits>::insert(const point_type& point)
{
// this generally won't work properly for sphere unless the whole point set is stored ...
// this is easy if the center is known ...
point;
clear(); // don't allow it to look like it might be doing this correctly ...
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::BoundingSphere<Traits>::insert(std::span<point_type> points)
{
    for (const auto& point : points)
    {
        insert(point);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
int quetzal::geometry::BoundingSphere<Traits>::compare(const point_type& point) const
{
    if (m_radius >= value_type(0))
    {
        value_type d = (point - m_center).norm_squared();
        value_type r2 = m_radius * m_radius;

        if (math::float_eq(d, r2))
        {
            return 0;
        }
        else if (d < r2)
        {
            return -1;
        }
    }

    return 1;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::BoundingSphere<Traits>::print(std::ostream& os) const
{
    os << "[" << m_center << "] " << m_radius;
    return;
}

#endif // QUETZAL_GEOMETRY_BOUNDINGSPHERE_HPP
