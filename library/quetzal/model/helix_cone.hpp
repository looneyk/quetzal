#if !defined(QUETZAL_MODEL_HELIX_CONE_HPP)
#define QUETZAL_MODEL_HELIX_CONE_HPP
//------------------------------------------------------------------------------
// model
// helix_cone.hpp
//------------------------------------------------------------------------------

#include "primitives_util.hpp"
#include "SurfaceName.hpp"
#include "quetzal/math/Vector.hpp"
#include "quetzal/math/math_util.hpp"
#include <cassert>

namespace quetzal
{

namespace model
{

    // nAzimuth is number of sides
    template<typename M>
    void create_helix_cone(M& mesh, const std::string& name, size_type nAzimuth, size_type nz, value_type<M> rBase, value_type<M> dz, value_type<M> revs, bool bSurfacesDistinct = true);

    template<typename M>
    void create_helix_cone(M& mesh, const std::string& name, size_type nz, value_type<M> dz, const geometry::Polygon<typename M::vector_traits>& polygon, value_type<M> revs, bool bSurfacesDistinct = true);

namespace helix_cone_internal
{

    template<typename V>
    V prototype_face_normal(size_type nAzimuth, size_type nz, typename V::value_type dr, typename V::value_type dz, typename V::value_type revs, size_type i)
    {
        using T = typename V::value_type;

        T t = T(2 * i + 1) / T(2 * nz); 
        T t1 = T(1) - t;
        T r = dr * cos(math::Pi<T> / nAzimuth);
        T dphi = math::PiTwo<T> * revs;
        T phi = dphi * t;
        V position = {r * t1 * cos(phi), r * t1 * sin(phi), dz * t};
        V tangent = {-r * (cos(phi) + dphi * t1 * sin(phi)), r * (dphi * t1 * cos(phi) - sin(phi)), dz}; // dv / dt
        V binormal = {-r * cos(phi), -r * sin(phi), dz};
        V normal = math::normalize(math::cross(tangent, binormal));
//std::cout << i << "\t" << t << "\t" << normal << std::endl;
        return normal;
    }

} // helix_cone_internal

} // namespace model

} // namespace quetzal

//--------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_helix_cone(M& mesh, const std::string& name, size_type nAzimuth, size_type nz, value_type<M> rBase, value_type<M> dz, value_type<M> revs, bool bSurfacesDistinct)
{
    using T = typename M::value_type;

    assert(nAzimuth > 2);
    assert(nz > 0);
    assert(rBase > 0);

    T t = T(1) / T(nz);

    T dr = -rBase;
    T r0 = rBase;
    T r1 = math::lerp(r0, T(0), t);

    T z0 = T(0);
    T z1 = math::lerp(z0, dz, t);

    T rev0 = T(0);
    T rev1 = math::lerp(rev0, revs, t);

    math::Interval azimuth(T(1));
	math::Interval azimuth0 = shift(azimuth, rev0);
	math::Interval azimuth1 = shift(azimuth, rev1);

    normal_vector_type<M> normalProto {dz, T(0), -dr};

    auto tsProto0 = texture_span<T>(0, nz, false, true);
    auto tsProto1 = texture_span<T>(1, nz, false, true);

    bool bLinear = revs == T(0);

    M m;
    id_type idSubmesh = m.create_submesh(name);

    size_type idSurface0 = m.surface_count();
    for (size_type i = 0; i < nAzimuth; ++i)
    {
        m.create_surface(idSubmesh, SurfaceName::BodySection + "_" + to_string(i));
    }

    if (nz == 1)
    {
        create_apex_cusp(m, nAzimuth, r0, z0, dz, azimuth, normalProto, normalProto, idSurface0, bSurfacesDistinct); // azimuth1? ...
    }
    else
    {
std::cout << "create_helix_cone" << nAzimuth<< "\t" << nz<< "\t" << rBase<< "\t" << dz<< "\t" << revs << std::endl;

auto nn = math::normalize(normalProto);
std::cout << "\t" << nn << std::endl;
auto fn = helix_cone_internal::prototype_face_normal<typename M::vector_type>(nAzimuth, nz, rBase, dz, revs, 0);
    	create_band(m, nAzimuth, r0, r1, z0, z1, azimuth0, azimuth1, normalProto, normalProto, tsProto0, tsProto1, idSurface0, bSurfacesDistinct);

        for (size_type i = 2; i < nz; ++i)
        {
            t = T(i) / T(nz);
            r1 = math::lerp(r0, T(0), t);
            z1 = math::lerp(z0, dz, t);
            tsProto1 = texture_span<T>(i, nz, false, true);
            rev1 = math::lerp(rev0, revs, t);
    	    azimuth1 = shift(azimuth, rev1);

fn = helix_cone_internal::prototype_face_normal<typename M::vector_type>(nAzimuth, nz, rBase, dz, revs, i - 1);
    	    connect_band(m, nAzimuth, r1, z1, azimuth1, normalProto, tsProto1, idSurface0, bSurfacesDistinct, bLinear);
        }

fn = helix_cone_internal::prototype_face_normal<typename M::vector_type>(nAzimuth, nz, rBase, dz, revs, nz - 1);
        connect_apex_cusp(m, nAzimuth, dz, azimuth1, normalProto, idSurface0, bSurfacesDistinct, bLinear);
    }

    triangulate(m); // Needs to be done here for better (?) surface normal calculation

    seal_cylinder(m, nAzimuth, nz, false, true, false, ExtentSideFlat<T>(), ExtentEndsFlat<T>(), idSubmesh);

    // This has to be done after sealing bottom so that vertex halfedge iterators will work, fix that ...
    for (auto& surface : m.submesh(idSubmesh).surfaces())
    {
        model::calculate_surface_normals(surface);
    }

    m.check();
    mesh.append(m);
    return;
}

