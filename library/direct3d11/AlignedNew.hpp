#if !defined(QUETZAL_DIRECT3D11_ALIGNEDNEW_HPP)
#define QUETZAL_DIRECT3D11_ALIGNEDNEW_HPP
//------------------------------------------------------------------------------
// direct3d11
// AlignedNew.hpp
//------------------------------------------------------------------------------

#include <malloc.h>
#include <exception>

namespace quetzal::direct3d11
{

    // Derive from this to customize operator new and delete for
    // types that have special heap alignment requirements.
    //
    // Example usage:
    //
    //      __declspec(align(16)) struct MyAlignedType : public AlignedNew<MyAlignedType>

    template<typename TDerived>
    struct AlignedNew
    {
        // Allocate aligned memory.
        static void* operator new(size_t size)
        {
            const size_t alignment = __alignof(TDerived);

            static_assert(alignment > 8, "AlignedNew is only useful for types with > 8 byte alignment. Did you forget a __declspec(align) on TDerived?");

            void* p = _aligned_malloc(size, alignment);

            if (p == nullptr)
            {
                throw std::bad_alloc();
            }

            return p;
        }

        // Free aligned memory.
        static void operator delete(void* p)
        {
            _aligned_free(p);
        }

        // Array overloads.
        static void* operator new[](size_t size)
        {
            return operator new(size);
        }

        static void operator delete[](void* p)
        {
            operator delete(p);
        }
    };

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_ALIGNEDNEW_HPP
