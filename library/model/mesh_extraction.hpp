#if !defined(QUETZAL_MODEL_MESH_EXTRACTION_HPP)
#define QUETZAL_MODEL_MESH_EXTRACTION_HPP
//------------------------------------------------------------------------------
// model
// mesh_extraction.hpp
//------------------------------------------------------------------------------

#include <functional>
#include <tuple>
#include <vector>

namespace quetzal
{

namespace model
{

    template<typename V, typename I, typename M>
    std::tuple<std::vector<V>, std::vector<I>> extract_input(const M& mesh, std::function<V(const typename M::vertex_attributes_type&)> to_vertex_type);

    template<typename V, typename M>
    std::vector<V> extract_vertices(const M& mesh, std::function<V(const typename M::vertex_attributes_type&)> to_vertex_type);

    template<typename I, typename M>
    std::vector<I> extract_indices(const M& mesh);

} // namespace model

} // namespace quetzal

//------------------------------------------------------------------------------
template<typename V, typename I, typename M>
std::tuple<std::vector<V>, std::vector<I>> quetzal::model::extract_input(const M& mesh, std::function<V(const typename M::vertex_attributes_type&)> to_vertex_type)
{
    std::vector<V> vertices;
    std::vector<I> indices;

    for (const auto& vertex : mesh.vertices())
    {
        vertices.push_back(to_vertex_type(vertex.attributes()));
    }

    for (const auto& face : mesh.faces())
    {
        for (const auto& halfedge : face.halfedges())
        {
            indices.push_back(static_cast<I>(halfedge.vertex_id()));
        }
    }

    return {vertices, indices};
}

//------------------------------------------------------------------------------
template<typename V, typename M>
std::vector<V> quetzal::model::extract_vertices(const M& mesh, std::function<V(const typename M::vertex_attributes_type&)> to_vertex_type)
{
    std::vector<V> vertices;

    for (const auto& vertex : mesh.vertices())
    {
        vertices.push_back(to_vertex_type(vertex.attributes()));
    }

    return vertices;
}

//------------------------------------------------------------------------------
template<typename I, typename M>
std::vector<I> quetzal::model::extract_indices(const M& mesh)
{
    std::vector<I> indices;

    for (const auto& face : mesh.faces())
    {
        for (const auto& halfedge : face.halfedges())
        {
            indices.push_back(static_cast<I>(halfedge.vertex_id()));
        }
    }

    return indices;
}

#endif // QUETZAL_MODEL_MESH_EXTRACTION_HPP
