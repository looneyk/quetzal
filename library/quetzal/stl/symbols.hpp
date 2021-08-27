#if !defined(QUETZAL_STL_SYMBOLS_HPP)
#define QUETZAL_STL_SYMBOLS_HPP
//------------------------------------------------------------------------------
// STL file symbols and default values
// symbols.hpp
//------------------------------------------------------------------------------

#include <string>

namespace quetzal::stl
{

    //--------------------------------------------------------------------------
    namespace Keyword
    {
        static const std::string Solid = "solid";
        static const std::string Facet = "facet";
        static const std::string Normal = "normal";
        static const std::string Outer = "outer";
        static const std::string Loop = "loop";
        static const std::string Vertex = "vertex";
        static const std::string EndLoop = "endloop";
        static const std::string EndFacet = "endfacet";
        static const std::string EndSolid = "endsolid";
    }

    static const std::string ObjectNameDefault = "default";
    static const std::string GroupNameDefault = "default";

} // namespace quetzal::stl

#endif // QUETZAL_STL_SYMBOLS_HPP