//--------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_helix_cone(M& mesh, const std::string& name, size_type nz, value_type<M> dz, const geometry::Polygon<typename M::vector_traits>& polygon, value_type<M> revs, bool bSurfacesDistinct)
{
    using T = typename M::value_type;

    size_type nAzimuth = polygon.edge_count();
    assert(nAzimuth > 2);
    assert(nz > 0);

    T t = T(1) / T(nz);

    T scale0 = T(1);
    T scale1 = T(1) - t;

    T rev0 = T(0);
    T rev1 = math::lerp(rev0, revs, t);

    T z0 = T(0);
    T z1 = math::lerp(z0, dz, t);

    auto vertex_normal = [dz](const typename M::point_type& point, T rev) -> typename M::vector_type
    {
        T dr = typename M::point_type(point.x(), point.y(), T(0)).norm();
        T phi = math::PiTwo<T> * rev;
        T cos_phi = cos(phi);
        T sin_phi = sin(phi);
        typename M::vector_type normal = math::normalize<typename M::vector_traits>({dz * cos_phi, dz * sin_phi, -dr});
//        typename M::vector_type normal = math::normalize<typename M::vector_traits>({cos_phi * point.x() - sin_phi * point.y(), sin_phi * point.x() + cos_phi * point.y(), T(0)});
// or is this the normal proto? ...
        return normal;
    };

    auto tsProto0 = texture_span<T>(0, nz, false, true);
    auto tsProto1 = texture_span<T>(1, nz, false, true);

    bool bLinear = revs == T(0);

    M m;
    id_type idSubmesh = m.create_submesh(name);

    size_type idSurface0 = m.surface_count();
    for (size_type i = 0; i < nAzimuth; ++i)
    {
        m.create_surface(idSubmesh, SurfaceName::BodySection + "_" + to_string(i)); // normal ...
    }

    std::vector<typename M::vertex_attributes_type> avs0 = vertices_attributes<M>(polygon, scale0, rev0, z0, vertex_normal, tsProto0[2]);

    if (nz == 1)
    {
        std::vector<typename M::vertex_attributes_type> avs1 = apex_vertices_attributes<M>(polygon, dz, revs, dz);
        create_apex_cusp(m, avs0, avs1, true, idSurface0, bSurfacesDistinct);
    }
    else
    {
        std::vector<typename M::vertex_attributes_type> avs1 = vertices_attributes<M>(polygon, scale1, rev1, z1, vertex_normal, tsProto1[2]);
    	create_band(m, avs0, avs1, tsProto0, tsProto1, true, idSurface0, bSurfacesDistinct);

        for (size_type i = 2; i < nz; ++i)
        {
            t = T(i) / T(nz);
            scale1 = T(1) - t;
            rev1 = math::lerp(rev0, revs, t);
            z1 = math::lerp(z0, dz, t);
            tsProto1 = texture_span<T>(i, nz, false, true);

            avs1 = vertices_attributes<M>(polygon, scale1, rev1, z1, vertex_normal, tsProto1[2]);
    	    connect_band(m, avs1, tsProto1, true, idSurface0, bSurfacesDistinct, bLinear);
        }

        avs1 = apex_vertices_attributes<M>(polygon, dz, revs, dz);
        connect_apex_cusp(m, avs1, true, idSurface0, bSurfacesDistinct, bLinear);
    }

    seal_cylinder(m, nAzimuth, nz, false, true, false, ExtentSideFlat<T>(), ExtentEndsFlat<T>(), idSubmesh);

    // This has to be done after sealing bottom so that vertex halfedge iterators will work, fix that ...
    // face normals should be calculated above and applied to vertices here ...
    for (auto& surface : m.submesh(idSubmesh).surfaces())
    {
        model::calculate_surface_normals(surface);
    }

    m.check();
    mesh.append(m);
    return;
}

#endif // QUETZAL_MODEL_HELIX_CONE_HPP
