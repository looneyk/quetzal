#if !defined(QUETZAL_DIRECT3D11_VECTOR2_HPP)
#define QUETZAL_DIRECT3D11_VECTOR2_HPP
//------------------------------------------------------------------------------
// direct3d11
// Vector2.hpp
//------------------------------------------------------------------------------

#include "math/Vector.hpp"
#include <DirectXMath.h>

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    class XMFLOAT2_Traits
    {
    public:

        using value_type = float;
        using size_type = size_t;
        static constexpr size_type dimension = 2;
        using rep_type = DirectX::XMFLOAT2;

        // Element access
                
        static float get(const rep_type& rep, size_t i)
        {
            assert(i < 2);
            return reinterpret_cast<const float*>(&rep)[i];
        }
                
        static float& get(rep_type& rep, size_t i)
        {
            assert(i < 2);
            return reinterpret_cast<float*>(&rep)[i];
        }
                
        template<size_t I>
        static float& get(rep_type& rep) noexcept
        {
            static_assert(I < 2);
            if constexpr (I == 0) return rep.x;
            else if constexpr (I == 1) return rep.y;
        }

        template<size_t I>
        static float&& get(rep_type&& rep) noexcept
        {
            static_assert(I < 2);
            if constexpr (I == 0) return rep.x;
            else if constexpr (I == 1) return rep.y;
        }

        template<size_t I>
        static const float& get(const rep_type& rep) noexcept
        {
            static_assert(I < 2);
            if constexpr (I == 0) return rep.x;
            else if constexpr (I == 1) return rep.y;
        }

        template<size_t I>
        static const float&& get(const rep_type&& rep) noexcept
        {
            static_assert(I < 2);
            if constexpr (I == 0) return rep.x;
            else if constexpr (I == 1) return rep.y;
        }

        static void clear(rep_type& rep);

        static rep_type& assign(rep_type& lhs, const float(&rhs)[2]);

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

    using Vector2 = math::Vector<XMFLOAT2_Traits>;

} // namespace direct3d11::quetzal

#endif // QUETZAL_DIRECT3D11_VECTOR2_HPP
