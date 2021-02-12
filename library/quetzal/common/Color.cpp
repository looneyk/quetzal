//------------------------------------------------------------------------------
// Color.cpp
//
//------------------------------------------------------------------------------

#include "Color.hpp"
#include <algorithm>
#include <iostream>
#include <cassert>

quetzal::Color quetzal::Color::Transparent(0, 0, 0, 0);
quetzal::Color quetzal::Color::Black(0, 0, 0, 1);
quetzal::Color quetzal::Color::White(1, 1, 1, 1);

//------------------------------------------------------------------------------
quetzal::Color::Color() :
    m_red(0.0f),
    m_green(0.0f),
    m_blue(0.0f),
    m_alpha(1.0f),
    m_packed(0xff000000),
    m_bPackedValid(true)
{
}

//------------------------------------------------------------------------------
quetzal::Color::Color(const Color& c) :
    m_red(c.m_red),
    m_green(c.m_green),
    m_blue(c.m_blue),
    m_alpha(c.m_alpha),
    m_packed(c.m_packed),
    m_bPackedValid(c.m_bPackedValid)
{
}

//------------------------------------------------------------------------------
quetzal::Color::Color(rgba_component_type red, rgba_component_type green, rgba_component_type blue, rgba_component_type alpha) :
    m_red(red),
    m_green(green),
    m_blue(blue),
    m_alpha(alpha),
    m_packed(0),
    m_bPackedValid(false)
{
}

//------------------------------------------------------------------------------
quetzal::Color::Color(rgba_packed_type rgba)
{
    set_packed(rgba);
}

//----------------------------------------------------------------------
quetzal::Color::rgba_packed_type quetzal::Color::packed() const
{
    if (!m_bPackedValid)
    {
        m_packed = calculate_packed_();
        m_bPackedValid = true;
    }

    return m_packed;
}

//------------------------------------------------------------------------------
void quetzal::Color::set_packed(rgba_packed_type rgba)
{
    m_packed = rgba;

    m_blue  = static_cast<rgba_component_type>(rgba & 0xff) / 255.f;
    rgba >>= 8;
    m_green = static_cast<rgba_component_type>(rgba & 0xff) / 255.f;
    rgba >>= 8;
    m_red   = static_cast<rgba_component_type>(rgba & 0xff) / 255.f;
    rgba >>= 8;
    m_alpha = static_cast<rgba_component_type>(rgba & 0xff) / 255.f;

    m_bPackedValid = true;

    return;
}

//----------------------------------------------------------------------
quetzal::Color::rgba_component_type quetzal::Color::red() const
{
    return m_red;
}

//----------------------------------------------------------------------
quetzal::Color::rgba_component_type quetzal::Color::green() const
{
    return m_green;
}

//----------------------------------------------------------------------
quetzal::Color::rgba_component_type quetzal::Color::blue() const
{
    return m_blue;
}

//----------------------------------------------------------------------
quetzal::Color::rgba_component_type quetzal::Color::alpha() const
{
    return m_alpha;
}

//----------------------------------------------------------------------
void quetzal::Color::set_red(rgba_component_type red)
{
    m_bPackedValid = false;
    m_red = red;

    return;
}

//----------------------------------------------------------------------
void quetzal::Color::set_green(rgba_component_type green)
{
    m_bPackedValid = false;
    m_green = green;

    return;
}

//----------------------------------------------------------------------
void quetzal::Color::set_blue(rgba_component_type blue)
{
    m_bPackedValid = false;
    m_blue = blue;

    return;
}

//----------------------------------------------------------------------
void quetzal::Color::set_alpha(rgba_component_type alpha)
{
    m_bPackedValid = false;
    m_alpha = alpha;

    return;
}

//----------------------------------------------------------------------
void quetzal::Color::set_rgba(rgba_component_type red, rgba_component_type green, rgba_component_type blue, rgba_component_type alpha)
{
    m_bPackedValid = false;
    m_red = red;
    m_green = green;
    m_blue = blue;
    m_alpha = alpha;

    return;
}

//------------------------------------------------------------------------------
quetzal::Color::hsl_component_type quetzal::Color::hue() const
{
    rgba_component_type cMax = std::max(std::max(m_red, m_green), m_blue);
    rgba_component_type cMin = std::min(std::min(m_red, m_green), m_blue);

    hsl_component_type h;

    if (cMax == cMin)
    {
        h = 0;
    }
    else
    {
        if (cMax == m_red)
        {
            h = (m_green - m_blue) / (cMax - cMin);
        }
        else if (cMax == m_green)
        {
            h = 2 + (m_blue - m_red) / (cMax - cMin);
        }
        else
        {
            h = 4 + (m_red - m_green) / (cMax - cMin);
        }
    }

    h /= 6;
    if (h < 0)
    {
        h += 1;
    }

    return h;
}

//------------------------------------------------------------------------------
quetzal::Color::hsl_component_type quetzal::Color::saturation() const
{
    rgba_component_type cMax = std::max(std::max(m_red, m_green), m_blue);
    rgba_component_type cMin = std::min(std::min(m_red, m_green), m_blue);

    hsl_component_type s;

    if (cMax == cMin)
    {
        s = 0;
    }
    else
    {
        hsl_component_type lightness = (cMax + cMin) / 2;
        if (lightness < 0.5)
        {
            s = (cMax - cMin) / (cMax + cMin);
        }
        else
        {
            s = (cMax - cMin) / (2 - cMax - cMin);
        }
    }

    return s;
}

