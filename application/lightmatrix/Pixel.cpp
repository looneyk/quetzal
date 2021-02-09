//------------------------------------------------------------------------------
// Light Matrix Simulation
// Copyright (c) 2010 Keith Michael Looney. All Rights Reserved.
// Pixel.cpp
//------------------------------------------------------------------------------

#include <iostream>
#include <cassert>
#include "Pixel.hpp"

using namespace std;

//------------------------------------------------------------------------------
quetzal::lightmatrix::Pixel::Pixel() :
    m_radius(1.0f),
    m_x(0.0f),
    m_y(0.0f),
    m_color()
{
}

//------------------------------------------------------------------------------
float quetzal::lightmatrix::Pixel::radius() const
{
    return m_radius;
}

//------------------------------------------------------------------------------
float quetzal::lightmatrix::Pixel::x() const
{
    return m_x;
}

//------------------------------------------------------------------------------
float quetzal::lightmatrix::Pixel::y() const
{
    return m_y;
}

//------------------------------------------------------------------------------
quetzal::lightmatrix::Color quetzal::lightmatrix::Pixel::color() const
{
    return m_color;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Pixel::set_layout(const Pixel& pixel)
{
    m_radius = pixel.m_radius;
    m_x = pixel.m_x;
    m_y = pixel.m_y;
    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Pixel::set_radius(float r)
{
    m_radius = r;
    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Pixel::set_position(float x, float y)
{
    m_x = x;
    m_y = y;
    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Pixel::set_color(Color color)
{
    m_color = color;

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Pixel::set_rgb(Color color)
{
    m_color.set_rgb(color);

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Pixel::set_a(Color color)
{
    m_color.set_a(color.a());

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Pixel::set_rgb(Color::component_type r, Color::component_type g, Color::component_type b)
{
    m_color.set_rgb(r, g, b);

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Pixel::set_a(Color::component_type a)
{
    m_color.set_a(a);

    return;
}

//------------------------------------------------------------------------------
ostream& quetzal::lightmatrix::operator<<(ostream& os, const quetzal::lightmatrix::Pixel& pixel)
{
    os << pixel.color();

    return os;
}

//------------------------------------------------------------------------------
istream& quetzal::lightmatrix::operator>>(istream& is, quetzal::lightmatrix::Pixel& pixel)
{
    Color color;

    is >> color;
    pixel.set_color(color);

    return is;
}
