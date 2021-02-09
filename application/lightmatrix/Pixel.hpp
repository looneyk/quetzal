#if !defined(QUETZAL_LIGHTMATRIX_PIXEL_HPP)
#define QUETZAL_LIGHTMATRIX_PIXEL_HPP
//------------------------------------------------------------------------------
// Light Matrix Simulation
// Copyright (c) 2010 Keith Michael Looney. All Rights Reserved.
// Pixel.hpp
//------------------------------------------------------------------------------

#include <iosfwd>
#include "Color.hpp"

namespace quetzal
{

namespace lightmatrix
{

    //----------------------------------------------------------------------
    class Pixel
    {
    public:

        typedef float component_type; // 0.0 .. 1.0

        Pixel();
        Pixel(const Pixel&) = default;
        ~Pixel() = default;

        Pixel& operator=(const Pixel&) = default;

        float radius() const;
        float x() const;
        float y() const;

        Color color() const;

        void set_layout(const Pixel& pixel);

        void set_radius(float r);
        void set_position(float x, float y);

        void set_color(Color color);
        void set_rgb(Color color);
        void set_a(Color color);

        void set_rgb(Color::component_type r, Color::component_type g, Color::component_type b);
        void set_a(Color::component_type a);

    private:

        float m_radius;

        float m_x;
        float m_y;

        Color m_color;
    };

    // These write and read only color values
    std::ostream& operator<<(std::ostream& os, const quetzal::lightmatrix::Pixel& pixel);
    std::istream& operator>>(std::istream& is, quetzal::lightmatrix::Pixel& pixel);

} // namespace lightmatrix

} // namespace quetzal

#endif // QUETZAL_LIGHTMATRIX_PIXEL_HPP
