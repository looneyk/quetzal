#if !defined(QUETZAL_GEOMETRY_AXISALIGNEDBOUNDINGBOX_HPP)
#define QUETZAL_GEOMETRY_AXISALIGNEDBOUNDINGBOX_HPP
//------------------------------------------------------------------------------
// geometry
// AxisAlignedBoundingBox.hpp
//------------------------------------------------------------------------------

#include "BoundingVolume.hpp"
#include "quetzal/math/Vector.hpp"
#include "quetzal/math/floating_point.hpp"
#include <iostream>
#include <cassert>

namespace quetzal::geometry
{

    //--------------------------------------------------------------------------
    template<typename Traits>
    class AxisAlignedBoundingBox : public BoundingVolume<Traits>
    {
    public:

        using typename BoundingVolume<Traits>::value_type;
        using typename BoundingVolume<Traits>::point_type;
        using vector_type = math::Vector<Traits>;

        AxisAlignedBoundingBox();
        AxisAlignedBoundingBox(const point_type& pointLower, const point_type& pointUpper);
        AxisAlignedBoundingBox(std::span<point_type> points);
        AxisAlignedBoundingBox(const AxisAlignedBoundingBox&) = default;
        AxisAlignedBoundingBox(AxisAlignedBoundingBox&&) = default;
        ~AxisAlignedBoundingBox() = default;

        AxisAlignedBoundingBox& operator=(const AxisAlignedBoundingBox&) = default;
        AxisAlignedBoundingBox& operator=(AxisAlignedBoundingBox&&) = default;

        point_type lower() const;
        point_type upper() const;
        vector_type extent() const; // This is the full first octant vector extent, not relative from the center as is done in direct3d11, and not necessarily semantically equivalent to other usages ...

        void set_lower(const point_type& point);
        void set_upper(const point_type& point);
        void set_bounds(const point_type& pointLower, const point_type& pointUpper);

        void clear() override;
        void insert(const point_type& point) override;
        void insert(std::span<point_type> points) override;

        // Returns -1, 0, 1: interior, boundary, exterior
        int compare(const point_type& point) const override;

        void print(std::ostream& os) const override;

    private:

        bool disjoint() const;
        bool ordered() const;
        void order();

//        std::array<point_type, 2> m_points; // 0 - min, 1 - max ...
        point_type m_pointLower;
        point_type m_pointUpper;
    };

} // namespace quetzal::geometry

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::AxisAlignedBoundingBox<Traits>::AxisAlignedBoundingBox() :
    AxisAlignedBoundingBox(point_type::max(), point_type::min())
{
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::AxisAlignedBoundingBox<Traits>::AxisAlignedBoundingBox(const point_type& pointLower, const point_type& pointUpper) :
    m_pointLower(pointLower),
    m_pointUpper(pointUpper)
{
    order();
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::AxisAlignedBoundingBox<Traits>::AxisAlignedBoundingBox(std::span<point_type> points) :
    AxisAlignedBoundingBox()
{
    insert(points);
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::AxisAlignedBoundingBox<Traits>::point_type quetzal::geometry::AxisAlignedBoundingBox<Traits>::lower() const
{
    return m_pointLower;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::AxisAlignedBoundingBox<Traits>::point_type quetzal::geometry::AxisAlignedBoundingBox<Traits>::upper() const
{
    return m_pointUpper;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::AxisAlignedBoundingBox<Traits>::vector_type quetzal::geometry::AxisAlignedBoundingBox<Traits>::extent() const
{
    return m_pointUpper - m_pointLower;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::AxisAlignedBoundingBox<Traits>::set_lower(const point_type& point)
{
    m_pointLower = point;
    order();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::AxisAlignedBoundingBox<Traits>::set_upper(const point_type& point)
{
    m_pointUpper = point;
    order();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::AxisAlignedBoundingBox<Traits>::set_bounds(const point_type& pointLower, const point_type& pointUpper)
{
    m_pointLower = pointLower;
    m_pointUpper = pointUpper;
    order();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::AxisAlignedBoundingBox<Traits>::clear()
{
    m_pointLower = point_type::max();
    m_pointUpper = point_type::min();
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::AxisAlignedBoundingBox<Traits>::insert(const point_type& point)
{
    assert(ordered());

    m_pointLower = min(m_pointLower, point);
    m_pointUpper = max(m_pointUpper, point);
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::AxisAlignedBoundingBox<Traits>::insert(std::span<point_type> points)
{
    assert(ordered());

    for (const auto& point : points)
    {
        insert(point);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
int quetzal::geometry::AxisAlignedBoundingBox<Traits>::compare(const point_type& point) const
{
    assert(ordered());

    int result = -1;

    for (size_t i = 0; i < Traits::dimension; ++i)
    {
        if constexpr (std::is_floating_point_v<Traits::value_type>)
        {
            if (math::float_lt(point[i], m_pointLower[i]) || math::float_gt(point[i], m_pointUpper[i]))
            {
                return 1;
            }

            if (math::float_eq(point[i], m_pointLower[i]) || math::float_eq(point[i], m_pointUpper[i]))
            {
                result = 0;
            }
        }
        else
        {
            if (point[i] < m_pointLower[i] || point[i] > m_pointUpper[i])
            {
                return 1;
            }

            if (point[i] == m_pointLower[i] || point[i] == m_pointUpper[i])
            {
                result = 0;
            }
        }
    }

    return result;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::AxisAlignedBoundingBox<Traits>::print(std::ostream& os) const
{
    os << "[" << m_pointLower << "] .. [" << m_pointUpper << "]";
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::AxisAlignedBoundingBox<Traits>::disjoint() const
{
    return m_pointLower == point_type::max() && m_pointUpper == point_type::min();
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::AxisAlignedBoundingBox<Traits>::ordered() const
{
    if (disjoint())
    {
        return true;
    }

    for (size_t i = 0; i < Traits::dimension; ++i)
    {
        if (m_pointLower[i] > m_pointUpper[i])
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::AxisAlignedBoundingBox<Traits>::order()
{
    if (disjoint())
    {
        return;
    }

    for (size_t i = 0; i < Traits::dimension; ++i)
    {
        if (m_pointLower[i] > m_pointUpper[i])
        {
            std::swap(m_pointLower[i], m_pointUpper[i]);
        }
    }

    return;
}

#endif // QUETZAL_GEOMETRY_AXISALIGNEDBOUNDINGBOX_HPP
