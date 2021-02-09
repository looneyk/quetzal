//------------------------------------------------------------------------------
// quetzal framework
// Window.cpp
//------------------------------------------------------------------------------

#include "Window.hpp"
#include "common/Exception.hpp"
#include "common/string_util.hpp"
#include <vector>
#include <sstream>
#include <cassert>

using namespace std;

namespace
{

    wstring s_window_class_name = L"quetzal_framework";

    //--------------------------------------------------------------------------
    LRESULT CALLBACK window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (uMsg == WM_NCCREATE)
        {
            CREATESTRUCT* pcs = reinterpret_cast<CREATESTRUCT*>(lParam);
            assert(pcs != nullptr);
            assert(pcs->lpCreateParams != nullptr);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pcs->lpCreateParams));
            return TRUE;
        }

        bool bHandled = false;

        quetzal::framework::Window* pwindow = reinterpret_cast<quetzal::framework::Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (pwindow != nullptr)
        {
            // Intercept window move/size/min/max messages here first to update position/size tracking variables
            switch (uMsg)
            {
                case WM_MOVE:
                    pwindow->set_position({LOWORD(lParam), HIWORD(lParam)});
                    bHandled = true;
                    break;

                case WM_SIZE:
                    if (wParam == SIZE_MAXIMIZED)
                    {
                        pwindow->set_size_flags(true, false);
//                        pwindow->set_size({LOWORD(lParam), HIWORD(lParam)}); // ...
                    }
                    else if (wParam == SIZE_MINIMIZED)
                    {
                        pwindow->set_size_flags(false, true);
                    }
                    else if (wParam == SIZE_RESTORED)
                    {
                        pwindow->set_size_flags(false, false);
                        pwindow->set_size({LOWORD(lParam), HIWORD(lParam)});
                    }
                    pwindow->update_client_size(); // even if minimized? ...
                    bHandled = true;
                    break;

//                case WM_EXITSIZEMOVE:
//                    bHandled = true;
//                    break;

                case WM_DESTROY:
                    PostQuitMessage(0);
                    return 0;
            }

            if (pwindow->message_handler())
            {
                bHandled = pwindow->message_handler()(uMsg, wParam, lParam);
            }
        }

        if (bHandled)
        {
            return 0;
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

} // namespace

bool quetzal::framework::Window::m_bRegistered = false;

//------------------------------------------------------------------------------
quetzal::framework::Window::Mode::Mode(position_type position, size_type size) :
    m_position(position),
    m_size(size)
{
}

//------------------------------------------------------------------------------
quetzal::framework::Window::position_type quetzal::framework::Window::Mode::position() const
{
    return m_position;
}

//------------------------------------------------------------------------------
quetzal::framework::Window::size_type quetzal::framework::Window::Mode::size() const
{
    return m_size;
}

//------------------------------------------------------------------------------
void quetzal::framework::Window::Mode::set_position(position_type position)
{
    m_position = position;
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Window::Mode::set_size(size_type size)
{
    m_size = size;
    return;
}

//------------------------------------------------------------------------------
quetzal::framework::Window::ModeFull::ModeFull() :
    Mode({}, {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)})
{
}

//------------------------------------------------------------------------------
quetzal::framework::Window::style_type quetzal::framework::Window::ModeFull::style() const
{
    return WS_POPUPWINDOW; // WS_SYSMENU? ...
}

//------------------------------------------------------------------------------
quetzal::framework::Window::ModeExpanded::ModeExpanded() :
    Mode({}, {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)})
{
}

//------------------------------------------------------------------------------
quetzal::framework::Window::style_type quetzal::framework::Window::ModeExpanded::style() const
{
    return WS_POPUPWINDOW; // WS_SYSMENU? ...
}

//------------------------------------------------------------------------------
void quetzal::framework::Window::ModeExpanded::set_dimensions(double aspect_ratio, double position)
{
    size_type sizeFull = {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};

    if (sizeFull.y * aspect_ratio >= sizeFull.x)
    {
        set_size({sizeFull.x, static_cast<int>(round(sizeFull.x / aspect_ratio))});
    }
    else
    {
        set_size({static_cast<int>(round(sizeFull.y * aspect_ratio)), sizeFull.y});
    }

    set_position({static_cast<int>((sizeFull.x - m_size.x) * position), static_cast<int>((sizeFull.y - m_size.y) * position)});

    return;
}

