//------------------------------------------------------------------------------
// quetzal framework
// Application.cpp
//------------------------------------------------------------------------------

#include "Application.hpp"
#include "quetzal/common/string_util.hpp"
#include <vector>
#include <cassert>

using namespace std;

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

//------------------------------------------------------------------------------
quetzal::framework::Application::Application() :
    m_window(bind(&quetzal::framework::Application::message_handler, this, _1, _2, _3)),
//    m_renderer(m_window, false), // ...
    m_bSuspended(false),
    m_bMinimized(false),
    m_bSizing(false),
    m_timer(),
    m_tsMessage(),
    m_nFrameStatistics(0),
    m_tStatisticsPrev(0.f),
    m_frame_rate(0.f),
    m_raw_input()
{
}

//------------------------------------------------------------------------------
int quetzal::framework::Application::run()
{
    MSG msg = {NULL, WM_NULL};

    m_timer.reset();

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) // Process window messages
        {
            m_tsMessage.update(); // msg.time does not have sufficient resolution

            TranslateMessage(&msg); // Only needed for WM_CHAR generation
            DispatchMessage(&msg);
        }
        else // Frame updating
        {
            if (m_bSuspended) // Give other applications some extra execution time
            {
                this_thread::sleep_for(10ms);
                continue;
            }

            m_timer.tick();
            on_frame();
            update(static_cast<float>(m_timer.delta()));
            render();
        }
    }

    return static_cast<int>(msg.wParam);
}

//------------------------------------------------------------------------------
float quetzal::framework::Application::frame_rate() const
{
    return m_frame_rate;
}

//------------------------------------------------------------------------------
bool quetzal::framework::Application::message_handler(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_ACTIVATE:
            m_bSuspended = LOWORD(wParam) == WA_INACTIVE;
            if (m_bSuspended)
            {
                m_timer.stop();
                on_suspend();
            }
            else
            {
                on_resume();
                m_timer.start();
            }
            return true;

        case WM_SIZE:
            if (wParam == SIZE_MINIMIZED)
            {
                assert(!m_bMinimized);
                m_bMinimized = true;
                if (!m_bSuspended)
                {
                    m_bSuspended = true;
                    on_suspend();
                }
            }
//            else if (wParam != SIZE_MAXHIDE && wParam != SIZE_MAXSHOW) // ...
            else if (m_bMinimized)
            {
                assert(m_bSuspended);
                m_bMinimized = false;
                m_bSuspended = false;
                on_resume();
                on_size({LOWORD(lParam), HIWORD(lParam)});
            }
            else if (!m_bSizing)
            {
                on_size({LOWORD(lParam), HIWORD(lParam)});
            }

            return true;

        case WM_ENTERSIZEMOVE:
            m_timer.stop();
            m_bSuspended = true;
            m_bSizing = true;
            return true;

        case WM_EXITSIZEMOVE:
            on_size(m_window.client_size());
            m_bSizing = false;
            m_bSuspended = false;
            m_timer.start();
            return true;

        case WM_CLOSE:
        {
            bool b = on_close();
            if (b)
            {
                m_window.destroy();
            }
            return true;
        }

        case WM_KEYDOWN:
        {
            input::key::Code code = input::translate_key(static_cast<input::key::Code>(wParam), static_cast<unsigned int>(lParam));
            on_key_down(m_tsMessage, input::query_key_modifiers(), code);
            return true;
        }

        case WM_KEYUP:
        {
            input::key::Code code = input::translate_key(static_cast<input::key::Code>(wParam), static_cast<unsigned int>(lParam));
            on_key_up(m_tsMessage, input::query_key_modifiers(), code);
            return true;
        }

        case WM_SYSKEYDOWN:
        {
            input::key::Code code = input::translate_key(static_cast<input::key::Code>(wParam), static_cast<unsigned int>(lParam));
            on_key_down(m_tsMessage, input::query_key_modifiers(), code);
            return true; // break; ? probably only on alt tab if allowed ...
        }

        case WM_SYSKEYUP:
        {
            input::key::Code code = input::translate_key(static_cast<input::key::Code>(wParam), static_cast<unsigned int>(lParam));
            on_key_up(m_tsMessage, input::query_key_modifiers(), code);
            return true;
        }

        case WM_CHAR:
        {
            wstring wstr(1, static_cast<wchar_t>(wParam)); // utf16 ...
            string str = to_string(wstr); // utf8
            on_char(m_tsMessage, input::query_key_modifiers(), str);
            return true;
        }

        case WM_INPUT:
        {
            const RAWINPUTHEADER& rih = m_raw_input.header(lParam);

            if (rih.wParam == RIM_INPUT)
            {
                if (rih.dwType == RIM_TYPEMOUSE)
                {
                    input::key::Modifiers modifiers = input::query_key_modifiers();
                    static position_type position;

                    const RAWMOUSE& rm = m_raw_input.mouse();

//m_ofs << "ri mouse " << x << ", " << y << ", " << std::hex << std::setw(4) << std::setfill('0') << rm.usButtonFlags << std::endl;
                    int x = rm.lLastX;
                    int y = rm.lLastY;
                    if (rm.usFlags & MOUSE_MOVE_ABSOLUTE)
                    {
                        position = {x, y};
                    }
                    else
                    {
//                        position += {x, y}; // pos_type operator += ...
                        position.x += x;
                        position.y += y;
                    }
                    // MOUSE_ATTRIBUTES_CHANGED, MOUSE_VIRTUAL_DESKTOP ...

                    switch (rm.usButtonFlags)
                    {
                        case RI_MOUSE_BUTTON_1_DOWN:
                            on_mouse_down(m_tsMessage, modifiers, position, input::key::Code::V_LBUTTON);
                            return true;

                        case RI_MOUSE_BUTTON_1_UP:
                            on_mouse_up(m_tsMessage, modifiers, position, input::key::Code::V_LBUTTON);
                            return true;

                        case RI_MOUSE_BUTTON_2_DOWN:
                            on_mouse_down(m_tsMessage, modifiers, position, input::key::Code::V_RBUTTON);
                            return true;

                        case RI_MOUSE_BUTTON_2_UP:
                            on_mouse_up(m_tsMessage, modifiers, position, input::key::Code::V_RBUTTON);
                            return true;

                        case RI_MOUSE_BUTTON_3_DOWN:
                            on_mouse_down(m_tsMessage, modifiers, position, input::key::Code::V_MBUTTON);
                            return true;

                        case RI_MOUSE_BUTTON_3_UP:
                            on_mouse_up(m_tsMessage, modifiers, position, input::key::Code::V_MBUTTON);
                            return true;

                        case RI_MOUSE_BUTTON_4_DOWN:
                            on_mouse_down(m_tsMessage, modifiers, position, input::key::Code::V_XBUTTON1);
                            return true;

                        case RI_MOUSE_BUTTON_4_UP:
                            on_mouse_up(m_tsMessage, modifiers, position, input::key::Code::V_XBUTTON1);
                            return true;

                        case RI_MOUSE_BUTTON_5_DOWN:
                            on_mouse_down(m_tsMessage, modifiers, position, input::key::Code::V_XBUTTON2);
                            return true;

                        case RI_MOUSE_BUTTON_5_UP:
                            on_mouse_up(m_tsMessage, modifiers, position, input::key::Code::V_XBUTTON2);
                            return true;

                        case RI_MOUSE_WHEEL:
                            on_mouse_scroll(m_tsMessage, modifiers, position);
                            return true;

                        default:
                            on_mouse_move(m_tsMessage, modifiers, position);
                            return true;
                    }
                }
            }

//            DefRawInputProc(reinterpret_cast<RAWINPUT**>(&lParam), 1, sizeof (RAWINPUTHEADER));
            break;
        }
    }

    return false;
}

