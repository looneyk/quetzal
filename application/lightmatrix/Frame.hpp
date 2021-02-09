#if !defined(QUETZAL_LIGHTMATRIX_FRAME_HPP)
#define QUETZAL_LIGHTMATRIX_FRAME_HPP
//------------------------------------------------------------------------------
// Light Matrix Simulation
// Frame.hpp
// Copyright (c) 2010-2017 Keith Michael Looney. All Rights Reserved.
//------------------------------------------------------------------------------

#include <vector>
#include <iosfwd>
#include "Pixel.hpp"

namespace quetzal
{

namespace lightmatrix
{

    //----------------------------------------------------------------------
    class Frame
    {
    public:

        typedef std::vector<Pixel> Pixels;

        Frame();
        Frame(int w, int h);
        Frame(const Frame&) = default;
        ~Frame() = default;

        Frame& operator=(const Frame&) = default;

        int width() const;
        int height() const;

        void set_size(int w, int h);

        const Pixel& pixel(int x, int y) const;
        Pixel& pixel(int x, int y);

        const Pixels& pixels() const;
        Pixels& pixels();

    private:

        int m_w;
        int m_h;

        Pixels m_pixels;
    };

    void blend(const Frame& fa, float wa, const Frame& fb, float wb, Frame& f);

    std::ostream& operator<<(std::ostream& os, const Frame& frame);

} // namespace lightmatrix

} // namespace quetzal

#endif // QUETZAL_LIGHTMATRIX_FRAME_HPP
