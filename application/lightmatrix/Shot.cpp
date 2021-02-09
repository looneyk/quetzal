//------------------------------------------------------------------------------
// Light Matrix Simulation
// Shot.cpp
//------------------------------------------------------------------------------

#include "Shot.hpp"
#include <DirectXMath.h> // Pi
#include <cmath>

using namespace std;

//------------------------------------------------------------------------------
quetzal::lightmatrix::Shot::Shot() :
    m_mutex(),
    m_frame(),
    m_pattern(),
    m_xViewport(0.0f),
    m_yViewport(0.0f),
    m_interpolation_index_x(),
    m_interpolation_index_y()
{
}

//------------------------------------------------------------------------------
const quetzal::lightmatrix::Frame& quetzal::lightmatrix::Shot::frame() const
{
    return m_frame;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Shot::load_pattern(const string& path)
{
    lock_guard<mutex> lock(m_mutex);

    m_pattern.load(path);
    size_changed();

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Shot::set_radial(bool bRadial)
{
    m_pattern.set_radial(bRadial);

    layout_changed();

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Shot::toggle_shift_color()
{
    m_pattern.set_shift_color(!m_pattern.shift_color());
    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Shot::toggle_shift_brightness()
{
    m_pattern.set_shift_brightness(!m_pattern.shift_brightness());
    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Shot::cycle_smoothing()
{
    switch (m_pattern.smoothing())
    {
        case Smoothing::Nearest:
            m_pattern.set_smoothing(Smoothing::Linear);
            break;

        case Smoothing::Linear:
            m_pattern.set_smoothing(Smoothing::Cubic);
            break;

        case Smoothing::Cubic:
            m_pattern.set_smoothing(Smoothing::Nearest);
            break;
    }
    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Shot::set_direction(float dx, float dy)
{
    m_pattern.set_direction(dx, dy);
    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Shot::increment_velocity(float vx, float vy)
{
    m_pattern.set_velocity(m_pattern.velocity_x() + vx, m_pattern.velocity_y() + vy);
    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Shot::update(float dt)
{
    lock_guard<mutex> lock(m_mutex);

    m_xViewport += m_pattern.direction_x() * m_pattern.velocity_x() * dt;
    m_xViewport -= floor(m_xViewport);

    m_yViewport += m_pattern.direction_y() * m_pattern.velocity_y() * dt;
    m_yViewport -= floor(m_yViewport);

    update_frame();

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Shot::size_changed()
{
    const int w = m_pattern.display_width();
    const int h = m_pattern.display_height();

    m_interpolation_index_x.resize(w * 2);
    m_interpolation_index_y.resize(h * 2);

    for (int i = 0; i < w; ++i)
    {
        m_interpolation_index_x[i] = i;
        m_interpolation_index_x[i + w] = i;
    }

    for (int j = 0; j < h; ++j)
    {
        m_interpolation_index_y[j] = j;
        m_interpolation_index_y[j + h] = j;
    }

    layout_changed();

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Shot::layout_changed()
{
    int w = m_pattern.display_width();
    int h = m_pattern.display_height();

    m_frame.set_size(w, h);
    m_xViewport = 0.0f;
    m_yViewport = 0.0f;

    if (w == 0 || h == 0)
    {
        return;
    }

    update_frame();

    float rPixel = m_pattern.pixel_radius();

    if (m_pattern.radial())
    {
        float r = m_pattern.spacing_x();
        float dphi = DirectX::XM_2PI / w;

        for (int j = 0; j < h; ++j)
        {
            float phi = -DirectX::XM_PIDIV2;

            for (int i = 0; i < w; ++i)
            {
                float x = r * cos(phi);
                float y = r * sin(phi);

                Pixel& pixel = m_frame.pixel(i, j);
                pixel.set_radius(rPixel);
                pixel.set_position(x, y);

                phi += dphi;
            }

            r += m_pattern.spacing_y();
        }
    }
    else
    {
        float y = m_pattern.spacing_y() * (-(h - 1) / 2.0f);

        for (int j = 0; j < h; ++j)
        {
            float x = m_pattern.spacing_x() * (-(w - 1) / 2.0f);

            for (int i = 0; i < w; ++i)
            {
                Pixel& pixel = m_frame.pixel(i, j);
                pixel.set_radius(rPixel);
                pixel.set_position(x, y);

                x += m_pattern.spacing_x();
            }

            y += m_pattern.spacing_y();
        }
    }

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Shot::update_frame()
{
    assert(m_xViewport >= 0.0f && m_xViewport < 1.0f);
    assert(m_yViewport >= 0.0f && m_yViewport < 1.0f);

    const int w = m_frame.width();
    const int h = m_frame.height();

    int nx = int(m_xViewport * w);
    float wx1 = m_xViewport - float(nx) / w;
    assert(wx1 >= 0.0f && wx1 < 1.0f);

    int ny = int(m_yViewport * h);
    float wy1 = m_yViewport - float(ny) / h;
    assert(wy1 >= 0.0f && wy1 < 1.0f);

    switch (m_pattern.smoothing())
    {
        case Smoothing::Nearest:
            update_frame_nearest(nx, wx1, ny, wy1);
            break;

        case Smoothing::Linear:
            update_frame_linear(nx, wx1, ny, wy1);
            break;

        case Smoothing::Cubic:
            update_frame_cubic(nx, wx1, ny, wy1);
            break;
    }

    return;
}

//------------------------------------------------------------------------------
void  quetzal::lightmatrix::Shot::update_frame_nearest(int nx0, float wx1, int ny0, float wy1)
{
    const int w = m_frame.width();
    const int h = m_frame.height();

    if (wx1 >= 0.5f)
    {
        if (++nx0 >= w)
        {
            nx0 = 0;
        }
    }

    if (wy1 >= 0.5f)
    {
        if (++ny0 >= h)
        {
            ny0 = 0;
        }
    }

    const Frame& frameOrig = m_pattern.frame();

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            const Pixel& pixelOrig = frameOrig.pixel(x, y);
            Pixel& pixel = m_frame.pixel(m_interpolation_index_x[x + nx0], m_interpolation_index_y[y + ny0]);

            if (m_pattern.shift_color())
            {
                pixel.set_rgb(pixelOrig.color());
            }

            if (m_pattern.shift_brightness())
            {
                pixel.set_a(pixelOrig.color());
            }
        }
    }

    return;
}

//------------------------------------------------------------------------------
void  quetzal::lightmatrix::Shot::update_frame_linear(int nx0, float wx1, int ny0, float wy1)
{
    const int w = m_frame.width();
    const int h = m_frame.height();

    int nx1 = nx0 + 1;
    if (nx1 >= w)
    {
        nx1 = 0;
    }

    int ny1 = ny0 + 1;
    if (ny1 >= h)
    {
        ny1 = 0;
    }

    float wx0 = 1.0f - wx1;
    float wy0 = 1.0f - wy1;

    float w00 = wx0 * wy0;
    float w01 = wx0 * wy1;
    float w10 = wx1 * wy0;
    float w11 = wx1 * wy1;

    const Frame& frameOrig = m_pattern.frame();

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            const Color& color00 = frameOrig.pixel(m_interpolation_index_x[x + w - nx0], m_interpolation_index_y[y + h - ny0]).color();
            const Color& color01 = frameOrig.pixel(m_interpolation_index_x[x + w - nx0], m_interpolation_index_y[y + h - ny1]).color();
            const Color& color10 = frameOrig.pixel(m_interpolation_index_x[x + w - nx1], m_interpolation_index_y[y + h - ny0]).color();
            const Color& color11 = frameOrig.pixel(m_interpolation_index_x[x + w - nx1], m_interpolation_index_y[y + h - ny1]).color();

            Pixel& pixel = m_frame.pixel(x, y);

            if (m_pattern.shift_color())
            {
                pixel.set_rgb(
                    w00 * color00.r() + w01 * color01.r() * w10 * color10.r() + w11 * color11.r(),
                    w00 * color00.g() + w01 * color01.g() * w10 * color10.g() + w11 * color11.g(),
                    w00 * color00.b() + w01 * color01.b() * w10 * color10.b() + w11 * color11.b());
            }

            if (m_pattern.shift_brightness())
            {
                pixel.set_a(w00 * color00.a() + w01 * color01.a() * w10 * color10.a() + w11 * color11.a());
            }
        }
    }

    return;
}

//------------------------------------------------------------------------------
void  quetzal::lightmatrix::Shot::update_frame_cubic(int nx1, float wx1, int ny1, float wy1)
{
nx1; wx1; ny1; wy1; // ...

    return;
}
