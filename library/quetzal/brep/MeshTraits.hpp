#if !defined(QUETZAL_BREP_MESHTRAITS_HPP)
#define QUETZAL_BREP_MESHTRAITS_HPP
//------------------------------------------------------------------------------
// brep
// MeshTraits.hpp
//
// NormalTangent used for Submesh default attributes to represent orientation up and right
//
//------------------------------------------------------------------------------

#include "quetzal/geometry/Attributes.hpp"
#include "quetzal/math/Vector.hpp"

namespace quetzal::brep
{

    //--------------------------------------------------------------------------
    template<typename Traits, typename V = geometry::PositionNormalTexture<Traits>, typename F = geometry::Normal<Traits>, typename S = F, typename O = geometry::NormalTangent<Traits>>
    class MeshTraits
    {
    public:

        using vector_traits = Traits;
        
        using vertex_attributes_type = V;
        using face_attributes_type = F;
        using surface_attributes_type = S;
        using submesh_attributes_type = O;

        using size_type = size_t;
        using value_type = Traits::value_type;
        using vector_type = math::Vector<vector_traits>;
        using point_type = math::Vector<vector_traits>;

        static constexpr size_t dimension = vector_traits::dimension;

        template<typename U>
        static constexpr value_type val(const U& u)
        {
            return static_cast<value_type>(u);
        }
    };

} // namespace quetzal::brep

#endif // QUETZAL_BREP_MESHTRAITS_HPP
