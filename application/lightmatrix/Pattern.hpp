#if !defined(QUETZAL_LIGHTMATRIX_PATTERN_HPP)
#define QUETZAL_LIGHTMATRIX_PATTERN_HPP
//------------------------------------------------------------------------------
// Light Matrix Simulation
// Copyright (c) 2010 Keith Michael Looney. All Rights Reserved.
// Pattern.hpp
//------------------------------------------------------------------------------

#include <string>
#include "Frame.hpp"

namespace quetzal
{

namespace lightmatrix
{

    //----------------------------------------------------------------------
    enum class Smoothing
    {
        Null,
        Nearest,
        Linear,
        Cubic
    };

    //----------------------------------------------------------------------
    class Pattern
    {
    public:

        Pattern();
        Pattern(const Pattern&) = default;
        ~Pattern() = default;

        Pattern& operator=(const Pattern&) = default;

        const Frame& frame() const;
        void set_frame(const Frame& frame);

        float viewport_width() const;
        float viewport_height() const;
        void set_viewport_size(float w, float h);

        // Layout

        int display_width() const;
        int display_height() const;
        void set_display_size(int w, int h);

        bool radial() const;
        void set_radial(bool bRadial);

        float pixel_radius() const;
        void set_pixel_radius(float r);

        float spacing_x() const;
        float spacing_y() const;
        void set_spacing(float sx, float sy);

        // Animation

        bool shift_color() const;
        void set_shift_color(bool bShift);

        bool shift_brightness() const;
        void set_shift_brightness(bool bShift);

        Smoothing smoothing() const;
        void set_smoothing(Smoothing s);

        float direction_x() const;
        float direction_y() const;
        void set_direction(float dx, float dy);

        float velocity_x() const;
        float velocity_y() const;
        void set_velocity(float vx, float vy);

        void load(const std::string& strFile);
        void store(const std::string& strFile);

    private:

        // Original frame data
        Frame m_frame;

        // Area of original frame data that maps to display
        float m_wViewport;
        float m_hViewport;

        // Layout
        int m_wDisplay;
        int m_hDisplay;
        bool m_bRadial;
        float m_rPixel;
        float m_sx; // rectangular: pixel x spacing; radial: radius of innermost ring
        float m_sy; // rectangular: pixel y spacing; radial: spacing between rings

        // Animation
        bool m_bShiftColor;
        bool m_bShiftBrightness;
        Smoothing m_smoothing;
        float m_dx;
        float m_dy;
        float m_vx;
        float m_vy;
    };

} // namespace lightmatrix

} // namespace quetzal

#endif // QUETZAL_LIGHTMATRIX_PATTERN_HPP
