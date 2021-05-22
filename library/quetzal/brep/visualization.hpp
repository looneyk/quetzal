#if !defined(QUETZAL_BREP_VISUALIZATION_HPP)
#define QUETZAL_BREP_VISUALIZATION_HPP
//------------------------------------------------------------------------------
// brep
// visualization.hpp
//------------------------------------------------------------------------------

#include "id.hpp"
#include "quetzal/math/Matrix.hpp"
#include "quetzal/svg/Document.hpp"
#include <iomanip>
#include <sstream>

namespace quetzal::brep
{

    template<typename M>
    void draw_surface(const std::string& filename, const typename M::surface_type& surface, const math::Matrix<typename M::value_type>& matrix);

    template<typename M>
    void draw_surface(svg::Document& document, const typename M::surface_type& surface, const math::Matrix<typename M::value_type>& matrix);

    template<typename M>
    void draw_face(const std::string& filename, const typename M::face_type& face, const math::Matrix<typename M::value_type>& matrix);

    template<typename M>
    void draw_face(svg::Document& document, const typename M::face_type& face, const math::Matrix<typename M::value_type>& matrix);

    template<typename M>
    void draw_border(const std::string& filename, const M& mesh, id_type idHalfedge, const math::Matrix<typename M::value_type>& matrix);

    template<typename M>
    void draw_border(svg::Document& document, const M& mesh, id_type idHalfedge, const math::Matrix<typename M::value_type>& matrix);

    template<typename M>
    void draw_halfedge(svg::Document& document, const typename M::halfedge_type& halfedge, const math::Matrix<typename M::value_type>& matrix);

    template<typename M>
    void draw_missing_halfedge(svg::Document& document, const typename M::halfedge_type& halfedge, const math::Matrix<typename M::value_type>& matrix);

    template<typename M>
    void draw_edge(svg::Document& document, const typename M::halfedge_type& halfedge, const math::Matrix<typename M::value_type>& matrix);

} // namespace quetzal::brep

//--------------------------------------------------------------------------
template<typename M>
void quetzal::brep::draw_surface(const std::string& filename, const typename M::surface_type& surface, const math::Matrix<typename M::value_type>& matrix)
{
    // ...

    return;
}

//--------------------------------------------------------------------------
template<typename M>
void quetzal::brep::draw_surface(svg::Document& document, const typename M::surface_type& surface, const math::Matrix<typename M::value_type>& matrix)
{
    return;
}

//--------------------------------------------------------------------------
template<typename M>
void quetzal::brep::draw_face(const std::string& filename, const typename M::face_type& face, const math::Matrix<typename M::value_type>& matrix)
{
    svg::Document document({0.15, 0.15});
    document.set_viewport({800.0, 800.0});
if (face.halfedge_count() > 20)
document.set_viewport({8000.0, 8000.0});
//    document.open_symbol("vertex", {2.0, 2.0}, xml::Attribute("viewBox", "-1 -1 2 2"));
//    document.circle({0.0, 0.0}, 1.0, xml::Attribute("style", "stroke:black;stroke-width:2;fill:none;vector-effect:non-scaling-stroke;"));
//    document.close_symbol();
    draw_face<M>(document, face, matrix);
    document.write(filename);
    return;
}

//--------------------------------------------------------------------------
template<typename M>
void quetzal::brep::draw_face(svg::Document& document, const typename M::face_type& face, const math::Matrix<typename M::value_type>& matrix)
{
    for (const auto& halfedge : face.halfedges())
    {
        draw_halfedge<M>(document, halfedge, matrix);

        if (!halfedge.border())
        {
            draw_halfedge<M>(document, halfedge.partner(), matrix);
        }
        else
        {
            draw_missing_halfedge<M>(document, halfedge, matrix);
        }

        draw_edge<M>(document, halfedge, matrix);
    }

    return;
}

//--------------------------------------------------------------------------
template<typename M>
void quetzal::brep::draw_border(const std::string& filename, const M& mesh, id_type idHalfedge, const math::Matrix<typename M::value_type>& matrix)
{
    svg::Document document({0.15, 0.15});
    document.set_viewport({800.0, 800.0});
if (border_edge_count(mesh, idHalfedge) > 20)
document.set_viewport({8000.0, 8000.0});
//    document.open_symbol("vertex", {2.0, 2.0}, xml::Attribute("viewBox", "-1 -1 2 2"));
//    document.circle({0.0, 0.0}, 1.0, xml::Attribute("style", "stroke:black;stroke-width:2;fill:none;vector-effect:non-scaling-stroke;"));
//    document.close_symbol();
    draw_border<M>(document, mesh, idHalfedge, matrix);
    document.write(filename);
    return;
}

//--------------------------------------------------------------------------
template<typename M>
void quetzal::brep::draw_border(svg::Document& document, const M& mesh, id_type idHalfedge, const math::Matrix<typename M::value_type>& matrix)
{
    id_type id = idHalfedge;
    do
    {
        const auto& halfedge = mesh.halfedge(id);

        draw_halfedge<M>(document, halfedge, matrix);
        draw_missing_halfedge<M>(document, halfedge, matrix);
        draw_edge<M>(document, halfedge, matrix);

        id = next_border_halfedge(mesh, id);
    } while (id != idHalfedge);

    return;
}

