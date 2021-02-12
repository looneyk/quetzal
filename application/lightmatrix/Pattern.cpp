//------------------------------------------------------------------------------
// Light Matrix Simulation
// Copyright (c) 2010 Keith Michael Looney. All Rights Reserved.
// Pattern.cpp
//------------------------------------------------------------------------------

#include "Pattern.hpp"
#include "quetzal/xml/Document.hpp"
#include "quetzal/common/string_util.hpp"
#include <DirectXMath.h> // Pi
#include <fstream>
#include <sstream>
#include <cassert>

using namespace std;

namespace
{

    //--------------------------------------------------------------------------
    template<typename T>
    void parse2(const string& str, T& x, T& y)
    {
        istringstream iss(str);
        iss >> x >> y;
        return;
    }

    //--------------------------------------------------------------------------
    template<typename T>
    string format2(T x, T y)
    {
        ostringstream oss;
        oss << x << " " << y;
        return oss.str();
    }

} // namespace

//------------------------------------------------------------------------------
quetzal::lightmatrix::Pattern::Pattern() :
    m_frame(),
    m_wViewport(0.0f),
    m_hViewport(0.0f),
    m_wDisplay(0),
    m_hDisplay(0),
    m_bRadial(false),
    m_rPixel(0.0f),
    m_sx(0.0f),
    m_sy(0.0f),
    m_bShiftColor(false),
    m_bShiftBrightness(false),
    m_smoothing(Smoothing::Null),
    m_dx(0.0f),
    m_dy(0.0f),
    m_vx(0.0f),
    m_vy(0.0f)
{
}

