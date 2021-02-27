#if !defined(QUETZAL_WAVEFRONT_OBJ_MATERIAL_HPP)
#define QUETZAL_WAVEFRONT_OBJ_MATERIAL_HPP
//------------------------------------------------------------------------------
// wavefront_obj
// Material.hpp
//------------------------------------------------------------------------------

#include <array>
#include <string>
#include <vector>

// template class, value_type?

namespace quetzal::wavefront_obj
{

    using Color = std::array<float, 3>;

    //--------------------------------------------------------------------------
    class Material
    {
    public:

        Material() = default;
        Material(const Material&) = default;
        Material(Material&&) = default;
        ~Material() = default;

        Material& operator=(const Material&) = default;
        Material& operator=(Material&&) = default;

        void set(const std::vector<std::string>& tokens, const std::string& filename, size_t line); // apply statement that has been parsed into tokens

//    private:

        std::string m_name;
        float m_specularExponent; // Ns
        float m_alpha; // d, 1 - Tr
        Color m_ambientColor; // Ka
        Color m_diffuseColor; // Kd
        Color m_specularColor; // Ks
        Color m_emissiveColor; // Ks
        std::string m_ambientTextureName; // map_Ka
        std::string m_diffuseTextureName; // map_Kd
        std::string m_specularTextureName; // map_Ks
        std::string m_emissiveTextureName; // map_Ke
        std::string m_normalMapName; // norm
    };

} // namespace quetzal::wavefront_obj

#endif // QUETZAL_WAVEFRONT_OBJ_MATERIAL_HPP