//------------------------------------------------------------------------------
quetzal::framework::Window::ModeFloating::ModeFloating() :
    Mode({}, {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)})
{
}

//------------------------------------------------------------------------------
quetzal::framework::Window::style_type quetzal::framework::Window::ModeFloating::style() const
{
    return WS_OVERLAPPEDWINDOW;
}

//--------------------------------------------------------------------------
void quetzal::framework::Window::ModeFloating::set_dimensions(position_type position, size_type size)
{
    size_type sizeFull = {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};

    // Check that floating window dimensions fit within screen
    // Preserve aspect ratio if adjustment is necessary

    m_position = position;
    m_size = size;
    auto aspect_ratio = m_size.aspect_ratio();

    if (m_size.x > sizeFull.x)
    {
        m_size.x = sizeFull.x;
        m_size.y = static_cast<int>(round(m_size.x / aspect_ratio));
    }

    if (m_size.y > sizeFull.y)
    {
        m_size.y = sizeFull.y;
        m_size.x = static_cast<int>(round(m_size.y * aspect_ratio));
    }

    if (m_position.x + m_size.x > sizeFull.x)
    {
        m_position.x = (sizeFull.x - m_size.x) / 2;
    }

    if (m_position.y + m_size.y > sizeFull.y)
    {
        m_position.y = (sizeFull.y - m_size.y) / 2;
    }

    return;
}

//------------------------------------------------------------------------------
quetzal::framework::Window::Window(message_handler_type f) :
    m_hwnd(nullptr),
    m_message_handler(f),
    m_modeFull(),
    m_modeExpanded(),
    m_modeFloating(),
    m_pmode(nullptr),
    m_bBorderless(false),
    m_bMaximized(false),
    m_bMinimized(false),
    m_client_size(),
    m_mutex()
{
    register_window_class();
}

//------------------------------------------------------------------------------
quetzal::framework::Window::~Window()
{
}

//------------------------------------------------------------------------------
HWND quetzal::framework::Window::hwnd()
{
    return m_hwnd;
}

//------------------------------------------------------------------------------
bool quetzal::framework::Window::full() const
{
    return m_pmode == &m_modeFull;
}

//------------------------------------------------------------------------------
quetzal::framework::Window::position_type quetzal::framework::Window::position() const
{
    assert(m_pmode != nullptr);
    return m_pmode->position();
}

//------------------------------------------------------------------------------
quetzal::framework::Window::size_type quetzal::framework::Window::size() const
{
    assert(m_pmode != nullptr);
    return m_pmode->size();
}

//------------------------------------------------------------------------------
quetzal::framework::Window::size_type quetzal::framework::Window::client_size() const
{
    return m_client_size;
}

