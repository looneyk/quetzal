#if !defined(QUETZAL_LIGHTMATRIX_CONTROLLER_HPP)
#define QUETZAL_LIGHTMATRIX_CONTROLLER_HPP
//------------------------------------------------------------------------------
// Light Matrix Simulation
// Copyright (c) 2010 Keith Michael Looney. All Rights Reserved.
// Controller.hpp
//------------------------------------------------------------------------------

#include <string>
#include "Frame.hpp"
#include "Shot.hpp"

namespace quetzal
{

namespace lightmatrix
{

    //----------------------------------------------------------------------
    class Controller
    {
    public:

        Controller(Frame& frame);
        Controller(const Controller&) = delete;
        ~Controller() = default;

        Controller& operator=(const Controller&) = delete;

        const Frame& frame() const;

        void load_pattern(const std::string& path, float tDissolve = 0.0f);

        void set_radial(bool bRadial);
        void toggle_shift_color();
        void toggle_shift_brightness();
        void cycle_smoothing();
        void set_direction(float dx, float dy);
        void increment_velocity(float vx, float vy);

        void update(float dt);

    private:

        void size_changed();
        void layout_changed();

        void update_();
        void update_nearest_(int nx0, float wx1, int ny0, float wy1);
        void update_linear_(int nx0, float wx1, int ny0, float wy1);
        void update_cubic_(int nx1, float wx1, int ny1, float wy1);

        Frame& m_frame;

        Shot m_shot0;
        Shot m_shot1;

        Shot* m_pShotFront;
        Shot* m_pShotBack;

        // Transition
        bool m_bTransition;
        float m_tTransition;
        float m_tDissolve;

        bool m_bFirst;
    };

} // namespace lightmatrix

} // namespace quetzal

#endif // QUETZAL_LIGHTMATRIX_CONTROLLER_HPP