//------------------------------------------------------------------------------
const quetzal::lightmatrix::Frame& quetzal::lightmatrix::Pattern::frame() const
{
    return m_frame;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Pattern::set_frame(const Frame& frame)
{
    m_frame = frame;

    return;
}

//------------------------------------------------------------------------------
float quetzal::lightmatrix::Pattern::viewport_width() const
{
    return m_wViewport;
}

//------------------------------------------------------------------------------
float quetzal::lightmatrix::Pattern::viewport_height() const
{
    return m_hViewport;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Pattern::set_viewport_size(float w, float h)
{
    m_wViewport = w;
    m_hViewport = h;

    return;
}

//------------------------------------------------------------------------------
int quetzal::lightmatrix::Pattern::display_width() const
{
    return m_wDisplay;
}

//------------------------------------------------------------------------------
int quetzal::lightmatrix::Pattern::display_height() const
{
    return m_hDisplay;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Pattern::set_display_size(int w, int h)
{
    m_wDisplay = w;
    m_hDisplay = h;

    return;
}

//------------------------------------------------------------------------------
bool quetzal::lightmatrix::Pattern::radial() const
{
    return m_bRadial;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Pattern::set_radial(bool bRadial)
{
    m_bRadial = bRadial;

    return;
}

//------------------------------------------------------------------------------
float quetzal::lightmatrix::Pattern::pixel_radius() const
{
    return m_rPixel;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Pattern::set_pixel_radius(float r)
{
    m_rPixel = r;

    return;
}

//------------------------------------------------------------------------------
float quetzal::lightmatrix::Pattern::spacing_x() const
{
    return m_sx;
}

//------------------------------------------------------------------------------
float quetzal::lightmatrix::Pattern::spacing_y() const
{
    return m_sy;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Pattern::set_spacing(float sx, float sy)
{
    m_sx = sx;
    m_sy = sy;

    return;
}

//------------------------------------------------------------------------------
bool quetzal::lightmatrix::Pattern::shift_color() const
{
    return m_bShiftColor;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Pattern::set_shift_color(bool bShift)
{
    m_bShiftColor = bShift;

    return;
}

//------------------------------------------------------------------------------
bool quetzal::lightmatrix::Pattern::shift_brightness() const
{
    return m_bShiftBrightness;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Pattern::set_shift_brightness(bool bShift)
{
    m_bShiftBrightness = bShift;

    return;
}

//------------------------------------------------------------------------------
quetzal::lightmatrix::Smoothing quetzal::lightmatrix::Pattern::smoothing() const
{
    return m_smoothing;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Pattern::set_smoothing(Smoothing s)
{
    m_smoothing = s;

    return;
}

//------------------------------------------------------------------------------
float quetzal::lightmatrix::Pattern::direction_x() const
{
    return m_dx;
}

//------------------------------------------------------------------------------
float quetzal::lightmatrix::Pattern::direction_y() const
{
    return m_dy;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Pattern::set_direction(float dx, float dy)
{
    m_dx = dx;
    m_dy = dy;

    return;
}

//------------------------------------------------------------------------------
float quetzal::lightmatrix::Pattern::velocity_x() const
{
    return m_vx;
}

//------------------------------------------------------------------------------
float quetzal::lightmatrix::Pattern::velocity_y() const
{
    return m_vy;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Pattern::set_velocity(float vx, float vy)
{
    if (vx < 0.0f)
    {
        vx = 0.0f;
    }

    if (vy < 0.0f)
    {
        vy = 0.0f;
    }

    m_vx = vx;
    m_vy = vy;

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Pattern::load(const string& strFile)
{
    xml::Document document;

    ifstream is(strFile);
    is >> document;

    const xml::Element& elm = document.element();
    if (elm.name() != "pattern")
    {
        return;
    }

    // Frame
    shared_ptr<xml::Element> pe = elm.sub_element("frame");
    if (pe == nullptr)
    {
        return;
    }

    // Size
    int w = to_type<int>(pe->attribute_value("width"));
    int h = to_type<int>(pe->attribute_value("height"));
    m_frame.set_size(w, h);

    // Colors
    istringstream iss(pe->value());
    for (int j = 0; j < h; ++j)
    {
        for (int i = 0; i < w; ++i)
        {
            iss >> m_frame.pixel(i, j);
        }
    }

    // Layout
    pe = elm.sub_element("layout");
    if (pe == nullptr)
    {
        return;
    }

    string str = pe->element_value("viewport");
    parse2(str, m_wViewport, m_hViewport);

    str = pe->element_value("display");
    parse2(str, m_wDisplay, m_hDisplay);

    str = pe->element_value("arrangement");
    m_bRadial = (str == "radial");

    str = pe->element_value("pixel_radius");
    m_rPixel = to_type<float>(str);

    str = pe->element_value("spacing");
    parse2(str, m_sx, m_sy);

    // Animation
    pe = elm.sub_element("animation");
    if (pe == nullptr)
    {
        return;
    }

    str = pe->element_value("shift_color");
    m_bShiftColor = to_type<bool>(str);

    str = pe->element_value("shift_brightness");
    m_bShiftBrightness = to_type<bool>(str);

    str = pe->element_value("smoothing");
    if (str == "nearest")
    {
        m_smoothing = Smoothing::Nearest;
    }
    else if (str == "linear")
    {
        m_smoothing = Smoothing::Linear;
    }
    else if (str == "cubic")
    {
        m_smoothing = Smoothing::Cubic;
    }

    str = pe->element_value("direction");
    parse2(str, m_dx, m_dy);
    // directions should be -1, 0 or 1 only ...

    str = pe->element_value("velocity");
    parse2(str, m_vx, m_vy);

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Pattern::store(const string& strFile)
{
    xml::Document document;

    xml::Element& elm = document.element();
    elm.set_name("pattern");

    // Frame
    shared_ptr<xml::Element> peframe = elm.add_element("frame");

    // Size
    const int w = m_frame.width();
    const int h = m_frame.height();
    peframe->set_attribute("width", to_string(w));
    peframe->set_attribute("height", to_string(h));

    // Colors
    ostringstream oss;
    oss << endl << string((peframe->depth() + 1) * 4, ' ');
    for (int j = 0; j < h; ++j)
    {
        for (int i = 0; i < w; ++i)
        {
            oss << m_frame.pixel(i, j);

            if (i < w - 1)
            {
                oss << " ";
            }
            else
            {
                if (j < h - 1)
                {
                    oss << endl << string((peframe->depth() + 1) * 4, ' ');
                }
                else
                {
                    oss << endl << string(peframe->depth() * 4, ' ');
                }
            }
        }
    }

    peframe->set_value(oss.str());

    // Layout
    shared_ptr<xml::Element> pea = elm.add_element("layout");

    pea->add_element("viewport", format2(m_wViewport, m_hViewport));
    pea->add_element("display", format2(m_wDisplay, m_hDisplay));
    pea->add_element("arrangement", m_bRadial ? "radial" : "rectangular");
    pea->add_element("pixel_radius", to_string(m_rPixel));
    pea->add_element("spacing", format2(m_sx, m_sy));

    // Animation
    pea = elm.add_element("animation");

    pea->add_element("shift_color", m_bShiftColor ? "true" : "false");
    pea->add_element("shift_brightness", m_bShiftBrightness ? "true" : "false");

    switch (m_smoothing)
    {
        case Smoothing::Nearest:
            pea->add_element("smoothing", "nearest");
            break;

        case Smoothing::Linear:
            pea->add_element("smoothing", "linear");
            break;

        case Smoothing::Cubic:
            pea->add_element("smoothing", "cubic");
            break;
    }

    pea->add_element("direction", format2(m_dx, m_dy));
    pea->add_element("velocity", format2(m_vx, m_vy));

    ofstream os(strFile);
    os << document;

    return;
}
