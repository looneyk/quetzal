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
        static const std::string Comment = "#";
        static const std::string Position = "v";
        static const std::string Texcoord = "vt";
        static const std::string Normal = "vn";
        static const std::string Face = "f";
        static const std::string Group = "g";
        static const std::string Object = "o";
        static const std::string Materials = "mtllib";
        static const std::string Material = "usemtl";
    }

    static const std::string ObjectNameDefault = "";
    static const std::string GroupNameDefault = "default";

} // namespace quetzal::wavefront_obj

#endif // QUETZAL_WAVEFRONT_OBJ_SYMBOLS_HPP
