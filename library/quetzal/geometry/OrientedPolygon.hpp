#if !defined(QUETZAL_GEOMETRY_ORIENTEDPOLYGON_HPP)
#define QUETZAL_GEOMETRY_ORIENTEDPOLYGON_HPP
//------------------------------------------------------------------------------
// geometry
// OrientedPolygon.hpp
//------------------------------------------------------------------------------

#include "Orientation.hpp"
#include "Polygon.hpp"
#include <filesystem>

namespace quetzal
{

namespace geometry
{

    // OrientedPolygon is a Polygon with added Orientation
    // Orientation should provide normal/up and right vectors

	// Not orientation, but just normal, orientation should be build by client ...
    // OrientedPolygon with orientation should then be separate composite class ...

    //--------------------------------------------------------------------------
    template<typename Traits>
    class OrientedPolygon : public Polygon<Traits>
    {
    public:

        using size_type = typename Traits::size_type;

        OrientedPolygon() = default;
        explicit OrientedPolygon(const Polygon<Traits>& polygon, Orientation<Traits> orientation = Orientation<Traits>());
        explicit OrientedPolygon(size_type n);
        OrientedPolygon(const OrientedPolygon&) = default;
        ~OrientedPolygon() = default;

        OrientedPolygon& operator=(const OrientedPolygon&) = default;
        OrientedPolygon& operator=(OrientedPolygon&&) = default;

        Orientation<Traits> orientation() const;
        void set_orientation(const Orientation<Traits>& orientation);

		virtual void clear();

        virtual void load(const std::filesystem::path& filepath);

    private:

        Orientation<Traits> m_orientation;
    };

} // namespace geometry

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::OrientedPolygon<Traits>::OrientedPolygon(const Polygon<Traits>& polygon, Orientation<Traits> orientation) :
    Polygon<Traits>(polygon),
    m_orientation(orientation)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::OrientedPolygon<Traits>::OrientedPolygon(size_type n) :
    Polygon<Traits>(n),
    m_orientation()
{
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Orientation<Traits> quetzal::geometry::OrientedPolygon<Traits>::orientation() const
{
    return m_orientation;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::OrientedPolygon<Traits>::set_orientation(const Orientation<Traits>& orientation)
{
    m_orientation = orientation;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::OrientedPolygon<Traits>::clear()
{
	Polygon<Traits>::clear();
    m_orientation.clear();
	return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::OrientedPolygon<Traits>::load(const std::filesystem::path& filepath)
{
    // load ...
    filepath;

    return;
}

#endif // QUETZAL_GEOMETRY_ORIENTEDPOLYGON_HPP
