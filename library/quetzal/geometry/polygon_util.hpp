#if !defined(QUETZAL_GEOMETRY_POLYGON_UTIL_HPP)
#define QUETZAL_GEOMETRY_POLYGON_UTIL_HPP
//------------------------------------------------------------------------------
// geometry
// polygon_util.hpp
//------------------------------------------------------------------------------

#include "Polygon.hpp"
#include "PolygonWithHoles.hpp"
#include "quetzal/xml/Document.hpp"
#include <filesystem>
#include <fstream>
#include <vector>

#include "intersect.hpp"

namespace quetzal::geometry
{

    template<typename Traits>
    Polygon<Traits> expand(const Polygon<Traits>& polygon, typename Traits::value_type amount);

    // These functions do not clear existing polygon contents. Do this. Leaving original may make sense for points, but not for polygon ...
    // return Polygon? ...

    // Load a list of polygon vertex positions from an XML file with a top-level polygon or points element.
    template<typename Traits>
    void load_polygon(const std::filesystem::path& filepath, Polygon<Traits>& polygon);

    // Load a list of polygon vertex positions and hole polygons from an XML file with a top-level polygon element and polygon sub elements.
    template<typename Traits>
    void load_polygon(const std::filesystem::path& filepath, PolygonWithHoles<Traits>& polygon);

    // Load a list of polygon vertex positions from vertex or point sub elements.
    // Position element tag consistency is not checked (taking a list of tags is partial solution; won't ensure consistency) ...
    template<typename Traits>
    void load_polygon(const xml::Element& element, Polygon<Traits>& polygon, size_t dimension = Traits::dimension);

namespace polygon_util_internal
{

    // math_util or something, use in points_util too ...
    template<typename Traits>
    math::Vector<Traits> parse_point_value(const std::string& s, size_t dimension = Traits::dimension)
    {
        assert (dimension <= Traits::dimension);
        math::Vector<Traits> point;
        if (dimension == Traits::dimension)
        {
            std::istringstream iss(s);
            iss >> point;
        }
        else if (dimension < Traits::dimension)
        {
            assert(dimension == Traits::dimension - 1); // not general, but serves current use case ...
            math::Vector<typename Traits::reduced_traits> point_1; // Traits::reduced_traits::reduced_traits ...
            std::istringstream iss(s);
            iss >> point_1;
            for (size_t i = 0; i < dimension; ++i)
            {
                point[i] = point_1[i];
            }

            for (size_t i = dimension; i < Traits::dimension; ++i)
            {
                point[i] = Traits::val(0);
            }
        }
        return point;
    }

} // namespace polygon_util_internal

} // namespace quetzal::geometry

//------------------------------------------------------------------------------
template<typename Traits>
quetzal::geometry::Polygon<Traits> quetzal::geometry::expand(const Polygon<Traits>& a, typename Traits::value_type amount)
{
    // Assumes 2D or constant z ...

    size_t n = a.vertex_count();
    Polygon<Traits> polygon(n);

    auto offset_line = [amount](const math::Vector<Traits>& a, const math::Vector<Traits>& b) -> Line<Traits> {
        Line<Traits> line(a, normalize(b - a));
        math::Vector<Traits> right;
        right.set_x(line.direction().y());
        right.set_y(-line.direction().x());
        right.normalize();
        line.set_point(line.point() + right * amount);
        return line;
    };

    Line<Traits> linePrev = offset_line(a.vertex(n - 1), a.vertex(0));

    for (size_t i = 0; i < n; ++i)
    {
        Line<Traits> line = offset_line(a.vertex(i), a.vertex((i + 1) % n));
        Intersection<Traits> inter = intersection(linePrev, line);
        assert(inter.locus() == Locus::Point); // Handle degenerate cases, line when colinear vertices, none when angle is 0 or Pi ...
        polygon.vertex(i) = inter.point();
        linePrev = line;
    }

    return polygon;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::load_polygon(const std::filesystem::path& filepath, Polygon<Traits>& polygon)
{
    static const std::string s_polygon("polygon");
    static const std::string s_points("points");
    static const std::string s_dimension("dimension");
    static const std::string s_vertex("vertex");
    static const std::string s_point("point");

    std::ifstream is(filepath);
    if (!is)
    {
        return;
    }

    xml::Document document;
    is >> document;

    const xml::Element& e = document.element();
    if (e.name() != s_polygon && e.name() != s_points)
    {
        return;
    }

    const xml::Attribute* pAttribute = e.attribute(s_dimension);
    if (pAttribute == nullptr)
    {
        return;
    }

    size_t dimension = to_type<size_t>(e.attribute_value(s_dimension));

    load_polygon(e, polygon, dimension);

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::load_polygon(const std::filesystem::path& filepath, PolygonWithHoles<Traits>& polygons)
{
    static const std::string s_polygon("polygon");
    static const std::string s_dimension("dimension");
    static const std::string s_vertex("vertex");

    std::ifstream is(filepath);
    if (!is)
    {
        return;
    }

    xml::Document document;
    is >> document;

    const xml::Element& e = document.element();
    if (e.name() != s_polygon)
    {
        return;
    }

    const xml::Attribute* pAttribute = e.attribute(s_dimension);
    if (pAttribute == nullptr)
    {
        return;
    }

    size_t dimension = to_type<size_t>(e.attribute_value(s_dimension));

    for (auto p : e.elements())
    {
        const xml::Element* pe = p.get();
        assert(pe != nullptr);

        const std::string element_name = pe->name();
        if (element_name == s_vertex)
        {
            math::Vector<Traits> point = polygon_util_internal::parse_point_value<Traits>(pe->value(), dimension);
            polygons.polygon().vertices().push_back(point);
        }
        else if (element_name == s_polygon)
        {
            Polygon<Traits> hole;
            load_polygon(*pe, hole, dimension);
            polygons.holes().push_back(hole);
        }
    }

    return;
}

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::load_polygon(const xml::Element& element, Polygon<Traits>& polygon, size_t dimension)
{
    static const std::string s_vertex("vertex");
    static const std::string s_point("point");

    for (auto p : element.elements())
    {
        const xml::Element* pe = p.get();
        assert(pe != nullptr);

        const std::string element_name = pe->name();
        if (element_name == s_vertex || element_name == s_point)
        {
            math::Vector<Traits> point = polygon_util_internal::parse_point_value<Traits>(pe->value(), dimension);
            polygon.vertices().push_back(point);
        }
    }

    return;
}

#endif // QUETZAL_GEOMETRY_POLYGON_UTIL_HPP
