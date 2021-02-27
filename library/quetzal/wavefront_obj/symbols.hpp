#if !defined(QUETZAL_WAVEFRONT_OBJ_SYMBOLS_HPP)
#define QUETZAL_WAVEFRONT_OBJ_SYMBOLS_HPP
//------------------------------------------------------------------------------
// Wavefront obj file symbols and default values
// symbols.hpp
//------------------------------------------------------------------------------

#include <string>

namespace quetzal::wavefront_obj
{

    //--------------------------------------------------------------------------
    namespace Keyword
    {
        // obj
        static const std::string Comment = "#";
        static const std::string Position = "v";
        static const std::string Texcoord = "vt";
        static const std::string Normal = "vn";
        static const std::string Face = "f";
        static const std::string Group = "g";
        static const std::string Object = "o";
        static const std::string Materials = "mtllib";
        static const std::string Material = "usemtl";

        // mtl
        static const std::string Newmtl = "newmtl";
        static const std::string AmbientColor = "Ka";
        static const std::string DiffuseColor = "Kd";
        static const std::string SpecularColor = "Ks";
        static const std::string EmissiveColor = "Ke";
        static const std::string TransmissionFilter = "Tf";
        static const std::string IlluminationModel = "illum";
        static const std::string Dissolve = "d";
        static const std::string SpecularExponent = "Ns";
        static const std::string Sharpness = "sharpness";
        static const std::string OpticalDensity = "Ni";
        static const std::string AmbientTexture = "map_Ka";
        static const std::string DiffuseTexture = "map_Kd";
        static const std::string SpecularTexture = "map_Ks";
        static const std::string EmissiveTexture = "map_Ke";
        static const std::string SpecularExponentMap = "map_Ns";
        static const std::string DissolveMap = "map_d";
        static const std::string DisplacementMap = "disp";
        static const std::string Decal = "decal";
        static const std::string BumpMap = "bump";
        static const std::string NormalMap = "norm";
        static const std::string ReflectionMap = "refl";
    }

    static const std::string ObjectNameDefault = "";
    static const std::string GroupNameDefault = "default";

} // namespace quetzal::wavefront_obj

#endif // QUETZAL_WAVEFRONT_OBJ_SYMBOLS_HPP
