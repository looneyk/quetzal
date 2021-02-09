//------------------------------------------------------------------------------
// Light Matrix Simulation
// Copyright (c) 2010 Keith Michael Looney. All Rights Reserved.
// Color.cpp
//------------------------------------------------------------------------------

#include <iostream>
#include "Color.hpp"

using namespace std;

//------------------------------------------------------------------------------
quetzal::lightmatrix::Color::Color() :
    Color(0.0f, 0.0f, 0.0f, 1.0f)
{
}

//------------------------------------------------------------------------------
quetzal::lightmatrix::Color::Color(component_type r, component_type g, component_type b, component_type a) :
    m_r(r),
    m_g(g),
    m_b(b),
    m_a(a)
{
}

//------------------------------------------------------------------------------
quetzal::lightmatrix::Color::component_type quetzal::lightmatrix::Color::r() const
{
    return m_r;
}

//------------------------------------------------------------------------------
quetzal::lightmatrix::Color::component_type quetzal::lightmatrix::Color::g() const
{
    return m_g;
}

//------------------------------------------------------------------------------
quetzal::lightmatrix::Color::component_type quetzal::lightmatrix::Color::b() const
{
    return m_b;
}

//------------------------------------------------------------------------------
quetzal::lightmatrix::Color::component_type quetzal::lightmatrix::Color::a() const
{
    return m_a;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Color::set_rgb(const Color& color)
{
    m_r = color.m_r;
    m_g = color.m_g;
    m_b = color.m_b;

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Color::set_rgb(component_type r, component_type g, component_type b)
{
    m_r = r;
    m_g = g;
    m_b = b;

    return;
}

//------------------------------------------------------------------------------
void quetzal::lightmatrix::Color::set_a(component_type a)
{
    m_a = a;

    return;
}

//------------------------------------------------------------------------------
ostream& quetzal::lightmatrix::operator<<(ostream& os, const quetzal::lightmatrix::Color& color)
{
    os << color.r() << " " << color.g() << " " << color.b() << " " << color.a();

    return os;
}

//------------------------------------------------------------------------------
istream& quetzal::lightmatrix::operator>>(istream& is, quetzal::lightmatrix::Color& color)
{
    Color::component_type r, g, b, a;

    is >> r >> g >> b >> a;
    color = {r, g, b, a};

    return is;
}
