#if !defined(QUETZAL_SVG_SVG_HPP)
#define QUETZAL_SVG_SVG_HPP

#include "Attribute.hpp"
#include "quetzal/xml/Element.hpp"
#include "quetzal/math/Vector.hpp"

namespace quetzal::svg
{

    using point_type = math::Vector<VectorTraits<double, 2>>;
    using vector_type = math::Vector<VectorTraits<double, 2>>;

    // public

    template<typename... Args>
    std::shared_ptr<xml::Element> rectangle(point_type position, double dx, double dy, Args... args);

    template<typename... Args>
    std::shared_ptr<xml::Element> circle(point_type position, double radius, Args... args);

    template<typename... Args>
    std::shared_ptr<xml::Element> ellipse(point_type position, double rx, double ry, Args... args);

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

// facility for adding elements to sections: style, defs, ...
// for group/embedded svg, need a context with current group ...

    // private

    void set_attributes(std::shared_ptr<xml::Element> pelement);

    template<typename T, typename... Args>
    void set_attributes(std::shared_ptr<xml::Element> pelement, T t, Args... args);

} // namespace quetzal::svg

//------------------------------------------------------------------------------
void quetzal::svg::set_attributes(std::shared_ptr<xml::Element> pelement)
{
    pelement;
    return;
}

