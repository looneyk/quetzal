#if !defined(QUETZAL_PDF_DOCUMENT_HPP)
#define QUETZAL_PDF_DOCUMENT_HPP
//------------------------------------------------------------------------------
// Document.hpp
//------------------------------------------------------------------------------

#include "Font.hpp"
#include <string>
#include <vector>

namespace quetzal
{

namespace pdf
{

    typedef unsigned int id_type;

    //--------------------------------------------------------------------------
    struct Color
    {
        Color(unsigned char red, unsigned char green, unsigned char blue) :
            m_red(red),
            m_green(green),
            m_blue(blue)
        {
        }

        unsigned char m_red;
        unsigned char m_green;
        unsigned char m_blue;
    };

    typedef std::vector<Color> Image;

    // images and image references ...

    //--------------------------------------------------------------------------
    struct ImageInfo
    {
        ImageInfo(id_type idObject, const std::string& name, int width, int height) :
            m_idObject(idObject),
            m_name(name),
            m_width(width),
            m_height(height)
        {
        }

        id_type m_idObject;
        std::string m_name;
        int m_width;
        int m_height;
    };

    //--------------------------------------------------------------------------
    struct ImageEntry
    {
        ImageEntry(const ImageInfo& info, const Image& image) :
            m_info(info),
            m_image(image)
        {
        }

        ImageInfo m_info;
        Image m_image;
    };

    //--------------------------------------------------------------------------
    struct Point
    {
        Point(double x, double y) :
            m_x(x),
            m_y(y)
        {
        }

        double m_x;
        double m_y;
    };

    bool operator==(const Point& lhs, const Point& rhs);
    bool operator!=(const Point& lhs, const Point& rhs);
    std::ostream& operator<<(std::ostream& os, const Point& point);

    typedef std::vector<Point> points_type;

    bool closed(const points_type& points);
    std::ostream& operator<<(std::ostream& os, const points_type& points);

    //--------------------------------------------------------------------------
    class Document
    {
    public:

        friend std::ostream& operator<<(std::ostream& os, const Document& document);

        Document();
        Document(double wPage, double hPage);

        double page_width()  const;
        double page_height() const;

        void advance_page();

        void write(const std::string& filename); // deprecated ....

        double text_width(const Font* pfont, double size, const std::string& s);
        double text_width(const std::string& s);

        void set_font(const std::string& name, double size);
        void show_text(const std::string& text, double x, double y);
        // centered/justified flags for these? ...
        void show_text_centered(const std::string& text, double x, double y);
        void show_text_centered(const std::vector<std::string>& lines, double x, double y);
        void show_text_right_justified(const std::string& text, double x, double y);

        std::vector<std::string> wrap_text(const std::string& text, double wMax, bool rightJustify);

//        ImageInfo processImage(const Image& image);

        void show_image(const ImageInfo& info, double x, double y, double xScale, double yScale);
        void show_image(const ImageInfo& info, double x, double y, double scale);
        void show_image(const ImageInfo& info, double x, double y);

        void set_line_color(double red, double green, double blue);
        void set_fill_color(double red, double green, double blue);
        void set_line_width(double w);

        void draw_line(double x0, double y0, double x1, double y1);
        void draw_line(const points_type& points);

        void draw_rectangle(double x, double y, double w, double h);
        void fill_rectangle(double x, double y, double w, double h);

        void draw_polygon(const points_type& points);
        void fill_polygon(const points_type& points);

        void draw_ellipse(double x, double y, double xRadius, double yRadius);
        void fill_ellipse(double x, double y, double xRadius, double yRadius);

        void draw_circle(double x, double y, double radius);
        void fill_circle(double x, double y, double radius);

    private:

        void add_operation(const std::string& s);
        void set_color(double red, double green, double blue, bool bStroke);
        void draw_rectangle(double x, double y, double w, double h, bool bStroke);
        void draw_polygon(const points_type& points, bool bStroke);
        void draw_ellipse(double x, double y, double xRadius, double yRadius, bool bStroke);
        void draw_circle(double x, double y, double radius, bool bStroke);

        double m_wPage;
        double m_hPage;

        const Font* m_pFont;
        double m_sizeFont;

        std::vector<ImageEntry> m_entries;

        std::string m_page;
        std::vector<std::string> m_pages;
    };

    std::ostream& operator<<(std::ostream& os, const Document& document);

} // namespace pdf

} // namespace quetzal

#endif // QUETZAL_PDF_DOCUMENT_HPP
