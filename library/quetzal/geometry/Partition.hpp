#if !defined(QUETZAL_GEOMETRY_PARTITION_HPP)
#define QUETZAL_GEOMETRY_PARTITION_HPP
//------------------------------------------------------------------------------
// geometry
// Partition.hpp
//------------------------------------------------------------------------------

#include "Point.hpp"

namespace quetzal::geometry
{

    //--------------------------------------------------------------------------
    template<typename Traits>
    class Partition
    {
    public:

        using point_type = Point<Traits>;

        Partition() = default;
        Partition(const Partition&) = default;
        Partition(Partition&&) = default;
        ~Partition() = default;

        Partition& operator=(const Partition&) = default;
        Partition& operator=(Partition&&) = default;

        // disjoint, touches, contains, covers ...
        bool exterior(const point_type& point) const;
        bool boundary(const point_type& point) const;
        bool interior(const point_type& point) const;
        bool contains(const point_type& point) const; // interior || boundary

        // Returns -1, 0, 1: interior, boundary, exterior
        virtual int compare(const point_type& point) const = 0;
    };

} // namespace quetzal::geometry

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Partition<Traits>::exterior(const point_type& point) const
{
    return compare(point) > 0;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Partition<Traits>::boundary(const point_type& point) const
{
    return compare(point) == 0;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Partition<Traits>::interior(const point_type& point) const
{
    return compare(point) < 0;
}

//------------------------------------------------------------------------------
template<typename Traits>
bool quetzal::geometry::Partition<Traits>::contains(const point_type& point) const
{
    return compare(point) <= 0;
}

#endif // QUETZAL_GEOMETRY_PARTITION_HPP
