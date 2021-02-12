#if !defined(QUETZAL_MODEL_GEOMETRY_HPP)
#define QUETZAL_MODEL_GEOMETRY_HPP
//------------------------------------------------------------------------------
// model
// geometry.hpp
//------------------------------------------------------------------------------

#include "quetzal/geometry/Polygon.hpp"
#include "quetzal/geometry/Polyline.hpp"
#include "quetzal/math/Interval.hpp"
#include "quetzal/math/math_util.hpp"

namespace quetzal
{

namespace model
{

    // Create figures in the xy-plane with ccw winding order relative to actual or effective z-axis

    template<typename Traits>
    geometry::Polygon<Traits> create_square(typename Traits::value_type side, math::Vector<Traits> position = math::Vector<Traits>());

    template<typename Traits>
    geometry::Polygon<Traits> create_rectangle(typename Traits::value_type dx, typename Traits::value_type dy, math::Vector<Traits> position = math::Vector<Traits>());

    template<typename Traits>
    geometry::Polygon<Traits> create_rectangle(math::Vector<Traits> position0, math::Vector<Traits> position1);

    template<typename Traits>
    quetzal::geometry::Polygon<Traits> create_rectangle(math::Vector<Traits> position0, math::Vector<Traits> position1);

    template<typename Traits>
    geometry::Polygon<Traits> create_circle(typename Traits::value_type radius, size_t nAzimuth, math::Vector<Traits> position = math::Vector<Traits>());

    template<typename Traits>
    geometry::Polygon<Traits> create_ellipse(typename Traits::value_type rx, typename Traits::value_type ry, size_t nAzimuth, math::Vector<Traits> position = math::Vector<Traits>());

    template<typename Traits>
    geometry::Polyline<Traits> create_circle_arc(typename Traits::value_type radius, size_t nAzimuth, math::Interval<typename Traits::value_type> azimuth, math::Vector<Traits> position = math::Vector<Traits>());

    template<typename Traits>
    geometry::Polyline<Traits> create_ellipse_arc(typename Traits::value_type rx, typename Traits::value_type ry, size_t nAzimuth, math::Interval<typename Traits::value_type> azimuth, math::Vector<Traits> position = math::Vector<Traits>());

    // curve ...

    template<typename Traits>
    void reverse(geometry::Polygon<Traits>& polygon);

    template<typename Traits>
    void translate(geometry::Polygon<Traits>& polygon, math::Vector<Traits> offset);

} // namespace model

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polygon<Traits> quetzal::model::create_square(typename Traits::value_type side, math::Vector<Traits> position)
{
    return create_rectangle(side, side, position);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polygon<Traits> quetzal::model::create_rectangle(typename Traits::value_type dx, typename Traits::value_type dy, math::Vector<Traits> position)
{
    typename Traits::value_type dxHalf = dx / Traits::val(2);
    typename Traits::value_type dyHalf = dy / Traits::val(2);

    math::Vector<Traits> position0;
    math::Vector<Traits> position1;

    position0.set_x(position.x() - dxHalf);
    position0.set_y(position.y() - dyHalf);

    position1.set_x(position.x() + dxHalf);
    position1.set_y(position.y() + dyHalf);

    if constexpr (Traits::dimension == 3)
    {
        position0.set_z(position.z());
        position1.set_z(position.z());
    }

    return create_rectangle(position0, position1);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polygon<Traits> quetzal::model::create_rectangle(math::Vector<Traits> position0, math::Vector<Traits> position1)
{
    geometry::Polygon<Traits> polygon(4);

    polygon.vertex(0).set_x(position0.x());
    polygon.vertex(0).set_y(position0.y()),
    polygon.vertex(1).set_x(position1.x());
    polygon.vertex(1).set_y(position0.y());
    polygon.vertex(2).set_x(position1.x());
    polygon.vertex(2).set_y(position1.y());
    polygon.vertex(3).set_x(position0.x());
    polygon.vertex(3).set_y(position1.y());

    return polygon;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polygon<Traits> quetzal::model::create_circle(typename Traits::value_type radius, size_t nAzimuth, math::Vector<Traits> position)
{
    return create_ellipse(radius, radius, nAzimuth, position);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polygon<Traits> quetzal::model::create_ellipse(typename Traits::value_type rx, typename Traits::value_type ry, size_t nAzimuth, math::Vector<Traits> position)
{
    geometry::Polygon<Traits> polygon(nAzimuth);

    for (size_t i = 0; i < nAzimuth; ++i)
    {
        typename Traits::value_type t = Traits::val(i) / Traits::val(nAzimuth);
        typename Traits::value_type phi = math::PiTwo<typename Traits::value_type> * t;
        polygon.vertex(i).set_x(position.x() + rx * cos(phi));
        polygon.vertex(i).set_y(position.y() + ry * sin(phi));
        if constexpr (Traits::dimension == 3)
        {
            polygon.vertex(i).set_z(position.z());
        }
    }

    return polygon;
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polyline<Traits> quetzal::model::create_circle_arc(typename Traits::value_type radius, size_t nAzimuth, math::Interval<typename Traits::value_type> azimuth, math::Vector<Traits> position)
{
    return create_ellipse_arc(radius, radius, nAzimuth, azimuth, position);
}

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polyline<Traits> quetzal::model::create_ellipse_arc(typename Traits::value_type rx, typename Traits::value_type ry, size_t nAzimuth, math::Interval<typename Traits::value_type> azimuth, math::Vector<Traits> position)
{
    geometry::Polygon<Traits> polygon(nAzimuth);

    for (size_t i = 0; i < nAzimuth; ++i)
    {
        typename Traits::value_type t = Traits::val(i) / Traits::val(nAzimuth);
        typename Traits::value_type phi = math::PiTwo<typename Traits::value_type> * azimuth.lerp(t);
        polygon.vertex(i).set_x(position.x() + rx * cos(phi));
        polygon.vertex(i).set_y(position.y() + ry * sin(phi));
        if constexpr (Traits::dimension == 3)
        {
            polygon.vertex(i).set_z(position.z());
        }
    }

    return polygon;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::model::reverse(quetzal::geometry::Polygon<Traits>& polygon)
{
    size_t n = polygon.vertex_count();
    size_t nHalf = n / 2;
    for (size_t i = 0; i < nHalf; ++i)
    {
        std::swap(polygon.vertices()[i], polygon.vertices()[n - 1 - i]);
    }

	return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::model::translate(quetzal::geometry::Polygon<Traits>& polygon, math::Vector<Traits> offset)
{
    for (auto& point : polygon.vertices())
    {
        point += offset;
    }

	return;
}

#endif // QUETZAL_MODEL_GEOMETRY_HPP
