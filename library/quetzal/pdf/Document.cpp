//------------------------------------------------------------------------------
// Document.cpp
//------------------------------------------------------------------------------

#include "Document.hpp"
#include "quetzal/Exception.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cassert>

using namespace std;

namespace
{

    const double wPageDefault = 612.;
    const double hPageDefault = 792.;

    const string OP_BEGIN_TEXT    = "BT";
    const string OP_END_TEXT      = "ET";
    const string OP_SAVE_STATE    = "q";
    const string OP_RESTORE_STATE = "Q";
    const string OP_CONCAT_MATRIX = "cm";
    const string OP_DO            = "Do";
    const string OP_MOVETO        = "m";
    const string OP_LINETO        = "l";
    const string OP_CLOSE_SUBPATH = "h";
    const string OP_STROKE        = "S";
    const string OP_FILL          = "f";
    const string OP_STROKE_COLOR  = "RG";
    const string OP_FILL_COLOR    = "rg";
    const string OP_STROKE_GRAY   = "G";
    const string OP_FILL_GRAY     = "g";
    const string OP_TEXT_FONT     = "Tf";
    const string OP_TEXT_SHOW     = "Tj";
    const string OP_TEXT_MATRIX   = "Tm";
    const string OP_RECTANGLE     = "re";
    const string OP_WIDTH         = "w";

    //--------------------------------------------------------------------------
    quetzal::pdf::id_type getImageObjectID(int nImage)
    {
        // Images are stored just after fonts
        return quetzal::pdf::fonts() + nImage + 1;
    }

} // namespace

//--------------------------------------------------------------------------
bool quetzal::pdf::operator==(const Point& lhs, const Point& rhs)
{
    return lhs.m_x == rhs.m_x && lhs.m_y == rhs.m_y;
}

//--------------------------------------------------------------------------
bool quetzal::pdf::operator!=(const Point& lhs, const Point& rhs)
{
    return !operator==(lhs, rhs);
}

//--------------------------------------------------------------------------
ostream& quetzal::pdf::operator<<(ostream& os, const Point& point)
{
    os << point.m_x << " " << point.m_y;
    return os;
}

//--------------------------------------------------------------------------
bool quetzal::pdf::closed(const points_type& points)
{
    assert(!points.empty());
    return points.front() == points.back();
}

//--------------------------------------------------------------------------
ostream& quetzal::pdf::operator<<(ostream& os, const points_type& points)
{
    string op = OP_MOVETO;
    for (const auto& i : points)
    {
        os << i << " " << op << endl;
        op = OP_LINETO;
    }

    return os;
}

//------------------------------------------------------------------------------
quetzal::pdf::Document::Document() :
    Document(wPageDefault, hPageDefault)
{
}

//------------------------------------------------------------------------------
quetzal::pdf::Document::Document(double wPage, double hPage) :
    m_wPage(wPage),
    m_hPage(hPage),
    m_pFont(nullptr),
    m_sizeFont(0.),
    m_entries(),
    m_page(),
    m_pages()
{
    init_fonts();
}

//------------------------------------------------------------------------------
double quetzal::pdf::Document::page_width() const
{
    return m_wPage;
}

