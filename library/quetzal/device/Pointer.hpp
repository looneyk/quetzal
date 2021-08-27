//------------------------------------------------------------------------------
// Pointer.hpp
//
//------------------------------------------------------------------------------

#if !defined(QUETZAL_DEVICE_POINTER_HPP)
#define QUETZAL_DEVICE_POINTER_HPP

#include <windows.h>

namespace quetzal
{

namespace device
{

    //----------------------------------------------------------------------
    class Pointer
    {
    public:

        Pointer();
        ~Pointer();

        void init(HWND hwnd);
//        void update(); // refresh, reattach, reinit, ...

    protected:

        HANDLE m_handle;
        unsigned long m_id;
        int m_nButtons;
        int m_nSampleRate;
        bool m_bHorizontalWheel;

    private:

        Pointer(const Pointer&); // Not copyable
        Pointer& operator=(const Pointer&); // Not assignable
    };

} // namespace device

} // namespace quetzal

#endif // QUETZAL_DEVICE_POINTER_HPP
