#if !defined(QUETZAL_GEOMETRY_HALFPLANE_HPP)
#define QUETZAL_GEOMETRY_HALFPLANE_HPP
//------------------------------------------------------------------------------
// geometry
// HalfPlane.hpp
//------------------------------------------------------------------------------

#include "Partition.hpp"
#include "Line.hpp"
#include "Plane.hpp"
#include "Point.hpp"
#include "quetzal/math/Vector.hpp"
#include <iostream>
#include <cassert>

namespace quetzal::geometry
{

    // Exterior is to the left of the line relative to the line's direction vector, the line's perp direction

    //--------------------------------------------------------------------------
    template<typename Traits> requires (Traits::dimension == 2)
    class HalfPlane : public Partition<Traits>
    {
    public:

        using traits_type = Traits;
        using vector_type = math::Vector<Traits>;
        using line_type = Line<Traits>;
        using plane_type = Plane<Traits>;
        using point_type = Point<Traits>;

        HalfPlane() = default;
        HalfPlane(const line_type& line);
        HalfPlane(const HalfPlane&) = default;
        HalfPlane(HalfPlane&&) = default;
        ~HalfPlane() = default;

        HalfPlane& operator=(const HalfPlane&) = default;
        HalfPlane& operator=(HalfPlane&&) = default;

        line_type line() const;
        void set_line(const line_type& line);

        // Returns -1, 0, 1: interior, boundary, exterior
        int compare(const point_type& point) const override;

    private:

        line_type m_line;
    };

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, HalfPlane<Traits> halfplane);

} // namespace quetzal::geometry

//------------------------------------------------------------------------------
template<typename Traits> requires (Traits::dimension == 2)
quetzal::geometry::HalfPlane<Traits>::HalfPlane(const line_type& line)
    Partition<Traits>(),
    m_line(line)
{
}

//------------------------------------------------------------------------------
template<typename Traits> requires (Traits::dimension == 2)
typename quetzal::geometry::HalfPlane<Traits>::line_type quetzal::geometry::HalfPlane<Traits>::line() const
{
    return m_line;
}

//------------------------------------------------------------------------------
template<typename Traits> requires (Traits::dimension == 2)
void quetzal::geometry::HalfPlane<Traits>::set_point(const line_type& line)
{
    m_line = line;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits> requires (Traits::dimension == 2)
int quetzal::geometry::HalfPlane<Traits>::compare(const point_type& point) const
{
    auto d = dot(perp(m_line.direction(), (point - m_line.point()));
    return math::float_eq0(d) ? 0 : (d < Traits::val(0) ? -1 : 1);
}

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, HalfPlane<Traits> halfplane)
{
    os << halfplane.line();
    return os;
}

#endif // QUETZAL_GEOMETRY_HALFPLANE_HPP
