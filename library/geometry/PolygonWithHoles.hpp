#if !defined(QUETZAL_GEOMETRY_POLYGONWITHHOLES_HPP)
#define QUETZAL_GEOMETRY_POLYGONWITHHOLES_HPP
//------------------------------------------------------------------------------
// geometry
// PolygonWithHoles.hpp
//------------------------------------------------------------------------------

#include "Polygon.hpp"
#include <vector>

namespace quetzal
{

namespace geometry
{

    // PolygonWithHoles has a single exterior/enclosing Polygon and 0 or more interior/hole Polygon's

    //--------------------------------------------------------------------------
    template<typename Traits>
    class PolygonWithHoles
    {
    public:

        using value_type = typename Traits::value_type;
        using polygon_type = Polygon<Traits>;
        using hole_type = Polygon<Traits>;
        using holes_type = std::vector<hole_type>;
        using size_type = typename Traits::size_type;

        PolygonWithHoles() = default;
        PolygonWithHoles(const PolygonWithHoles&) = default;
        ~PolygonWithHoles() = default;

        PolygonWithHoles& operator=(const PolygonWithHoles&) = default;
        PolygonWithHoles& operator=(PolygonWithHoles&&) = default;

		const polygon_type& polygon() const;
		polygon_type& polygon();
        void set_polygon(const polygon_type& polygon);

		const holes_type& holes() const;
		holes_type& holes();

		const hole_type& hole(size_type n) const;
		hole_type& hole(size_type n);

        size_type hole_count() const;

		void clear();

    private:

        polygon_type m_polygon;
        holes_type m_holes;
    };

} // namespace geometry

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::geometry::PolygonWithHoles<Traits>::polygon_type& quetzal::geometry::PolygonWithHoles<Traits>::polygon() const
{
    return m_polygon;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::PolygonWithHoles<Traits>::polygon_type& quetzal::geometry::PolygonWithHoles<Traits>::polygon()
{
    return m_polygon;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PolygonWithHoles<Traits>::set_polygon(const polygon_type& polygon)
{
    m_polygon = polygon;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::geometry::PolygonWithHoles<Traits>::holes_type& quetzal::geometry::PolygonWithHoles<Traits>::holes() const
{
    return m_holes;
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::PolygonWithHoles<Traits>::holes_type& quetzal::geometry::PolygonWithHoles<Traits>::holes()
{
    return m_holes;
}

//------------------------------------------------------------------------------
template<typename Traits>
const typename quetzal::geometry::PolygonWithHoles<Traits>::hole_type& quetzal::geometry::PolygonWithHoles<Traits>::hole(size_type n) const
{
    assert(n < m_holes.size());
    return m_holes[n];
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::PolygonWithHoles<Traits>::hole_type& quetzal::geometry::PolygonWithHoles<Traits>::hole(size_type n)
{
    assert(n < m_holes.size());
    return m_holes[n];
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::PolygonWithHoles<Traits>::size_type quetzal::geometry::PolygonWithHoles<Traits>::hole_count() const
{
    return m_holes.size();
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::PolygonWithHoles<Traits>::clear()
{
    m_polygon.clear();
    m_holes.clear();
}

#endif // QUETZAL_GEOMETRY_POLYGONWITHHOLES_HPP