//------------------------------------------------------------------------------
template<typename T, typename... Args>
void quetzal::svg::set_attributes(std::shared_ptr<xml::Element> pelement, T t, Args... args)
{
    pelement->set_attribute(t.name(), to_string(t.value()));
    set_attributes(pelement, args...);
    return;
}

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::rectangle(point_type position, double dx, double dy, Args... args)
{
    std::shared_ptr<xml::Element> pelement = std::make_shared<xml::Element>("rect");
    set_attributes(pelement, Attribute("x", position.x()), Attribute("y", position.y()), Attribute("width", dx), Attribute("height", dy), args...);
    return pelement;
}

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::circle(point_type position, double radius, Args... args)
{
    std::shared_ptr<xml::Element> pelement = std::make_shared<xml::Element>("circle");
    set_attributes(pelement, Attribute("cx", position.x()), Attribute("cy", position.y()), Attribute("r", radius), args...);
    return pelement;
}

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::ellipse(point_type position, double rx, double ry, Args... args)
{
    std::shared_ptr<xml::Element> pelement = std::make_shared<xml::Element>("ellipse");
    set_attributes(pelement, Attribute("cx", position.x()), Attribute("cy", position.y()), Attribute("rx", rx), Attribute("ry", ry), args...);
    return pelement;
}

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::line(point_type p0, point_type p1, Args... args)
{
    std::shared_ptr<xml::Element> pelement = std::make_shared<xml::Element>("line");
    set_attributes(pelement, Attribute("x1", p0.x()), Attribute("y1", p0.y()), Attribute("x2", p1.x()), Attribute("y2", p1.y()), args...);
    return pelement;
}

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::polyline(std::vector<point_type> points, Args... args)
{
    std::shared_ptr<xml::Element> pelement = std::make_shared<xml::Element>("polyline");
    ostringstream oss;
    for (point_type point : points)
    {
        if (!oss.str().empty())
        {
            oss << " ";
        }
        oss << point.x() << "," << point.y();
    }
    set_attributes(pelement, Attribute("points", oss.str()), args...);
    return pelement;
}

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::polygon(std::vector<point_type> points, Args... args)
{
    std::shared_ptr<xml::Element> pelement = std::make_shared<xml::Element>("polygone");
    ostringstream oss;
    for (point_type point : points)
    {
        if (!oss.str().empty())
        {
            oss << " ";
        }
        oss << point.x() << "," << point.y();
    }
    set_attributes(pelement, Attribute("points", oss.str()), args...);
    return pelement;
}

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::path(const std::string& commands, Args... args)
{
    std::shared_ptr<xml::Element> pelement = std::make_shared<xml::Element>("path");
    set_attributes(pelement, Attribute("d", commands, args...);
    return pelement;
}

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::text(const point_type& position, const std::string& s, Args... args)
{
    std::shared_ptr<xml::Element> pelement = std::make_shared<xml::Element>("text");
    pelement->set_value(s);
    set_attributes(pelement, Attribute("x", position.x()), Attribute("y", position.y()), args...);
    return pelement;
}

//------------------------------------------------------------------------------
template<typename... Args>
std::shared_ptr<quetzal::xml::Element> quetzal::svg::text_path(const std::string& s, const std::string& s, Args... args)
{
    std::shared_ptr<xml::Element> pelement = std::make_shared<xml::Element>("text");
    std::shared_ptr<xml::Element> pelementPath = pelement.add_element("textPath");
    pelementPath->set_value(s);
    set_attributes(pelementPath, args...);
    return pelement;
}



/*
    class Color
    {
    public:
        enum Defaults { Transparent = -1, Aqua, Black, Blue, Brown, Cyan, Fuchsia,
            Green, Lime, Magenta, Orange, Purple, Red, Silver, White, Yellow };

        Color(int r, int g, int b) : transparent(false), red(r), green(g), blue(b) { }
        Color(Defaults color)
            : transparent(false), red(0), green(0), blue(0)
        {
            switch (color)
            {
                case Aqua: assign(0, 255, 255); break;
                case Black: assign(0, 0, 0); break;
                case Blue: assign(0, 0, 255); break;
                case Brown: assign(165, 42, 42); break;
                case Cyan: assign(0, 255, 255); break;
                case Fuchsia: assign(255, 0, 255); break;
                case Green: assign(0, 128, 0); break;
                case Lime: assign(0, 255, 0); break;
                case Magenta: assign(255, 0, 255); break;
                case Orange: assign(255, 165, 0); break;
                case Purple: assign(128, 0, 128); break;
                case Red: assign(255, 0, 0); break;
                case Silver: assign(192, 192, 192); break;
                case White: assign(255, 255, 255); break;
                case Yellow: assign(255, 255, 0); break;
                default: transparent = true; break;
            }
        }
        virtual ~Color() { }
        std::string toString() const
        {
            std::stringstream ss;
            if (transparent)
                ss << "transparent";
            else
                ss << "rgb(" << red << "," << green << "," << blue << ")";
            return ss.str();
        }
    private:
        bool transparent;
        int red;
        int green;
        int blue;

        void assign(int r, int g, int b)
        {
            red = r;
            green = g;
            blue = b;
        }
    };

    class Fill
    {
    public:
        Fill(Color::Defaults color) : color(color) { }
        Fill(Color color = Color::Transparent)
            : color(color) { }
        std::string toString() const
        {
            std::stringstream ss;
            ss << attribute("fill", color.toString());
            return ss.str();
        }
    private:
        Color color;
    };

    class Stroke
    {
    public:
        Stroke(double width = -1, Color color = Color::Transparent)
            : width(width), color(color) { }
        std::string toString() const
        {
            // If stroke width is invalid.
            if (width < 0)
                return std::string();

            std::stringstream ss;
            ss << attribute("stroke-width", width) << attribute("stroke", color.toString());
            return ss.str();
        }
    private:
        double width;
        Color color;
    };

    class Font
    {
    public:
        Font(double size = 12, std::string const & family = "Verdana") : size(size), family(family) { }
        std::string toString() const
        {
            std::stringstream ss;
            ss << attribute("font-size", size) << attribute("font-family", family);
            return ss.str();
        }
    private:
        double size;
        std::string family;
    };

    class Shape
    {
    public:
        Shape(Fill const & fill = Fill(), Stroke const & stroke = Stroke())
            : fill(fill), stroke(stroke) { }
        virtual ~Shape() { }
        virtual std::string toString() const = 0;
    protected:
        Fill fill;
        Stroke stroke;
    };
*/

#endif // QUETZAL_SVG_SVG_HPP