//------------------------------------------------------------------------------
double quetzal::pdf::Document::page_height() const
{
    return m_hPage;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::write(const string& filename)
{
    ofstream ofs(filename, ios_base::binary);
    if (!ofs)
    {
        ostringstream oss;
        oss << "Error opening file: " + filename;
        throw Exception(oss.str(), __FILE__, __LINE__);
    }

    ofs << *this;
    return;
}

//------------------------------------------------------------------------------
/*
quetzal::pdf::ImageInfo quetzal::pdf::Document::processImage(const Image& image)
{
    int id = getImageObjectID(m_entries.size());

    ostringstream oss;

    oss << "/Img" << id;

    int height = image.size();
    int width = (height == 0 ? 0 : image[0].size());

    ImageInfo info(id, oss.str(), width, height);

    m_entries.push_back(ImageEntry(image, info));

    return info;
}
*/

//------------------------------------------------------------------------------
void quetzal::pdf::Document::advance_page()
{
    m_pages.push_back(m_page);

    m_page.clear();
    m_pFont = nullptr;
    m_sizeFont = 0.;
    return;
}

//------------------------------------------------------------------------------
double quetzal::pdf::Document::text_width(const Font* pfont, double size, const string& s)
{
    assert(m_pFont != nullptr);

    return pfont->width(size, s);
}

//------------------------------------------------------------------------------
double quetzal::pdf::Document::text_width(const string& s)
{
    if (m_pFont == nullptr)
    {
        return 0.;
    }

    return text_width(m_pFont, m_sizeFont, s);
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::set_font(const string& name, double size)
{
    m_pFont = font(name);
    if (m_pFont == nullptr)
    {
        return; // throw, add font, ...
    }

    m_sizeFont = size;

    ostringstream oss;
    oss << "/F" << m_pFont->id() << " " << m_sizeFont << " " << OP_TEXT_FONT << endl;

    m_page += oss.str();
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::show_text(const string& text, double x, double y)
{
    string s;

    for (const auto& c : text)
    {
        if (c == '(' ||c == ')')
        {
             s += "\\";
        }

        s += c;
    }

    ostringstream oss;
    oss << "1 0 0 1" << " " << x << " " << y << " " << OP_TEXT_MATRIX << endl;
    oss << "(" << s << ")" << " " << OP_TEXT_SHOW << endl;

    m_page += oss.str();
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::show_text_centered(const string& text, double x, double y)
{
    x -= text_width(text) / 2.0;

    show_text(text, x, y);
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::show_text_centered(const vector<string>& lines, double x, double y)
{
//    y += m_sizeFont * (lines.size() - 2.0) / 2.0; // Centered at x, y
    y -= m_sizeFont; // Starting below x, y

    for (const auto& line : lines)
    {
        // Use line advance here instead, no x, y? ...
        show_text_centered(line, x, y);
        y -= m_sizeFont;
    }

    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::show_text_right_justified(const string& text, double x, double y)
{
    if (m_pFont != nullptr)
    {
        x -= m_pFont->width(m_sizeFont, text);
    }

    show_text(text, x, y);
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::show_image(const ImageInfo& info, double x, double y, double xScale, double yScale)
{
    double width = info.m_width  * xScale;
    double height = info.m_height * yScale;

    ostringstream oss;

    oss << OP_SAVE_STATE << endl;
    oss << width << " " << 0 << " " << 0 << " " << height << " " << x << " " << y << " " << OP_CONCAT_MATRIX << endl;
    oss << info.m_name << " " << OP_DO << endl;
    oss << OP_RESTORE_STATE << endl;

    m_page += oss.str();
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::show_image(const ImageInfo& info, double x, double y, double scale)
{
    show_image(info, x, y, scale, scale);
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::show_image(const ImageInfo& info, double x, double y)
{
    show_image(info, x, y, 1.0, 1.0);
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::set_line_color(double red, double green, double blue)
{
    set_color(red, green, blue, true);
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::set_fill_color(double red, double green, double blue)
{
    set_color(red, green, blue, false);
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::set_line_width(double w)
{
    ostringstream oss;
    oss << w << " " << OP_WIDTH << endl;
    add_operation(oss.str());
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::draw_line(double x0, double y0, double x1, double y1)
{
    points_type points =
    {
        {x0, y0},
        {x1, y1}
    };

    draw_line(points);
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::draw_line(const points_type& points)
{
    assert(points.size() == 2);

    ostringstream oss;
    oss << points;
    oss << OP_STROKE << endl;
    add_operation(oss.str());
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::draw_rectangle(double x, double y, double w, double h)
{
    draw_rectangle(x, y, w, h, true);
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::fill_rectangle(double x, double y, double w, double h)
{
    draw_rectangle(x, y, w, h, false);
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::draw_polygon(const points_type& points)
{
    draw_polygon(points, true);
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::fill_polygon(const points_type& points)
{
    draw_polygon(points, false);
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::draw_ellipse(double x, double y, double xRadius, double yRadius)
{
    draw_ellipse(x, y, xRadius, yRadius, true);
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::fill_ellipse(double x, double y, double xRadius, double yRadius)
{
    draw_ellipse(x, y, xRadius, yRadius, false);
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::draw_circle(double x, double y, double radius)
{
    draw_ellipse(x, y, radius, radius, true);
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::fill_circle(double x, double y, double radius)
{
    draw_ellipse(x, y, radius, radius, false);
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::add_operation(const string& s)
{
    m_page += s;
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::set_color(double red, double green, double blue, bool bStroke)
{
    ostringstream oss;
    oss << red << " " << green << " " << blue << " " << (bStroke ? OP_STROKE_COLOR : OP_FILL_COLOR) << endl;
    add_operation(oss.str());
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::draw_rectangle(double x, double y, double w, double h, bool bStroke)
{
    ostringstream oss;
    oss << x << " " << y << " " << w << " " << h << " " << OP_RECTANGLE << " " << (bStroke ? OP_STROKE : OP_FILL) << endl;
    add_operation(oss.str());
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::draw_polygon(const points_type& points, bool bStroke)
{
    assert(points.size() > 2);

    ostringstream oss;
    string op = OP_MOVETO;

    for (const auto& i : points)
    {
        oss << i << " " << op << endl;
        op = OP_LINETO;
    }

    // Close the polygon if necessary
    if (!closed(points))
    {
        oss << OP_CLOSE_SUBPATH << endl;
    }

    oss << (bStroke ? OP_STROKE : OP_FILL) << endl;
    add_operation(oss.str());
    return;
}

//------------------------------------------------------------------------------
void quetzal::pdf::Document::draw_ellipse(double x, double y, double xRadius, double yRadius, bool bStroke)
{
    assert(xRadius > 0.);
    assert(yRadius > 0.);

    double a2 = xRadius * xRadius;
    double b2 = yRadius * yRadius;

    quetzal::pdf::points_type points;

    for (double i = xRadius; i >= 0; --i)
    {
        points.push_back({x + i, y + sqrt(b2 * (1 - i * i / a2))});
    }

    for (auto i = points.size() - 2; i >= 0; --i)
    {
        points.push_back({x - (points[i].m_x - x), points[i].m_y});
    }

    for (auto i = points.size() - 2; i >= 0; --i)
    {
        points.push_back({points[i].m_x, y - (points[i].m_y - y)});
    }

    draw_polygon(points, bStroke);
    return;
}

//------------------------------------------------------------------------------
ostream& quetzal::pdf::operator<<(ostream& os, const Document& document)
{
    ostringstream oss;

    // Header

    oss << "%PDF-1.1" << endl;
    oss << "%\xe2\xe3\xcf\xd3" << endl;

    // Fonts

    vector<size_t> offsets;
    for (unsigned int i = 0; i < fonts(); ++i)
    {
        offsets.push_back(oss.str().size());

        oss << (1 + i) << " 0 obj" << endl;
        oss << "<<" << endl;
        oss << "/Type /Font" << endl;
        oss << "/Subtype /Type1" << endl;
        oss << "/BaseFont /" << quetzal::pdf::font(i)->name() << endl;
        oss << "/Encoding /WinAnsiEncoding" << endl;
        oss << ">>" << endl;
        oss << "endobj" << endl;
    }

    // Images

    int id = getImageObjectID(0);
    auto nImages = document.m_entries.size();

    for (unsigned int i = 0; i < nImages; ++i)
    {
        offsets.push_back(oss.str().size());

        id = document.m_entries[i].m_info.m_idObject;
        int width = document.m_entries[i].m_info.m_width;
        int height = document.m_entries[i].m_info.m_height;

        // Don't count the final newline in our /Length value
        int length = (3 * 2 * width + 1) * height - 1;

        oss << id << " 0 obj" << endl;
        oss << "<<" << endl;
        oss << "/Type /XObject" << endl;
        oss << "/Subtype /Image" << endl;
        oss << "/Width " << width << endl;
        oss << "/Height " << height << endl;
        oss << "/BitsPerComponent 8" << endl;
        oss << "/ColorSpace /DeviceRGB" << endl;
        oss << "/Filter /ASCIIHexDecode" << endl;
        oss << "/Length " << length << endl;
        oss << ">>" << endl;

        oss << "stream" << endl;

        auto j = document.m_entries[i].m_image.begin();

        for (int y = 0; y < height; ++y)
        {
             for (int x = 0; x < width; ++x)
             {
                 int r = j->m_red;
                 int g = j->m_green;
                 int b = j->m_blue;

                 ostringstream tmp;
                 tmp << hex << setfill('0') << setw(2) << r << setw(2) << g << setw(2) << b;
                 oss << tmp.str();

                 ++j;
             }

             oss << "\n"; // should be single 0x0a ...
        }

        oss << "endstream" << endl;
        oss << "endobj" << endl;
    }

    if (nImages > 0)
    {
        ++id;
    }

    // Set up an obj for our common resources, first our fonts and then any images

    offsets.push_back(oss.str().size());

    int idResources = id;

    oss << idResources << " 0 obj" << endl;
    oss << "<<" << endl;
    oss << "/Font" << endl;
    oss << "<<" << endl;

    for (unsigned int i = 0; i < fonts(); ++i)
    {
        oss << "/F" << (i + 1) << " " << (i + 1) << " 0 R" << endl;
    }

    oss << ">>" << endl;

    if (nImages > 0)
    {
        oss << "/XObject << " << endl;

        for (unsigned int i = 0; i < nImages; ++i)
        {
             oss << document.m_entries[i].m_info.m_name << " " << document.m_entries[i].m_info.m_idObject << " 0 R" << endl;
        }

        oss << ">>" << endl;
    }

    oss << ">>" << endl;
    oss << "endobj" << endl;

    // Top-level object

    offsets.push_back(oss.str().size());

    int idCatalog = idResources + 1;
    int idPages = idCatalog + 1;

    oss << idCatalog << " 0 obj" << endl;
    oss << "<<" << endl;
    oss << "/Type /Catalog" << endl;
    oss << "/Pages " << idPages << " 0 R" << endl;
    oss << ">>" << endl;
    oss << "endobj" << endl;

    // Pages

    vector<string> pages;

    for (size_t i = 0, n = document.m_pages.size(); i < n; ++i)
    {
        pages.push_back(document.m_pages[i]);
    }

    // Append the current page if it is not empty

    if (!document.m_page.empty())
    {
        pages.push_back(document.m_page);
    }

    // Pages object

    offsets.push_back(oss.str().size());

    oss << idPages << " 0 obj" << endl;
    oss << "<<" << endl;
    oss << "/Type /Pages" << endl;
    oss << "/MediaBox [0 0 " << document.page_width() << " " << document.page_height() << "]" << endl;
    oss << "/Count " << pages.size() << endl;
    oss << "/Kids [";

    for (size_t i = 0, n = pages.size(); i < n; ++i)
    {
        oss << " " << (1 + idPages + i) << " 0 R";
    }

    oss << "]" << endl;
    oss << ">>" << endl;
    oss << "endobj" << endl;

    // Pages

    for (size_t i = 0, n = pages.size(); i < n; ++i)
    {
        offsets.push_back(oss.str().size());
        auto idPage = (1 + idPages + i);
        oss << idPage << " 0 obj" << endl;
        oss << "<<" << endl;
        oss << "/Type /Page" << endl;
        oss << "/Parent " << idPages << " 0 R" << endl;
        oss << "/Contents " << (idPage + n) << " 0 R" << endl;
        oss << "/Resources " << idResources << " 0 R" << endl;
        oss << ">>" << endl;
        oss << "endobj" << endl;
    }

    // Page contents

    auto idPageContent = idPages + pages.size() + 1;

    for (size_t i = 0, n = pages.size(); i < n; ++i)
    {
        string theContents = pages[i];

        theContents = OP_BEGIN_TEXT + "\n" + theContents + OP_END_TEXT;
        auto size = theContents.size();

        offsets.push_back(oss.str().size());

        oss << idPageContent << " 0 obj" << endl;
        oss << "<<" << endl;
        oss << "/Length " << size << endl;
        oss << ">>" << endl;
        oss << "stream" << endl;
        oss << theContents << endl;
        oss << "endstream" << endl;
        oss << "endobj" << endl;

        ++idPageContent;
    }

    auto xrefOffset = oss.str().size();
    auto nEntries = (offsets.size() + 1);

    string append = " ";

    oss << "xref" << endl;
    oss << "0 " << nEntries << endl;
    oss << "0000000000 65535 f" << append << endl;

    for (size_t i = 0, n = offsets.size(); i < n; ++i)
    {
        ostringstream tmp;

        tmp << offsets[i];

        for (size_t j = 0, nn = 10 - tmp.str().size(); j < nn; ++j)
        {
             oss << "0";
        }

        oss << offsets[i] << " 00000 n" << append << endl;
    }

    oss << "trailer" << endl;
    oss << "<<" << endl;
    oss << "/Size " << nEntries << endl;
    oss << "/Root " << idCatalog << " 0 R" << endl;
    oss << ">>" << endl;
    oss << "startxref" << endl;
    oss << xrefOffset << endl;
    oss << "%%EOF" << endl;

    os << oss.str();

    return os;
}
