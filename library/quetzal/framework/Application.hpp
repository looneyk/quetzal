#if !defined(QUETZAL_FRAMEWORK_APPLICATION_HPP)
#define QUETZAL_FRAMEWORK_APPLICATION_HPP
//------------------------------------------------------------------------------
// quetzal framework
// Application.hpp
//------------------------------------------------------------------------------

#include "Window.hpp"
//#include "Renderer.hpp"
#include "quetzal/common/Timer.hpp"
#include "quetzal/input/keyboard.hpp"
#include "quetzal/input/RawInput.hpp"
#include <thread>

namespace quetzal::framework
{

    //----------------------------------------------------------------------
    class Application
    {
    public:

        typedef Window::position_type position_type;
        typedef Window::size_type size_type;

        Application();
        Application(const Application&) = delete;
        virtual ~Application() = default;

        Application& operator=(const Application&) = delete;

        int run();

        float frame_rate() const;

private: // ok, so why wasn't it? ...
        bool message_handler(UINT uMsg, WPARAM wParam, LPARAM lParam);

    protected:

        virtual void update(float dt) = 0;
        virtual void render() = 0;

        void on_frame();
        void on_size(size_type size);
        void on_suspend();
        void on_resume();
        bool on_close();

        virtual void on_key_down(Timestamp ts, input::key::Modifiers modifiers, input::key::Code code);
        virtual void on_key_up(Timestamp ts, input::key::Modifiers modifiers, input::key::Code code);
        virtual void on_char(Timestamp ts, input::key::Modifiers modifiers, const std::string& utf8);
        virtual void on_mouse_down(Timestamp ts, input::key::Modifiers modifiers, position_type position, input::key::Code code);
        virtual void on_mouse_up(Timestamp ts, input::key::Modifiers modifiers, position_type position, input::key::Code code);
        virtual void on_mouse_move(Timestamp ts, input::key::Modifiers modifiers, position_type position);
        virtual void on_mouse_scroll(Timestamp ts, input::key::Modifiers modifiers, position_type position);

        Window m_window;
//        Renderer m_renderer;
        bool m_bSuspended;
        bool m_bMinimized;
        bool m_bSizing;
        Timer m_timer;
        Timestamp m_tsMessage;
        int m_nFrameStatistics;
        float m_tStatisticsPrev;
        float m_frame_rate;

    private:

        virtual void on_frame_();
        virtual void on_size_(size_type size);
        virtual void on_suspend_();
        virtual void on_resume_();
        virtual bool on_close_();

        input::RawInput m_raw_input; // Mouse
    };

} // namespace quetzal::framework

#endif // QUETZAL_FRAMEWORK_APPLICATION_HPP
