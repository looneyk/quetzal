#if !defined(QUETZAL_GEOMETRY_VISUALIZATION_HPP)
#define QUETZAL_GEOMETRY_VISUALIZATION_HPP
//------------------------------------------------------------------------------
// geometry
// visualization.hpp
//------------------------------------------------------------------------------

#include "id.hpp"
#include "quetzal/math/Matrix.hpp"
#include "quetzal/svg/Document.hpp"
#include <iomanip>
#include <sstream>

namespace quetzal::geometry
{

    template<typename M>
    void draw_polygon(const std::string& filename, const geometry::Polygon<vector_traits>& polygon, const math::Matrix<typename M::value_type>& matrix);

    template<typename M>
    void draw_polygon(svg::Document& document, const geometry::Polygon<vector_traits>& polygon, const math::Matrix<typename M::value_type>& matrix);

    template<typename T>
    void draw_edge(svg::Document& document, size_t id, const vector_type& position0, const vector_type& position1, const math::Matrix<typename T::value_type>& matrix)

} // namespace quetzal::geometry

//------------------------------------------------------------------------------
template<typename T>
void draw_polygon(const std::string& filename, const geometry::Polygon<vector_traits>& polygon, const math::Matrix<typename T::value_type>& matrix)
{
    svg::Document document({0.15, 0.15});
    document.set_viewport({800.0, 800.0});
if (polygon.vertex_count() > 20)
document.set_viewport({8000.0, 8000.0});
//    document.open_symbol("vertex", {2.0, 2.0}, xml::Attribute("viewBox", "-1 -1 2 2"));
//    document.circle({0.0, 0.0}, 1.0, xml::Attribute("style", "stroke:black;stroke-width:2;fill:none;vector-effect:non-scaling-stroke;"));
//    document.close_symbol();
    draw_polygon<T>(document, polygon, matrix);
    document.write(filename);
    return;
}

//------------------------------------------------------------------------------
template<typename T>
void draw_polygon(svg::Document& document, const geometry::Polygon<vector_traits>& polygon, const math::Matrix<typename T::value_type>& matrix)
{
    for (size_t i = 1; i < polygon.vertex_count(); ++i)
    {
        draw_edge<T>(document, i, polygon.vertex(i - 1), polygon.vertex(i), matrix);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename T>
void draw_edge(svg::Document& document, size_t id, const vector_type& position0, const vector_type& position1, const math::Matrix<typename T::value_type>& matrix)
{
    using point_type = svg::Document::point_type;
    using vector_type = svg::Document::vector_type;

    std::ostringstream oss;
    oss << id << " (" << std::fixed << std::setprecision(2) << position0.x() << ", " << std::fixed << std::setprecision(2) << position0.y() << ", " << std::fixed << std::setprecision(2) << position0.z() << ")";
    std::string coordinates = oss.str();

    position0 *= matrix;
    point_type p0 = {position0.x(), position0.y()}; // position0.z() should be 0 ...
    position1 *= matrix;
    point_type p1 = {position1.x(), position1.y()}; // position1.z() should be 0 ...

    vector_type v = p1 - p0;
    vector_type uForward = normalize(v);
    vector_type uRight = {uForward.y(), -uForward.x()};

    document.line(p0, p1, xml::Attribute("style", "stroke:black;vector-effect:non-scaling-stroke;"));

    p0 += uRight * 0.05;
    p1 = p0 + uForward * 20.0;

    oss.str("");
    oss << "M " << p0.x() << " " << p0.y() << " L " << p1.x() << " " << p1.y();
    document.text_path(coordinates, xml::Attribute("path", oss.str()), xml::Attribute("font-size", "0.1"));

    return;
}

#endif // QUETZAL_GEOMETRY_VISUALIZATION_HPP