//------------------------------------------------------------------------------
quetzal::Color::hsl_component_type quetzal::Color::lightness() const
{
    rgba_component_type cMax = std::max(std::max(m_red, m_green), m_blue);
    rgba_component_type cMin = std::min(std::min(m_red, m_green), m_blue);

    return (cMax + cMin) / 2;
}

//------------------------------------------------------------------------------
void quetzal::Color::set_hsl(hsl_component_type hue, hsl_component_type saturation, hsl_component_type lightness, rgba_component_type alpha)
{
    m_alpha = alpha;

    if (saturation == 0)
    {
        m_red = lightness;
        m_green = lightness;
        m_blue = lightness;
    }
    else
    {
        hsl_component_type temp2;
        if (lightness < 0.5f)
        {
            temp2 = lightness * (1 + saturation);
        }
        else
        {
            temp2 = lightness + saturation - lightness * saturation;
        }

        hsl_component_type temp1 = 2 * lightness - temp2;

        hsl_component_type temp3[3];
        temp3[0] = hue + (1.0f / 3);
        temp3[1] = hue;
        temp3[2] = hue - (1.0f / 3);

        for (int n = 0; n < 3; n ++)
        {
            if (temp3[n] < 0)
            {
                temp3[n] ++;
            }
            if (temp3[n] > 1)
            {
                temp3[n] --;
            }

            if ((temp3[n] * 6) < 1)
            {
                temp3[n] = temp1 + (temp2 - temp1) * 6 * temp3[n];
            }
            else
            {
                if ((temp3[n] * 2) < 1)
                {
                    temp3[n] = temp2;
                }
                else
                {
                    if ((temp3[n] * 3) < 2)
                    {
                        temp3[n] = temp1 + (temp2 - temp1) * ((2.0f / 3) - temp3[n]) * 6;
                    }
                    else
                    {
                        temp3[n] = temp1;
                    }
                }
            }
        }

        m_red = temp3[0];
        m_green = temp3[1];
        m_blue = temp3[2];
    }

    m_bPackedValid = false;

    return;
}

//------------------------------------------------------------------------------
quetzal::Color::yuv_component_type quetzal::Color::y() const
{
    return 0.299f * m_red + 0.587f * m_green + 0.114f * m_blue;
}

//------------------------------------------------------------------------------
quetzal::Color::yuv_component_type quetzal::Color::u() const
{
    return -0.14713f * m_red - 0.28886f * m_green + 0.436f * m_blue;
}

//------------------------------------------------------------------------------
quetzal::Color::yuv_component_type quetzal::Color::v() const
{
    return 0.615f * m_red - 0.51499f * m_green - 0.10001f * m_blue;
}

//------------------------------------------------------------------------------
void quetzal::Color::set_yuv(yuv_component_type y, yuv_component_type u, yuv_component_type v, rgba_component_type alpha)
{
    m_alpha = alpha;

    m_red = y + 1.13983f * v;
    m_green = y - 0.39465f * u - 0.58060f * v;
    m_blue = y + 2.03211f * u;

    m_bPackedValid = false;

    return;
}

//------------------------------------------------------------------------------
quetzal::Color::rgba_packed_type quetzal::Color::calculate_packed_() const
{
    return (
        static_cast<rgba_packed_type>(m_alpha * 255.999) << 24 |
        static_cast<rgba_packed_type>(m_red * 255.999) << 16 |
        static_cast<rgba_packed_type>(m_green * 255.999) << 8 |
        static_cast<rgba_packed_type>(m_blue * 255.999));
}

//----------------------------------------------------------------------
quetzal::Color& quetzal::Color::operator=(rgba_packed_type c)
{
    set_packed(c);
    return *this;
}

//----------------------------------------------------------------------
quetzal::Color& quetzal::Color::operator=(Color other)
{
    using std::swap;

    swap(m_red, other.m_red);
    swap(m_green, other.m_green);
    swap(m_blue, other.m_blue);
    swap(m_alpha, other.m_alpha);
    swap(m_packed, other.m_packed);
    swap(m_bPackedValid, other.m_bPackedValid);

    return *this;
}

//----------------------------------------------------------------------
quetzal::Color quetzal::Color::operator*(const rgba_component_type f) const
{
    assert(f >= 0. && f <= 1.);

    return Color(m_red * f, m_green * f, m_blue * f, m_alpha * f); // make sure that for alpha this is mathematically correct ...
}

//----------------------------------------------------------------------
bool quetzal::Color::operator==(const Color& other) const
{
    return m_red == other.m_red && m_green == other.m_green && m_blue == other.m_blue && m_alpha == other.m_alpha;
}

//----------------------------------------------------------------------
bool quetzal::Color::operator!=(const Color& other) const
{
    return !(*this == other);
}

//----------------------------------------------------------------------
quetzal::Color::operator rgba_packed_type() const
{
    return packed();
}

//----------------------------------------------------------------------
std::istream& quetzal::operator>>(std::istream& is, Color& color)
{
    Color::rgba_component_type r;
    Color::rgba_component_type g;
    Color::rgba_component_type b;
    Color::rgba_component_type a;

    is >> r >> g >> b >> a;

    color.set_rgba(r, g, b, a);

    return is;
}

//----------------------------------------------------------------------
std::ostream& quetzal::operator<<(std::ostream& os, const Color& color)
{
    os << color.red() << " " << color.green() << " " << color.blue() << " " << color.alpha();

    return os;
}