//------------------------------------------------------------------------------
void quetzal::framework::Application::update(float dt)
{
    dt;
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Application::render()
{
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Application::on_frame()
{
    on_frame_();

    ++m_nFrameStatistics;

    // Compute statistics once per second
    if ((m_timer.total() - m_tStatisticsPrev) >= 1.f)
    {
        m_frame_rate = m_nFrameStatistics / 1.f;
        // Reset for next data point
        m_nFrameStatistics = 0;
        m_tStatisticsPrev += 1.f;
    }

    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Application::on_size(size_type size)
{
//    m_renderer.resize();
    on_size_(size);
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Application::on_suspend()
{
    on_suspend_();
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Application::on_resume()
{
    on_resume_();
    return;
}

//------------------------------------------------------------------------------
bool quetzal::framework::Application::on_close()
{
    return on_close_();
}

//------------------------------------------------------------------------------
void quetzal::framework::Application::on_key_down(Timestamp ts, input::key::Modifiers modifiers, input::key::Code code)
{
    ts;
    modifiers;
    code;
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Application::on_key_up(Timestamp ts, input::key::Modifiers modifiers, input::key::Code code)
{
    ts;
    modifiers;
    code;
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Application::on_char(Timestamp ts, input::key::Modifiers modifiers, const string& utf8)
{
    ts;
    modifiers;
    utf8;
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Application::on_mouse_down(Timestamp ts, input::key::Modifiers modifiers, position_type position, input::key::Code code)
{
    ts;
    modifiers;
    position;
    code;
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Application::on_mouse_up(Timestamp ts, input::key::Modifiers modifiers, position_type position, input::key::Code code)
{
    ts;
    modifiers;
    position;
    code;
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Application::on_mouse_move(Timestamp ts, input::key::Modifiers modifiers, position_type position)
{
    ts;
    modifiers;
    position;
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Application::on_mouse_scroll(Timestamp ts, input::key::Modifiers modifiers, position_type position)
{
    ts;
    modifiers;
    position;
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Application::on_frame_()
{
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Application::on_size_(size_type size)
{
    size;
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Application::on_suspend_()
{
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Application::on_resume_()
{
    return;
}

//------------------------------------------------------------------------------
bool quetzal::framework::Application::on_close_()
{
    return true;
}
