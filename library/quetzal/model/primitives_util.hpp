#if !defined(QUETZAL_MODEL_PRIMITIVES_UTIL_HPP)
#define QUETZAL_MODEL_PRIMITIVES_UTIL_HPP
//------------------------------------------------------------------------------
// model
// primitives_util.hpp
//------------------------------------------------------------------------------

#include "mesh_attributes.hpp"
#include "quetzal/brep/mesh_geometry.hpp"
#include "quetzal/geometry/OrientedPoint.hpp"
#include "quetzal/geometry/OrientedPolygon.hpp"
#include "quetzal/geometry/Polygon.hpp"
#include "quetzal/math/Interval.hpp"
#include "quetzal/math/Vector.hpp"
#include "quetzal/math/VectorTraits.hpp"
#include "quetzal/math/floating_point.hpp"
#include "quetzal/math/math_util.hpp"
#include "quetzal/math/transformation_matrix.hpp"
#include "quetzal/model/Extent.hpp"
#include <algorithm>
#include <array>
#include <functional>

namespace quetzal::model
{

    using size_type = size_t;

    template<typename T>
    using value_type = T::value_type;

    template<typename M>
    using vertices_attributes_type = std::vector<typename M::vertex_attributes_type>;

    template<typename M>
    using faces_attributes_type = std::vector<typename M::face_attributes_type>;

    template<typename M>
    using azimuth_interval_type = math::Interval<value_type<M>>;

    // typically used for prototype normal vector at the azimuth 0 position
    template<typename M>
    using normal_vector_type = M::vector_type; // may not be necessary with face attributes ...

    template<typename M>
    using texture_span_type = std::array<value_type<M>, 3>;

    template<typename M>
    const texture_span_type<M> texture_span_top = {value_type<M>(0), value_type<M>(1), value_type<M>(0)};

    // azimuth values provided in form of unit circle and connected flag or unit slice outline ...
    // these need extra parameters for properly calculating normal ...
    // just document how each does it and use value forms where a different calculation is required ...
    // at top or bottom cusp, normals need to be calculated differently ...

    // The following create/connect functions can be used to construct cylinders, cones, spheres, and contoured cylinders by succesive layering
    // All angles and intervals that represent angles are measured in turns, 0..1 representing one turn and/or full range
    // Cylindrical coordinates (r, phi, z) with azimuth interval 0..1 -> phi 0..2Pi
    // Spherical coordinates (r, phi, theta) with azimuth interval 0..1 -> phi 0..2Pi; elevation interval 0..1 -> theta -Pi/2..Pi/2

    // Create an open cone with base at z0 and apex at z1 and with nAzimuth divisions
    template<typename M>
    void create_apex_cusp(M& mesh, size_type nAzimuth, value_type<M> r0, value_type<M> z0, value_type<M> z1, azimuth_interval_type<M> azimuth0, id_type idSurface0, bool bSurfacesDistinct = false);

    template<typename M>
    void create_apex_cusp(M& mesh, size_type nAzimuth, value_type<M> r0, value_type<M> z0, value_type<M> z1, azimuth_interval_type<M> azimuth0, normal_vector_type<M> normalProto0, normal_vector_type<M> normalProto1, id_type idSurface0, bool bSurfacesDistinct = false);

    template<typename M>
    void create_apex_cusp(M& mesh, const vertices_attributes_type<M>& avs0, const vertices_attributes_type<M>& avs1, bool bClosed, id_type idSurface0, bool bSurfacesDistinct);

    // Create an open inverted cone with apex at z0 and base at z1 and with nAzimuth divisions
    template<typename M>
    void create_base_cusp(M& mesh, size_type nAzimuth, value_type<M> r1, value_type<M> z0, value_type<M> z1, azimuth_interval_type<M> azimuth1, texture_span_type<M> tsProto1, id_type idSurface0, bool bSurfacesDistinct = false);

    template<typename M>
    void create_base_cusp(M& mesh, size_type nAzimuth, value_type<M> r1, value_type<M> z0, value_type<M> z1, azimuth_interval_type<M> azimuth1, normal_vector_type<M> normalProto0, normal_vector_type<M> normalProto1, texture_span_type<M> tsProto1, id_type idSurface0, bool bSurfacesDistinct = false);

    template<typename M>
    void create_base_cusp(M& mesh, const vertices_attributes_type<M>& avs0, const vertices_attributes_type<M>& avs1, texture_span_type<M> tsProto1, bool bClosed, id_type idSurface0, bool bSurfacesDistinct = false);

    // Create an open cylindrical section with nAzimuth divisions
    template<typename M>
    void create_band(M& mesh, size_type nAzimuth, value_type<M> r0, value_type<M> r1, value_type<M> z0, value_type<M> z1, azimuth_interval_type<M> azimuth0, azimuth_interval_type<M> azimuth1, texture_span_type<M> tsProto0, texture_span_type<M> tsProto1, id_type idSurface0, bool bSurfacesDistinct = false);

    // Create an open cylindrical section with nAzimuth divisions and creating normals based on normalProto
    template<typename M>
    void create_band(M& mesh, size_type nAzimuth, value_type<M> r0, value_type<M> r1, value_type<M> z0, value_type<M> z1, azimuth_interval_type<M> azimuth0, azimuth_interval_type<M> azimuth1, normal_vector_type<M> normalProto0, normal_vector_type<M> normalProto1, texture_span_type<M> tsProto0, texture_span_type<M> tsProto1, id_type idSurface0, bool bSurfacesDistinct = false);

    // Create an open cylindrical section between two vertex value sets
    template<typename M>
    void create_band(M& mesh, const vertices_attributes_type<M>& avs0, const vertices_attributes_type<M>& avs1, texture_span_type<M> tsProto0, texture_span_type<M> tsProto1, bool bClosed, id_type idSurface0, bool bSurfacesDistinct = false);

    // Create an open antiprism with nAzimuth divisions
    template<typename M>
    void create_antiband(M& mesh, size_type nAzimuth, value_type<M> r0, value_type<M> r1, value_type<M> z0, value_type<M> z1, normal_vector_type<M> normalProto0, normal_vector_type<M> normalProto1, value_type<M> ty0, value_type<M> ty1, id_type idSurface0, bool bSurfacesDistinct = false);

    // Create an open antiprism section between two vertex value sets
    template<typename M>
    void create_antiband(M& mesh, const vertices_attributes_type<M>& avs0, const vertices_attributes_type<M>& avs1, id_type idSurface0, bool bSurfacesDistinct = false);

    // Append a cone with base connected to previous layer in mesh with apex at z1 and with nAzimuth divisions
    template<typename M>
    void connect_apex_cusp(M& mesh, size_type nAzimuth, value_type<M> z1, azimuth_interval_type<M> azimuth0, normal_vector_type<M> normalProto1, id_type idSurface0, bool bSurfacesDistinct = false, bool bLinear = false);

    // Append a cone with base connected to previous layer in mesh with apex at z1 and with nAzimuth divisions
    template<typename M>
    void connect_apex_cusp(M& mesh, const vertices_attributes_type<M>& avs1, bool bClosed, id_type idSurface0, bool bSurfacesDistinct = false, bool bLinear = false);

    // Append an open cylinder connected to the previous layer in the mesh with nAzimuth divisions
    template<typename M>
    void connect_band(M& mesh, size_type nAzimuth, value_type<M> r1, value_type<M> z1, azimuth_interval_type<M> azimuth1, texture_span_type<M> tsProto1, id_type idSurface0, bool bSurfacesDistinct = false, bool bLinear = false);

    template<typename M>
    void connect_band(M& mesh, size_type nAzimuth, value_type<M> r1, value_type<M> z1, azimuth_interval_type<M> azimuth1, normal_vector_type<M> normalProto1, texture_span_type<M> tsProto1, id_type idSurface0, bool bSurfacesDistinct = false, bool bLinear = false);

    // Create an open cylindrical section connected to the previous section
    template<typename M>
    void connect_band(M& mesh, const geometry::OrientedPolygon<typename M::vector_traits>& polygon1, const geometry::OrientedPoint<typename M::vector_traits>& op1, texture_span_type<M> tsProto1, id_type idSurface0, bool bSurfacesDistinct = false, bool bLinear = false);

    // Create an open cylindrical section connected to the previous section
    template<typename M>
    void connect_band(M& mesh, const vertices_attributes_type<M>& avs1, texture_span_type<M> tsProto1, bool bClosed, id_type idSurface0, bool bSurfacesDistinct = false, bool bLinear = false);

    // Create an open antiprism section connected to the previous section
    template<typename M>
    void connect_antiband(M& mesh, size_type nAzimuth, value_type<M> r1, value_type<M> z1, value_type<M> ty1, id_type idSurface0, bool bSurfacesDistinct = false);

    // Create an open antiprism section connected to the previous section
    template<typename M>
    void connect_antiband(M& mesh, const vertices_attributes_type<M>& avs1, id_type idSurface0, bool bSurfacesDistinct = false);

    // Connect the borders of an open side with a flat surface divided into nb faces
    template<typename M>
    void connect_side_flat(M& mesh, size_type na, size_type nb, bool bCuspLower, bool bCuspUpper, id_type idSurface, bool bPlanar);

    // Connect the borders of an open side with two surfaces that extend to the vertical center, each divided into nb faces
    // This assumes vertical orientation and flat ends ...
    // need positions for torus ...
    // need triangular, centered faces for sphere with ends center ...
    template<typename M>
    void connect_side_center(M& mesh, size_type na, size_type nb, bool bCuspLower, bool bCuspUpper, id_type idSurface0, id_type idSurface1, std::function<typename M::point_type(const typename M::point_type&)> center_position, bool bPlanar);

    // Connect the borders of an open side with a flat surface consisting of a single face
    // Normals are undefined if side face is not planar
    template<typename M>
    void close_side_flat(M& mesh, size_type na, size_type nb, bool bCuspLower, bool bCuspUpper, id_type idSurface, bool bPlanar);

    // Connect the borders of an open side with two surfaces that extend to the vertical center, each consisting of a single face
    // Normals are undefined if side faces are not planar
    template<typename M>
    void close_side_center(M& mesh, size_type na, size_type nb, bool bCuspLower, bool bCuspUpper, id_type idSurface0, id_type idSurface1, std::function<typename M::point_type(const typename M::point_type&)> center_position, bool bPlanar);

    // Close off all open extents
    template<typename M>
    void seal_cylinder(M& mesh, size_type nAzimuth, size_type nz, bool bCuspLower, bool bCuspUpper, bool bOpenSide, const Extent<value_type<M>>& extentAzimuth, const Extent<value_type<M>>& extentZ, id_type idSubmesh,
            normal_vector_type<M> normalLower = {M::val(0), M::val(0), M::val(-1)}, normal_vector_type<M> normalUpper = {M::val(0), M::val(0), M::val(1)});

    // Close off all open extents
    template<typename M>
    void seal_sphere(M& mesh, size_type nAzimuth, size_type nElevation, bool bCuspLower, bool bCuspUpper, bool bOpenSide, const Extent<value_type<M>>& extentAzimuth, const Extent<value_type<M>>& extentElevation, id_type idSubmesh);

    // Close off all open extents
    template<typename M>
    void seal_torus(M& mesh, size_type nMinor, size_type nMajor, value_type<M> rMajor, bool bCuspLower, bool bCuspUpper, bool bOpenMinor, bool bOpenMajor, const Extent<value_type<M>>& extentMinor, const Extent<value_type<M>>& extentMajor, id_type idSubmesh);

    // Triangulate a triangle with segmented edges to produce a uniform triangulation
    // With rows arranged with the largest across the bottom and getting successively shorter as you go up:
    // id is interior halfedge at right side of longest row
    // n is segments per triangle side
    // Creates a trapezoid from the bottom row, triangulate it, call recursively for remaining area above
    template<typename M>
    void triangulate_rows(M& mesh, id_type idHalfedge, size_type n);

    // Project vertices onto sphere and calculate normals and texture coordinates
    // Internally used for geodesic sphere
    // ForwardIterator specifies vertex range
    template<typename ForwardIterator>
    void project_vertices(ForwardIterator first, ForwardIterator last, typename ForwardIterator::value_type::attributes_type::value_type radius);

    // Precalculate vertex values for a circle parallel to the xy-plane
    // Calculates normals and texture coordinates appropriate for a section of a single encompassing surface
    // Produces nAzimuth + 1 points
    template<typename M>
    vertices_attributes_type<M> vertices_attributes(size_type nAzimuth, value_type<M> radius, value_type<M> z, azimuth_interval_type<M> azimuth, normal_vector_type<M> normalProto, texture_span_type<M> tsProto);

    // Precalculate vertex values for a circle parallel to the xy-plane
    // Calculates normals and texture coordinates appropriate for a section of a single encompassing surface
    // Produces nAzimuth + 1 points
    template<typename M>
    vertices_attributes_type<M> vertices_attributes(const geometry::Polygon<typename M::vector_traits>& polygon, value_type<M> ty);

    // Precalculate vertex values for a layer parallel to the xy-plane
    // Calculates normals and texture coordinates appropriate for a section of a single encompassing surface
    // Produces nAzimuth + 1 points
    template<typename M>
    vertices_attributes_type<M> vertices_attributes(const geometry::Polygon<typename M::vector_traits>& polygon, value_type<M> z, value_type<M> ty);

    // Precalculate vertex values for a layer between two polygons
    // Calculates normals and texture coordinates appropriate for a section of a single encompassing surface
    // Produces nAzimuth + 1 points
    template<typename M>
    vertices_attributes_type<M> vertices_attributes(const geometry::Polygon<typename M::vector_traits>& polygonLower, const geometry::Polygon<typename M::vector_traits>& polygonUpper, value_type<M> t, value_type<M> ty);

    // Precalculate vertex values for a layer parallel to the xy-plane
    // Calculates normals and texture coordinates appropriate for a section of a single encompassing surface
    // Produces nAzimuth + 1 points
    template<typename M>
    vertices_attributes_type<M> vertices_attributes(const geometry::Polygon<typename M::vector_traits>& polygon, value_type<M> scale, value_type<M> revs, value_type<M> z, std::function<typename M::vector_type(const typename M::point_type&, value_type<M>)> vertex_normal, value_type<M> ty);

