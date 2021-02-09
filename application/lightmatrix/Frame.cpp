//------------------------------------------------------------------------------
// Light Matrix Simulation
// Frame.cpp
// Copyright (c) 2010-2017 Keith Michael Looney. All Rights Reserved.
//------------------------------------------------------------------------------

#include <iostream>
#include <cassert>
#include "Frame.hpp"

using namespace std;

//------------------------------------------------------------------------------
quetzal::lightmatrix::Frame::Frame() :
    Frame(0, 0)
{
}

//------------------------------------------------------------------------------
quetzal::lightmatrix::Frame::Frame(int w, int h) :
    m_w(w),
    m_h(h),
    m_pixels(w * h)
{
}

//------------------------------------------------------------------------------
int quetzal::lightmatrix::Frame::width() const
{
    return m_w;
}

//------------------------------------------------------------------------------
int quetzal::lightmatrix::Frame::height() const
{
    return m_h;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Frame::set_size(int w, int h)
{
    m_w = w;
    m_h = h;

    m_pixels.resize(m_w * m_h);

    return;
}

//------------------------------------------------------------------------------
const quetzal::lightmatrix::Pixel& quetzal::lightmatrix::Frame::pixel(int x, int y) const
{
    assert(x < m_w);
    assert(y < m_h);

    return m_pixels[y * m_w + x];
}

//------------------------------------------------------------------------------
quetzal::lightmatrix::Pixel& quetzal::lightmatrix::Frame::pixel(int x, int y)
{
    assert(x < m_w);
    assert(y < m_h);

    return m_pixels[y * m_w + x];
}

//------------------------------------------------------------------------------
const quetzal::lightmatrix::Frame::Pixels& quetzal::lightmatrix::Frame::pixels() const
{
    return m_pixels;
}

//------------------------------------------------------------------------------
quetzal::lightmatrix::Frame::Pixels& quetzal::lightmatrix::Frame::pixels()
{
    return m_pixels;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::blend(const Frame& fa, float wa, const Frame& fb, float wb, Frame& f)
{
    assert(fa.pixels().size() == f.pixels().size());
    assert(fb.pixels().size() == f.pixels().size());

    auto ia = fa.pixels().begin();
    auto ib = fb.pixels().begin();

    for (Pixel& p : f.pixels())
    {
        Color ca = ia->color();
        Color cb = ib->color();

        p.set_color({
            ca.r() * ca.a() * wa + cb.r() * cb.a() * wb,
            ca.g() * ca.a() * wa + cb.g() * cb.a() * wb,
            ca.b() * ca.a() * wa + cb.b() * cb.a() * wb,
            ca.a() * wa + cb.a() * wb}); // weighted average is not the right operation here ...

        ++ia;
        ++ib;
    }

    return;
}

//------------------------------------------------------------------------------
ostream& quetzal::lightmatrix::operator<<(ostream& os, const Frame& frame)
{
    const int w = frame.width();
    const int h = frame.height();

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            os << frame.pixel(x, y);
            if (x < w - 1)
            {
                os << " ";
            }
            else
            {
                os << endl;
            }
        }
    }

    return os;
}
