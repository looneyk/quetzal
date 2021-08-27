#if !defined(QUETZAL_MODEL_TRANSFORMS_HPP)
#define QUETZAL_MODEL_TRANSFORMS_HPP
//------------------------------------------------------------------------------
// model
// transforms.hpp
//------------------------------------------------------------------------------

#include "quetzal/brep/mesh_geometry.hpp"
#include "quetzal/math/Matrix.hpp"
#include "quetzal/math/transformation_matrix.hpp"
#include <functional>

namespace quetzal::model
{

    template<typename M>
    void tuskify(M& mesh, const typename M::vector_type& axis, typename M::value_type angle, typename M::value_type z0, typename M::value_type z1);

    template<typename M>
    void tuskify(M& mesh, const typename M::vector_type& axis, typename M::value_type angle);

    template<typename M>
    void undulate(M& mesh, typename M::value_type amplitude, typename M::value_type period, typename M::value_type z0, typename M::value_type z1);

    template<typename M>
    void swirl(M& mesh, typename M::value_type angle, typename M::value_type z0, typename M::value_type z1);

    template<typename M>
    void swirl(M& mesh, typename M::value_type angle);

    template<typename M>
    void swirler(M& mesh, typename M::value_type angle_z, typename M::value_type angle_r, typename M::value_type r_max);

} // namespace quetzal::model

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::tuskify(M& mesh, const typename M::vector_type& axis, typename M::value_type angle, typename M::value_type z0, typename M::value_type z1)
{
    typename M::value_type dz = z1 - z0;

    for (auto& vertex : mesh.vertices())
    {
        auto& av = vertex.attributes();
        typename M::value_type theta = -angle * (M::val(0.5) * av.position().z() - z0) / dz;
        math::Matrix<typename M::value_type> matrix = math::rotation_axis_unit(axis, theta);
        av.position() *= matrix;
        av.normal() *= matrix;
    }

    for (auto& face : mesh.faces())
    {
        auto& af = face.attributes();
        typename M::value_type theta = -angle * (M::val(0.5) * centroid(face).z() - z0) / dz;
        math::Matrix<typename M::value_type> matrix = math::rotation_axis_unit(axis, theta);
        af.normal() *= matrix;
    }

    // something not right with normal calculations above, so this ...
//    calculate_surface_normals(mesh); // once that gets fixed ...
    calculate_face_normals(mesh);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::tuskify(M& mesh, const typename M::vector_type& axis, typename M::value_type angle)
{
    // Find lowest and highest points in mesh
    typename M::value_type z0 = mesh.vertex(0).attributes().position().z();
    typename M::value_type z1 = z0;

    for (const auto& vertex : mesh.vertices())
    {
        const auto& av = vertex.attributes();

        if (av.position().z() < z0)
        {
            z0 = av.position().z();
        }
        else if (av.position().z() > z1)
        {
            z1 = av.position().z();
        }
    }

    tuskify(mesh, axis, angle, z0, z1);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::undulate(M& mesh, typename M::value_type amplitude, typename M::value_type period, typename M::value_type z0, typename M::value_type z1)
{
    period;

    using T = M::value_type;

    T dz = z1 - z0;

    for (auto& vertex : mesh.vertices())
    {
        size_t nLevel = 2; // needs to be passed in (or as a function of dz) ...

        if (nLevel == 3) amplitude *= T(0.6666667);
        if (nLevel == 2) amplitude *= T(0.3333333);

        T zRelative = (vertex.attributes().position().z() - z0) / dz;
        T rScale = sin(zRelative * amplitude);
        rScale *= T(1) - zRelative;
//        rScale *= sin(zRelative * math::Pi<T>);
        rScale = T(1) - T(0.5) * rScale;
        math::Matrix<T> matrixPosition = math::scaling(rScale, rScale, T(1));
        math::Matrix<T> matrixRotation = math::transpose(math::inverse(matrixPosition));

        vertex.attributes().position() *= matrixPosition;

/* better? ...
        T tz = (vertex.attributes().position().z() - z0) / dz;
        T xyScale = T(1) - T(0.5) * amplitude * cos(tz * period);
        xyScale *= T(1) - tz;
        math::Matrix<T> matrixPosition = math::scaling(xyScale, xyScale, T(1));
        math::Matrix<T> matrixNormal = math::transpose(math::inverse(matrixPosition));

        if (math::float_ge(vertex.attributes().position().z(), z0)) // better/smoother condition ...
        {
            vertex.attributes().position() *= matrixPosition;
            vertex.attributes().normal() *= matrixNormal;
        }
*/
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::swirl(M& mesh, typename M::value_type angle, typename M::value_type z0, typename M::value_type z1)
{
    using T = M::value_type;

    T dz = z1 - z0;
    typename M::vector_type axis = {T(0), T(0), T(1)};

    for (auto& vertex : mesh.vertices())
    {
        T phi = angle * (vertex.attributes().position().z() - z0) / dz;

        vertex.attributes().position() *= rotation_axis_unit(axis, phi); // Rotate an amount proportional to z position from z0 to z1
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::swirl(M& mesh, typename M::value_type angle)
{
    using T = M::value_type;

    // Find lowest and highest points in mesh
    T z0 = mesh.vertex(0).attributes().position().z();
    T z1 = z0;

    for (const auto& vertex : mesh.vertices())
    {
        const auto& av = vertex.attributes();

        if (av.position().z() < z0)
        {
            z0 = av.position().z();
        }
        else if (av.position().z() > z1)
        {
            z1 = av.position().z();
        }
    }

    swirl(mesh, angle, z0, z1);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::swirler(M& mesh, typename M::value_type angle_z, typename M::value_type angle_r, typename M::value_type r_max)
{
    using T = M::value_type;

    // Find lowest and highest points in mesh
    T z0 = mesh.vertex(0).attributes().position().z();
    T z1 = z0;

    for (const auto& vertex : mesh.vertices())
    {
        const auto& av = vertex.attributes();

        if (av.position().z() < z0)
        {
            z0 = av.position().z();
        }
        else if (av.position().z() > z1)
        {
            z1 = av.position().z();
        }
    }

    T dz = z1 - z0;
    typename M::vector_type axis = {T(0), T(0), T(1)};

    for (auto& vertex : mesh.vertices())
    {
        T r = sqrt(vertex.attributes().position().x() * vertex.attributes().position().x() + vertex.attributes().position().y() * vertex.attributes().position().y());
        T phi = angle_z * (vertex.attributes().position().z() - z0) / dz + angle_r * r / r_max;

        vertex.attributes().position() *= rotation_axis_unit(axis, phi);
    }

    return;
}

#endif // QUETZAL_MODEL_TRANSFORMS_HPP
