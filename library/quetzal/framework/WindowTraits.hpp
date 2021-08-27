#if !defined(QUETZAL_FRAMEWORK_WINDOWTRAITS_HPP)
#define QUETZAL_FRAMEWORK_WINDOWTRAITS_HPP
//------------------------------------------------------------------------------
// quetzal framework
// Window.hpp
//------------------------------------------------------------------------------

#include "Position.hpp"
#include "Size.hpp"
#include <Windows.h>
#include <string>
#include <functional>

namespace quetzal::framework
{

    //--------------------------------------------------------------------------
    class WindowTraits
    {
    public:

        


        typedef std::function<bool(UINT, WPARAM, LPARAM)> message_handler_type;
        typedef DWORD style_type;
        typedef Position position_type;
        typedef Size size_type;

        //----------------------------------------------------------------------
        class Mode
        {
        public:

            Mode() = default;
            Mode(position_type position, size_type size);
            Mode(const Mode&) = default;
            virtual ~Mode() = default;

            Mode& operator=(const Mode&) = default;

            virtual style_type style() const = 0;

            position_type position() const;
            size_type size() const;

            void set_position(position_type position);
            void set_size(size_type size);

        protected:

            position_type m_position;
            size_type m_size;
        };

        //----------------------------------------------------------------------
        class ModeFull : public Mode
        {
        public:

            ModeFull();
            ModeFull(const ModeFull&) = default;
            virtual ~ModeFull() = default;

            ModeFull& operator=(const ModeFull&) = default;

            virtual style_type style() const;
        };

        //----------------------------------------------------------------------
        class ModeExpanded : public Mode
        {
        public:

            ModeExpanded();
            ModeExpanded(const ModeExpanded&) = default;
            virtual ~ModeExpanded() = default;

            ModeExpanded& operator=(const ModeExpanded&) = default;

            virtual style_type style() const;

            void set_dimensions(double aspect_ratio, double position);
        };

        //----------------------------------------------------------------------
        class ModeFloating : public Mode
        {
        public:

            ModeFloating();
            ModeFloating(const ModeFloating&) = default;
            virtual ~ModeFloating() = default;

            ModeFloating& operator=(const ModeFloating&) = default;

            virtual style_type style() const;

            void set_dimensions(position_type position, size_type size);
        };

        Window(message_handler_type f);
        Window(const Window&) = delete;
        ~Window();

        Window& operator=(const Window&) = delete;

        HWND hwnd();
        bool full() const;

        position_type position() const;
        size_type size() const; // not updated when maximized, need to track floating restored size too ...
        size_type client_size() const;

        void create_full(const std::string& caption);
        void create_expanded(const std::string& caption, double aspect_ratio, double position = 0.5f, bool bBorderless = true);
        void create_floating(const std::string& caption, position_type position, size_type size, bool bBorderless = false, bool bMaximized = false, bool bMinimized = false);

        void set_full();
        void set_expanded();
        void set_floating();

        void show();
        void destroy();

        // set_dimensions(optional position, optional size) ...

        // For use by window_proc
        message_handler_type message_handler() const;
        void update_client_size();
        void set_position(position_type position);
        void set_size(size_type size);
        void set_size_flags(bool bMaximized, bool bMinimized);

    private:

        void create(const std::string& caption);
        void set_mode(const Mode& mode);

        void register_window_class();

        static bool m_bRegistered;
        static HINSTANCE m_hinstance;

        HWND m_hwnd;

        message_handler_type m_message_handler;

        ModeFull m_modeFull;
        ModeExpanded m_modeExpanded;
        ModeFloating m_modeFloating;
        Mode* m_pmode;

        bool m_bBorderless;
        bool m_bMaximized;
        bool m_bMinimized;

        size_type m_client_size;
    };

} // namespace quetzal::framework

#endif // QUETZAL_FRAMEWORK_WINDOWTRAITS_HPP