//--------------------------------------------------------------------------
template<typename M>
double fold_factor(const typename M::halfedge_type& halfedge, double fMin, double fMax)
{
    double angle = 0.0;
    if (halfedge.border())
    {
        // ...
        angle = quetzal::math::PiHalf<double>; // ...
    }
    else
    {
        // ...
    }

    assert(angle >= 0.0);
    assert(angle <= quetzal::math::PiTwo<double>);

    if (angle > quetzal::math::Pi<double>)
    {
        return fMax;
    }

    return fMin + (angle / quetzal::math::Pi<double>) * (fMax - fMin);
}

//--------------------------------------------------------------------------
template<typename M>
void quetzal::brep::draw_halfedge(svg::Document& document, const typename M::halfedge_type& halfedge, const math::Matrix<typename M::value_type>& matrix)
{
    using point_type = typename svg::Document::point_type;
    using vector_type = typename svg::Document::vector_type;

    typename M::point_type position0 = halfedge.attributes().position();
    typename M::point_type position1 = halfedge.next().attributes().position();

    std::ostringstream oss;
    oss << halfedge.id() << " (" << std::fixed << std::setprecision(2) << position0.x() << ", " << std::fixed << std::setprecision(2) << position0.y() << ", " << std::fixed << std::setprecision(2) << position0.z() << ")";
    std::string coordinates = oss.str();

    position0 *= matrix;
    point_type p0 = {position0.x(), position0.y()}; // position0.z() should be 0 ...
    position1 *= matrix;
    point_type p1 = {position1.x(), position1.y()}; // position1.z() should be 0 ...

    vector_type v = p1 - p0;
    vector_type uForward = normalize(v);
    vector_type uRight = {uForward.y(), -uForward.x()};

    p0 += uForward * fold_factor<M>(halfedge, 0.25, 0.1); // factor should range from .25 to .1 for angles from 0 to >=pi for start angle ...
    p1 -= uForward * fold_factor<M>(halfedge, 0.25, 0.1); // factor should range from .25 to .1 for angles from 0 to >=pi for start angle ...

    p0 += uRight * 0.05;
    p1 += uRight * 0.05;
    point_type p2 = p1 + 0.05 * uRight - 0.1 * uForward;

    document.line(p0, p1, xml::Attribute("style", "stroke:black;vector-effect:non-scaling-stroke;"));
    document.line(p1, p2, xml::Attribute("style", "stroke:black;vector-effect:non-scaling-stroke;"));

    p0 += uRight * 0.05;
    p1 = p0 + uForward * 20.0;

    oss.str("");
    oss << "M " << p0.x() << " " << p0.y() << " L " << p1.x() << " " << p1.y();
    document.text_path(coordinates, xml::Attribute("path", oss.str()), xml::Attribute("font-size", "0.1"));

    // add validation failure codes here too ...

    return;
}

//--------------------------------------------------------------------------
template<typename M>
void quetzal::brep::draw_missing_halfedge(svg::Document& document, const typename M::halfedge_type& halfedge, const math::Matrix<typename M::value_type>& matrix)
{
    using value_type = typename svg::Document::value_type;
    using point_type = typename svg::Document::point_type;
    using vector_type = typename svg::Document::vector_type;

    typename M::point_type position0 = halfedge.attributes().position() * matrix;
    typename M::point_type position1 = halfedge.next().attributes().position() * matrix;

    point_type p0 = {position0.x(), position0.y()}; // position0.z() should be 0 ...
    point_type p1 = {position1.x(), position1.y()}; // position1.z() should be 0 ...

    vector_type v = p1 - p0;
    vector_type uForward = normalize(v);
    vector_type uRight = {uForward.y(), -uForward.x()};

    point_type position = p0 + v / 2.0 - uRight * 0.05;
    document.line(position + uForward * 0.25, position - uForward * 0.25, xml::Attribute("style", "stroke:black;vector-effect:non-scaling-stroke;"));

    return;
}

//--------------------------------------------------------------------------
template<typename M>
void quetzal::brep::draw_edge(svg::Document& document, const typename M::halfedge_type& halfedge, const math::Matrix<typename M::value_type>& matrix)
{
    using value_type = typename svg::Document::value_type;
    using point_type = typename svg::Document::point_type;

    typename M::point_type position0 = halfedge.attributes().position() * matrix;
    typename M::point_type position1 = halfedge.next().attributes().position() * matrix;

    point_type p0 = {position0.x(), position0.y()}; // position0.z() should be 0 ...
    point_type p1 = {position1.x(), position1.y()}; // position1.z() should be 0 ...

    document.circle(p0, 0.02, xml::Attribute("style", "stroke:black;stroke-width:2;fill:none;vector-effect:non-scaling-stroke;"));
//    document.use("#vertex", p0, {0.04, 0.04});
    document.line(p0, p1, xml::Attribute("style", "stroke:black;stroke-dasharray:4;vector-effect:non-scaling-stroke;"));
    return;
}

#endif // QUETZAL_BREP_VISUALIZATION_HPP
                                    