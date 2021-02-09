#if !defined(QUETZAL_SVG_DOCUMENT_HPP)
#define QUETZAL_SVG_DOCUMENT_HPP
//------------------------------------------------------------------------------
// svg
// Document.hpp
//------------------------------------------------------------------------------

#include "Attribute.hpp"
#include "math/Vector.hpp"
#include "math/VectorTraits.hpp"
#include "xml/Document.hpp"
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <vector>

namespace quetzal
{

namespace svg
{

    //--------------------------------------------------------------------------
    class Document
    {
    public:

        using value_type = double;
        using point_type = math::Vector<math::VectorTraits<value_type, 2>>;
        using vector_type = math::Vector<math::VectorTraits<value_type, 2>>;
        using dimensions_type = math::Vector<math::VectorTraits<value_type, 2>>;

        Document();
        Document(const dimensions_type& sizeBorder);
        Document(const Document&) = delete;
        ~Document() = default;

        Document& operator=(const Document&) = delete;

        void set_viewport(const dimensions_type& size);
        void set_view_box(const point_type& position, const dimensions_type& size);
        void set_border(const dimensions_type& size); // used for automatic recalculation of view box

        void write(const std::string& filename);

        // return Element* here instead? ...
        // any need to return anything at all? not currently, except to allow for additional attributes (text args in text_path)

        template<typename... Args>
        std::shared_ptr<xml::Element> rectangle(point_type position, value_type dx, value_type dy, Args... args);

        template<typename... Args>
        std::shared_ptr<xml::Element> circle(point_type position, value_type radius, Args... args);

        template<typename... Args>
        std::shared_ptr<xml::Element> ellipse(point_type position, value_type rx, value_type ry, Args... args);

        template<typename... Args>
        std::shared_ptr<xml::Element> line(point_type p0, point_type p1, Args... args);

        template<typename... Args>
        std::shared_ptr<xml::Element> polyline(const std::vector<point_type>& points, Args... args);

        template<typename... Args>
        std::shared_ptr<xml::Element> polygon(const std::vector<point_type>& points, Args... args);

        template<typename... Args>
        std::shared_ptr<xml::Element> path(const std::string& commands, Args... args);

        // expand text elements to handle nodes instead of just a single string (interleaved text and elements) ...
        template<typename... Args>
        std::shared_ptr<xml::Element> text(point_type position, const std::string& s, Args... args);

        // as with text, but also separate args for text and textPath elements ...
        template<typename... Args>
        std::shared_ptr<xml::Element> text_path(const std::string& s, Args... args);

        template<typename... Args>
        std::shared_ptr<xml::Element> use(const std::string& href, point_type position, dimensions_type size, Args... args);

        template<typename... Args>
        std::shared_ptr<xml::Element> open_symbol(const std::string& id, dimensions_type size, Args... args);

        void close_symbol();

        template<typename... Args>
        std::shared_ptr<xml::Element> open_group(Args... args);

        void close_group();

        template<typename... Args>
        std::shared_ptr<xml::Element> open_defs(Args... args);

        void close_defs();

        template<typename... Args>
        std::shared_ptr<xml::Element> open_svg(Args... args);

        void close_svg();

        template<typename F, typename... Args>
        std::shared_ptr<xml::Element> defs(F&& f, Args&&... args);

    private:

        void set_attributes(std::shared_ptr<xml::Element> pelement); // Prameter pack recursion terminating case

        template<typename T, typename... Args>
        void set_attributes(std::shared_ptr<xml::Element> pelement, T t, Args... args);

        void update_size(point_type point);

        xml::Document m_document;
        xml::Element* m_pelement;

