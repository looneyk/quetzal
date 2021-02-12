#if !defined(QUETZAL_GEOMETRY_POINTS_UTIL_HPP)
#define QUETZAL_GEOMETRY_POINTS_UTIL_HPP
//------------------------------------------------------------------------------
// geometry
// points_util.hpp
//------------------------------------------------------------------------------

#include "Points.hpp"
#include "quetzal/xml/Document.hpp"
#include "quetzal/common/string_util.hpp"
#include <filesystem>
#include <fstream>
#include <vector>

namespace quetzal
{

namespace geometry
{

    // Load a list of point positions from an XML file with a top-level points element
    template<typename Traits>
    void load_points(const std::filesystem::path& filepath, std::vector<Point<Traits>>& points);

} // namespace geometry

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename Traits>
void quetzal::geometry::load_points(const std::filesystem::path& filepath, Points<Traits>& points)
{
    static const std::string s_points("points");
    static const std::string s_dimension("dimension");
    static const std::string s_point("point");

    std::ifstream is(filepath);
    if (!is)
    {
        return;
    }

    xml::Document document;
    is >> document;

    const xml::Element& e = document.element();
    if (e.name() != s_points)
    {
        return;
    }

    const xml::Attribute* pAttribute = e.attribute(s_dimension);
    if (pAttribute == nullptr)
    {
        return;
    }

    size_t dimension = quetzal::to_type<size_t>(e.attribute_value(s_dimension));
    if (dimension != Traits::dimension) // allow for this possibility? ...
    {
        return;
    }

    for (auto p : e.elements())
    {
        const xml::Element* pe = p.get();
        assert(pe != nullptr);

        const std::string element_name = pe->name();
        if (element_name == s_point)
        {
            Point<Traits> point;
            std::istringstream iss(pe->value());
            iss >> point;
            points.push_back(point);
        }
    }

    return;
}

#endif // QUETZAL_GEOMETRY_POINTS_UTIL_HPP