//------------------------------------------------------------------------------
void quetzal::framework::Window::create_full(const string& title)
{
    m_pmode = &m_modeFull;
    create(title);
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Window::create_expanded(const string& title, double aspect_ratio, double position, bool bBorderless)
{
    m_modeExpanded.set_dimensions(aspect_ratio, position);
    m_modeFloating.set_dimensions(m_modeExpanded.position(), m_modeExpanded.size());

    m_bBorderless = bBorderless;

    m_pmode = &m_modeExpanded;
    create(title);
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Window::create_floating(const string& title, position_type position, size_type size, bool bBorderless, bool bMaximized, bool bMinimized)
{
    m_modeFloating.set_dimensions(position, size);
    m_modeExpanded.set_dimensions(m_modeFloating.size().aspect_ratio(), 0.5);

    m_bBorderless = bBorderless;
    m_bMaximized = bMaximized;
    m_bMinimized = bMinimized;

    m_pmode = &m_modeFloating;
    create(title);
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Window::set_full()
{
    set_mode(m_modeFull);
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Window::set_expanded()
{
    set_mode(m_modeExpanded);
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Window::set_floating()
{
    set_mode(m_modeFloating);
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Window::show()
{
    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);

    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Window::destroy()
{
    DestroyWindow(m_hwnd);
    return;
}

//------------------------------------------------------------------------------
quetzal::framework::Window::message_handler_type quetzal::framework::Window::message_handler() const
{
    return m_message_handler;
}

//------------------------------------------------------------------------------
void quetzal::framework::Window::update_client_size()
{
    RECT rect {};
    GetClientRect(m_hwnd, &rect);
    m_client_size = {rect.right, rect.bottom};
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Window::set_position(position_type position)
{
    assert(m_pmode != nullptr);
    m_pmode->set_position(position);
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Window::set_size(size_type size)
{
    assert(m_pmode != nullptr);
    m_pmode->set_size(size);
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Window::set_size_flags(bool bMaximized, bool bMinimized)
{
    m_bMaximized = bMaximized;
    m_bMinimized = bMinimized;
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Window::create(const string& title)
{
    assert(m_pmode != nullptr);

    auto style = m_pmode->style();
    auto position = m_pmode->position();
    auto size = m_pmode->size();

    // Modify style based on flags
    if (style && WS_POPUP)
    {
        if (m_bBorderless)
        {
            style = WS_POPUP;
        }
    }
    else
    {
        if (m_bBorderless)
        {
            style = WS_OVERLAPPED;
        }

        if (m_bMaximized)
        {
            style |= WS_MAXIMIZE;
        }
        else if (m_bMinimized)
        {
            style |= WS_MINIMIZE;
        }
    }

    HINSTANCE hinstance = GetModuleHandle(nullptr);

    m_hwnd = CreateWindowEx(0, s_window_class_name.c_str(), to_type<wstring>(title).c_str(), style,
        position.x, position.y, size.x, size.y, nullptr, nullptr, hinstance, this);
    if (m_hwnd == nullptr)
    {
        ostringstream oss;
        oss << "Error creating window " << title << ", error " << GetLastError();
        throw Exception(oss.str().c_str(), __FILE__, __LINE__);
    }

//    if (m_mode == Mode::Windowed || m_mode == Mode::Expanded) // any harm for full too? ...
// is this needed at all here? if for show, it should be a separate operation ...
//    {
//        SetWindowPos(m_hwnd, HWND_TOP, position.x, position.y, size.x, size.y, SWP_NOZORDER | SWP_SHOWWINDOW);
//    }

    update_client_size();
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Window::set_mode(const Mode& mode)
{
    assert(m_pmode != &mode);

    auto style = mode.style();
    auto position = mode.position();
    auto size = mode.size();

    SetWindowLongPtr(m_hwnd, GWL_STYLE, style);
    SetWindowPos(m_hwnd, HWND_TOP, position.x, position.y, size.x, size.y, SWP_NOZORDER | SWP_SHOWWINDOW);

    update_client_size();
    return;
}

//------------------------------------------------------------------------------
void quetzal::framework::Window::register_window_class()
{
    lock_guard<mutex> lock(m_mutex);

    if (!m_bRegistered)
    {
        HINSTANCE hinstance = GetModuleHandle(nullptr);

        WNDCLASSEX wc;
        wc.cbSize = sizeof wc;
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = window_proc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hinstance;
        wc.hIcon = LoadIcon(hinstance, IDI_APPLICATION);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = s_window_class_name.c_str();
        wc.hIconSm = LoadIcon(hinstance, IDI_APPLICATION);
        if (RegisterClassEx(&wc) == 0)
        {
            ostringstream oss;
            oss << "Error registering window class " << to_string(s_window_class_name) << ", error " << GetLastError();
            throw Exception(oss.str().c_str(), __FILE__, __LINE__);
        }

        m_bRegistered = true;
    }

    return;
}

/* GetModuleHandle(nullptr) above returns hmodule of exe, this returns hmodule of exe or dll it resides in
HMODULE module_handle()
{
    HMODULE hmodule = nullptr;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCTSTR)module_handle, &hmodule);
    return hmodule;
}
*/
