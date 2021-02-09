#if !defined(QUETZAL_DIRECT3D11_INPUT_ELEMENTS_HPP)
#define QUETZAL_DIRECT3D11_INPUT_ELEMENTS_HPP
//------------------------------------------------------------------------------
// direct3d11
// input_elements.hpp
// deprecated ...
//------------------------------------------------------------------------------

#include "geometry/Attributes.hpp"
#include <array>

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    template<typename A, typename E>
    class InputElements
    {
    public:
    
        static E value();
        static size_t stride();
    };

    template<typename A, typename E>
    size_t InputElements<A, E>::stride()
    {
        return  sizeof (A);
    }

    // Attributes need size_t elements()
    // use this with array instead of E
    // need similar set for conversion from Attribute to an XMFLOATn form ...

    template<>
    std::array<D3D11_INPUT_ELEMENT_DESC, 1> InputElements<typename geometry::Position, std::array<D3D11_INPUT_ELEMENT_DESC, 1>>::value()
    {
        return
        {
            {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
    }

    template<>
    std::array<D3D11_INPUT_ELEMENT_DESC, 2> InputElements<typename geometry::PositionNormal, std::array<D3D11_INPUT_ELEMENT_DESC, 2>>::value()
    {
        return
        {
            {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
    }

    template<>
    std::array<D3D11_INPUT_ELEMENT_DESC, 2> InputElements<typename geometry::PositionTexture, std::array<D3D11_INPUT_ELEMENT_DESC, 2>>::value()
    {
        return
        {
            {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
    }

    template<>
    std::array<D3D11_INPUT_ELEMENT_DESC, 2> InputElements<typename geometry::PositionColor, std::array<D3D11_INPUT_ELEMENT_DESC, 2>>::value()
    {
        return
        {
            {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
    }

    template<>
    std::array<D3D11_INPUT_ELEMENT_DESC, 3> InputElements<typename geometry::PositionNormalTexture, std::array<D3D11_INPUT_ELEMENT_DESC, 3>>::value()
    {
        return
        {
            {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
    }

    template<>
    std::array<D3D11_INPUT_ELEMENT_DESC, 3> InputElements<typename geometry::PositionNormalColor, std::array<D3D11_INPUT_ELEMENT_DESC, 3>>::value()
    {
        return
        {
            {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
    }

    template<>
    std::array<D3D11_INPUT_ELEMENT_DESC, 3> InputElements<typename geometry::PositionTextureColor, std::array<D3D11_INPUT_ELEMENT_DESC, 3>>::value()
    {
        return
        {
            {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
    }

    template<>
    std::array<D3D11_INPUT_ELEMENT_DESC, 4> InputElements<typename geometry::PositionNormalTextureTangent, std::array<D3D11_INPUT_ELEMENT_DESC, 4>>::value()
    {
        return
        {
            {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TANGENT",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
    }

    template<>
    std::array<D3D11_INPUT_ELEMENT_DESC, 4> InputElements<typename geometry::PositionNormalTextureColor, std::array<D3D11_INPUT_ELEMENT_DESC, 4>>::value()
    {
        return
        {
            {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
    }

    template<>
    std::array<D3D11_INPUT_ELEMENT_DESC, 5> InputElements<typename geometry::PositionNormalTextureTangentColor, std::array<D3D11_INPUT_ELEMENT_DESC, 5>>::value()
    {
        return
        {
            {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TANGENT",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
    }

    template<>
    std::array<D3D11_INPUT_ELEMENT_DESC, 3> InputElements<typename geometry::PositionDualTexture, std::array<D3D11_INPUT_ELEMENT_DESC, 3>>::value()
    {
        return
        {
            {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD",    1, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
    }

    template<>
    std::array<D3D11_INPUT_ELEMENT_DESC, 5> InputElements<typename geometry::PositionNormalTangentColorTexture, std::array<D3D11_INPUT_ELEMENT_DESC, 5>>::value()
    {
        return
        {
            {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TANGENT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
    }

    template<>
    std::array<D3D11_INPUT_ELEMENT_DESC, 7> InputElements<typename geometry::PositionNormalTangentColorTextureSkinning, std::array<D3D11_INPUT_ELEMENT_DESC, 7>>::value()
    {
        return
        {
            {"SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TANGENT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR",       0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"BLENDINDICES",0, DXGI_FORMAT_R8G8B8A8_UINT,      0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"BLENDWEIGHT", 0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
    }

/*
// Example 4: Illustrating Moral #2 
// 
    template<typename T> 
    struct FImpl;

    template<typename T> 
    void f( T t ) { FImpl<T>::f( t ); } // users, don't touch this!

    template<typename T> 
    struct FImpl 
    { 
      static void f( T t ); // users, go ahead and specialize this 
    };

*/

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_INPUT_ELEMENTS_HPP
