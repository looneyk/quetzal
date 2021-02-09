#if !defined(QUETZAL_LIGHTMATRIX_SHOT_HPP)
#define QUETZAL_LIGHTMATRIX_SHOT_HPP
//------------------------------------------------------------------------------
// Light Matrix Simulation
// Shot.hpp
//------------------------------------------------------------------------------

#include "Pattern.hpp"
#include <string>
#include <vector>
#include <mutex>

namespace quetzal
{

namespace lightmatrix
{

    //----------------------------------------------------------------------
    class Shot
    {
    public:

        Shot();
        Shot(const Shot&) = delete;
        ~Shot() = default;

        Shot& operator=(Shot&) = delete;

        const Frame& frame() const;

        void load_pattern(const std::string& path);

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

        void update_frame();
        void update_frame_nearest(int nx0, float wx1, int ny0, float wy1);
        void update_frame_linear(int nx0, float wx1, int ny0, float wy1);
        void update_frame_cubic(int nx1, float wx1, int ny1, float wy1);

        std::mutex m_mutex;

        Frame m_frame;

        Pattern m_pattern;

        float m_xViewport;
        float m_yViewport;

        std::vector<int> m_interpolation_index_x;
        std::vector<int> m_interpolation_index_y;
    };

} // namespace lightmatrix

} // namespace quetzal

#endif // QUETZAL_LIGHTMATRIX_SHOT_HPP
