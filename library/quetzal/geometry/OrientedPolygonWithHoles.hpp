#if !defined(QUETZAL_GEOMETRY_ORIENTEDPOLYGONWITHHOLES_HPP)
#define QUETZAL_GEOMETRY_ORIENTEDPOLYGONWITHHOLES_HPP
//------------------------------------------------------------------------------
// geometry
// OrientedPolygonWithHoles.hpp
//------------------------------------------------------------------------------

#include "Orientation.hpp"
#include "PolygonWithHoles.hpp"
#include <filesystem>

namespace quetzal
{

namespace geometry
{

    // OrientedPolygonWithHoles is a PolygonWithHoles with added Orientation
    // Orientation should provide normal and right vectors

    //--------------------------------------------------------------------------
    template<typename Traits>
    class OrientedPolygonWithHoles : public PolygonWithHoles<Traits>
    {
    public:

        using size_type = typename Traits::size_type;

        OrientedPolygonWithHoles() = default;
        explicit OrientedPolygonWithHoles(const PolygonWithHoles<Traits>& polygon, Orientation<Traits> orientation = Orientation<Traits>());
        OrientedPolygonWithHoles(const OrientedPolygonWithHoles&) = default;
        ~OrientedPolygonWithHoles() = default;

        OrientedPolygonWithHoles& operator=(const OrientedPolygonWithHoles&) = default;
        OrientedPolygonWithHoles& operator=(OrientedPolygonWithHoles&&) = default;

		// Not orientation, but just normal, orientation should be build by client ...
        // OrientedPolygonWithHoles with orientation should then be separate composite class ...
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
quetzal::geometry::OrientedPolygonWithHoles<Traits>::OrientedPolygonWithHoles(const PolygonWithHoles<Traits>& polygon, Orientation<Traits> orientation) :
    PolygonWithHoles<Traits>(polygon),
    m_orientation(orientation)
{
}

//------------------------------------------------------------------------------
template<typename Traits>
typename quetzal::geometry::Orientation<Traits> quetzal::geometry::OrientedPolygonWithHoles<Traits>::orientation() const
{
    return m_orientation;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::OrientedPolygonWithHoles<Traits>::set_orientation(const Orientation<Traits>& orientation)
{
    m_orientation = orientation;
    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::OrientedPolygonWithHoles<Traits>::clear()
{
	PolygonWithHoles<Traits>::clear();
    m_orientation.clear();
	return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::OrientedPolygonWithHoles<Traits>::load(const std::filesystem::path& filepath)
{
    // load ...
    filepath;

    return;
}

#endif // QUETZAL_GEOMETRY_ORIENTEDPOLYGONWITHHOLES_HPP
