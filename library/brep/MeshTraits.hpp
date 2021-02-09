#if !defined(QUETZAL_BREP_MESHTRAITS_HPP)
#define QUETZAL_BREP_MESHTRAITS_HPP
//------------------------------------------------------------------------------
// brep
// MeshTraits.hpp
//
// NormalTangent used for Submesh default attributes to represent orientation up and right
//
//------------------------------------------------------------------------------

#include "geometry/Attributes.hpp"
#include "math/Vector.hpp"

namespace quetzal
{

namespace brep
{

    //--------------------------------------------------------------------------
    template<typename Traits, typename V = geometry::PositionNormalTexture<Traits>, typename F = geometry::Normal<Traits>, typename S = F, typename O = geometry::NormalTangent<Traits>>
    class MeshTraits
    {
    public:

        using vector_traits = Traits;
        using texcoord_traits = typename vector_traits::reduced_traits;
        
        using vertex_attributes_type = V;
        using face_attributes_type = F;
        using surface_attributes_type = S;
        using submesh_attributes_type = O;

        using value_type = typename Traits::value_type;
        using vector_type = math::Vector<vector_traits>;
        using point_type = math::Vector<vector_traits>;
        using texcoord_type = math::Vector<texcoord_traits>;
        using size_type = size_t;

        static constexpr size_t dimension = vector_traits::dimension;

        template<typename U>
        static value_type val(const U& u)
        {
            return static_cast<value_type>(u);
        }
    };

} // namespace brep

} // namespace quetzal

#endif // QUETZAL_BREP_MESHTRAITS_HPP
