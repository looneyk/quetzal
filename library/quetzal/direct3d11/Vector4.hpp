#if !defined(QUETZAL_DIRECT3D11_VECTOR4_HPP)
#define QUETZAL_DIRECT3D11_VECTOR4_HPP
//------------------------------------------------------------------------------
// direct3d11
// Vector4.hpp
//------------------------------------------------------------------------------

#include "quetzal/math/Vector.hpp"
#include <DirectXMath.h>

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    class XMFLOAT4_Traits
    {
    public:

        using value_type = float;
        using size_type = size_t;
        static constexpr size_type dimension = 4;
        using rep_type = DirectX::XMFLOAT4;

        // Element access
                
        static float get(const rep_type& rep, size_t i)
        {
            assert(i < 4);
            return reinterpret_cast<const float*>(&rep)[i];
        }
                
        static float& get(rep_type& rep, size_t i)
        {
            assert(i < 4);
            return reinterpret_cast<float*>(&rep)[i];
        }
                
        template<size_t I>
        static float& get(rep_type& rep) noexcept
        {
            static_assert(I < 4);
            if constexpr (I == 0) return rep.x;
            else if constexpr (I == 1) return rep.y;
            else if constexpr (I == 2) return rep.z;
            else if constexpr (I == 3) return rep.w;
        }

        template<size_t I>
        static float&& get(rep_type&& rep) noexcept
        {
            static_assert(I < 4);
            if constexpr (I == 0) return rep.x;
            else if constexpr (I == 1) return rep.y;
            else if constexpr (I == 2) return rep.z;
            else if constexpr (I == 3) return rep.w;
        }

        template<size_t I>
        static const float& get(const rep_type& rep) noexcept
        {
            static_assert(I < 4);
            if constexpr (I == 0) return rep.x;
            else if constexpr (I == 1) return rep.y;
            else if constexpr (I == 2) return rep.z;
            else if constexpr (I == 3) return rep.w;
        }

        template<size_t I>
        static const float&& get(const rep_type&& rep) noexcept
        {
            static_assert(I < 4);
            if constexpr (I == 0) return rep.x;
            else if constexpr (I == 1) return rep.y;
            else if constexpr (I == 2) return rep.z;
            else if constexpr (I == 3) return rep.w;
        }

        static void clear(rep_type& rep);

        static rep_type& assign(rep_type& lhs, const float(&rhs)[4]);

        static rep_type unary_minus(rep_type rep);

        static rep_type& assign_add(rep_type& lhs, const rep_type& rhs);

        static rep_type& assign_subtract(rep_type& lhs, const rep_type& rhs);

        static rep_type& assign_multiply(rep_type& lhs, const float& rhs);

        static rep_type& assign_divide(rep_type& lhs, const float& rhs);

        static float dot_product(const rep_type& lhs, const rep_type& rhs);

        static bool zero(const rep_type& rep);

        static bool equals(const rep_type& lhs, const rep_type& rhs);

        static bool less(const rep_type& lhs, const rep_type& rhs);

        static bool float_zero(const rep_type& rep, int ulp);

        static bool float_equals(const rep_type& lhs, const rep_type& rhs, int ulp);

        static rep_type round_zero(rep_type rep, int ulp);

        static std::istream& extract(std::istream& is, rep_type& rhs);

        static std::ostream& insert(std::ostream& os, const rep_type& rhs);
    };

    using Vector4 = math::Vector<XMFLOAT4_Traits>;

} // namespace direct3d11::quetzal

#endif // QUETZAL_DIRECT3D11_VECTOR4_HPP
