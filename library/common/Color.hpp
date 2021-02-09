//------------------------------------------------------------------------------
// Color.hpp
//
//------------------------------------------------------------------------------

#if !defined(QUETZAL_COLOR_HPP)
#define QUETZAL_COLOR_HPP

#include <cstdint>
#include <iosfwd>

namespace quetzal
{

    //----------------------------------------------------------------------
    class Color
    {
    public:

        static Color Transparent;
        static Color Black;
        static Color White;

        typedef uint32_t rgba_packed_type; // 32 bit packed ARGB representation
        typedef uint8_t rgba_packed_component_type;

         // 0 <= component <= 1
        typedef float rgba_component_type;
        typedef float hsl_component_type;
        typedef float yuv_component_type;

        Color();
        Color(const Color& c);
        Color(rgba_component_type red, rgba_component_type green, rgba_component_type blue, rgba_component_type alpha = rgba_component_type(1));
        explicit Color(rgba_packed_type rgba); // Potentially dangerous ...

        rgba_packed_type packed() const;
        void set_packed(rgba_packed_type rgba);

        rgba_component_type red() const;
        rgba_component_type green() const;
        rgba_component_type blue() const;
        rgba_component_type alpha() const;

        void set_red(rgba_component_type red);
        void set_green(rgba_component_type green);
        void set_blue(rgba_component_type blue);
        void set_alpha(rgba_component_type alpha);

        void set_rgba(rgba_component_type red, rgba_component_type green, rgba_component_type blue, rgba_component_type alpha = rgba_component_type(1));

        // get_hsl or optimize these if they are called often (hsvValid?)
        hsl_component_type hue() const;
        hsl_component_type saturation() const;
        hsl_component_type lightness() const;

        void set_hsl(hsl_component_type hue, hsl_component_type saturation, hsl_component_type lightness, rgba_component_type alpha = rgba_component_type(1));

        // Using 601 coefficients ...
        yuv_component_type y() const;
        yuv_component_type u() const;
        yuv_component_type v() const;

        void set_yuv(yuv_component_type y, yuv_component_type u, yuv_component_type v, rgba_component_type alpha = rgba_component_type(1));

        // Operators

        Color& operator=(rgba_packed_type c);
        Color& operator=(Color other);

        Color operator*(const rgba_component_type f) const;

        bool operator==(const Color& other) const;
        bool operator!=(const Color& other) const;

        operator rgba_packed_type() const; // Potentially dangerous ...

    private:

        rgba_packed_type calculate_packed_() const;

        rgba_component_type m_red;
        rgba_component_type m_green;
        rgba_component_type m_blue;
        rgba_component_type m_alpha;

        mutable rgba_packed_type m_packed;
        mutable bool m_bPackedValid;
    };

    std::istream& operator>>(std::istream& is, Color& color);
    std::ostream& operator<<(std::ostream& os, const Color& color);

} // namespace quetzal

#endif // QUETZAL_COLOR_HPP
