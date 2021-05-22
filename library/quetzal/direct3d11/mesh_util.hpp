#if !defined(QUETZAL_DIRECT3D11_MESH_UTIL_HPP)
#define QUETZAL_DIRECT3D11_MESH_UTIL_HPP
//------------------------------------------------------------------------------
// direct3d11
// mesh_util.hpp
//------------------------------------------------------------------------------

#include "quetzal/direct3d11/Vector2.hpp"
#include "quetzal/direct3d11/Vector3.hpp"
#include <tuple>
#include <vector>

namespace quetzal::direct3d11
{

    // not to_xm, not to_dx, to_dv? ...

    template<typename M>
    quetzal::direct3d11::Vector3 to_xm(const typename M::vector_type& v);

    template<typename M>
    quetzal::direct3d11::Vector2 to_xm(const typename M::texcoord_type& v);

    template<typename V, typename I, typename M>
    std::tuple<std::vector<V>, std::vector<I>> mesh_geometry(const M& m, std::function<V(const typename M::vertex_type::attributes_type&)> transfer_vertex);

} // namespace quetzal::direct3d11

//------------------------------------------------------------------------------
template<typename M>
quetzal::direct3d11::Vector3 quetzal::direct3d11::to_xm(const typename M::vector_type& v)
{
    return {v.x(), v.y(), v.z()};
}

//------------------------------------------------------------------------------
template<typename M>
quetzal::direct3d11::Vector2 quetzal::direct3d11::to_xm(const typename M::texcoord_type& v)
{
    return {v.x(), v.y()};
}

//------------------------------------------------------------------------------
template<typename V, typename I, typename M>
std::tuple<std::vector<V>, std::vector<I>> quetzal::direct3d11::mesh_geometry(const M& m, std::function<V(const typename M::vertex_type::attributes_type&)> transfer_vertex)
{
    std::vector<V> vertices;
    std::vector<I> indices;

    for (const auto& face : m.faces())
    {
        assert(face.halfedge_count() == 3);
        for (const auto& halfedge : face.halfedges())
        {
            const auto& av = halfedge.attributes();
            Vector3 position = {av.position().x(), av.position().y(), -300.0f - av.position().z()}; // ...

            V v = transfer_vertex(av);

            indices.push_back(static_cast<I>(vertices.size()));
            vertices.emplace_back(v);
        }
    }

    return {vertices, indices};
}

#endif // QUETZAL_DIRECT3D11_MESH_UTIL_HPP
