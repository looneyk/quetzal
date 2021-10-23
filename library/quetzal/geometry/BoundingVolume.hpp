#if !defined(QUETZAL_GEOMETRY_BOUNDINGVOLUME_HPP)
#define QUETZAL_GEOMETRY_BOUNDINGVOLUME_HPP
//------------------------------------------------------------------------------
// geometry
// BoundingVolume.hpp
//------------------------------------------------------------------------------

#include "Partition.hpp"
#include "Point.hpp"
#include <iostream>
#include <span>

namespace quetzal::geometry
{

    //--------------------------------------------------------------------------
    template<typename Traits>
    class BoundingVolume : public Partition<Traits>
    {
    public:

        using value_type = Traits::value_type;
        using point_type = Point<Traits>;

        BoundingVolume() = default;
        BoundingVolume(const BoundingVolume&) = default;
        BoundingVolume(BoundingVolume&&) = default;
        ~BoundingVolume() = default;

        BoundingVolume& operator=(const BoundingVolume&) = default;
        BoundingVolume& operator=(BoundingVolume&&) = default;

        virtual void clear() = 0;
        virtual void insert(const point_type& point) = 0;
        virtual void insert(std::span<point_type> points) = 0;

        // Returns -1, 0, 1: interior, boundary, exterior
        virtual int compare(const point_type& point) const = 0;

        virtual void print(std::ostream& os) const = 0;
    };

    template<typename Traits>
    std::ostream& operator<<(std::ostream& os, const BoundingVolume<Traits>& bv);

} // namespace quetzal::geometry

//------------------------------------------------------------------------------
template<typename Traits>
std::ostream& quetzal::geometry::operator<<(std::ostream& os, const BoundingVolume<Traits>& bv)
{
    bv.print(os);
    return os;
}

#endif // QUETZAL_GEOMETRY_BOUNDINGVOLUME_HPP