        bool m_bAutoViewBox;
        dimensions_type m_sizeBorder;
        point_type m_pointMin;
        point_type m_pointMax;
        bool m_bFirst;
    };

} // namespace svg

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::Document::rectangle(point_type position, value_type dx, value_type dy, Args... args)
{
    std::shared_ptr<xml::Element> pelement = m_pelement->add_element("rect");
    set_attributes(pelement, Attribute("x", position.x()), Attribute("y", position.y()), Attribute("width", dx), Attribute("height", dy), args...);
    update_size(position);
    return pelement;
}

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::Document::circle(point_type position, value_type radius, Args... args)
{
    std::shared_ptr<xml::Element> pelement = m_pelement->add_element("circle");
    set_attributes(pelement, Attribute("cx", position.x()), Attribute("cy", position.y()), Attribute("r", radius), args...);
    update_size(position);
    return pelement;
}

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::Document::ellipse(point_type position, value_type rx, value_type ry, Args... args)
{
    std::shared_ptr<xml::Element> pelement = m_pelement->add_element("ellipse");
    set_attributes(pelement, Attribute("cx", position.x()), Attribute("cy", position.y()), Attribute("rx", rx), Attribute("ry", ry), args...);
    update_size(position);
    return pelement;
}

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::Document::line(point_type p0, point_type p1, Args... args)
{
    std::shared_ptr<xml::Element> pelement = m_pelement->add_element("line");
    set_attributes(pelement, Attribute("x1", p0.x()), Attribute("y1", p0.y()), Attribute("x2", p1.x()), Attribute("y2", p1.y()), args...);
    update_size(p0);
    update_size(p1);
    return pelement;
}

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::Document::polyline(const std::vector<point_type>& points, Args... args)
{
    std::shared_ptr<xml::Element> pelement = m_pelement->add_element("polyline");
    std::ostringstream oss;
    for (point_type point : points)
    {
        if (!oss.str().empty())
        {
            oss << " ";
        }
        oss << point.x() << "," << point.y();
        update_size(point);
    }
    set_attributes(pelement, Attribute("points", oss.str()), args...);
    return pelement;
}

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::Document::polygon(const std::vector<point_type>& points, Args... args)
{
    std::shared_ptr<xml::Element> pelement = m_pelement->add_element("polygone");
    std::ostringstream oss;
    for (point_type point : points)
    {
        if (!oss.str().empty())
        {
            oss << " ";
        }
        oss << point.x() << "," << point.y();
        update_size(point);
    }
    set_attributes(pelement, Attribute("points", oss.str()), args...);
    return pelement;
}

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::Document::path(const std::string& commands, Args... args)
{
    std::shared_ptr<xml::Element> pelement = m_pelement->add_element("path");
    set_attributes(pelement, Attribute("d", commands), args...);
    return pelement;
}

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::Document::text(point_type position, const std::string& s, Args... args)
{
    std::shared_ptr<xml::Element> pelement = m_pelement->add_element("text");
    pelement->set_value(s);
    set_attributes(pelement, Attribute("x", position.x()), Attribute("y", position.y()), args...);
    update_size(position);
    return pelement;
}

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::Document::text_path(const std::string& s, Args... args)
{
    std::shared_ptr<xml::Element> pelement = m_pelement->add_element("text");
    std::shared_ptr<xml::Element> pelementPath = pelement->add_element("textPath");
    pelementPath->set_value(s);
    set_attributes(pelementPath, args...);
    return pelement;
}

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::Document::use(const std::string& href, point_type position, dimensions_type size, Args... args)
{
    std::shared_ptr<xml::Element> pelement = m_pelement->add_element("use");
    set_attributes(pelement, Attribute("href", href), Attribute("x", position.x()), Attribute("y", position.y()), Attribute("width", size.x()), Attribute("height", size.y()), args...);
    update_size(position);
    return pelement;
}

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::Document::open_symbol(const std::string& id, dimensions_type size, Args... args)
{
    std::shared_ptr<xml::Element> pelement = m_pelement->add_element("symbol");
//    set_attributes(pelement, Attribute("id", id), Attribute("width", size.x()), Attribute("height", size.y()), xml::Attribute("refX", "center"), xml::Attribute("refY", "center"), args...);
size;
    set_attributes(pelement, Attribute("id", id), xml::Attribute("refX", "center"), xml::Attribute("refY", "center"), args...);
    m_pelement = pelement.get();
    return pelement;
}

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::Document::open_group(Args... args)
{
    std::shared_ptr<xml::Element> pelement = m_pelement->add_element("g");
    set_attributes(pelement, args...);
    m_pelement = pelement.get();
    return pelement;
}

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::Document::open_defs(Args... args)
{
    std::shared_ptr<xml::Element> pelement = m_pelement->add_element("defs");
    set_attributes(pelement, args...);
    m_pelement = pelement.get();
    return pelement;
}

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::Document::open_svg(Args... args)
{
    std::shared_ptr<xml::Element> pelement = m_pelement->add_element("svg");
    set_attributes(pelement, args...);
    m_pelement = pelement.get();
    return pelement;
}

//------------------------------------------------------------------------------
template<typename F, typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::Document::defs(F&& f, Args&&... args)
{
    std::shared_ptr<xml::Element> pelementPrev = m_pelement;

    std::shared_ptr<xml::Element> pelementDefs = m_document.element().sub_element("defs");
    if (!pelementDefs)
    {
        if (m_document.element().elements().empty())
        {
            pelementDefs = open_defs();
            close_defs();
        }
        else
        {
            pelementDefs = m_document.element().elements().insert(m_document.element().elements().begin(), "defs");
        }
    }

    assert(pelementDefs);
    m_pelement = pelementDefs;
    std::shared_ptr<xml::Element> pelement = std::bind(std::forward<F>(f), std::forward<Args>(args)...)();
    m_pelement = pelementPrev;
    return pelement;
}

//------------------------------------------------------------------------------
template<typename T, typename... Args>
void quetzal::svg::Document::set_attributes(std::shared_ptr<xml::Element> pelement, T t, Args... args)
{
    pelement->set_attribute(t.name(), to_string(t.value()));
    set_attributes(pelement, args...);
    return;
}

#endif // QUETZAL_SVG_DOCUMENT_HPP
