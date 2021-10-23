#if !defined(QUETZAL_GEOMETRY_HALFSPACE_HPP)
#define QUETZAL_GEOMETRY_HALFSPACE_HPP
//------------------------------------------------------------------------------
// geometry
// HalfSpace.hpp
//------------------------------------------------------------------------------

#include "Partition.hpp"
#include "Plane.hpp"
#include "Point.hpp"
#include "quetzal/math/Vector.hpp"
#include <iostream>
#include <cassert>

namespace quetzal::geometry
{

    //--------------------------------------------------------------------------
    template<typename Traits> requires (Traits::dimension >= 3)
    class HalfSpace : public Partition<Traits>
    {
    public:

        using traits_type = Traits;
        using vector_type = math::Vector<Traits>;
        using plane_type = Plane<Traits>;
        using point_type = Point<Traits>;

        HalfSpace() = default;
        HalfSpace(const plane_type& plane);
        HalfSpace(const HalfSpace&) = default;
        HalfSpace(HalfSpace&&) = default;
        ~HalfSpace() = default;

        HalfSpace& operator=(const HalfSpace&) = default;
        HalfSpace& operator=(HalfSpace&&) = default;

        plane_type plane() const;
        void set_plane(const plane_type& plane);

        // Returns -1, 0, 1: interior, boundary, exterior
        int compare(const point_type& point) const override;

    private:

        plane_type m_plane;
    };

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, HalfSpace<Traits> plane);

} // namespace quetzal::geometry

//------------------------------------------------------------------------------
template<typename Traits> requires (Traits::dimension >= 3)
quetzal::geometry::HalfSpace<Traits>::HalfSpace(const plane_type& plane) :
    Partition<Traits>(),
    m_plane(plane)
{
}

//------------------------------------------------------------------------------
template<typename Traits> requires (Traits::dimension >= 3)
typename quetzal::geometry::HalfSpace<Traits>::plane_type quetzal::geometry::HalfSpace<Traits>::plane() const
{
    return m_plane;
}

//------------------------------------------------------------------------------
template<typename Traits> requires (Traits::dimension >= 3)
void quetzal::geometry::HalfSpace<Traits>::set_plane(const plane_type& plane)
{
    m_plane = plane;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits> requires (Traits::dimension >= 3)
int quetzal::geometry::HalfSpace<Traits>::compare(const point_type& point) const
{
    static_assert(Traits::dimension >= 3);

    auto d = dot(plane().normal(), (point - plane().point()));
    return math::float_eq0(d) ? 0 : (d < Traits::val(0) ? -1 : 1);
}

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, HalfSpace<Traits> halfspace)
{
    os << halfspace.plane();
    return os;
}

#endif // QUETZAL_GEOMETRY_HALFSPACE_HPP
