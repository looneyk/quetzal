#if !defined(QUETZAL_DIRECT3D11_CHECKED_CREATE_HPP)
#define QUETZAL_DIRECT3D11_CHECKED_CREATE_HPP
//------------------------------------------------------------------------------
// Direct3D 11
// checked_create.hpp
//------------------------------------------------------------------------------

#include "common/com_ptr.hpp"
#include "common/ComException.hpp"
#include <windows.h>
#include <mutex>

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    template<typename T, typename TCreator>
    static T* checked_create(com_ptr<T>& cp, std::mutex& mutex, TCreator create)
    {
        T* pT = cp;

        // Double-checked lock pattern
        MemoryBarrier();

        if (!pT)
        {
            std::lock_guard<std::mutex> lock(mutex);

            pT = cp;

            if (!pT)
            {
                COM_CHECK(create(&pT)); // Create an instance

                MemoryBarrier();

                cp = pT;
            }
        }

        return pT;
    }

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_CHECKED_CREATE_HPP