    // Precalculate vertex values for a layer
    // Produces nAzimuth + 1 points
	// Polygon 3 (could be 2 since it's transformed) ...
    template<typename M>
    vertices_attributes_type<M> vertices_attributes(const geometry::OrientedPolygon<typename M::vector_traits>& polygon, const geometry::OrientedPoint<typename M::vector_traits> op, value_type<M> ty);

    // for apex ...
    // Precalculate vertex values for a layer parallel to the xy-plane
    // Calculates normals and texture coordinates appropriate for a section of a single encompassing surface
    // Produces nAzimuth points
    template<typename M>
    vertices_attributes_type<M> apex_vertices_attributes(const geometry::Polygon<typename M::vector_traits>& polygon, value_type<M> revs, value_type<M> z, value_type<M> dz);

    template<typename T>
    std::array<T, 3> texture_span(T t, bool bCuspLower, bool bCuspUpper, T alignment = T(0.5));

    template<typename T>
    std::array<T, 3> texture_span(size_type i, size_type n, bool bCuspLower, bool bCuspUpper, T alignment = T(0.5));

} // namespace quetzal::model

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_apex_cusp(M& mesh, size_type nAzimuth, value_type<M> r0, value_type<M> z0, value_type<M> z1, azimuth_interval_type<M> azimuth0, id_type idSurface0, bool bSurfacesDistinct)
{
    using T = M::value_type;

    normal_vector_type<M> normalProto {-r0, T(0), z1 - z0};
    create_apex_cusp(mesh, nAzimuth, r0, z0, z1, azimuth0, normalProto, normalProto, idSurface0, bSurfacesDistinct);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_apex_cusp(M& mesh, size_type nAzimuth, value_type<M> r0, value_type<M> z0, value_type<M> z1, azimuth_interval_type<M> azimuth0, normal_vector_type<M> normalProto0, normal_vector_type<M> normalProto1, id_type idSurface0, bool bSurfacesDistinct)
{
    using T = M::value_type;

    assert(nAzimuth > 2);
    assert(r0 > T(0));

    vertices_attributes_type<M> avs0 = vertices_attributes<M>(nAzimuth, r0, z0, azimuth0, normalProto0, {T(0), T(1), T(1)});
    T dAzimuth = T(0.5) * azimuth0.length() / T(nAzimuth);
    azimuth_interval_type<M> azimuth1 = {azimuth0.lower() + dAzimuth, azimuth0.upper() - dAzimuth};
    vertices_attributes_type<M> avs1 = vertices_attributes<M>(nAzimuth - 1, T(0), z1, azimuth1, normalProto1, {T(0.5), T(0.5), T(0)});
    bool bClosed = azimuth0.unit_length();

    create_apex_cusp(mesh, avs0, avs1, bClosed, idSurface0, bSurfacesDistinct);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_apex_cusp(M& mesh, const vertices_attributes_type<M>& avs0, const vertices_attributes_type<M>& avs1, bool bClosed, id_type idSurface0, bool bSurfacesDistinct)
{
    using T = M::value_type;

    size_type nAzimuth = avs0.size() - 1;
    assert(nAzimuth > 2);

    size_type nh = mesh.halfedge_store_count();
    size_type nv = mesh.vertex_store_count();

    mesh.halfedge_store().resize(nh + 3 * nAzimuth);
    mesh.vertex_store().resize(nv + 3 * nAzimuth);

    for (size_type j = 0; j < nAzimuth; ++j)
    {
        size_type j0 = nh;
        size_type j1 = j0 + nAzimuth;
        size_type j2 = j1 + nAzimuth;

        id_type idSurface = idSurface0;
        if (bSurfacesDistinct && idSurface != nullid)
        {
            idSurface += j;
        }

        typename M::vertex_attributes_type av0 = avs0[j];
        typename M::vertex_attributes_type av1 = avs0[j + 1];
        typename M::vertex_attributes_type av2 = {avs1[j].position(), avs1[j].normal(), {T(0.5), T(0)}};

        typename M::vector_type normal = normalize(cross(av0.position() - av2.position(), av1.position() - av2.position()));
        id_type idFace = mesh.create_face(idSurface, j0, {normal});

        id_type idPartner = nullid;
        mesh.halfedge(j0) = {mesh, j0, idPartner, j1, j2, j0, idFace};
        idPartner = (j < nAzimuth - 1 ? j2 + 1 : (bClosed ? j2 - nAzimuth + 1 : nullid));
        mesh.halfedge(j1) = {mesh, j1, idPartner, j2, j0, j1, idFace};
        idPartner = (j > 0 ? j1 - 1 : (bClosed ? j1 + nAzimuth - 1 : nullid));
        mesh.halfedge(j2) = {mesh, j2, idPartner, j0, j1, j2, idFace};

        mesh.vertex(j0) = {mesh, j0, j0, av0};
        mesh.vertex(j1) = {mesh, j1, j1, av1};
        mesh.vertex(j2) = {mesh, j2, j2, av2};

        if (bSurfacesDistinct)
        {
            mesh.vertex(j0).attributes().set_normal(normal);
            mesh.vertex(j1).attributes().set_normal(normal);
            mesh.vertex(j2).attributes().set_normal(normal);

            mesh.vertex(j0).attributes().set_texcoord({0.0, 1.0});
            mesh.vertex(j1).attributes().set_texcoord({1.0, 1.0});

            mesh.surface(idSurface).attributes().set_normal(normal);
        }

        // surface normal not set if !bSurfacesDistinct ...

        ++nh;
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_base_cusp(M& mesh, size_type nAzimuth, value_type<M> r1, value_type<M> z0, value_type<M> z1, azimuth_interval_type<M> azimuth1, texture_span_type<M> tsProto1, id_type idSurface0, bool bSurfacesDistinct)
{
    using T = M::value_type;

    normal_vector_type<M> normalProto {z1 - z0, T(0), -r1};
    create_base_cusp(mesh, nAzimuth, r1, z0, z1, normalProto, normalProto, azimuth1, tsProto1, idSurface0, bSurfacesDistinct);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_base_cusp(M& mesh, size_type nAzimuth, value_type<M> r1, value_type<M> z0, value_type<M> z1, azimuth_interval_type<M> azimuth1, normal_vector_type<M> normalProto0, normal_vector_type<M> normalProto1, texture_span_type<M> tsProto1, id_type idSurface0, bool bSurfacesDistinct)
{
    using T = M::value_type;

    assert(nAzimuth > 2);
    assert(r1 > T(0));

    T dAzimuth = T(0.5) * azimuth1.length() / T(nAzimuth);
    azimuth_interval_type<M> azimuth0 = {azimuth1.lower() + dAzimuth, azimuth1.upper() - dAzimuth};
    vertices_attributes_type<M> avs0 = vertices_attributes<M>(nAzimuth - 1, T(0), z0, azimuth0, normalProto0, {T(0.5), T(0.5), T(1)});
    vertices_attributes_type<M> avs1 = vertices_attributes<M>(nAzimuth, r1, z1, azimuth1, normalProto1, tsProto1);
    bool bClosed = azimuth1.unit_length();

    create_base_cusp(mesh, avs0, avs1, tsProto1, bClosed, idSurface0, bSurfacesDistinct);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_base_cusp(M& mesh, const vertices_attributes_type<M>& avs0, const vertices_attributes_type<M>& avs1, texture_span_type<M> tsProto1, bool bClosed, id_type idSurface0, bool bSurfacesDistinct)
{
    using T = M::value_type;

    size_type nAzimuth = avs1.size() - 1;
    assert(nAzimuth > 2);

    size_type nh = mesh.halfedge_store_count();
    size_type nv = mesh.vertex_store_count();

    mesh.halfedge_store().resize(nh + 3 * nAzimuth);
    mesh.vertex_store().resize(nv + 3 * nAzimuth);

    for (size_type j = 0; j < nAzimuth; ++j)
    {
        size_type j0 = nh;
        size_type j1 = j0 + nAzimuth;
        size_type j2 = j1 + nAzimuth;

        id_type idSurface = idSurface0;
        if (bSurfacesDistinct && idSurface != nullid)
        {
            idSurface += j;
        }

        typename M::vertex_attributes_type av0 = {avs0[j].position(), avs0[j].normal(), {T(0.5), T(1)}};
        typename M::vertex_attributes_type av1 = avs1[j + 1];
        typename M::vertex_attributes_type av2 = avs1[j];

        typename M::vector_type normal = normalize(cross(av1.position() - av0.position(), av2.position() - av0.position()));
        id_type idFace = mesh.create_face(idSurface, j0, {normal});

        id_type idPartner = (j < nAzimuth - 1 ? j2 + 1 : (bClosed ? j2 - nAzimuth + 1 : nullid));
        mesh.halfedge(j0) = {mesh, j0, idPartner, j1, j2, j0, idFace};
        idPartner = nullid;
        mesh.halfedge(j1) = {mesh, j1, idPartner, j2, j0, j1, idFace};
        idPartner = (j > 0 ? j0 - 1 : (bClosed ? j0 + nAzimuth - 1 : nullid));
        mesh.halfedge(j2) = {mesh, j2, idPartner, j0, j1, j2, idFace};

        mesh.vertex(j0) = {mesh, j0, j0, av0};
        mesh.vertex(j1) = {mesh, j1, j1, av1};
        mesh.vertex(j2) = {mesh, j2, j2, av2};

        if (bSurfacesDistinct)
        {
            mesh.vertex(j0).attributes().set_normal(normal);
            mesh.vertex(j1).attributes().set_normal(normal);
            mesh.vertex(j2).attributes().set_normal(normal);

            mesh.vertex(j1).attributes().texcoord().set_x(tsProto1[1]);
            mesh.vertex(j2).attributes().texcoord().set_x(tsProto1[0]);

            mesh.surface(idSurface).attributes().set_normal(normal);
        }

        // surface normal not set if !bSurfacesDistinct ...

        ++nh;
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_band(M& mesh, size_type nAzimuth, value_type<M> r0, value_type<M> r1, value_type<M> z0, value_type<M> z1, azimuth_interval_type<M> azimuth0, azimuth_interval_type<M> azimuth1, texture_span_type<M> tsProto0, texture_span_type<M> tsProto1, id_type idSurface0, bool bSurfacesDistinct)
{
    using T = M::value_type;
    T dr = r1 - r0;
    T dz = z1 - z0;
    create_band(mesh, nAzimuth, r0, r1, z0, z1, azimuth0, azimuth1, {dz, T(0), -dr}, {dz, T(0), -dr}, tsProto0, tsProto1, idSurface0, bSurfacesDistinct);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_band(M& mesh, size_type nAzimuth, value_type<M> r0, value_type<M> r1, value_type<M> z0, value_type<M> z1, azimuth_interval_type<M> azimuth0, azimuth_interval_type<M> azimuth1, normal_vector_type<M> normalProto0, normal_vector_type<M> normalProto1, texture_span_type<M> tsProto0, texture_span_type<M> tsProto1, id_type idSurface0, bool bSurfacesDistinct)
{
    using T = M::value_type;

    assert(nAzimuth > 2);
    assert(r0 >= 0);
    assert(r1 >= 0);
    assert(math::float_ne(r0, r1) || math::float_ne(z0, z1));

    vertices_attributes_type<M> avs0 = vertices_attributes<M>(nAzimuth, r0, z0, azimuth0, normalProto0, tsProto0);
    vertices_attributes_type<M> avs1 = vertices_attributes<M>(nAzimuth, r1, z1, azimuth1, normalProto1, tsProto1);
    bool bClosed = azimuth0.unit_length() && azimuth1.unit_length(); // handle differing lengths? ...

    create_band(mesh, avs0, avs1, tsProto0, tsProto1, bClosed, idSurface0, bSurfacesDistinct);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_band(M& mesh, const vertices_attributes_type<M>& avs0, const vertices_attributes_type<M>& avs1, texture_span_type<M> tsProto0, texture_span_type<M> tsProto1, bool bClosed, id_type idSurface0, bool bSurfacesDistinct)
{
    using T = M::value_type;

    size_type nAzimuth = avs0.size() - 1;
    assert(nAzimuth > 2);
    assert(avs0.size() == avs1.size());

    size_type nh = mesh.halfedge_store_count();
    size_type nv = mesh.vertex_store_count();

    mesh.halfedge_store().resize(nh + 4 * nAzimuth);
    mesh.vertex_store().resize(nv + 4 * nAzimuth);

    for (size_type j = 0; j < nAzimuth; ++j)
    {
        size_type j0 = nh;
        size_type j1 = j0 + nAzimuth;
        size_type j2 = j1 + nAzimuth;
        size_type j3 = j2 + nAzimuth;

        id_type idSurface = idSurface0;
        if (bSurfacesDistinct && idSurface != nullid)
        {
            idSurface += j;
        }

        typename M::vertex_attributes_type av0 = avs0[j];
        typename M::vertex_attributes_type av1 = avs0[j + 1];
        typename M::vertex_attributes_type av2 = avs1[j + 1];
        typename M::vertex_attributes_type av3 = avs1[j];

        typename M::vector_type normal = normalize(cross(av1.position() - av0.position(), av3.position() - av0.position()));
        id_type idFace = mesh.create_face(idSurface, j0, {normal});

        id_type idPartner = nullid;
        mesh.halfedge(j0) = {mesh, j0, idPartner, j1, j3, j0, idFace};
        idPartner = (j < nAzimuth - 1 ? j3 + 1 : (bClosed ? j3 - nAzimuth + 1 : nullid));
        mesh.halfedge(j1) = {mesh, j1, idPartner, j2, j0, j1, idFace};
        idPartner = nullid;
        mesh.halfedge(j2) = {mesh, j2, idPartner, j3, j1, j2, idFace};
        idPartner = (j > 0 ? j1 - 1 : (bClosed ? j1 + nAzimuth - 1 : nullid));
        mesh.halfedge(j3) = {mesh, j3, idPartner, j0, j2, j3, idFace};

        mesh.vertex(j0) = {mesh, j0, j0, av0};
        mesh.vertex(j1) = {mesh, j1, j1, av1};
        mesh.vertex(j2) = {mesh, j2, j2, av2};
        mesh.vertex(j3) = {mesh, j3, j3, av3};

        if (bSurfacesDistinct)
        {
            mesh.vertex(j0).attributes().set_normal(normal);
            mesh.vertex(j1).attributes().set_normal(normal);
            mesh.vertex(j2).attributes().set_normal(normal);
            mesh.vertex(j3).attributes().set_normal(normal);

            mesh.vertex(j0).attributes().texcoord().set_x(tsProto0[0]);
            mesh.vertex(j1).attributes().texcoord().set_x(tsProto0[1]);
            mesh.vertex(j2).attributes().texcoord().set_x(tsProto1[1]);
            mesh.vertex(j3).attributes().texcoord().set_x(tsProto1[0]);

            mesh.surface(idSurface).attributes().set_normal(normal);
        }

        // surface normal not set if !bSurfacesDistinct ...

        ++nh;
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_antiband(M& mesh, size_type nAzimuth, value_type<M> r0, value_type<M> r1, value_type<M> z0, value_type<M> z1, normal_vector_type<M> normalProto0, normal_vector_type<M> normalProto1, value_type<M> ty0, value_type<M> ty1, id_type idSurface0, bool bSurfacesDistinct)
{
    using T = M::value_type;

    assert(nAzimuth > 2);
    assert(r0 >= 0);
    assert(r1 >= 0);

    math::Interval<T> azimuth(T(1));
    T dAzimuth = T(0.5) / T(nAzimuth);

    vertices_attributes_type<M> avs0 = vertices_attributes<M>(nAzimuth, r0, z0, azimuth, normalProto0, {T(0), T(1), ty0});
    vertices_attributes_type<M> avs1 = vertices_attributes<M>(nAzimuth, r1, z1, shift(azimuth, dAzimuth), normalProto1, {T(0), T(1), ty1});

    create_antiband(mesh, avs0, avs1, idSurface0, bSurfacesDistinct);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_antiband(M& mesh, const vertices_attributes_type<M>& avs0, const vertices_attributes_type<M>& avs1, id_type idSurface0, bool bSurfacesDistinct)
{
    using T = M::value_type;

    assert(avs0.size() == avs1.size());
    size_type nAzimuth = avs0.size() - 1;
    assert(nAzimuth > 2);

    size_type nh = mesh.halfedge_store_count();

    mesh.halfedge_store().resize(nh + 6 * nAzimuth);
    mesh.vertex_store().resize(nh + 6 * nAzimuth);

    for (size_type j = 0; j < nAzimuth; ++j)
    {
        size_type j0 = nh;
        size_type j1 = j0 + nAzimuth;
        size_type j2 = j1 + nAzimuth;
        size_type j3 = j2 + nAzimuth;
        size_type j4 = j3 + nAzimuth;
        size_type j5 = j4 + nAzimuth;

        id_type idSurface = idSurface0;
        if (bSurfacesDistinct && idSurface != nullid)
        {
            idSurface += j;
        }

        id_type idFace = mesh.create_face(idSurface, j0, {avs1[j].normal()});

        id_type idPartner = nullid;
        mesh.halfedge(j0) = {mesh, j0, idPartner, j1, j2, j0, idFace};
        idPartner = j5;
        mesh.halfedge(j1) = {mesh, j1, idPartner, j2, j0, j1, idFace};
        idPartner = (j > 0 ? j3 - 1 : j3 + nAzimuth - 1);
        mesh.halfedge(j2) = {mesh, j2, idPartner, j0, j1, j2, idFace};

        mesh.vertex(j0) = {mesh, j0, j0, avs0[j]};
        mesh.vertex(j1) = {mesh, j1, j1, avs0[j + 1]};
        mesh.vertex(j2) = {mesh, j2, j2, avs1[j]};

        if (bSurfacesDistinct)
        {
            typename M::vector_type normal = face_normal(mesh.face(idFace));
            mesh.face(idFace).attributes().set_normal(normal);
            mesh.surface(idSurface).attributes().set_normal(normal);

            mesh.vertex(j0).attributes().set_normal(normal);
            mesh.vertex(j1).attributes().set_normal(normal);
            mesh.vertex(j2).attributes().set_normal(normal);

            mesh.vertex(j0).attributes().set_texcoord({T(0), T(1)});
            mesh.vertex(j1).attributes().set_texcoord({T(1), T(1)});
            mesh.vertex(j2).attributes().set_texcoord({T(0.5), T(0)});
        }

        idSurface = idSurface0;
        if (bSurfacesDistinct && idSurface != nullid)
        {
            idSurface += nAzimuth + j;
        }

        idFace = mesh.create_face(idSurface, j3, {avs0[j + 1].normal()});

        idPartner = (j < nAzimuth - 1 ? j2 + 1 : j2 - nAzimuth + 1);
        mesh.halfedge(j3) = {mesh, j3, idPartner, j4, j5, j3, idFace};
        idPartner = nullid;
        mesh.halfedge(j4) = {mesh, j4, idPartner, j5, j3, j4, idFace};
        idPartner = j1;
        mesh.halfedge(j5) = {mesh, j5, idPartner, j3, j4, j5, idFace};

        mesh.vertex(j3) = {mesh, j3, j3, avs0[j + 1]};
        mesh.vertex(j4) = {mesh, j4, j4, avs1[j + 1]};
        mesh.vertex(j5) = {mesh, j5, j5, avs1[j]};

        if (bSurfacesDistinct)
        {
            typename M::vector_type normal = face_normal(mesh.face(idFace));
            mesh.face(idFace).attributes().set_normal(normal);
            mesh.surface(idSurface).attributes().set_normal(normal);

            mesh.vertex(j3).attributes().set_normal(normal);
            mesh.vertex(j4).attributes().set_normal(normal);
            mesh.vertex(j5).attributes().set_normal(normal);

            mesh.vertex(j3).attributes().set_texcoord({T(0.5), T(1)});
            mesh.vertex(j4).attributes().set_texcoord({T(1), T(0)});
            mesh.vertex(j5).attributes().set_texcoord({T(0), T(0)});
        }

        ++nh;
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::connect_apex_cusp(M& mesh, size_type nAzimuth, value_type<M> z1, azimuth_interval_type<M> azimuth0, normal_vector_type<M> normalProto1, id_type idSurface0, bool bSurfacesDistinct, bool bLinear)
{
    using T = M::value_type;

    assert(nAzimuth > 2);
    assert(mesh.halfedge_count() >= 2 * nAzimuth);

    T dAzimuth = T(0.5) * azimuth0.length() / T(nAzimuth);
    azimuth_interval_type<M> azimuth1 = {azimuth0.lower() + dAzimuth, azimuth0.upper() - dAzimuth};
    vertices_attributes_type<M> avs1 = vertices_attributes<M>(nAzimuth - 1, T(0), z1, azimuth1, normalProto1, {T(0.5), T(0.5), T(0)});
    bool bClosed = azimuth0.unit_length();

    size_type nh = mesh.halfedge_store_count();
    size_type nv = mesh.vertex_store_count();

    mesh.halfedge_store().resize(nh + 3 * nAzimuth);
    mesh.vertex_store().resize(nv + 3 * nAzimuth);

    for (size_type j = 0; j < nAzimuth; ++j)
    {
        size_type j0 = nh;
        size_type j1 = j0 + nAzimuth;
        size_type j2 = j1 + nAzimuth;
        size_type j3 = j2 + nAzimuth;

        id_type idSurface = idSurface0;
        if (bSurfacesDistinct && idSurface != nullid)
        {
            idSurface += j;
        }

        typename M::vertex_attributes_type av0 = mesh.vertex(j3 - 4 * nAzimuth).attributes();
        typename M::vertex_attributes_type av1 = mesh.vertex(j2 - 4 * nAzimuth).attributes();
        typename M::vertex_attributes_type av2 = avs1[j];

        typename M::vector_type normal = mesh.face(mesh.face_count() - nAzimuth).attributes().normal();
        if (!bLinear)
        {
            normal = normalize(cross(av0.position() - av2.position(), av1.position() - av2.position()));
        }

        id_type idFace = mesh.create_face(idSurface, j0, {normal});

        size_type idPartner = j0 - 2 * nAzimuth;
        mesh.halfedge(idPartner).set_partner_id(j0);

        mesh.halfedge(j0) = {mesh, j0, idPartner, j1, j2, j0, idFace};
        idPartner = (j < nAzimuth - 1 ? j2 + 1 : (bClosed ? j2 - nAzimuth + 1 : nullid));
        mesh.halfedge(j1) = {mesh, j1, idPartner, j2, j0, j1, idFace};
        idPartner = (j > 0 ? j1 - 1 : (bClosed ? j1 + nAzimuth - 1 : nullid));
        mesh.halfedge(j2) = {mesh, j2, idPartner, j0, j1, j2, idFace};

        mesh.vertex(j0) = {mesh, j0, j0, av0};
        mesh.vertex(j1) = {mesh, j1, j1, av1};
        mesh.vertex(j2) = {mesh, j2, j2, av2};

        if (bSurfacesDistinct)
        {
            mesh.vertex(j0).attributes().set_normal(normal);
            mesh.vertex(j1).attributes().set_normal(normal);
            mesh.vertex(j2).attributes().set_normal(normal);
        }

        ++nh;
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::connect_apex_cusp(M& mesh, const vertices_attributes_type<M>& avs1, bool bClosed, id_type idSurface0, bool bSurfacesDistinct, bool bLinear)
{
    using T = M::value_type;

    size_type nAzimuth = avs1.size();
    assert(nAzimuth > 2);
    assert(mesh.halfedge_count() >= 2 * nAzimuth);

    size_type nh = mesh.halfedge_store_count();
    size_type nv = mesh.vertex_store_count();

    mesh.halfedge_store().resize(nh + 3 * nAzimuth);
    mesh.vertex_store().resize(nv + 3 * nAzimuth);

    for (size_type j = 0; j < nAzimuth; ++j)
    {
        size_type j0 = nh;
        size_type j1 = j0 + nAzimuth;
        size_type j2 = j1 + nAzimuth;
        size_type j3 = j2 + nAzimuth;

        id_type idSurface = idSurface0;
        if (bSurfacesDistinct && idSurface != nullid)
        {
            idSurface += j;
        }

        typename M::vertex_attributes_type av0 = mesh.vertex(j3 - 4 * nAzimuth).attributes();
        typename M::vertex_attributes_type av1 = mesh.vertex(j2 - 4 * nAzimuth).attributes();
        typename M::vertex_attributes_type av2 = avs1[j];

        typename M::vector_type normal = mesh.face(mesh.face_count() - nAzimuth).attributes().normal();
        if (!bLinear)
        {
            normal = normalize(cross(av0.position() - av2.position(), av1.position() - av2.position()));
        }

        id_type idFace = mesh.create_face(idSurface, j0, {normal});

        size_type idPartner = j0 - 2 * nAzimuth;
        mesh.halfedge(idPartner).set_partner_id(j0);

        mesh.halfedge(j0) = {mesh, j0, idPartner, j1, j2, j0, idFace};
        idPartner = (j < nAzimuth - 1 ? j2 + 1 : (bClosed ? j2 - nAzimuth + 1 : nullid));
        mesh.halfedge(j1) = {mesh, j1, idPartner, j2, j0, j1, idFace};
        idPartner = (j > 0 ? j1 - 1 : (bClosed ? j1 + nAzimuth - 1 : nullid));
        mesh.halfedge(j2) = {mesh, j2, idPartner, j0, j1, j2, idFace};

        mesh.vertex(j0) = {mesh, j0, j0, av0};
        mesh.vertex(j1) = {mesh, j1, j1, av1};
        mesh.vertex(j2) = {mesh, j2, j2, av2};

        if (bSurfacesDistinct)
        {
            mesh.vertex(j0).attributes().set_normal(normal);
            mesh.vertex(j1).attributes().set_normal(normal);
            mesh.vertex(j2).attributes().set_normal(normal);
        }

        ++nh;
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::connect_band(M& mesh, size_type nAzimuth, value_type<M> r1, value_type<M> z1, azimuth_interval_type<M> azimuth1, texture_span_type<M> tsProto1, id_type idSurface0, bool bSurfacesDistinct, bool bLinear)
{
    using T = M::value_type;

    assert(mesh.halfedge_count() >= 2 * nAzimuth);

    auto position0 = mesh.halfedge(mesh.halfedge_store_count() - 2 * nAzimuth).attributes().position();
    T z0 = position0.z();
    T dz = z1 - z0;

    position0.set_z(T(0));
    T r0 = position0.norm();
    T dr = r1 - r0;

    connect_band(mesh, nAzimuth, r1, z1, azimuth1, {dz, T(0), -dr}, tsProto1, idSurface0, bSurfacesDistinct, bLinear);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::connect_band(M& mesh, size_type nAzimuth, value_type<M> r1, value_type<M> z1, azimuth_interval_type<M> azimuth1, normal_vector_type<M> normalProto1, texture_span_type<M> tsProto1, id_type idSurface0, bool bSurfacesDistinct, bool bLinear)
{
    using T = M::value_type;

    assert(nAzimuth > 2);
    assert(r1 >= T(0));

    vertices_attributes_type<M> avs1 = vertices_attributes<M>(nAzimuth, r1, z1, azimuth1, normalProto1, tsProto1);
    bool bClosed = azimuth1.unit_length();

    connect_band(mesh, avs1, tsProto1, bClosed, idSurface0, bSurfacesDistinct, bLinear);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::connect_band(M& mesh, const geometry::OrientedPolygon<typename M::vector_traits>& polygon, const geometry::OrientedPoint<typename M::vector_traits>& op1, texture_span_type<M> tsProto1, id_type idSurface0, bool bSurfacesDistinct, bool bLinear)
{
    vertices_attributes_type<M> avs1 = vertices_attributes<M>(polygon, op1, tsProto1[2]);
    connect_band(mesh, avs1, true, idSurface0, bSurfacesDistinct, bLinear);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::connect_band(M& mesh, const vertices_attributes_type<M>& avs1, texture_span_type<M> tsProto1, bool bClosed, id_type idSurface0, bool bSurfacesDistinct, bool bLinear)
{
    using T = M::value_type;

    size_type nAzimuth = avs1.size() - 1;
    assert(nAzimuth > 2);
    assert(mesh.halfedge_count() >= 2 * nAzimuth);

    size_type nh = mesh.halfedge_store_count();
    size_type nv = mesh.vertex_store_count();

    mesh.halfedge_store().resize(nh + 4 * nAzimuth);
    mesh.vertex_store().resize(nv + 4 * nAzimuth);

    for (size_type j = 0; j < nAzimuth; ++j)
    {
        size_type j0 = nh;
        size_type j1 = j0 + nAzimuth;
        size_type j2 = j1 + nAzimuth;
        size_type j3 = j2 + nAzimuth;

        id_type idSurface = idSurface0;
        if (bSurfacesDistinct && idSurface != nullid)
        {
            idSurface += j;
        }

        typename M::vertex_attributes_type av0 = mesh.vertex(j3 - 4 * nAzimuth).attributes();
        typename M::vertex_attributes_type av1 = mesh.vertex(j2 - 4 * nAzimuth).attributes();
        typename M::vertex_attributes_type av2 = avs1[j + 1];
        typename M::vertex_attributes_type av3 = avs1[j];

        typename M::vector_type normal = mesh.face(mesh.face_count() - nAzimuth).attributes().normal();
        if (!bLinear)
        {
            normal = normalize(cross(av1.position() - av0.position(), av3.position() - av0.position()));
        }

        id_type idFace = mesh.create_face(idSurface, j0, {normal});

        id_type idPartner = j2 - 4 * nAzimuth;
        mesh.halfedge(idPartner).set_partner_id(j0);

        mesh.halfedge(j0) = {mesh, j0, idPartner, j1, j3, j0, idFace};
        idPartner = (j < nAzimuth - 1 ? j3 + 1 : (bClosed ? j3 - nAzimuth + 1 : nullid));
        mesh.halfedge(j1) = {mesh, j1, idPartner, j2, j0, j1, idFace};
        idPartner = nullid;
        mesh.halfedge(j2) = {mesh, j2, idPartner, j3, j1, j2, idFace};
        idPartner = (j > 0 ? j1 - 1 : (bClosed ? j1 + nAzimuth - 1 : nullid));
        mesh.halfedge(j3) = {mesh, j3, idPartner, j0, j2, j3, idFace};

        mesh.vertex(j0) = {mesh, j0, j0, av0};
        mesh.vertex(j1) = {mesh, j1, j1, av1};
        mesh.vertex(j2) = {mesh, j2, j2, av2};
        mesh.vertex(j3) = {mesh, j3, j3, av3};

        if (bSurfacesDistinct)
        {
            mesh.vertex(j0).attributes().set_normal(normal);
            mesh.vertex(j1).attributes().set_normal(normal);
            mesh.vertex(j2).attributes().set_normal(normal);
            mesh.vertex(j3).attributes().set_normal(normal);

            mesh.vertex(j2).attributes().texcoord().set_x(tsProto1[1]);
            mesh.vertex(j3).attributes().texcoord().set_x(tsProto1[0]);
        }
        else if (bLinear) // see if this really makes any difference once all other issue are resolved ...
        {
            assert(vector_eq(av2.normal(), av1.normal()));
            assert(vector_eq(av3.normal(), av0.normal()));
            av2.set_normal(av1.normal());
            av3.set_normal(av0.normal());
        }

        ++nh;
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::connect_antiband(M& mesh, size_type nAzimuth, value_type<M> r1, value_type<M> z1, value_type<M> ty1, id_type idSurface0, bool bSurfacesDistinct)
{
    using T = M::value_type;

    assert(nAzimuth > 2);
    assert(r1 >= T(0));
    assert(mesh.halfedge_count() >= 6 * nAzimuth);

    size_type nh = mesh.halfedge_store_count();

    vertices_attributes_type<M> avs1(nAzimuth + 1);
    for (size_t i = 0; i < nAzimuth; ++i)
    {
        avs1[i] = mesh.halfedge(nh - 3 * nAzimuth + i).attributes();
        avs1[i].position().set_z(z1);
        avs1[i].texcoord().set_y(ty1);
        avs1[i].texcoord().set_x(mesh.halfedge(nh - 4 * nAzimuth + i).attributes().texcoord().x());
    }

    avs1[nAzimuth] = avs1[0];
    avs1[nAzimuth].texcoord().set_x(T(1));

    connect_antiband(mesh, avs1, idSurface0, bSurfacesDistinct);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::connect_antiband(M& mesh, const vertices_attributes_type<M>& avs1, id_type idSurface0, bool bSurfacesDistinct)
{
    using T = M::value_type;

    size_type nAzimuth = avs1.size() - 1;
    assert(nAzimuth > 2);
    assert(mesh.halfedge_count() >= 6 * nAzimuth);

    size_type nh = mesh.halfedge_store_count();

    vertices_attributes_type<M> avs0(nAzimuth + 1);
    for (size_t i = 0; i < nAzimuth; ++i)
    {
        avs0[i] = mesh.halfedge(nh - nAzimuth + i).attributes();
    }

    avs0[nAzimuth] = avs0[0];
    avs0[nAzimuth].texcoord().set_x(T(1));

    create_antiband(mesh, avs0, avs1, idSurface0, bSurfacesDistinct);
    nh = mesh.halfedge_store_count();

    for (size_t i = 0; i < nAzimuth; ++i)
    {
        size_type idHalfedge0 = nh - 8 * nAzimuth + i;
        size_type idHalfedge1 = nh - 6 * nAzimuth + i;

        mesh.halfedge(idHalfedge0).set_partner_id(idHalfedge1);
        mesh.halfedge(idHalfedge1).set_partner_id(idHalfedge0);
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::connect_side_flat(M& mesh, size_type na, size_type nb, bool bCuspLower, bool bCuspUpper, id_type idSurface, bool bPlanar)
{
    using T = M::value_type;

    size_type nh = mesh.halfedge_store_count();
    size_type nv = mesh.vertex_store_count();

    size_type n = 4 * nb;
    if (bCuspLower)
    {
        --n;
    }

    if (bCuspUpper)
    {
        --n;
    }

    mesh.halfedge_store().resize(nh + n);
    mesh.vertex_store().resize(nv + n);

    id_type idPartner0 = nullid;
    id_type idPartner1 = 3 * na;
    id_type idPartner2 = nullid;
    id_type idPartner3 = 2 * na - 1;

    typename M::vector_type normal;

    for (size_type j = 0; j < nb; ++j)
    {
        size_type j0 = nh + 0;
        size_type j1 = nh + 1;
        size_type j2 = nh + 2;
        size_type j3 = nh + 3;

        if (idPartner0 != nullid)
        {
            mesh.halfedge(idPartner0).set_partner_id(j0);
        }

        id_type idFace = mesh.create_face(idSurface, j0);

        if (j == 0 && bCuspLower)
        {
            idPartner1 -= na;
            idPartner3 -= na;

            mesh.halfedge(j0) = {mesh, j0, idPartner1, j1, j2, j0, idFace};
            mesh.halfedge(j1) = {mesh, j1, idPartner2, j2, j0, j1, idFace};
            mesh.halfedge(j2) = {mesh, j2, idPartner3, j0, j1, j2, idFace};

            mesh.halfedge(idPartner1).set_partner_id(j0);
            mesh.halfedge(idPartner3).set_partner_id(j2);

            typename M::vertex_attributes_type av0 = mesh.halfedge(idPartner1).next().attributes();
            typename M::vertex_attributes_type av1 = mesh.halfedge(idPartner1).attributes();
            typename M::vertex_attributes_type av2 = mesh.halfedge(idPartner3).next().attributes();

            normal = normalize(cross(av1.position() - av0.position(), av2.position() - av0.position()));
            av0.set_normal(normal);
            av1.set_normal(normal);
            av2.set_normal(normal);

            std::array<T, 3> tsProto = texture_span<T>(j + 1, nb, bCuspLower, bCuspUpper);
            av0.set_texcoord({T(0.5), T(1)});
            av1.set_texcoord({tsProto[1], tsProto[2]});
            av2.set_texcoord({tsProto[0], tsProto[2]});
            assert(mesh.halfedge(idPartner3).next().attributes().texcoord().y() == tsProto[2]);
            assert(mesh.halfedge(idPartner1).attributes().texcoord().y() == tsProto[2]);

            mesh.vertex(j0) = {mesh, j0, j0, av0};
            mesh.vertex(j1) = {mesh, j1, j1, av1};
            mesh.vertex(j2) = {mesh, j2, j2, av2};
        }
        else if (j == nb - 1 && bCuspUpper)
        {
            idPartner1 -= na;

            mesh.halfedge(j0) = {mesh, j0, idPartner0, j1, j2, j0, idFace};
            mesh.halfedge(j1) = {mesh, j1, idPartner1, j2, j0, j1, idFace};
            mesh.halfedge(j2) = {mesh, j2, idPartner3, j0, j1, j2, idFace};

            mesh.halfedge(idPartner1).set_partner_id(j1);
            mesh.halfedge(idPartner3).set_partner_id(j2);

            typename M::vertex_attributes_type av0 = mesh.halfedge(idPartner3).attributes();
            typename M::vertex_attributes_type av1 = mesh.halfedge(idPartner1).next().attributes();
            typename M::vertex_attributes_type av2 = mesh.halfedge(idPartner3).next().attributes();

            if (!bPlanar || normal.zero())
            {
                normal = normalize(cross(av0.position() - av2.position(), av1.position() - av2.position()));
            }

            if (!bPlanar && idPartner0 != nullid)
            {
                av0.set_normal(normalize(mesh.halfedge(idPartner0).face().attributes().normal() + normal));
                av1.set_normal(av0.normal());

                mesh.halfedge(idPartner0).attributes().set_normal(av0.normal());
                mesh.halfedge(idPartner0).next().attributes().set_normal(av0.normal());
            }
            else
            {
                av0.set_normal(normal);
                av1.set_normal(normal);
            }

            av2.set_normal(normal);

            if (idPartner0 != nullid)
            {
                av0.set_texcoord(mesh.halfedge(idPartner0).next().attributes().texcoord());
                av1.set_texcoord(mesh.halfedge(idPartner0).attributes().texcoord());
            }
            else
            {
                av0.set_texcoord({T(0), T(1)});
                av1.set_texcoord({T(1), T(1)});
            }

            av2.set_texcoord({T(0.5), T(0)});

            mesh.vertex(j0) = {mesh, j0, j0, av0};
            mesh.vertex(j1) = {mesh, j1, j1, av1};
            mesh.vertex(j2) = {mesh, j2, j2, av2};
        }
        else
        {
            mesh.halfedge(j0) = {mesh, j0, idPartner0, j1, j3, j0, idFace};
            mesh.halfedge(j1) = {mesh, j1, idPartner1, j2, j0, j1, idFace};
            mesh.halfedge(j2) = {mesh, j2, idPartner2, j3, j1, j2, idFace};
            mesh.halfedge(j3) = {mesh, j3, idPartner3, j0, j2, j3, idFace};

            mesh.halfedge(idPartner1).set_partner_id(j1);
            mesh.halfedge(idPartner3).set_partner_id(j3);

            typename M::vertex_attributes_type av0 = mesh.halfedge(idPartner3).attributes();
            typename M::vertex_attributes_type av1 = mesh.halfedge(idPartner1).next().attributes();
            typename M::vertex_attributes_type av2 = mesh.halfedge(idPartner1).attributes();
            typename M::vertex_attributes_type av3 = mesh.halfedge(idPartner3).next().attributes();

            if (!bPlanar || normal.zero())
            {
                normal = normalize(cross(av1.position() - av0.position(), av3.position() - av0.position()));
            }

            if (!bPlanar && idPartner0 != nullid)
            {
                av0.set_normal(normalize(mesh.halfedge(idPartner0).face().attributes().normal() + normal));
                av1.set_normal(av0.normal());

                mesh.halfedge(idPartner0).attributes().set_normal(av0.normal());
                mesh.halfedge(idPartner0).next().attributes().set_normal(av0.normal());
            }
            else
            {
                av0.set_normal(normal);
                av1.set_normal(normal);
            }

            av2.set_normal(normal);
            av3.set_normal(normal);

            if (idPartner0 != nullid)
            {
                av0.set_texcoord(mesh.halfedge(idPartner0).next().attributes().texcoord());
                av1.set_texcoord(mesh.halfedge(idPartner0).attributes().texcoord());
            }
            else
            {
                av0.set_texcoord({T(0), T(1)});
                av1.set_texcoord({T(1), T(1)});
            }

            std::array<T, 3> tsProto = texture_span<T>(j + 1, nb, bCuspLower, bCuspUpper);
            av2.set_texcoord({tsProto[1], tsProto[2]});
            av3.set_texcoord({tsProto[0], tsProto[2]});
            assert(mesh.halfedge(idPartner3).next().attributes().texcoord().y() == tsProto[2]);
            assert(mesh.halfedge(idPartner1).attributes().texcoord().y() == tsProto[2]);

            mesh.vertex(j0) = {mesh, j0, j0, av0};
            mesh.vertex(j1) = {mesh, j1, j1, av1};
            mesh.vertex(j2) = {mesh, j2, j2, av2};
            mesh.vertex(j3) = {mesh, j3, j3, av3};
        }

        mesh.face(idFace).attributes().set_normal(normal);

        idPartner0 = j2;
        idPartner1 += 4 * na;
        idPartner3 += 4 * na;

        nh += 4;

        if (j == 0 && bCuspLower)
        {
            idPartner0 = j1;
            --nh;
        }
    }

    if (!bPlanar)
    {
        size_type nf = mesh.face_count();
        typename M::vector_type sum;
        for (size_t i = 1; i <= nb; ++i)
        {
            sum += mesh.face(nf - i).attributes().normal();
        }

        normal = normalize(sum);
    }

    mesh.surface(idSurface).attributes().set_normal(normal);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::connect_side_center(M& mesh, size_type na, size_type nb, bool bCuspLower, bool bCuspUpper, id_type idSurface0, id_type idSurface1, std::function<typename M::point_type(const typename M::point_type&)> center_position, bool bPlanar)
{
    using T = M::value_type;

    size_type nh = mesh.halfedge_store_count();
    size_type nv = mesh.vertex_store_count();

    size_type n = 4 * nb;
    if (bCuspLower)
    {
        --n;
    }

    if (bCuspUpper)
    {
        --n;
    }

    mesh.halfedge_store().resize(nh + 2 * n);
    mesh.vertex_store().resize(nv + 2 * n);

    size_type nhOrig = nh;

    // Lower

    id_type idPartner0 = nullid;
    id_type idPartner1 = 3 * na;
    id_type idPartner2 = nullid;
    id_type idPartner3 = nullid;

    typename M::vector_type normal;

    for (size_type j = 0; j < nb; ++j)
    {
        size_type j0 = nh + 0;
        size_type j1 = nh + 1;
        size_type j2 = nh + 2;
        size_type j3 = nh + 3;

        if (idPartner0 != nullid)
        {
            mesh.halfedge(idPartner0).set_partner_id(j0);
        }

        id_type idFace = mesh.create_face(idSurface0, j0);

        if (j == 0 && bCuspLower)
        {
            idPartner1 -= na;

            mesh.halfedge(j0) = {mesh, j0, idPartner1, j1, j2, j0, idFace};
            mesh.halfedge(j1) = {mesh, j1, idPartner2, j2, j0, j1, idFace};
            mesh.halfedge(j2) = {mesh, j2, idPartner3, j0, j1, j2, idFace};

            mesh.halfedge(idPartner1).set_partner_id(j0);

            typename M::vertex_attributes_type av0 = mesh.halfedge(idPartner1).next().attributes();
            typename M::vertex_attributes_type av1 = mesh.halfedge(idPartner1).attributes();
            typename M::vertex_attributes_type av2 = av1;

            av2.set_position(center_position(av2.position()));

            normal = normalize(cross(av1.position() - av0.position(), av2.position() - av0.position()));
            av0.set_normal(normal);
            av1.set_normal(normal);
            av2.set_normal(normal);

            std::array<T, 3> tsProto = texture_span<T>(j + 1, nb, bCuspLower, bCuspUpper, T(0));
            av0.set_texcoord({T(0), T(1)});
            av1.set_texcoord({tsProto[1], tsProto[2]});
            av2.set_texcoord({tsProto[0], tsProto[2]});
            assert(mesh.halfedge(idPartner1).attributes().texcoord().y() == tsProto[2]);

            mesh.vertex(j0) = {mesh, j0, j0, av0};
            mesh.vertex(j1) = {mesh, j1, j1, av1};
            mesh.vertex(j2) = {mesh, j2, j2, av2};

            --nh;
        }
        else if (j == nb - 1 && bCuspUpper)
        {
            idPartner1 -= na;

            mesh.halfedge(j0) = {mesh, j0, idPartner0, j1, j2, j0, idFace};
            mesh.halfedge(j1) = {mesh, j1, idPartner1, j2, j0, j1, idFace};
            mesh.halfedge(j2) = {mesh, j2, idPartner3, j0, j1, j2, idFace};

            mesh.halfedge(idPartner1).set_partner_id(j1);

            typename M::vertex_attributes_type av0 = mesh.halfedge(idPartner1).next().attributes();
            typename M::vertex_attributes_type av1 = av0;
            typename M::vertex_attributes_type av2 = mesh.halfedge(idPartner1).attributes();

            av0.set_position(center_position(av0.position()));

            if (!bPlanar || normal.zero())
            {
                normal = normalize(cross(av0.position() - av2.position(), av1.position() - av2.position()));
            }

            if (!bPlanar && idPartner0 != nullid)
            {
                av0.set_normal(normalize(mesh.halfedge(idPartner0).face().attributes().normal() + normal));
                av1.set_normal(av0.normal());

                mesh.halfedge(idPartner0).attributes().set_normal(av0.normal());
                mesh.halfedge(idPartner0).next().attributes().set_normal(av0.normal());
            }
            else
            {
                av0.set_normal(normal);
                av1.set_normal(normal);
            }

            av2.set_normal(normal);

            if (idPartner0 != nullid)
            {
                av0.set_texcoord(mesh.halfedge(idPartner0).next().attributes().texcoord());
                av1.set_texcoord(mesh.halfedge(idPartner0).attributes().texcoord());
            }
            else
            {
                av0.set_texcoord({T(0), T(1)});
                av1.set_texcoord({T(1), T(1)});
            }

            av2.set_texcoord({T(0), T(0)});

            mesh.vertex(j0) = {mesh, j0, j0, av0};
            mesh.vertex(j1) = {mesh, j1, j1, av1};
            mesh.vertex(j2) = {mesh, j2, j2, av2};

            mesh.vertex(j0).set_attributes(av0);
            mesh.vertex(j1).set_attributes(av1);
            mesh.vertex(j2).set_attributes(av2);

            --nh;
        }
        else
        {
            mesh.halfedge(j0) = {mesh, j0, idPartner0, j1, j3, j0, idFace};
            mesh.halfedge(j1) = {mesh, j1, idPartner1, j2, j0, j1, idFace};
            mesh.halfedge(j2) = {mesh, j2, idPartner2, j3, j1, j2, idFace};
            mesh.halfedge(j3) = {mesh, j3, idPartner3, j0, j2, j3, idFace};

            mesh.halfedge(idPartner1).set_partner_id(j1);

            typename M::vertex_attributes_type av0 = mesh.halfedge(idPartner1).next().attributes();
            typename M::vertex_attributes_type av1 = av0;
            typename M::vertex_attributes_type av2 = mesh.halfedge(idPartner1).attributes();
            typename M::vertex_attributes_type av3 = av2;

            av0.set_position(center_position(av0.position()));
            av3.set_position(center_position(av3.position()));

            if (!bPlanar || normal.zero())
            {
                normal = normalize(cross(av1.position() - av0.position(), av3.position() - av0.position()));
            }

            if (!bPlanar && idPartner0 != nullid)
            {
                av0.set_normal(normalize(mesh.halfedge(idPartner0).face().attributes().normal() + normal));
                av1.set_normal(av0.normal());

                mesh.halfedge(idPartner0).attributes().set_normal(av0.normal());
                mesh.halfedge(idPartner0).next().attributes().set_normal(av0.normal());
            }
            else
            {
                av0.set_normal(normal);
                av1.set_normal(normal);
            }

            av2.set_normal(normal);
            av3.set_normal(normal);

            if (idPartner0 != nullid)
            {
                av0.set_texcoord(mesh.halfedge(idPartner0).next().attributes().texcoord());
                av1.set_texcoord(mesh.halfedge(idPartner0).attributes().texcoord());
            }
            else
            {
                av0.set_texcoord({T(0), T(1)});
                av1.set_texcoord({T(1), T(1)});
            }

            std::array<T, 3> tsProto = texture_span<T>(j + 1, nb, bCuspLower, bCuspUpper, T(0));
            av2.set_texcoord({tsProto[1], tsProto[2]});
            av3.set_texcoord({tsProto[0], tsProto[2]});
            assert(mesh.halfedge(idPartner1).attributes().texcoord().y() == tsProto[2]);

            mesh.vertex(j0) = {mesh, j0, j0, av0};
            mesh.vertex(j1) = {mesh, j1, j1, av1};
            mesh.vertex(j2) = {mesh, j2, j2, av2};
            mesh.vertex(j3) = {mesh, j3, j3, av3};
        }

        mesh.face(idFace).attributes().set_normal(normal);

        idPartner0 = j2;
        idPartner1 += 4 * na;

        nh += 4;

        if (j == 0 && bCuspLower)
        {
            idPartner0 = j1;
        }
    }

    if (!bPlanar)
    {
        size_type nf = mesh.face_count();
        typename M::vector_type sum;
        for (size_t i = 1; i <= nb; ++i)
        {
            sum += mesh.face(nf - i).attributes().normal();
        }

        normal = normalize(sum);
    }

    mesh.surface(idSurface0).attributes().set_normal(normal);

    // Upper

    idPartner0 = nullid;
    idPartner1 = nhOrig + 3;
    idPartner2 = nullid;
    idPartner3 = 2 * na - 1;

    normal.clear();

    for (size_type j = 0; j < nb; ++j)
    {
        size_type j0 = nh + 0;
        size_type j1 = nh + 1;
        size_type j2 = nh + 2;
        size_type j3 = nh + 3;

        if (idPartner0 != nullid)
        {
            mesh.halfedge(idPartner0).set_partner_id(j0);
        }

        id_type idFace = mesh.create_face(idSurface1, j0);

        if (j == 0 && bCuspLower)
        {
            idPartner1 -= 1;
            idPartner3 -= na;

            mesh.halfedge(j0) = {mesh, j0, idPartner1, j1, j2, j0, idFace};
            mesh.halfedge(j1) = {mesh, j1, idPartner2, j2, j0, j1, idFace};
            mesh.halfedge(j2) = {mesh, j2, idPartner3, j0, j1, j2, idFace};

            mesh.halfedge(idPartner1).set_partner_id(j0);
            mesh.halfedge(idPartner3).set_partner_id(j2);

            typename M::vertex_attributes_type av0 = mesh.halfedge(idPartner1).next().attributes();
            typename M::vertex_attributes_type av1 = mesh.halfedge(idPartner1).attributes();
            typename M::vertex_attributes_type av2 = mesh.halfedge(idPartner3).next().attributes();

            av1.set_position(center_position(av1.position()));

            normal = normalize(cross(av1.position() - av0.position(), av2.position() - av0.position()));
            av0.set_normal(normal);
            av1.set_normal(normal);
            av2.set_normal(normal);

            std::array<T, 3> tsProto = texture_span<T>(j + 1, nb, bCuspLower, bCuspUpper, T(1));
            av0.set_texcoord({T(1), T(1)});
            av1.set_texcoord({tsProto[1], tsProto[2]});
            av2.set_texcoord({tsProto[0], tsProto[2]});
            assert(mesh.halfedge(idPartner3).next().attributes().texcoord().y() == tsProto[2]);
            assert(mesh.halfedge(idPartner1).attributes().texcoord().y() == tsProto[2]);

            mesh.vertex(j0) = {mesh, j0, j0, av0};
            mesh.vertex(j1) = {mesh, j1, j1, av1};
            mesh.vertex(j2) = {mesh, j2, j2, av2};

            --nh;
        }
        else if (j == nb - 1 && bCuspUpper)
        {
            idPartner1 -= 1;

            mesh.halfedge(j0) = {mesh, j0, idPartner0, j1, j2, j0, idFace};
            mesh.halfedge(j1) = {mesh, j1, idPartner1, j2, j0, j1, idFace};
            mesh.halfedge(j2) = {mesh, j2, idPartner3, j0, j1, j2, idFace};

            mesh.halfedge(idPartner1).set_partner_id(j1);
            mesh.halfedge(idPartner3).set_partner_id(j2);

            typename M::vertex_attributes_type av0 = mesh.halfedge(idPartner3).attributes();
            typename M::vertex_attributes_type av1 = mesh.halfedge(idPartner1).next().attributes();
            typename M::vertex_attributes_type av2 = mesh.halfedge(idPartner3).next().attributes();

            av1.set_position(center_position(av1.position()));



            if (!bPlanar || normal.zero())
            {
                normal = normalize(cross(av0.position() - av2.position(), av1.position() - av2.position()));
            }

            if (!bPlanar && idPartner0 != nullid)
            {
                av0.set_normal(normalize(mesh.halfedge(idPartner0).face().attributes().normal() + normal));
                av1.set_normal(av0.normal());

                mesh.halfedge(idPartner0).attributes().set_normal(av0.normal());
                mesh.halfedge(idPartner0).next().attributes().set_normal(av0.normal());
            }
            else
            {
                av0.set_normal(normal);
                av1.set_normal(normal);
            }

            av2.set_normal(normal);

            if (idPartner0 != nullid)
            {
                av0.set_texcoord(mesh.halfedge(idPartner0).next().attributes().texcoord());
                av1.set_texcoord(mesh.halfedge(idPartner0).attributes().texcoord());
            }
            else
            {
                av0.set_texcoord({T(0), T(1)});
                av1.set_texcoord({T(1), T(1)});
            }

            av2.set_texcoord({T(1), T(0)});

            mesh.vertex(j0) = {mesh, j0, j0, av0};
            mesh.vertex(j1) = {mesh, j1, j1, av1};
            mesh.vertex(j2) = {mesh, j2, j2, av2};

            --nh;
        }
        else
        {
            mesh.halfedge(j0) = {mesh, j0, idPartner0, j1, j3, j0, idFace};
            mesh.halfedge(j1) = {mesh, j1, idPartner1, j2, j0, j1, idFace};
            mesh.halfedge(j2) = {mesh, j2, idPartner2, j3, j1, j2, idFace};
            mesh.halfedge(j3) = {mesh, j3, idPartner3, j0, j2, j3, idFace};

            mesh.halfedge(idPartner1).set_partner_id(j1);
            mesh.halfedge(idPartner3).set_partner_id(j3);

            typename M::vertex_attributes_type av0 = mesh.halfedge(idPartner3).attributes();
            typename M::vertex_attributes_type av1 = mesh.halfedge(idPartner1).next().attributes();
            typename M::vertex_attributes_type av2 = mesh.halfedge(idPartner1).attributes();
            typename M::vertex_attributes_type av3 = mesh.halfedge(idPartner3).next().attributes();

            if (!bPlanar || normal.zero())
            {
                normal = normalize(cross(av1.position() - av0.position(), av3.position() - av0.position()));
            }

            if (!bPlanar && idPartner0 != nullid)
            {
                av0.set_normal(normalize(mesh.halfedge(idPartner0).face().attributes().normal() + normal));
                av1.set_normal(av0.normal());

                mesh.halfedge(idPartner0).attributes().set_normal(av0.normal());
                mesh.halfedge(idPartner0).next().attributes().set_normal(av0.normal());
            }
            else
            {
                av0.set_normal(normal);
                av1.set_normal(normal);
            }

            av2.set_normal(normal);
            av3.set_normal(normal);

            if (idPartner0 != nullid)
            {
                av0.set_texcoord(mesh.halfedge(idPartner0).next().attributes().texcoord());
                av1.set_texcoord(mesh.halfedge(idPartner0).attributes().texcoord());
            }
            else
            {
                av0.set_texcoord({T(0), T(1)});
                av1.set_texcoord({T(1), T(1)});
            }

            std::array<T, 3> tsProto = texture_span<T>(j + 1, nb, bCuspLower, bCuspUpper, T(1));
            av2.set_texcoord({tsProto[1], tsProto[2]});
            av3.set_texcoord({tsProto[0], tsProto[2]});
            assert(mesh.halfedge(idPartner3).next().attributes().texcoord().y() == tsProto[2]);
            assert(mesh.halfedge(idPartner1).attributes().texcoord().y() == tsProto[2]);

            mesh.vertex(j0) = {mesh, j0, j0, av0};
            mesh.vertex(j1) = {mesh, j1, j1, av1};
            mesh.vertex(j2) = {mesh, j2, j2, av2};
            mesh.vertex(j3) = {mesh, j3, j3, av3};
        }

        mesh.face(idFace).attributes().set_normal(normal);

        idPartner0 = j2;
        idPartner1 += 4;
        idPartner3 += 4 * na;

        nh += 4;

        if (j == 0 && bCuspLower)
        {
            idPartner0 = j1;
        }
    }

    if (!bPlanar)
    {
        size_type nf = mesh.face_count();
        typename M::vector_type sum;
        for (size_t i = 1; i <= nb; ++i)
        {
            sum += mesh.face(nf - i).attributes().normal();
        }

        normal = normalize(sum);
    }

    mesh.surface(idSurface1).attributes().set_normal(normal);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::close_side_flat(M& mesh, size_type na, size_type nb, bool bCuspLower, bool bCuspUpper, id_type idSurface, bool bPlanar)
{
    assert(bPlanar);

    using T = M::value_type;

    size_type nh = mesh.halfedge_store_count();
    size_type nv = mesh.vertex_store_count();

    id_type idPartnerLower0 = bCuspLower ? 2 * na : 3 * na;
    id_type idPartnerLower1 = bCuspLower ? na - 1 : 2 * na - 1;
    id_type idPartnerUpper0 = nh - na;
    id_type idPartnerUpper1 = bCuspUpper ? nh - na - 1 : nh - 2 * na - 1;

    id_type idUpper = nullid;

    id_type idPrev0 = nh;
    id_type idNext1 = nh;

    id_type idMid0 = idPartnerLower0;
    id_type idMid1 = idPartnerLower1 - (bCuspLower ? na : 0);
    idMid0 += (nb / 2) * 4 * na;
    idMid1 += (nb / 2) * 4 * na;

    typename M::point_type positionMid0 = mesh.halfedge(idMid0).next().attributes().position();
    typename M::point_type positionMid1 = mesh.halfedge(idMid1).attributes().position();
    typename M::point_type positionUpper0 = mesh.halfedge(idPartnerUpper0).attributes().position();
    typename M::vector_type normal = normalize(cross(positionMid1 - positionUpper0, positionMid0 - positionUpper0));

    id_type idFace = mesh.create_face(idSurface, nh, {normal});

    if (bCuspLower)
    {
        idPrev0 = nh + 1;
        idNext1 = nh;
    }
    else
    {
        typename M::vertex_attributes_type av = {mesh.halfedge(idPartnerLower1).attributes().position(), normal, {T(0), T(1)}};
        mesh.create_halfedge(nullid, nh + 1, nh + 2, nv, idFace);
        mesh.create_vertex(nh, av);

        ++nh;
        ++nv;
    }

    if (!bCuspUpper)
    {
        typename M::vertex_attributes_type av = {mesh.halfedge(idPartnerUpper0).attributes().position(), normal, {T(1), T(0)}};
        idUpper = mesh.create_halfedge(nullid, nh + 2 * nb - 1, nh + 2 * nb - 2, nv, idFace);
        mesh.create_vertex(nh, av);

        ++nh;
        ++nv;
    }

    id_type idPartner0 = idPartnerLower0;
    id_type idPartner1 = idPartnerLower1;

    for (size_type j = 0; j < nb; ++j)
    {
        id_type idNext0 = nh + 2;
        id_type idPrev1 = nh + 3;
        if (j == nb - 1)
        {
            if (bCuspUpper)
            {
                idPartner0 -= na;
            }

            assert(idPartner0 == idPartnerUpper0);
            assert(idPartner1 == idPartnerUpper1);

            idNext0 = bCuspUpper ? nh + 1 : idUpper;
            idPrev1 = bCuspUpper ? nh : idUpper;
        }

        idPrev0 = mesh.create_halfedge(idPartner0, idNext0, idPrev0, nv, idFace);
        idNext1 = mesh.create_halfedge(idPartner1, idNext1, idPrev1, nv + 1, idFace);

        mesh.halfedge(idPartner0).set_partner_id(nh);
        mesh.halfedge(idPartner1).set_partner_id(nh + 1);

        typename M::vertex_attributes_type av0 = mesh.halfedge(idPartner1).next().attributes();
        typename M::vertex_attributes_type av1 = mesh.halfedge(idPartner0).next().attributes();

        av0.set_normal(normal);
        av1.set_normal(normal);

        std::array<T, 3> tsProto = texture_span<T>(j, nb, bCuspLower, bCuspUpper);
        av0.set_texcoord({tsProto[1], tsProto[2]});
        assert(mesh.halfedge(idPartner0).next().attributes().texcoord().y() == tsProto[2]);
        
        tsProto = texture_span<T>(j + 1, nb, bCuspLower, bCuspUpper);
        av1.set_texcoord({tsProto[0], tsProto[2]});
        assert(mesh.halfedge(idPartner1).next().attributes().texcoord().y() == tsProto[2]);

        mesh.create_vertex(nh, av0);
        mesh.create_vertex(nh + 1, av1);

        idPartner0 += 4 * na;
        idPartner1 += 4 * na;

        nh += 2;
        nv += 2;
    }

    mesh.surface(idSurface).attributes().set_normal(normal);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::close_side_center(M& mesh, size_type na, size_type nb, bool bCuspLower, bool bCuspUpper, id_type idSurface0, id_type idSurface1, std::function<typename M::point_type(const typename M::point_type&)> center_position, bool bPlanar)
{
    assert(bPlanar);

    using T = M::value_type;

    size_type nh = mesh.halfedge_store_count();

    id_type idPartnerLower0 = bCuspLower ? 2 * na : 3 * na;
    id_type idPartnerLower1 = bCuspLower ? na - 1 : 2 * na - 1;
    id_type idPartnerUpper0 = nh - na;
    id_type idPartnerUpper1 = bCuspUpper ? nh - na - 1 : nh - 2 * na - 1;

    id_type idMid0 = idPartnerLower0;
    id_type idMid1 = idPartnerLower1;
    idMid0 += (nb / 2) * 4 * na;
    idMid1 += (nb / 2) * 4 * na;

    typename M::point_type positionMid0 = mesh.halfedge(idMid0).next().attributes().position();
    typename M::point_type positionMid1 = mesh.halfedge(idMid1).attributes().position();

    typename M::point_type positionCenterUpper = center_position(mesh.halfedge(idPartnerUpper0).attributes().position());
    typename M::point_type positionCenterLower = center_position(mesh.halfedge(idPartnerLower1).attributes().position());

    typename M::vector_type normal0 = normalize(cross(positionCenterUpper - positionMid0, positionCenterLower - positionMid0));
    typename M::vector_type normal1 = normalize(cross(positionCenterLower - positionMid1, positionCenterUpper - positionMid1));

    typename M::vertex_attributes_type av0 = {positionCenterUpper, normal0, {T(0), T(0)}};
    typename M::vertex_attributes_type av1 = {positionCenterLower, normal1, {T(1), T(1)}};

    id_type idFace0 = mesh.create_face(idSurface0, nh, {normal0});
    id_type idFace1 = mesh.create_face(idSurface1, nh + 1, {normal1});

    id_type idCenter0 = mesh.create_halfedge_vertex(nh + 1, nh + 2, nullid, idFace0, av0);
    id_type idCenter1 = mesh.create_halfedge_vertex(nh, nullid, nh + 3, idFace1, av1);

    nh += 2;

    id_type idPrev0 = idCenter0;
    id_type idNext1 = idCenter1;

    if (!bCuspLower)
    {
        av0 = {positionCenterLower, normal0, {T(0), T(1)}};
        idPrev0 = mesh.create_halfedge_vertex(nullid, nh + 2, idPrev0, idFace0, av0);

        av1 = {mesh.halfedge(idPartnerLower1).attributes().position(), normal1, {T(0), T(1)}};
        idNext1 = mesh.create_halfedge_vertex(nullid, idNext1, nh + 3, idFace1, av1);

        nh += 2;
    }

    id_type idPartner0 = idPartnerLower0;
    id_type idPartner1 = idPartnerLower1;

    for (size_type j = 0; j < nb; ++j)
    {
        id_type idNext0 = nh + 2;
        id_type idPrev1 = nh + 3;
        if (j == nb - 1)
        {
            if (bCuspUpper)
            {
                idPartner0 -= na;
            }

            assert(idPartner0 == idPartnerUpper0);
            assert(idPartner1 == idPartnerUpper1);

            idNext0 = idCenter0;
            idPrev1 = idCenter1;
        }

        av0 = mesh.halfedge(idPartner0).next().attributes();
        av1 = mesh.halfedge(idPartner1).next().attributes();

        av0.set_normal(normal0);
        av1.set_normal(normal1);

        std::array<T, 3> tsProto = texture_span<T>(j, nb, bCuspLower, bCuspUpper, T(0));
        av0.set_texcoord({tsProto[1], tsProto[2]});
        assert(mesh.halfedge(idPartner0).next().attributes().texcoord().y() == tsProto[2]);
        
        tsProto = texture_span<T>(j + 1, nb, bCuspLower, bCuspUpper, T(1));
        av1.set_texcoord({tsProto[0], tsProto[2]});
        assert(mesh.halfedge(idPartner1).next().attributes().texcoord().y() == tsProto[2]);

        idPrev0 = mesh.create_halfedge_vertex(idPartner0, idNext0, idPrev0, idFace0, av0);
        idNext1 = mesh.create_halfedge_vertex(idPartner1, idNext1, idPrev1, idFace1, av1);

        mesh.halfedge(idPartner0).set_partner_id(idPrev0);
        mesh.halfedge(idPartner1).set_partner_id(idNext1);

        idPartner0 += 4 * na;
        idPartner1 += 4 * na;

        nh += 2;
    }

    if (!bCuspUpper)
    {
        av0 = {mesh.halfedge(idPartnerUpper0).attributes().position(), normal0, {T(1), T(0)}};
        id_type idUpper0 = mesh.create_halfedge_vertex(nullid, idCenter0, idPrev0, idFace0, av0);

        av1 = {positionCenterUpper, normal1, {T(1), T(0)}};
        id_type idUpper1 = mesh.create_halfedge_vertex(nullid, idNext1, idCenter1, idFace1, av1);

        mesh.halfedge(idPrev0).set_next_id(idUpper0);
        mesh.halfedge(idNext1).set_prev_id(idUpper1);

        mesh.halfedge(idCenter0).set_prev_id(idUpper0);
        mesh.halfedge(idCenter1).set_next_id(idUpper1);

        nh += 2;
    }
    else
    {
        mesh.halfedge(idCenter0).set_prev_id(idPrev0);
        mesh.halfedge(idCenter1).set_next_id(idNext1);
    }

    mesh.surface(idSurface0).attributes().set_normal(normal0);
    mesh.surface(idSurface1).attributes().set_normal(normal1);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::seal_cylinder(M& mesh, size_type nAzimuth, size_type nz, bool bCuspLower, bool bCuspUpper, bool bOpenSide, const Extent<value_type<M>>& extentAzimuth, const Extent<value_type<M>>& extentZ, id_type idSubmesh,
        normal_vector_type<M> normalLower, normal_vector_type<M> normalUpper)
{
    using T = M::value_type;

    // Save halfedge id's before potential side closure
    id_type idLowerFirst = 0;
    id_type idUpperFirst = mesh.halfedge_store_count() - 2 * nAzimuth;

    if (bOpenSide)
    {
        if (extentAzimuth.termination().type == Termination::Type::Flat)
        {
            id_type idSurface = mesh.create_surface(idSubmesh, extentAzimuth.name());
            if (extentAzimuth.termination().banded)
            {
                connect_side_flat(mesh, nAzimuth, nz, bCuspLower, bCuspUpper, idSurface, true);
            }
            else
            {
                close_side_flat(mesh, nAzimuth, nz, bCuspLower, bCuspUpper, idSurface, true);
            }
            bOpenSide = false;
        }
        else if (extentAzimuth.termination().type == Termination::Type::Center)
        {
            id_type idSurface0 = mesh.create_surface(idSubmesh, extentAzimuth.lower_name());
            id_type idSurface1 = mesh.create_surface(idSubmesh, extentAzimuth.upper_name());
            if (extentAzimuth.termination().banded)
            {
                connect_side_center(mesh, nAzimuth, nz, bCuspLower, bCuspUpper, idSurface0, idSurface1,
                        [](const typename M::point_type& point) -> typename M::point_type { return {T(0), T(0), point.z()}; },
                        true);
            }
            else
            {
                close_side_center(mesh, nAzimuth, nz, bCuspLower, bCuspUpper, idSurface0, idSurface1,
                        [](const typename M::point_type& point) -> typename M::point_type { return {T(0), T(0), point.z()}; },
                        true);
            }
            bOpenSide = false;
        }
    }

    if (!bCuspLower)
    {
        if (extentZ.lower_termination().type == Termination::Type::Flat)
        {
            id_type idSurface0 = mesh.create_surface(idSubmesh, extentZ.lower_name(), {normalLower});
            if (!bOpenSide)
            {
                create_border_face(mesh, idLowerFirst, normalLower, idSurface0);
            }
            else
            {
                create_border_face(mesh, idLowerFirst + nAzimuth - 1, idLowerFirst, normalLower, idSurface0);
            }

            calculate_surface_texcoords(mesh, idSurface0);
        }
    }

    if (!bCuspUpper)
    {
        if (extentZ.upper_termination().type == Termination::Type::Flat)
        {
            id_type idSurface1 = mesh.create_surface(idSubmesh, extentZ.upper_name(), {normalUpper});
            if (!bOpenSide)
            {
                create_border_face(mesh, idUpperFirst, normalUpper, idSurface1);
            }
            else
            {
                create_border_face(mesh, idUpperFirst, idUpperFirst + nAzimuth - 1, normalUpper, idSurface1);
            }

            calculate_surface_texcoords(mesh, idSurface1);
        }
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::seal_sphere(M& mesh, size_type nAzimuth, size_type nElevation, bool bCuspLower, bool bCuspUpper, bool bOpenSide, const Extent<value_type<M>>& extentAzimuth, const Extent<value_type<M>>& extentElevation, id_type idSubmesh)
{
    using T = M::value_type;

    // Save halfedge id's before potential side closure
    id_type idLowerFirst = 0;
    id_type idUpperFirst = mesh.halfedge_store_count() - 2 * nAzimuth;

    if (bOpenSide) // Elevation Termination::Type::Center will affect side generation ...
    {
        if (extentAzimuth.termination().type == Termination::Type::Flat)
        {
            id_type idSurface = mesh.create_surface(idSubmesh, extentAzimuth.name());
            bool bPlanar = math::float_eq(extentAzimuth.interval().length(), T(0.5));
            connect_side_flat(mesh, nAzimuth, nElevation, bCuspLower, bCuspUpper, idSurface, bPlanar);
            if (bPlanar)
            {
                calculate_planar_surface_texcoords(mesh, idSurface); // really just need x ...
            }
            else
            {
                calculate_surface_texcoords(mesh, idSurface); // really just need x ...
            }
            bOpenSide = false;
        }
        else if (extentAzimuth.termination().type == Termination::Type::Center)
        {
            id_type idSurface0 = mesh.create_surface(idSubmesh, extentAzimuth.lower_name());
            id_type idSurface1 = mesh.create_surface(idSubmesh, extentAzimuth.upper_name());
            if (extentAzimuth.termination().banded)
            {
                connect_side_center(mesh, nAzimuth, nElevation, bCuspLower, bCuspUpper, idSurface0, idSurface1,
                        [](const typename M::point_type& point) -> typename M::point_type { return {T(0), T(0), point.z()}; },
                        true);
            }
            else
            {
                close_side_center(mesh, nAzimuth, nElevation, bCuspLower, bCuspUpper, idSurface0, idSurface1,
                        [](const typename M::point_type& point) -> typename M::point_type { return {T(0), T(0), point.z()}; },
                        true);
            }
            calculate_planar_surface_texcoords(mesh, idSurface0); // really just need right x ...
            calculate_planar_surface_texcoords(mesh, idSurface1); // really just need left x ...
            bOpenSide = false;
        }
    }

    if (!bCuspLower)
    {
        if (extentElevation.lower_termination().type == Termination::Type::Flat)
        {
            typename M::vector_type normal {T(0), T(0), T(-1)};
            id_type idSurface0 = mesh.create_surface(idSubmesh, extentElevation.lower_name(), {normal});
            if (!bOpenSide)
            {
                create_border_face(mesh, idLowerFirst, normal, idSurface0);
            }
            else
            {
                create_border_face(mesh, idLowerFirst + nAzimuth - 1, idLowerFirst, normal, idSurface0);
            }

            calculate_surface_texcoords(mesh, idSurface0);
        }
        else if (extentElevation.lower_termination().type == Termination::Type::Center)
        {
            assert(false); // Termination::Type::Center ...
        }
    }

    if (!bCuspUpper)
    {
        if (extentElevation.upper_termination().type == Termination::Type::Flat)
        {
            typename M::vector_type normal {T(0), T(0), T(1)};
            id_type idSurface1 = mesh.create_surface(idSubmesh, extentElevation.upper_name(), {normal});
            if (!bOpenSide)
            {
                create_border_face(mesh, idUpperFirst, normal, idSurface1);
            }
            else
            {
                create_border_face(mesh, idUpperFirst, idUpperFirst + nAzimuth - 1, normal, idSurface1);
            }

            calculate_surface_texcoords(mesh, idSurface1);
        }
        else if (extentElevation.upper_termination().type == Termination::Type::Center)
        {
            assert(false); // Termination::Type::Center ...
        }
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::seal_torus(M& mesh, size_type nMinor, size_type nMajor, value_type<M> rMajor, bool bCuspLower, bool bCuspUpper, bool bOpenMinor, bool bOpenMajor, const Extent<value_type<M>>& extentMinor, const Extent<value_type<M>>& extentMajor, id_type idSubmesh)
{
    using T = M::value_type;

    T theta0 = math::PiTwo<T> * extentMajor.interval().lower();
    T theta1 = math::PiTwo<T> * extentMajor.interval().upper();

    // Save halfedge id's before potential side closure
    id_type idLowerFirst = 0;
    id_type idUpperFirst = mesh.halfedge_store_count() - 2 * nMinor;
    size_type idSideFirst = mesh.halfedge_store_count();

    if (bOpenMinor)
    {
        if (extentMinor.termination().type == Termination::Type::Flat)
        {
            id_type idSurfaceSide = mesh.create_surface(idSubmesh, extentMinor.name());
            T sum = extentMinor.interval().lower() + extentMinor.interval().upper();
            sum -= floor(sum);
            bool bPlanar = math::float_eq(sum, T(0.5));
            connect_side_flat(mesh, nMinor, nMajor, bCuspLower, bCuspUpper, idSurfaceSide, bPlanar);
            bOpenMinor = false;

            if (!bOpenMajor)
            {
                id_type id0 = idSideFirst;
                id_type id2 = idSideFirst + 4 * nMajor - 2;
                mesh.halfedge(id0).set_partner_id(id2);
                mesh.halfedge(id2).set_partner_id(id0);
//                surface_vertex_normal(mesh.vertex(id0)); // ...
            }
        }
        else if (extentMinor.termination().type == Termination::Type::Center)
        {
            id_type idSurfaceSideLower = mesh.create_surface(idSubmesh, extentMinor.lower_name());
            id_type idSurfaceSideUpper = mesh.create_surface(idSubmesh, extentMinor.upper_name());
            connect_side_center(mesh, nMinor, nMajor, bCuspLower, bCuspUpper, idSurfaceSideLower, idSurfaceSideUpper,
                    [rMajor](typename M::point_type point) -> typename M::point_type { point.set_z(T(0)); return rMajor * normalize(point); },
                    false);
            bOpenMinor = false;

            if (!bOpenMajor)
            {
                id_type id0 = idSideFirst;
                id_type id2 = idSideFirst + 4 * nMajor - 2;
                mesh.halfedge(id0).set_partner_id(id2);
                mesh.halfedge(id2).set_partner_id(id0);
//                surface_vertex_normal(mesh.vertex(id0)); // ...

                idSideFirst += 4 * nMajor;
                id0 = idSideFirst;
                id2 = idSideFirst + 4 * nMajor - 2;
                mesh.halfedge(id0).set_partner_id(id2);
                mesh.halfedge(id2).set_partner_id(id0);
//                surface_vertex_normal(mesh.vertex(id0)); // ...
            }
        }
    }

    if (!bOpenMajor)
    {
        // Connect ends

        for (size_type j = 0; j < nMinor; ++j)
        {
            id_type j0 = idLowerFirst + j;
            id_type j2 = idUpperFirst + j;
            assert(mesh.halfedge(j0).border());
            assert(mesh.halfedge(j2).border());

            mesh.halfedge(j0).set_partner_id(j2);
            mesh.halfedge(j2).set_partner_id(j0);
//            surface_vertex_normal(mesh.vertex(j0)); // ...
        }
    }
    else
    {
        if (!bCuspLower)
        {
            if (extentMajor.lower_termination().type == Termination::Type::Flat)
            {
                typename M::vector_type normal {sin(theta0), -cos(theta0), T(0)};
                id_type idSurface0 = mesh.create_surface(idSubmesh, extentMajor.lower_name(), {normal});
                if (!bOpenMinor)
                {
                    create_border_face(mesh, idLowerFirst, normal, idSurface0);
                }
                else
                {
                    create_border_face(mesh, idLowerFirst + nMinor - 1, idLowerFirst, normal, idSurface0);
                }

                calculate_surface_texcoords(mesh, idSurface0);
            }
        }

        if (!bCuspUpper)
        {
            if (extentMajor.upper_termination().type == Termination::Type::Flat)
            {
                typename M::vector_type normal = {-sin(theta1), cos(theta1), T(0)};
                id_type idSurface1 = mesh.create_surface(idSubmesh, extentMajor.upper_name(), {normal});
                if (!bOpenMinor)
                {
                    create_border_face(mesh, idUpperFirst, normal, idSurface1);
                }
                else
                {
                    create_border_face(mesh, idUpperFirst, idUpperFirst + nMinor - 1, normal, idSurface1);
                }

                calculate_surface_texcoords(mesh, idSurface1);
            }
        }
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::triangulate_rows(M& mesh, id_type idHalfedge, size_type n)
{
    id_type idFace = mesh.halfedge(idHalfedge).face_id();
    if (mesh.face(idFace).halfedge_count() == 3)
    {
        assert(n == 1);
        return;
    }

    assert(n > 1);

    // Ensure that halfedge is at the right side of a bottom row trapezoid
    while (colinear(mesh.halfedge(idHalfedge)))
    {
        idHalfedge = mesh.halfedge(idHalfedge).next_id();
    }

    const id_type idNext = mesh.halfedge(idHalfedge).next_id();
    id_type idOpposite = idHalfedge;
    for (size_type i = 0; i <= n; ++i)
    {
        idOpposite = mesh.halfedge(idOpposite).prev_id();
    }

    split_face(mesh, idNext, idOpposite);
    split_edge(mesh, mesh.halfedge(idHalfedge).next_id(), n - 1);

    // Triangulate trapezoid
    for (size_type i = 0; i < n - 1; ++i)
    {
        split_face(mesh, mesh.halfedge(idHalfedge).next_id(), mesh.halfedge(idHalfedge).prev_id());
        split_face(mesh, mesh.halfedge(idHalfedge).next().partner().next().next_id(), mesh.halfedge(idHalfedge).next().partner_id());
        idHalfedge = mesh.halfedge(idHalfedge).next().partner().prev().partner_id();
    }

    triangulate_rows(mesh, idNext, n - 1);
    return;
}

//------------------------------------------------------------------------------
template<typename ForwardIterator>
void quetzal::model::project_vertices(ForwardIterator first, ForwardIterator last, typename ForwardIterator::value_type::attributes_type::value_type radius)
{
    using T = ForwardIterator::value_type::attributes_type::value_type;

    for (auto i = first; i != last; ++i)
    {
        auto& av = i->attributes();
        auto normal = normalize(av.position());

        av.set_position(radius * normal);
        av.set_normal(normal);

        // Derive texture coordinates from spherical coordinates of position
        T phi = atan2(av.position().y(), av.position().x());
        if (errno == EDOM) // Both x and y are 0
        {
            phi = math::Pi<T>;
        }
        if (phi < T(0))
        {
            phi += math::PiTwo<T>;
        }

        // theta [0, Pi] -> texcoord [0, 1]
        T theta = acos(std::clamp(av.position().z() / radius, T(-1), T(1)));
        av.set_texcoord({phi / math::PiTwo<T>, theta / math::Pi<T>});
    }

    return;
}

//------------------------------------------------------------------------------
template<typename M>
quetzal::model::vertices_attributes_type<M> quetzal::model::vertices_attributes(size_type nAzimuth, value_type<M> radius, value_type<M> z, azimuth_interval_type<M> azimuth, normal_vector_type<M> normalProto, texture_span_type<M> tsProto)
{
    using T = M::value_type;

    auto rotate_z = [](const typename M::vector_type& normal0, T cos_phi, T sin_phi) -> typename M::vector_type {
        return math::normalize<typename M::vector_traits>({normal0.x() * cos_phi - normal0.y() * sin_phi, normal0.x() * sin_phi + normal0.y() * cos_phi, normal0.z()});
    };

// ...
    T phi0 = math::PiTwo<T> * azimuth.lower();
    T dphi = math::PiTwo<T> * azimuth.length();
    T dtx = tsProto[1] - tsProto[0];

    T tx0 = tsProto[0];
//    T tx1 = tsProto[1];
    T ty = tsProto[2];

    vertices_attributes_type<M> avs(nAzimuth + 1);

    // optimizations can be made in cases other than unit based on symmetry ...

    if (azimuth.unit())
    {
        size_type nAzimuth2 = nAzimuth / 2;

        if (math::even(nAzimuth)) // Use first quadrant vertex attributes for reference
        {
            size_type nAzimuth4 = nAzimuth2 / 2;

            if (math::even(nAzimuth2)) // Use first octant vertex attributes for reference
            {
                size_type nAzimuth8 = nAzimuth4 / 2;

                size_type i0 = 0;
                size_type i1 = nAzimuth8;
                for (size_type i = i0; i <= i1; ++i)
                {
//                    T t = T(i) / T(nAzimuth);
//                    T phi = math::PiTwo<T> * azimuth.lerp(t);
//                    T phi = math::PiTwo<T> * (azimuth.lower() + (azimuth.upper() - azimuth.lower()) * T(i) / T(nAzimuth));
                    T phi = phi0 + dphi * T(i) / T(nAzimuth);
//                    T tx = math::lerp(tx0, tx1, t);
                    T tx = tx0 + dtx * T(i) / T(nAzimuth);

                    T cos_phi = cos(phi);
                    T sin_phi = sin(phi);

                    avs[i].set_position({radius * cos_phi, radius * sin_phi, z});
                    avs[i].set_normal(rotate_z(normalProto, cos_phi, sin_phi));
                    avs[i].set_texcoord({tx, ty});

                    size_type j = nAzimuth4 - i;
                    if (j != i)
                    {
                        tx = tx0 + dtx * T(nAzimuth4 - i) / T(nAzimuth);
                        avs[j].set_position({avs[i].position().y(), avs[i].position().x(), avs[i].position().z()});
                        avs[j].set_normal({avs[i].normal().y(), avs[i].normal().x(), avs[i].normal().z()});
                        avs[j].set_texcoord({tx, ty});
                    }
                }
            }
            else
            {
                size_type i0 = 0;
                size_type i1 = nAzimuth4;
                for (size_type i = i0; i <= i1; ++i)
                {
//                    T t = T(i) / T(nAzimuth);
//                    T phi = math::PiTwo<T> * azimuth.lerp(t);
//                    T phi = math::PiTwo<T> * (azimuth.lower() + (azimuth.upper() - azimuth.lower()) * T(i) / T(nAzimuth));
                    T phi = phi0 + dphi * T(i) / T(nAzimuth);
//                    T tx = math::lerp(tx0, tx1, t);
                    T tx = tx0 + dtx * T(i) / T(nAzimuth);

                    T cos_phi = cos(phi);
                    T sin_phi = sin(phi);

                    avs[i].set_position({radius * cos_phi, radius * sin_phi, z});
                    avs[i].set_normal(rotate_z(normalProto, cos_phi, sin_phi));
                    avs[i].set_texcoord({tx, ty});
                }
            }

            size_t i0 = nAzimuth4 + 1;
            size_t i1 = nAzimuth2;
            for (size_type i = i0; i <= i1; ++i)
            {
//                T t = T(i) / T(nAzimuth);
//                T tx = math::lerp(tx0, tx1, t);
                T tx = tx0 + dtx * T(i) / T(nAzimuth);

                size_type j = nAzimuth2 - i;
                avs[i].set_position({-avs[j].position().x(), avs[j].position().y(), avs[j].position().z()});
                avs[i].set_normal({-avs[j].normal().x(), avs[j].normal().y(), avs[j].normal().z()});
                avs[i].set_texcoord({tx, ty});
            }

            i0 = nAzimuth2 + 1;
            i1 = nAzimuth;
            for (size_type i = i0; i <= i1; ++i)
            {
//                T t = T(i) / T(nAzimuth);
//                T tx = math::lerp(tx0, tx1, t);
                T tx = tx0 + dtx * T(i) / T(nAzimuth);

                size_type j = nAzimuth - i;
                avs[i].set_position({avs[j].position().x(), -avs[j].position().y(), avs[j].position().z()});
                avs[i].set_normal({avs[j].normal().x(), -avs[j].normal().y(), avs[j].normal().z()});
                avs[i].set_texcoord({tx, ty});
            }
        }
        else // Use first and second quadrant vertex attributes for reference
        {
            for (size_type i = 0; i <= nAzimuth2; ++i)
            {
//                T t = T(i) / T(nAzimuth);
//                T phi = math::PiTwo<T> * azimuth.lerp(t);
//                T phi = math::PiTwo<T> * (azimuth.lower() + (azimuth.upper() - azimuth.lower()) * T(i) / T(nAzimuth));
                T phi = phi0 + dphi * T(i) / T(nAzimuth);
//                T tx = math::lerp(tx0, tx1, t);
                T tx = tx0 + dtx * T(i) / T(nAzimuth);
  
                T cos_phi = cos(phi);
                T sin_phi = sin(phi);

                avs[i].set_position({radius * cos_phi, radius * sin_phi, z});
                avs[i].set_normal(rotate_z(normalProto, cos_phi, sin_phi));
                avs[i].set_texcoord({tx, ty});

                size_t j = nAzimuth - i;
                tx = tx0 + dtx * T(j) / T(nAzimuth);
                avs[j].set_position({avs[i].position().x(), -avs[i].position().y(), avs[i].position().z()});
                avs[j].set_normal({avs[i].normal().x(), -avs[i].normal().y(), avs[i].normal().z()});
                avs[j].set_texcoord({tx, ty});
            }
        }
    }
    else
    {
        for (size_type i = 0; i <= nAzimuth; ++i)
        {
//            T t = T(i) / T(nAzimuth);
//            T phi = math::PiTwo<T> * azimuth.lerp(t);
//            T phi = math::PiTwo<T> * (azimuth.lower() + (azimuth.upper() - azimuth.lower()) * T(i) / T(nAzimuth));
            T phi = phi0 + dphi * T(i) / T(nAzimuth);
//            T tx = math::lerp(tx0, tx1, t);
            T tx = tx0 + dtx * T(i) / T(nAzimuth);

            T cos_phi = cos(phi);
            T sin_phi = sin(phi);

            avs[i].set_position({radius * cos_phi, radius * sin_phi, z});
            avs[i].set_normal(rotate_z(normalProto, cos_phi, sin_phi));
            avs[i].set_texcoord({tx, ty});
        }

        // If azimuth is one turn, ensure that first and last values are the same except for tx = 1
        if (azimuth.unit_length())
        {
            avs[nAzimuth] = avs[0];
            avs[nAzimuth].texcoord().set_x(T(1));
        }
    }

    return avs;
}

//------------------------------------------------------------------------------
template<typename M>
quetzal::model::vertices_attributes_type<M> quetzal::model::vertices_attributes(const geometry::Polygon<typename M::vector_traits>& polygon, value_type<M> ty)
{
    using T = M::value_type;

    size_type nAzimuth = polygon.edge_count();
    vertices_attributes_type<M> avs(nAzimuth + 1);

    auto center = polygon.center();

    for (size_type i = 0; i < nAzimuth; ++i)
    {
        avs[i].set_position(polygon.vertex(i));
        avs[i].set_normal(normalize(avs[i].position() - center)); // normal should be calculated based on perpendicular to polygon tangent at each point ...
        avs[i].set_texcoord({T(i) / nAzimuth, ty}); // texcoord should be spaced proportionally to the position of the vertex along the perimeter ...
    }

    avs[nAzimuth] = avs[0];
    avs[nAzimuth].texcoord().set_x(T(1));
    return avs;
}

//------------------------------------------------------------------------------
template<typename M>
quetzal::model::vertices_attributes_type<M> quetzal::model::vertices_attributes(const geometry::Polygon<typename M::vector_traits>& polygon, value_type<M> z, value_type<M> ty)
{
    using T = M::value_type;

    size_type nAzimuth = polygon.edge_count();
    vertices_attributes_type<M> avs(nAzimuth + 1);

    auto center = polygon.center();
    center.set_z(z);

    for (size_type i = 0; i < nAzimuth; ++i)
    {
        avs[i].set_position(polygon.vertex(i));
        avs[i].position().set_z(z);
        avs[i].set_normal(normalize(avs[i].position() - center)); // normal should be calculated based on perpendicular to polygon tangent at each point ...
        avs[i].set_texcoord({T(i) / nAzimuth, ty}); // texcoord should be spaced proportionally to the position of the vertex along the perimeter ...
    }

    avs[nAzimuth] = avs[0];
    avs[nAzimuth].texcoord().set_x(T(1));
    return avs;
}

//------------------------------------------------------------------------------
template<typename M>
quetzal::model::vertices_attributes_type<M> quetzal::model::vertices_attributes(const geometry::Polygon<typename M::vector_traits>& polygonLower, const geometry::Polygon<typename M::vector_traits>& polygonUpper, value_type<M> t, value_type<M> ty)
{
    assert(polygonLower.vertex_count() == polygonUpper.vertex_count());

    using T = M::value_type;

    size_type nAzimuth = polygonLower.edge_count();
    vertices_attributes_type<M> avs(nAzimuth + 1);

    auto polygon = lerp(polygonLower, polygonUpper, t);
    auto center = polygon.center();

    for (size_type i = 0; i < nAzimuth; ++i)
    {
        avs[i].set_position(polygon.vertex(i));
        avs[i].set_normal(normalize(avs[i].position() - center)); // normal should be calculated based on perpendicular to polygon tangent at each point ...
        avs[i].set_texcoord({T(i) / nAzimuth, ty}); // texcoord should be spaced proportionally to the position of the vertex along the perimeter ...
    }

    avs[nAzimuth] = avs[0];
    avs[nAzimuth].texcoord().set_x(T(1));
    return avs;
}

//------------------------------------------------------------------------------
template<typename M>
quetzal::model::vertices_attributes_type<M> quetzal::model::vertices_attributes(const geometry::Polygon<typename M::vector_traits>& polygon, value_type<M> scale, value_type<M> revs, value_type<M> z, std::function<typename M::vector_type(const typename M::point_type&, value_type<M>)> vertex_normal, value_type<M> ty)
{
    using T = M::value_type;

    size_type nAzimuth = polygon.edge_count();
    assert(nAzimuth > 2);
    vertices_attributes_type<M> avs(nAzimuth + 1);

    T phi = math::PiTwo<T> * revs;
    T cos_phi = cos(phi);
    T sin_phi = sin(phi);

    for (size_type i = 0; i < nAzimuth; ++i)
    {
        T t = T(i) / T(nAzimuth);
        typename M::point_type point = polygon.vertex(i);
        T x = point.x() * cos_phi - point.y() * sin_phi;
        T y = point.x() * sin_phi + point.y() * cos_phi;
        typename M::point_type position = {scale * x, scale * y, z};

        avs[i].set_position(position);
        avs[i].set_normal(vertex_normal(polygon.vertex(i), revs)); // normal should be calculated based on perpendicular to polygon tangent at each point ...
        avs[i].set_texcoord({t, ty}); // texcoord should be spaced proportionally to the position of the vertex along the perimeter ...
    }

    avs[nAzimuth] = avs[0];
    avs[nAzimuth].texcoord().set_x(T(1));
    return avs;
}

//------------------------------------------------------------------------------
template<typename M>
quetzal::model::vertices_attributes_type<M> quetzal::model::vertices_attributes(const geometry::OrientedPolygon<typename M::vector_traits>& polygon, const geometry::OrientedPoint<typename M::vector_traits> op, value_type<M> ty)
{
    using T = M::value_type;

    size_type nAzimuth = polygon.edge_count();
    vertices_attributes_type<M> avs(nAzimuth + 1);

    auto matrix = math::alignment(polygon.orientation().up(), polygon.orientation().right(), op.orientation().up(), op.orientation().right());
    matrix *= math::translation(op.position());

    auto center = polygon.center();

    for (size_type i = 0; i < nAzimuth; ++i)
    {
        avs[i].set_position((polygon.vertex(i) - center) * matrix);
        avs[i].set_normal(normalize(avs[i].position() - op.position()));
        avs[i].set_texcoord({T(i) / nAzimuth, ty});
    }

    avs[nAzimuth] = avs[0];
    avs[nAzimuth].texcoord().set_x(T(1));
    return avs;
}

//------------------------------------------------------------------------------
template<typename M>
quetzal::model::vertices_attributes_type<M> quetzal::model::apex_vertices_attributes(const geometry::Polygon<typename M::vector_traits>& polygon, value_type<M> revs, value_type<M> z, value_type<M> dz)
{
    using T = M::value_type;

    size_type nAzimuth = polygon.edge_count();
    assert(nAzimuth > 2);
    vertices_attributes_type<M> avs(nAzimuth);

    T phi = math::PiTwo<T> * revs;
    T cos_phi = cos(phi);
    T sin_phi = sin(phi);

    for (size_type i = 0; i < nAzimuth; ++i)
    {
        T dr = ((polygon.vertex(i) + polygon.vertex((i + 1) % nAzimuth)) / T(2)).norm();
        typename M::vector_type v = dz * math::normalize(polygon.vertex(i) + polygon.vertex((i + 1) % nAzimuth));
        typename M::vector_type normal = math::normalize<typename M::vector_traits>({v.x() * cos_phi - v.y() * sin_phi, v.x() * sin_phi + v.y() * cos_phi, -dr});

        avs[i].set_position({T(0), T(0), z});
        avs[i].set_normal(normal);
        avs[i].set_texcoord({T(0.5), T(0)});
    }

    return avs;
}

//------------------------------------------------------------------------------
template<typename T>
std::array<T, 3> quetzal::model::texture_span(T t, bool bCuspLower, bool bCuspUpper, T alignment)
{
    assert(t >= T(0) && t <= T(1));
//    assert(!bCuspLower || !bCuspUpper);

    T ty = T(1) - t;
    T tw = T(1);

// partial sphere may not have both cusps ...
if (bCuspLower && bCuspUpper) // need to handle this case for sphere, or create a special version; need to decide how sphere should map texture ...
{
    return {T(0), tw, ty};
}

    if (bCuspLower)
    {
        tw = t;
    }
    else if (bCuspUpper)
    {
        tw = ty;
    }
    assert(tw >= T(0) && tw <= T(1));

    T tx0 = alignment * (T(1) - tw);
    T tx1 = tx0 + tw;
//    T tx1 = alignment + (T(1) - alignment) * tw;
//    T tx1 = T(1) - tx0;
    assert(tx0 >= T(0) && tx0 <= T(1));
    assert(tx1 >= T(0) && tx1 <= T(1));
    return {tx0, tx1, ty};
}

//------------------------------------------------------------------------------
template<typename T>
std::array<T, 3> quetzal::model::texture_span(size_type i, size_type n, bool bCuspLower, bool bCuspUpper, T alignment)
{
    assert(i >= 0 && i <= n);
//    assert(!bCuspLower || !bCuspUpper);

//    T t = T(i) / T(n);
//    T ty = T(1) - t;
    T ty = T(n - i) / T(n);
    T tw = T(1);
//assert(t == T(1) - ty);

// partial sphere may not have both cusps ...
if (bCuspLower && bCuspUpper) // need to handle this case for sphere, or create a special version; need to decide how sphere should map texture ...
{
    return {T(0), tw, ty};
}

    if (bCuspLower)
    {
        tw = T(1) - ty;
    }
    else if (bCuspUpper)
    {
        tw = ty;
    }
    assert(tw >= T(0) && tw <= T(1));

    T tx0 = alignment * (T(1) - tw);
    T tx1 = tx0 + tw;
//    T tx1 = alignment + (T(1) - alignment) * tw;
//    T tx1 = T(1) - tx0;
    assert(tx0 >= T(0) && tx0 <= T(1));
    assert(tx1 >= T(0) && tx1 <= T(1));
    return {tx0, tx1, ty};
}

#endif // QUETZAL_MODEL_PRIMITIVES_UTIL_HPP
