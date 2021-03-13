#if !defined(QUETZAL_MODEL_PRIMITIVES_HPP)
#define QUETZAL_MODEL_PRIMITIVES_HPP
//------------------------------------------------------------------------------
// model
// primitives.hpp
//------------------------------------------------------------------------------

#include "Extent.hpp"
#include "geometry.hpp"
#include "mesh_attributes.hpp"
#include "primitives_util.hpp"
#include "SurfaceName.hpp"
#include "quetzal/brep/Submesh.hpp"
#include "quetzal/brep/mesh_inversion.hpp"
#include "quetzal/brep/mesh_geometry.hpp"
#include "quetzal/brep/mesh_util.hpp"
#include "quetzal/brep/validation.hpp"
#include "quetzal/geometry/OrientedPolygon.hpp"
#include "quetzal/geometry/Path.hpp"
#include "quetzal/geometry/Polyline.hpp"
#include "quetzal/geometry/Polygon.hpp"
#include "quetzal/geometry/PolygonWithHoles.hpp"
#include "quetzal/geometry/Path.hpp"
#include "quetzal/math/math_util.hpp"
#include <functional>

namespace quetzal::model
{

    // Primitives are created with planar, but not necessarily triangulated faces
    // Any normals and texture coordinates are continuous within each surface
    // All angles and intervals that represent angles are measured in turns, 0..1 representing one turn and/or full range
    // Cylindrical coordinates: radius, azimuth, z (r, phi, z) with azimuth interval 0..1 -> phi: 0..2Pi
    // Spherical coordinates: radius, azimuth, elevation (r, phi, theta) with azimuth interval 0..1 -> phi: 0..2Pi; elevation interval 0..1 -> theta: -Pi/2..Pi/2 ({0, 0, -1}..{0, 0, 1})
    // Each primitive given a unique submesh name produces a single submesh with that name
    // Primitives given a submesh name that already exists in the mesh will be merged into that submesh

    // Create a grid in the xy-plane centered at the origin with nx rows and ny columns
    // Surfaces: single "grid" or per face "grid_<i>_<j>"
    template<typename M>
    void create_grid(M& mesh, const std::string& name, size_type nx, size_type ny, value_type<M> dx, value_type<M> dy, bool bSurfacesDistinct);

    // Create a hexagonal grid in the xy-plane centered at the origin with corners on the x-axis and having hexagons of radius rHexagon
    // Surfaces: "hex_<i>_<j>" (need numbering scheme reference point ...)
    // dimension in three axes, nRadial as is means all three same ...
    // orientation parameter ...
    template<typename M>
    void create_hexagonal_grid(M& mesh, const std::string& name, size_type nRadial, value_type<M> rHexagon);

    // Create a disk in the xy-plane centered at the origin with nAzimuth divisions
    // Surfaces: "disk"
    template<typename M>
    void create_disk(M& mesh, const std::string& name, size_type nAzimuth, size_type nRadial, value_type<M> radius, const azimuth_interval_type<M>& intervalAzimuth = math::Interval(value_type<M>(1)));

    // Create a box (rectangular cuboid) centered at the origin
    // Surfaces: "left", "right", "back", "front", "bottom", "top"
    template<typename M>
    void create_box(M& mesh, const std::string& name, value_type<M> dx, value_type<M> dy, value_type<M> dz);

    // Create a prism parallel to the z-axis centered at the origin
    // Surfaces: per side "body_<i>", and "end0", "end1", "side", "side0", "size1" depending on extents
    template<typename M>
    void create_prism(M& mesh, const std::string& name, size_type nAzimuth, size_type nz, value_type<M> rLower, value_type<M> rUpper, value_type<M> zLower, value_type<M> zUpper, const Extent<value_type<M>>& extentAzimuth = ExtentSideFlat<value_type<M>>(), const Extent<value_type<M>>& extentZ = ExtentEndsFlat<value_type<M>>());

    // Create an antiprism parallel to the z-axis centered at the origin
    // Surfaces: per side "body_<i>", "end0", "end1"
    template<typename M>
    void create_antiprism(M& mesh, const std::string& name, size_type nAzimuth, value_type<M> rLower, value_type<M> rUpper, value_type<M> zLower, value_type<M> zUpper, const Extent<value_type<M>>& extentZ = ExtentEndsFlat<value_type<M>>());

    // Create a cylinder parallel to the z-axis centered at the origin
    // The separate radii can be used to produce frustrum shapes
    // The nAzimuth and nz parameters control the degree of tessellation
    // Surfaces: "body", and "end0", "end1", "side", "side0", "side1" depending on extents
    template<typename M>
    void create_cylinder(M& mesh, const std::string& name, size_type nAzimuth, size_type nz, value_type<M> rLower, value_type<M> rUpper, value_type<M> zLower, value_type<M> zUpper, const Extent<value_type<M>>& extentAzimuth = ExtentSideFlat<value_type<M>>(), const Extent<value_type<M>>& extentZ = ExtentEndsFlat<value_type<M>>(), bool bSurfacesDistinct = false);

    // Create a cylinder parallel to the z-axis centered at the origin
    // The separate radii can be used to produce frustrum shapes
    // The azimuthOffset can be used to produce a twist, the lower and upper values applying an offset in angle calculations at the corresponding ends
    // The nAzimuth and nz parameters control the degree of tessellation
    // Surfaces: "body", and "end0", "end1", "side", "side0", "side1" depending on extents
    template<typename M>
    void create_cylinder(M& mesh, const std::string& name, size_type nAzimuth, size_type nz, value_type<M> rLower, value_type<M> rUpper, value_type<M> zLower, value_type<M> zUpper, value_type<M> azimuthLower, value_type<M> azimuthUpper, const Extent<value_type<M>>& extentAzimuth = ExtentSideFlat<value_type<M>>(), const Extent<value_type<M>>& extentZ = ExtentEndsFlat<value_type<M>>(), bool bSurfacesDistinct = false);

    // Create a contoured cylinder given radii at a series of z positions
    // The nAzimuth and contour parameters control the degree of tessellation
    // Surfaces: "body", and "end0", "end1", "side", "side0", "size1" depending on extents
    template<typename M>
    void create_cylinder(M& mesh, const std::string& name, size_type nAzimuth, const geometry::Polyline<typename M::vector_reduced_traits>& contour, const Extent<value_type<M>>& extentAzimuth = ExtentSideFlat<value_type<M>>(), const Extent<value_type<M>>& extentZ = ExtentEndsFlat<value_type<M>>());

    // Create a cylinder given a cross section in the form of a simple polygon
    // The nz parameter controls the degree of tessellation along with polygon
    // Polygon vertices should be in ccw order relative to normal vector of 2D projection
    // Surfaces: "body", "end0", "end1"
    // bSurfacesDistinct not currently supported
    template<typename M>
    void create_cylinder(M& mesh, const std::string& name, size_type nz, value_type<M> zLower, value_type<M> zUpper, const geometry::Polygon<typename M::vector_traits>& polygon, const Extent<value_type<M>>& extentZ = ExtentEndsFlat<value_type<M>>(), bool bSurfacesDistinct = false);

    // Create a cylinder given a cross section in the form of a simple polygon
    // The nz parameter controls the degree of tessellation along with polygon
    // Polygon vertices should be in ccw order relative to normal vector of 2D projection
    // Surfaces: "body", "end0", "end1"
    // bSurfacesDistinct not currently supported
    template<typename M>
    void create_cylinder(M& mesh, const std::string& name, size_type nz, value_type<M> zLower, value_type<M> zUpper, value_type<M> revsLower, value_type<M> revsUpper, const geometry::Polygon<typename M::vector_traits>& polygon, const Extent<value_type<M>>& extentZ = ExtentEndsFlat<value_type<M>>(), bool bSurfacesDistinct = false);

    // Create a cylinder given a cross section of bottom and top in the form of simple polygons
    // The nz parameter controls the degree of tessellation along with polygon
    // Polygon vertices should be in ccw order relative to normal vector of 2D projection
    // Surfaces: "body", "end0", "end1"
    // bSurfacesDistinct not currently supported
    template<typename M>
    void create_cylinder(M& mesh, const std::string& name, size_type nz, const geometry::Polygon<typename M::vector_traits>& polygonLower, const geometry::Polygon<typename M::vector_traits>& polygonUpper, const Extent<value_type<M>>& extentZ = ExtentEndsFlat<value_type<M>>(), bool bSurfacesDistinct = false);

    // Create a cylinder given a cross section in the form of a polygon with holes
    // The nz parameter controls the degree of tessellation along with polygons
    // All polygon and hole vertices should be in ccw order relative to normal vector of 2D projection
    // Surfaces: "body", per hole "body_<i>", "end0", "end1" (body_<i> not yet ...)
    template<typename M>
    void create_cylinder(M& mesh, const std::string& name, size_type nz, value_type<M> zLower, value_type<M> zUpper, const geometry::PolygonWithHoles<typename M::vector_traits>& polygons, const Extent<value_type<M>>& extentZ = ExtentEndsFlat<value_type<M>>());

    // Create a cylinder given parametric functions for radius, z, and azimuth offset based on z
    // The nAzimuth and nz parameters control the degree of tessellation
    // Surfaces: "body", and "end0", "end1", "side", "side0", "size1" depending on extents
    template<typename M>
    void create_cylinder(M& mesh, const std::string& name, size_type nAzimuth, size_type nz, std::function<value_type<M>(value_type<M>)> fr, std::function<value_type<M>(value_type<M>)> fz, std::function<value_type<M>(value_type<M>)> fa = [](value_type<M>)->value_type<M>{return value_type<M>(0);}, const Extent<value_type<M>>& extentAzimuth = ExtentSideFlat<value_type<M>>(), const Extent<value_type<M>>& extentZ = ExtentEndsFlat<value_type<M>>());

    // Create a cylinder given parametric functions for radius, z, and azimuth offset based on phi and z
    // The nAzimuth and nz parameters control the degree of tessellation
    // Surfaces: "body", and "end0", "end1", "side", "side0", "size1" depending on extents
    template<typename M>
    void create_cylinder(M& mesh, const std::string& name, size_type nAzimuth, size_type nz, std::function<value_type<M>(value_type<M>, value_type<M>)> fr, std::function<value_type<M>(value_type<M>, value_type<M>)> fz, std::function<value_type<M>(value_type<M>, value_type<M>)> fa = [](value_type<M>)->value_type<M>{return value_type<M>(0);}, const Extent<value_type<M>>& extentAzimuth = ExtentSideFlat<value_type<M>>(), const Extent<value_type<M>>& extentZ = ExtentEndsFlat<value_type<M>>());

    // Create an anticylinder parallel to the z-axis centered at the origin
    // The separate radii can be used to produce anti-frustrum shapes
    // The nAzimuth and nz parameters control the degree of tessellation
    // Surfaces: "body", and "end0", "end1" depending on extents (body_<i>? ...)
    template<typename M>
    void create_anticylinder(M& mesh, const std::string& name, size_type nAzimuth, size_type nz, value_type<M> rLower, value_type<M> rUpper, value_type<M> zLower, value_type<M> zUpper, const Extent<value_type<M>>& extentZ = ExtentEndsFlat<value_type<M>>(), bool bSurfacesDistinct = false);

    // Create a sphere centered at the origin with nAzimuth divisions in the xy-plane and nElevation divisions from the negative to the positive z-axis
    // Surfaces: "body", and "end0", "end1", "side", "side0", "size1" depending on extents
    template<typename M>
    void create_sphere(M& mesh, const std::string& name, size_type nAzimuth, size_type nElevation, value_type<M> radius, const Extent<value_type<M>>& extentAzimuth = ExtentSideFlat<value_type<M>>(), const Extent<value_type<M>>& extentElevation = ExtentEndsFlat<value_type<M>>());

    // Create a sphere centered at the origin given radii at a series of theta positions
    // The nAzimuth and contour parameters control the degree of tessellation
    // Surfaces: "body", and "end0", "end1", "side", "side0", "size1" depending on extents
    template<typename M>
    void create_sphere(M& mesh, const std::string& name, size_type nAzimuth, const geometry::Polyline<typename M::vector_reduced_traits>& contour, const Extent<value_type<M>>& extentAzimuth = ExtentSideFlat<value_type<M>>(), const Extent<value_type<M>>& extentElevation = ExtentEndsFlat<value_type<M>>());

    // Create a sphere centered at the origin given a parametric function for radius based on v, the parameter in the elevation/theta direction
    // The nAzimuth and nElevation parameters control the degree of tessellation
    // Surfaces: "body", and "end0", "end1", "side", "side0", "size1" depending on extents
    template<typename M>
    void create_sphere(M& mesh, const std::string& name, size_type nAzimuth, size_type nElevation, std::function<value_type<M>(value_type<M>)> fr, const Extent<value_type<M>>& extentAzimuth = ExtentSideFlat<value_type<M>>(), const Extent<value_type<M>>& extentElevation = ExtentEndsFlat<value_type<M>>());

    // Create a sphere centered at the origin given a parametric function for radius based on u and v, the parameter in the azimuth/phi and elevation/theta directions respectively
    // The nAzimuth and nElevation parameters control the degree of tessellation
    // Surfaces: "body", and "end0", "end1", "side", "side0", "size1" depending on extents
    template<typename M>
    void create_sphere(M& mesh, const std::string& name, size_type nAzimuth, size_type nElevation, std::function<value_type<M>(value_type<M>, value_type<M>)> fr, const Extent<value_type<M>>& extentAzimuth = ExtentSideFlat<value_type<M>>(), const Extent<value_type<M>>& extentElevation = ExtentEndsFlat<value_type<M>>());

    // Create a torus centered at the origin around the z-axis
    // Minor extent endpoints are measured in the circular cross section in the xz-plane centered at (rMajor, 0, 0) with normal (0, 1, 0)
    // Surfaces: "body", and "end0", "end1", "side", "side0", "size1" depending on extents
    template<typename M>
    void create_torus(M& mesh, const std::string& name, size_type nMajor, size_type nMinor, value_type<M> rMajor, value_type<M> rMinor, const Extent<value_type<M>>& extentMajor = ExtentEndsFlat<value_type<M>>(), const Extent<value_type<M>>& extentMinor = ExtentSideFlat<value_type<M>>());

    // Create a torus centered at the origin around the z-axis
    // Minor extent endpoints are measured in the circular cross section in the xz-plane centered at (rMajor, 0, 0) with normal (0, 1, 0)
    // Surfaces: "body", and "end0", "end1", "side", "side0", "size1" depending on extents
    template<typename M>
    void create_torus(M& mesh, const std::string& name, size_type nMajor, size_type nMinor, value_type<M> rMajorLower, value_type<M> rMajorUpper, value_type<M> rMinorLower, value_type<M> rMinorUpper, value_type<M> zLower, value_type<M> zUpper, const Extent<value_type<M>>& extentMajor = ExtentEndsFlat<value_type<M>>(), const Extent<value_type<M>>& extentMinor = ExtentSideFlat<value_type<M>>());

    // Create a torus centered at the origin around the z-axis with parametric functions for major radius, minor radius, and z based on u and v, the parameters in the major and minor directions respectively
    // Minor extent endpoints are measured in the circular cross section in the xz-plane centered at (rMajor, 0, 0) with normal (0, 1, 0)
    // Surfaces: "body", and "end0", "end1", "side", "side0", "size1" depending on extents
    template<typename M>
    void create_torus(M& mesh, const std::string& name, size_type nMajor, size_type nMinor, std::function<value_type<M>(value_type<M>, value_type<M>)> frMajor, std::function<value_type<M>(value_type<M>, value_type<M>)> frMinor, std::function<value_type<M>(value_type<M>, value_type<M>)> fz, const Extent<value_type<M>>& extentMajor = ExtentEndsFlat<value_type<M>>(), const Extent<value_type<M>>& extentMinor = ExtentSideFlat<value_type<M>>());

    // Create an tetrahedron centered at the origin with the given radius
    // Surfaces: per face "body_<i>"
    template<typename M>
    void create_tetrahedron(M& mesh, const std::string& name, value_type<M> radius);

    // Create an octahedron centered at the origin with the given radius
    // bVertex refers to vertical orientation, vertex or face
    // Surfaces: per face "body_<i>"
    template<typename M>
    void create_octahedron(M& mesh, const std::string& name, value_type<M> radius, bool bVertex = true);

    // Create a dodecahedron centered at the origin
    // Surfaces: per face "body_<i>"
    template<typename M>
    void create_dodecahedron(M& mesh, const std::string& name, value_type<M> radius);

    // Create an icosahedron centered at the origin with the given radius
    // bVertex refers to vertical orientation, vertex or face
    // Surfaces: per face "body_<i>"
    template<typename M>
    void create_icosahedron(M& mesh, const std::string& name, value_type<M> radius, bool bVertex = true, bool bSmooth = false);

    // Create a geodesic sphere centered at the origin with the given radius
    // bVertex refers to vertical orientation, vertex or face
    // nSubdivisions (nSegments? ...)
    // Surfaces: per face "body_<i>"
    template<typename M>
    void create_geodesic_sphere(M& mesh, const std::string& name, value_type<M> radius, size_type nSubdivisions, bool bVertex = true, bool bSmooth = false);

    // single body surface instead opf per side? same as below? ...

    // Create volume by extruding polygon by the displacement vector
    // Surfaces: per side "body_<i>", "end0", "end1"
    template<typename M>
    void create_extrusion(M& mesh, const std::string& name, const geometry::Polygon<typename M::vector_traits>& polygon, const typename M::vector_type& normal, const typename M::vector_type& displacement, const Extent<value_type<M>>& extentZ = ExtentEndsFlat<value_type<M>>());

    // Create volume by extruding polygon by the displacement vector
    // Surfaces: "body_0", per hole "body_<i>", "end0", "end1"
    template<typename M>
    void create_extrusion(M& mesh, const std::string& name, const geometry::PolygonWithHoles<typename M::vector_traits>& polygons, const typename M::vector_type& normal, const typename M::vector_type& displacement, const Extent<value_type<M>>& extentZ = ExtentEndsFlat<value_type<M>>());

    // Create volume by sweeping polygon along points of path
    // Generalized cylinder
    // Surfaces: "body", "end0", "end1"
    template<typename M>
    void create_swept_volume(M& mesh, const std::string& name, const geometry::OrientedPolygon<typename M::vector_traits>& polygon, const geometry::Path<typename M::vector_traits>& path, const Extent<value_type<M>>& extentZ = ExtentEndsFlat<value_type<M>>(), bool bSurfacesDistinct = false);

    // Create volume by sweeping polygon along points of path with parametric function applied to polygon at each point
    // Generalized cylinder
    // Surfaces: "body", "end0", "end1"
    template<typename M>
    void create_swept_volume(M& mesh, const std::string& name, const geometry::OrientedPolygon<typename M::vector_traits>& polygon, const geometry::Path<typename M::vector_traits>& path, std::function<geometry::OrientedPolygon<typename M::vector_traits>(const geometry::OrientedPolygon<typename M::vector_traits>& polygon, value_type<M>)> f, const Extent<value_type<M>>& extentZ = ExtentEndsFlat<value_type<M>>(), bool bSurfacesDistinct = false);

} // namespace quetzal::model

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_grid(M& mesh, const std::string& name, size_type nx, size_type ny, value_type<M> dx, value_type<M> dy, bool bSurfacesDistinct)
{
    using T = typename M::value_type;

    T x0 = T(-0.5) * dx;
    T x1 = T(0.5) * dx;
    T y0 = T(-0.5) * dy;
    T y1 = T(0.5) * dy;

    std::vector<T> x(nx + 1);
    std::vector<T> u(nx + 1);
    for (size_type i = 0; i <= nx; ++i)
    {
        T t = T(i) / T(nx);
        x[i] = math::lerp(x0, x1, t);
        u[i] = t;
    }

    std::vector<T> y(ny + 1);
    std::vector<T> v(ny + 1);
    for (size_type i = 0; i <= ny; ++i)
    {
        T t = T(i) / T(ny);
        y[i] = math::lerp(y0, y1, t);
        v[i] = T(ny - i) / T(ny);
    }

    typename M::vector_type normal = {T(0), T(0), T(1)};
    typename M::face_attributes_type af = {normal};
    typename M::surface_attributes_type as = {normal};

    M m;
    id_type idSubmesh = m.create_submesh(name);
    id_type idSurface0 = bSurfacesDistinct ? nullid : m.create_surface(idSubmesh, "grid", as);

    for (size_type j = 0; j < ny; ++j)
    {
        for (size_type i = 0; i < nx; ++i)
        {
            size_type nh = m.halfedge_store_count();
            size_type nv = m.vertex_store_count();

            id_type idSurface = bSurfacesDistinct ? m.create_surface(idSubmesh, "grid_" + to_string(i) + "_" + to_string(j), as) : idSurface0;
            size_type idFace = m.create_face(idSurface, nh, af);

            m.create_halfedge((j == 0 ? nullid : nh - 4 * nx + 2), nh + 1, nh + 3, nv + 0, idFace);
            m.create_halfedge((i == nx - 1 ? nullid : nh + 7), nh + 2, nh + 0, nv + 1, idFace);
            m.create_halfedge((j == ny - 1 ? nullid : nh + 4 * nx), nh + 3, nh + 1, nv + 2, idFace);
            m.create_halfedge((i == 0 ? nullid : nh - 3), nh + 0, nh + 2, nv + 3, idFace);

            if (bSurfacesDistinct)
            {
                m.create_vertex(nh + 0, {{x[i + 0], y[j + 0], T(0)}, normal, {T(0), T(1)}});
                m.create_vertex(nh + 1, {{x[i + 1], y[j + 0], T(0)}, normal, {T(1), T(1)}});
                m.create_vertex(nh + 2, {{x[i + 1], y[j + 1], T(0)}, normal, {T(1), T(0)}});
                m.create_vertex(nh + 3, {{x[i + 0], y[j + 1], T(0)}, normal, {T(0), T(0)}});
            }
            else
            {
                m.create_vertex(nh + 0, {{x[i + 0], y[j + 0], T(0)}, normal, {u[i + 0], v[j + 0]}});
                m.create_vertex(nh + 1, {{x[i + 1], y[j + 0], T(0)}, normal, {u[i + 1], v[j + 0]}});
                m.create_vertex(nh + 2, {{x[i + 1], y[j + 1], T(0)}, normal, {u[i + 1], v[j + 1]}});
                m.create_vertex(nh + 3, {{x[i + 0], y[j + 1], T(0)}, normal, {u[i + 0], v[j + 1]}});
            }
        }
    }

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_hexagonal_grid(M& mesh, const std::string& name, size_type nRadial, value_type<M> rHexagon)
{
    using T = typename M::value_type;

    typename M::vector_type normal = {T(0), T(0), T(1)};
    typename M::face_attributes_type af = {normal};
    typename M::surface_attributes_type as = {normal};

    M m;
    id_type idSubmesh = m.create_submesh(name);
    id_type idSurface = m.create_surface(idSubmesh, "hexagonal_grid", normal);

/*
    need:
    vertical or horizontal orientation
    n in 3 directions

    size_type two_n = 1 << nRadial;
    size_type nHexagons = (two_n - 1) * 6 + 1;
    size_type nVertices = two_n * 6 + nHexagons;
    size_type nFaces = nHexagons * 6;

    m.vertex_store().resize(nVertices);
//    m.m_indices.resize(nFaces * 3);

    double dx = rHexagon * sqrt(T(3)) / T(2);
    double dy = rHexagon / T(2);

    size_type nRows = 2 * nRadial - 1;

// y -(n - 1) .. (n - 1)
// x same except y < 0 : start change, y > 0 : end change
// ...

*/
    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_disk(M& mesh, const std::string& name, size_type nAzimuth, size_type nRadial, value_type<M> radius, const azimuth_interval_type<M>& intervalAzimuth)
{
    using T = typename M::value_type;

    assert(nRadial > 0);
    assert(nAzimuth > 2);
    assert(radius > T(0));
    assert(intervalAzimuth.proper());

    M m;

    create_cylinder(m, name, nAzimuth, nRadial, radius, T(0), T(0), T(0), Extent(intervalAzimuth), Extent<T>(), false);

    T radius2 = radius * T(2);

    for (auto& vertex : m.vertices())
    {
        vertex.attributes().set_texcoord({(radius + vertex.attributes().position().x()) / radius2, (radius - vertex.attributes().position().y()) / radius2});
    }

    m.rename_surface(m.surface_id(m.submesh_id(name), SurfaceName::Body), "disk");

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_box(M& mesh, const std::string& name, value_type<M> dx, value_type<M> dy, value_type<M> dz)
{
    using T = typename M::value_type;

    M m;
    id_type idSubmesh = m.create_submesh(name);

    const std::vector<std::string> names =
    {
        "bottom",
        "right",
        "front",
        "left",
        "back",
        "top"
    };

    T x1 = T(0.5) * dx;
    T y1 = T(0.5) * dy;
    T z1 = T(0.5) * dz;
    T x0 = -x1;
    T y0 = -y1;
    T z0 = -z1;

    std::vector<typename M::vertex_attributes_type> avs =
    {
        // Bottom
        {{x1, y0, z0}, {T(0), T(0), T(-1)}, {T(0), T(1)}},
        {{x0, y0, z0}, {T(0), T(0), T(-1)}, {T(1), T(1)}},
        {{x0, y1, z0}, {T(0), T(0), T(-1)}, {T(1), T(0)}},
        {{x1, y1, z0}, {T(0), T(0), T(-1)}, {T(0), T(0)}},

        // Right
        {{x1, y0, z0}, {T(1), T(0), T(0)}, {T(0), T(1)}},
        {{x1, y1, z0}, {T(1), T(0), T(0)}, {T(1), T(1)}},
        {{x1, y1, z1}, {T(1), T(0), T(0)}, {T(1), T(0)}},
        {{x1, y0, z1}, {T(1), T(0), T(0)}, {T(0), T(0)}},

        // Front
        {{x1, y1, z0}, {T(0), T(1), T(0)}, {T(0), T(1)}},
        {{x0, y1, z0}, {T(0), T(1), T(0)}, {T(1), T(1)}},
        {{x0, y1, z1}, {T(0), T(1), T(0)}, {T(1), T(0)}},
        {{x1, y1, z1}, {T(0), T(1), T(0)}, {T(0), T(0)}},

        // Left
        {{x0, y1, z0}, {T(-1), T(0), T(0)}, {T(0), T(1)}},
        {{x0, y0, z0}, {T(-1), T(0), T(0)}, {T(1), T(1)}},
        {{x0, y0, z1}, {T(-1), T(0), T(0)}, {T(1), T(0)}},
        {{x0, y1, z1}, {T(-1), T(0), T(0)}, {T(0), T(0)}},

        // Back
        {{x0, y0, z0}, {T(0), T(-1), T(0)}, {T(0), T(1)}},
        {{x1, y0, z0}, {T(0), T(-1), T(0)}, {T(1), T(1)}},
        {{x1, y0, z1}, {T(0), T(-1), T(0)}, {T(1), T(0)}},
        {{x0, y0, z1}, {T(0), T(-1), T(0)}, {T(0), T(0)}},

        // Top
        {{x0, y0, z1}, {T(0), T(0), T(1)}, {T(0), T(1)}},
        {{x1, y0, z1}, {T(0), T(0), T(1)}, {T(1), T(1)}},
        {{x1, y1, z1}, {T(0), T(0), T(1)}, {T(1), T(0)}},
        {{x0, y1, z1}, {T(0), T(0), T(1)}, {T(0), T(0)}},
    };

    for (size_t i = 0; i < 6; ++i)
    {
        typename M::face_attributes_type af {avs[i * 4].normal()};
        m.create_surface(idSubmesh, names[i], af);
        m.create_face(i, i * 4, af);
    }

    for (size_t i = 0; i < 24; ++i)
    {
        m.create_vertex(i, avs[i]);
    }

    m.create_halfedge(16,  1,  3,  0,  0);
    m.create_halfedge(12,  2,  0,  1,  0);
    m.create_halfedge( 8,  3,  1,  2,  0);
    m.create_halfedge( 4,  0,  2,  3,  0);
    m.create_halfedge( 3,  5,  7,  4,  1);
    m.create_halfedge(11,  6,  4,  5,  1);
    m.create_halfedge(21,  7,  5,  6,  1);
    m.create_halfedge(17,  4,  6,  7,  1);
    m.create_halfedge( 2,  9, 11,  8,  2);
    m.create_halfedge(15, 10,  8,  9,  2);
    m.create_halfedge(22, 11,  9, 10,  2);
    m.create_halfedge( 5,  8, 10, 11,  2);
    m.create_halfedge( 1, 13, 15, 12,  3);
    m.create_halfedge(19, 14, 12, 13,  3);
    m.create_halfedge(23, 15, 13, 14,  3);
    m.create_halfedge( 9, 12, 14, 15,  3);
    m.create_halfedge( 0, 17, 19, 16,  4);
    m.create_halfedge( 7, 18, 16, 17,  4);
    m.create_halfedge(20, 19, 17, 18,  4);
    m.create_halfedge(13, 16, 18, 19,  4);
    m.create_halfedge(18, 21, 23, 20,  5);
    m.create_halfedge( 6, 22, 20, 21,  5);
    m.create_halfedge(10, 23, 21, 22,  5);
    m.create_halfedge(14, 20, 22, 23,  5);

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_prism(M& mesh, const std::string& name, size_type nAzimuth, size_type nz, value_type<M> rLower, value_type<M> rUpper, value_type<M> zLower, value_type<M> zUpper, const Extent<value_type<M>>& extentAzimuth, const Extent<value_type<M>>& extentZ)
{
    create_cylinder(mesh, name, nAzimuth, nz, rLower, rUpper, zLower, zUpper, extentAzimuth, extentZ, true);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_antiprism(M& mesh, const std::string& name, size_type nAzimuth, value_type<M> rLower, value_type<M> rUpper, value_type<M> zLower, value_type<M> zUpper, const Extent<value_type<M>>& extentZ)
{
    using T = typename M::value_type;

    assert(nAzimuth > 2);
    assert(rLower > T(0));
    assert(rUpper > T(0));

    math::Interval<T> azimuth(T(1));
    T dAzimuth = T(0.5) / T(nAzimuth);

    std::vector<typename M::vertex_attributes_type> avs0 = vertices_attributes<M>(nAzimuth, rLower, zLower, azimuth, {T(1), T(0), T(0)}, {T(0), T(1), T(1)});
    std::vector<typename M::vertex_attributes_type> avs1 = vertices_attributes<M>(nAzimuth, rUpper, zUpper, shift(azimuth, dAzimuth), {T(1), T(0), T(0)}, {T(0), T(1), T(0)});

    for (size_t i = 0; i < nAzimuth; ++i)
    {
        typename M::point_type position00 = avs0[i].position();
        typename M::point_type position01 = avs0[i + 1].position();
        typename M::point_type position10 = avs1[i].position();
        typename M::point_type position11 = avs1[i + 1].position();

        avs0[i + 1].set_normal(normalize(cross(position11 - position01, position10 - position01)));
        avs1[i].set_normal(normalize(cross(position00 - position10, position01 - position10)));
    }

    M m;
    id_type idSubmesh = m.create_submesh(name);

    size_type nh = 0;

    m.halfedge_store().resize(6 * nAzimuth);
    m.vertex_store().resize(6 * nAzimuth);

    for (size_type j = 0; j < nAzimuth; ++j)
    {
        size_type j0 = nh;
        size_type j1 = j0 + nAzimuth;
        size_type j2 = j1 + nAzimuth;
        size_type j3 = j2 + nAzimuth;
        size_type j4 = j3 + nAzimuth;
        size_type j5 = j4 + nAzimuth;

        id_type idSurface = m.create_surface(idSubmesh, SurfaceName::BodySection + "_" + to_string(2 * j), {avs1[j].normal()});
        id_type idFace = m.create_face(idSurface, j0, {avs1[j].normal()});

        id_type idPartner = nullid;
        m.halfedge(j0) = {m, j0, idPartner, j1, j2, j0, idFace};
        idPartner = j5;
        m.halfedge(j1) = {m, j1, idPartner, j2, j0, j1, idFace};
        idPartner = (j > 0 ? j3 - 1 : j3 + nAzimuth - 1);
        m.halfedge(j2) = {m, j2, idPartner, j0, j1, j2, idFace};

        idSurface = m.create_surface(idSubmesh, SurfaceName::BodySection + "_" + to_string(2 * j + 1), {avs0[j + 1].normal()});
        idFace = m.create_face(idSurface, j3, {avs0[j + 1].normal()});

        idPartner = (j < nAzimuth - 1 ? j2 + 1 : j2 - nAzimuth + 1);
        m.halfedge(j3) = {m, j3, idPartner, j4, j5, j3, idFace};
        idPartner = nullid;
        m.halfedge(j4) = {m, j4, idPartner, j5, j3, j4, idFace};
        idPartner = j1;
        m.halfedge(j5) = {m, j5, idPartner, j3, j4, j5, idFace};

        typename M::vertex_attributes_type attributes;
        attributes.set(avs0[j].position(), avs1[j].normal(), {T(0), T(1)});
        m.vertex(j0) = {m, j0, j0, attributes};
        attributes.set(avs0[j + 1].position(), avs1[j].normal(), {T(1), T(1)});
        m.vertex(j1) = {m, j1, j1, attributes};
        attributes.set(avs1[j].position(), avs1[j].normal(), {T(0.5), T(0)});
        m.vertex(j2) = {m, j2, j2, attributes};
        attributes.set(avs0[j + 1].position(), avs0[j + 1].normal(), {T(0.5), T(1)});
        m.vertex(j3) = {m, j3, j3, attributes};
        attributes.set(avs1[j + 1].position(), avs0[j + 1].normal(), {T(1), T(0)});
        m.vertex(j4) = {m, j4, j4, attributes};
        attributes.set(avs1[j].position(), avs0[j + 1].normal(), {T(0), T(0)});
        m.vertex(j5) = {m, j5, j5, attributes};

        ++nh;
    }

    seal_cylinder(m, nAzimuth, 1, false, false, false, Extent<T>(), extentZ, idSubmesh);

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_cylinder(M& mesh, const std::string& name, size_type nAzimuth, size_type nz, value_type<M> rLower, value_type<M> rUpper, value_type<M> zLower, value_type<M> zUpper, const Extent<value_type<M>>& extentAzimuth, const Extent<value_type<M>>& extentZ, bool bSurfacesDistinct)
{
    using T = typename M::value_type;

    create_cylinder(mesh, name, nAzimuth, nz, rLower, rUpper, zLower, zUpper, T(0), T(0), extentAzimuth, extentZ, bSurfacesDistinct);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_cylinder(M& mesh, const std::string& name, size_type nAzimuth, size_type nz, value_type<M> rLower, value_type<M> rUpper, value_type<M> zLower, value_type<M> zUpper, value_type<M> azimuthLower, value_type<M> azimuthUpper, const Extent<value_type<M>>& extentAzimuth, const Extent<value_type<M>>& extentZ, bool bSurfacesDistinct)
{
    using T = typename M::value_type;

    assert(nAzimuth > 2);
    assert(nz > 0);
    assert(rLower >= T(0));
    assert(rUpper >= T(0));
    assert(extentAzimuth.proper());
    assert(extentZ.proper());

    bool bCuspLower = math::float_eq0(rLower);
    bool bCuspUpper = math::float_eq0(rUpper);
    bool bOpenSide = !extentAzimuth.full();
    assert(!bCuspLower || !bCuspUpper);

    T t = T(1) / T(nz);

    T dr = rUpper - rLower;
    T r0 = rLower;
    T r1 = math::lerp(rLower, rUpper, t);

    T dz = zUpper - zLower;
    T z0 = zLower;
    T z1 = math::lerp(zLower, zUpper, t);

    T azimuth0 = azimuthLower;
    T azimuth1 = math::lerp(azimuthLower, azimuthUpper, t);

    normal_vector_type<M> normalProto {dz, T(0), -dr};

    M m;
    id_type idSubmesh = m.create_submesh(name);

    id_type idSurface0 = nullid;
    if (bSurfacesDistinct)
    {
        idSurface0 = m.surface_store_count();
        for (size_t i = 0; i < nAzimuth; ++i)
        {
            m.create_surface(idSubmesh, SurfaceName::BodySection + "_" + to_string(i));
        }
    }
    else
    {
        idSurface0 = m.create_surface(idSubmesh, SurfaceName::Body);
    }

    if (bCuspLower)
    {
        auto tsProto1 = texture_span<T>(1, nz, bCuspLower, bCuspUpper);
        create_base_cusp(m, nAzimuth, r1, z0, z1, shift(extentAzimuth.interval(), azimuth1), normalProto, normalProto, tsProto1, idSurface0, bSurfacesDistinct);
    }
    else if (nz == 1 && bCuspUpper)
    {
        create_apex_cusp(m, nAzimuth, r0, z0, z1, shift(extentAzimuth.interval(), azimuth0), normalProto, normalProto, idSurface0, bSurfacesDistinct);
    }
    else
    {
        auto tsProto0 = texture_span<T>(0, nz, bCuspLower, bCuspUpper);
        auto tsProto1 = texture_span<T>(1, nz, bCuspLower, bCuspUpper);
        create_band(m, nAzimuth, r0, r1, z0, z1, shift(extentAzimuth.interval(), azimuth0), shift(extentAzimuth.interval(), azimuth1), normalProto, normalProto, tsProto0, tsProto1, idSurface0, bSurfacesDistinct);
    }

    for (size_type i = 2; i <= nz; ++i)
    {
        t = T(i) / T(nz);
        r1 = math::lerp(rLower, rUpper, t);
        z1 = math::lerp(zLower, zUpper, t);
        azimuth1 = math::lerp(azimuthLower, azimuthUpper, t);

        if (i == nz && bCuspUpper)
        {
            connect_apex_cusp(m, nAzimuth, z1, shift(extentAzimuth.interval(), azimuth1), normalProto, idSurface0, bSurfacesDistinct, true);
        }
        else
        {
            auto tsProto1 = texture_span<T>(i, nz, bCuspLower, bCuspUpper);
            connect_band(m, nAzimuth, r1, z1, shift(extentAzimuth.interval(), azimuth1), normalProto, tsProto1, idSurface0, bSurfacesDistinct, true);
        }
    }

    seal_cylinder(m, nAzimuth, nz, bCuspLower, bCuspUpper, bOpenSide, extentAzimuth, extentZ, idSubmesh);

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_cylinder(M& mesh, const std::string& name, size_type nAzimuth, const geometry::Polyline<typename M::vector_reduced_traits>& contour, const Extent<value_type<M>>& extentAzimuth, const Extent<value_type<M>>& extentZ)
{
    using T = typename M::value_type;

    assert(nAzimuth > 2);
    assert(contour.edge_count() > 0);
    assert(extentAzimuth.proper());
    assert(extentZ.proper());

    size_type nz = contour.edge_count();

    T rLower = contour.vertex(0).y();
    T rUpper = contour.vertex(nz).y();

    bool bCuspLower = math::float_eq0(rLower);
    bool bCuspUpper = math::float_eq0(rUpper);
    bool bOpenSide = !extentAzimuth.full();

    T r0 = rLower;
    T r1 = contour.vertex(1).y();

    T z0 = contour[0].x();
    T z1 = contour[1].x();

    T dd = contour.length();
    T d1 = contour.length(0, 1);

    auto tsProto0 = texture_span<T>(T(0), nz, bCuspLower, bCuspUpper);
    auto tsProto1 = texture_span<T>(T(1), nz, bCuspLower, bCuspUpper);

    M m;
    id_type idSubmesh = m.create_submesh(name);
    id_type idSurface = m.create_surface(idSubmesh, SurfaceName::Body);

    if (bCuspLower)
    {
        create_base_cusp(m, nAzimuth, r1, z0, z1, extentAzimuth.interval(), tsProto1, idSurface, false);
    }
    else if (nz == 1 && bCuspUpper)
    {
        create_apex_cusp(m, nAzimuth, r0, z0, z1, extentAzimuth.interval(), idSurface);
    }
    else
    {
        create_band(m, nAzimuth, r0, r1, z0, z1, extentAzimuth.interval(), extentAzimuth.interval(), tsProto0, tsProto1, idSurface, false);
    }

    for (size_type i = 2; i <= nz; ++i)
    {
        const auto& point = contour.vertex(i);
        z1 = point.x();
        r1 = point.y();
        d1 += contour.length(i - 1, i);
        if (i == nz)
        {
            d1 = dd;
        }

        if (i == nz && bCuspUpper)
        {
            connect_apex_cusp(m, nAzimuth, z1, {r1, z1}, extentAzimuth.interval(), idSurface, false, true);
        }
        else
        {
            auto tsProto1 = texture_span<T>(i, nz, bCuspLower, bCuspUpper);
            connect_band(m, nAzimuth, r1, z1, extentAzimuth.interval(), idSurface, false, true, tsProto1);
        }
    }

//    calculate_surface_normals(m); ...
calculate_axial_vertex_normals(m, nAzimuth);

    seal_cylinder(m, nAzimuth, nz, bCuspLower, bCuspUpper, bOpenSide, extentAzimuth, extentZ, idSubmesh);

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_cylinder(M& mesh, const std::string& name, size_type nz, value_type<M> zLower, value_type<M> zUpper, const geometry::Polygon<typename M::vector_traits>& polygon, const Extent<value_type<M>>& extentZ, bool bSurfacesDistinct)
{
    using T = typename M::value_type;

    size_type nAzimuth = polygon.edge_count();
    assert(nAzimuth > 2);
    assert(nz > 0);

    T t = T(1) / T(nz);

    T z0 = zLower;
    T z1 = math::lerp(zLower, zUpper, t);

    T ty0 = T(1);
    T ty1 = ty0 - t;

    M m;
    id_type idSubmesh = m.create_submesh(name);

    id_type idSurface0 = nullid;
    if (bSurfacesDistinct)
    {
        idSurface0 = m.surface_store_count();
        for (size_t i = 0; i < nAzimuth; ++i)
        {
            m.create_surface(idSubmesh, SurfaceName::BodySection + "_" + to_string(i));
        }
    }
    else
    {
        idSurface0 = m.create_surface(idSubmesh, SurfaceName::Body);
    }

    std::vector<typename M::vertex_attributes_type> avs0 = vertices_attributes<M>(polygon, z0, ty0);
    std::vector<typename M::vertex_attributes_type> avs1 = vertices_attributes<M>(polygon, z1, ty1);
    
    auto tsProto0 = texture_span<T>(0, nz, false, false);
    auto tsProto1 = texture_span<T>(1, nz, false, false);

    create_band(m, avs0, avs1, tsProto0, tsProto1, true, idSurface0, bSurfacesDistinct);

    for (size_type i = 2; i <= nz; ++i)
    {
        t = T(i) / T(nz);
        z1 = math::lerp(zLower, zUpper, t);
        ty1 = ty0 - t;
        for (auto& attributes : avs1)
        {
            attributes.position().set_z(z1);
            attributes.texcoord().set_y(ty1);
        }

        tsProto1 = texture_span<T>(i, nz, false, false); // use this instead of above ...

        connect_band(m, avs1, tsProto1, true, idSurface0, bSurfacesDistinct, true);
    }

    seal_cylinder(m, nAzimuth, nz, false, false, false, {}, extentZ, idSubmesh);

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_cylinder(M& mesh, const std::string& name, size_type nz, value_type<M> zLower, value_type<M> zUpper, value_type<M> revsLower, value_type<M> revsUpper, const geometry::Polygon<typename M::vector_traits>& polygon, const Extent<value_type<M>>& extentZ, bool bSurfacesDistinct)
{
    using T = typename M::value_type;

    size_type nAzimuth = polygon.edge_count();
    assert(nAzimuth > 2);
    assert(nz > 0);

    T t = T(1) / T(nz);

    T scale0 = T(1);
    T scale1 = T(1);

    T rev0 = revsLower;
    T rev1 = math::lerp(revsLower, revsUpper, t);

    T z0 = zLower;
    T z1 = math::lerp(zLower, zUpper, t);

    auto vertex_normal = [](const typename M::point_type& point, T rev) -> typename M::vector_type
    {
        T phi = math::PiTwo<T> * rev;
        T cos_phi = cos(phi);
        T sin_phi = sin(phi);
        typename M::vector_type normal = math::normalize<typename M::vector_traits>({cos_phi * point.x() - sin_phi * point.y(), sin_phi * point.x() + cos_phi * point.y(), T(0)});
        return normal;
    };

    auto tsProto0 = texture_span<T>(0, nz, false, false);
    auto tsProto1 = texture_span<T>(1, nz, false, false);

    bool bLinear = revsUpper == revsLower;

    M m;
    id_type idSubmesh = m.create_submesh(name);

    id_type idSurface0 = nullid;
    if (bSurfacesDistinct)
    {
        idSurface0 = m.surface_store_count();
        for (size_t i = 0; i < nAzimuth; ++i)
        {
            m.create_surface(idSubmesh, SurfaceName::BodySection + "_" + to_string(i));
        }
    }
    else
    {
        idSurface0 = m.create_surface(idSubmesh, SurfaceName::Body);
    }

    std::vector<typename M::vertex_attributes_type> avs0 = vertices_attributes<M>(polygon, scale0, rev0, z0, vertex_normal, tsProto0[2]);
    std::vector<typename M::vertex_attributes_type> avs1 = vertices_attributes<M>(polygon, scale1, rev1, z1, vertex_normal, tsProto1[2]);

	create_band(m, avs0, avs1, tsProto0, tsProto1, true, idSurface0, bSurfacesDistinct);

    for (size_type i = 2; i <= nz; ++i)
    {
        t = T(i) / T(nz);
        z1 = math::lerp(zLower, zUpper, t);
        rev1 = math::lerp(revsLower, revsUpper, t);
        tsProto1 = texture_span<T>(i, nz, false, false);

        avs1 = vertices_attributes<M>(polygon, scale1, rev1, z1, vertex_normal, tsProto1[2]);
	    connect_band(m, avs1, tsProto1, true, idSurface0, bSurfacesDistinct, bLinear);
    }

    seal_cylinder(m, nAzimuth, nz, false, false, false, Extent<T>(), extentZ, idSubmesh);

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_cylinder(M& mesh, const std::string& name, size_type nz, const geometry::Polygon<typename M::vector_traits>& polygonLower, const geometry::Polygon<typename M::vector_traits>& polygonUpper, const Extent<value_type<M>>& extentZ, bool bSurfacesDistinct)
{
    assert(polygonLower.vertex_count() == polygonUpper.vertex_count());

    using T = typename M::value_type;

    size_type nAzimuth = polygonLower.edge_count();
    assert(nAzimuth > 2);
    assert(nz > 0);

    T t = T(1) / T(nz);

    T ty0 = T(1);
    T ty1 = ty0 - t;

    M m;
    id_type idSubmesh = m.create_submesh(name);

    id_type idSurface0 = nullid;
    if (bSurfacesDistinct)
    {
        idSurface0 = m.surface_store_count();
        for (size_t i = 0; i < nAzimuth; ++i)
        {
            m.create_surface(idSubmesh, SurfaceName::BodySection + "_" + to_string(i));
        }
    }
    else
    {
        idSurface0 = m.create_surface(idSubmesh, SurfaceName::Body);
    }

    std::vector<typename M::vertex_attributes_type> avs0 = vertices_attributes<M>(polygonLower, ty0);
    std::vector<typename M::vertex_attributes_type> avs1 = vertices_attributes<M>(polygonLower, polygonUpper, t, ty1);
    create_band(m, avs0, avs1, true, idSurface0, bSurfacesDistinct);

    for (size_type i = 2; i <= nz; ++i)
    {
        t = T(i) / T(nz);
        ty1 = ty0 - t;

        avs1 = vertices_attributes<M>(polygonLower, polygonUpper, t, ty1);
        connect_band(m, avs1, true, idSurface0, bSurfacesDistinct, true);
    }

    seal_cylinder(m, nAzimuth, nz, false, false, false, Extent<T>(), extentZ, idSubmesh);

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_cylinder(M& mesh, const std::string& name, size_type nz, value_type<M> zLower, value_type<M> zUpper, const geometry::PolygonWithHoles<typename M::vector_traits>& polygons, const Extent<value_type<M>>& extentZ)
{
    using T = typename M::value_type;

    assert(nz > 0);
    assert(extentZ.proper());

    std::string nameSurface = SurfaceName::BodySection + "_" + to_string(0); // surface for cylinder ...

    M m;
    create_cylinder(m, name, nz, zLower, zUpper, polygons.polygon(), {});
    id_type idSubmesh = m.submesh_id(name);

    id_type idHalfedgeLower = 0;
    assert(m.halfedge(idHalfedgeLower).border());
    id_type idHalfedgeUpper = m.halfedge_store_count() - 2 * polygons.polygon().edge_count();
    assert(m.halfedge(idHalfedgeUpper).border());

    std::vector<id_type> idHalfedgeHolesLower(polygons.hole_count());
    std::vector<id_type> idHalfedgeHolesUpper(polygons.hole_count());
    for (size_t i = 0; i < polygons.hole_count(); ++i)
    {
        const auto& polygon = polygons.hole(i);

        idHalfedgeHolesLower[i] = m.halfedge_store_count();

        nameSurface = SurfaceName::BodySection + "_" + to_string(i + 1); // surface for cylinder ...

        M mm;
        create_cylinder(mm, name, nz, zLower, zUpper, polygon, {});
        brep::invert(mm);
        m.append(mm);

        assert(m.halfedge(idHalfedgeHolesLower[i]).border());
        idHalfedgeHolesUpper[i] = m.halfedge_store_count() - 2 * polygon.edge_count();
        assert(m.halfedge(idHalfedgeHolesUpper[i]).border());
    }

    // make sure that the orientation of holes is correct due the the reversed winding order ...

    if (extentZ.lower_termination().type == Termination::Type::Flat)
    {
        typename M::vector_type normal = {T(0), T(0), T(-1)};
        id_type idSurfaceLower = m.create_surface(idSubmesh, extentZ.lower_name(), {normal});

        create_border_with_holes_face(m, idHalfedgeLower, idHalfedgeHolesLower, normal, idSurfaceLower);
        calculate_surface_texcoords(m, idSurfaceLower);
    }

    if (extentZ.upper_termination().type == Termination::Type::Flat)
    {
        typename M::vector_type normal = {T(0), T(0), T(1)};
        id_type idSurfaceUpper = m.create_surface(idSubmesh, extentZ.upper_name(), {normal});

        create_border_with_holes_face(m, idHalfedgeUpper, idHalfedgeHolesUpper, normal, idSurfaceUpper);
        calculate_surface_texcoords(m, idSurfaceUpper);
    }

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_cylinder(M& mesh, const std::string& name, size_type nAzimuth, size_type nz, std::function<value_type<M>(value_type<M>)> fr, std::function<value_type<M>(value_type<M>)> fz, std::function<value_type<M>(value_type<M>)> fa, const Extent<value_type<M>>& extentAzimuth, const Extent<value_type<M>>& extentZ)
{
    using T = typename M::value_type;

    assert(nAzimuth > 2);
    assert(nz > 0);
    assert(extentAzimuth.proper());
    assert(extentZ.proper());

    T rLower = fr(T(0));
    T rUpper = fr(T(1));

    bool bCuspLower = math::float_eq0(rLower);
    bool bCuspUpper = math::float_eq0(rUpper);
    bool bOpenSide = !extentAzimuth.full();

    T t0 = T(0);
    T t1 = T(1) / T(nz);

    T r0 = rLower;
    T r1 = fr(t1);

    T z0 = fz(t0);
    T z1 = fz(t1);

    T a0 = fa(t0);
    T a1 = fa(t1);

    M m;
    id_type idSubmesh = m.create_submesh(name);
    id_type idSurface = m.create_surface(idSubmesh, SurfaceName::Body);

	math::Interval azimuth0 = shift(extentAzimuth.interval(), a0);
	math::Interval azimuth1 = shift(extentAzimuth.interval(), a1);

    if (bCuspLower)
    {
        auto tsProto1 = texture_span<T>(1, nz, bCuspLower, bCuspUpper);
        create_base_cusp(m, nAzimuth, r1, z0, z1, azimuth1, tsProto1, idSurface, false);
    }
    else if (nz == 1 && bCuspUpper)
    {
        create_apex_cusp(m, nAzimuth, r0, z0, z1, azimuth0, idSurface);
    }
    else
    {
        auto tsProto0 = texture_span<T>(0, nz, bCuspLower, bCuspUpper);
        auto tsProto1 = texture_span<T>(1, nz, bCuspLower, bCuspUpper);
        create_band(m, nAzimuth, r0, r1, z0, z1, azimuth0, azimuth1, tsProto0, tsProto1, idSurface, false);
    }

    for (size_type i = 2; i <= nz; ++i)
    {
        t1 = T(i) / T(nz);

        r1 = fr(t1);
        z1 = fz(t1);
        a1 = fa(t1);
    	azimuth1 = shift(extentAzimuth.interval(), a1);

        if (i == nz && bCuspUpper)
        {
            connect_apex_cusp(m, nAzimuth, z1, azimuth1, {r1, T(0), z1}, idSurface, false, true);
        }
        else
        {
            auto tsProto1 = texture_span<T>(i, nz, bCuspLower, bCuspUpper);
            connect_band(m, nAzimuth, r1, z1, azimuth1, tsProto1, idSurface, false, true);
        }
    }

//    calculate_surface_normals(m); ...
calculate_axial_vertex_normals(m, nAzimuth);

    seal_cylinder(m, nAzimuth, nz, bCuspLower, bCuspUpper, bOpenSide, extentAzimuth, extentZ, idSubmesh);

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_cylinder(M& mesh, const std::string& name, size_type nAzimuth, size_type nz, std::function<value_type<M>(value_type<M>, value_type<M>)> fr, std::function<value_type<M>(value_type<M>, value_type<M>)> fz, std::function<value_type<M>(value_type<M>, value_type<M>)> fa, const Extent<value_type<M>>& extentAzimuth, const Extent<value_type<M>>& extentZ)
{
    // create_cylinder ...
    id_type idSubmesh = nullid;
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_anticylinder(M& mesh, const std::string& name, size_type nAzimuth, size_type nz, value_type<M> rLower, value_type<M> rUpper, value_type<M> zLower, value_type<M> zUpper, const Extent<value_type<M>>& extentZ, bool bSurfacesDistinct)
{
    using T = typename M::value_type;

    assert(nAzimuth > 2);
    assert(nz > 0);
    assert(rLower > T(0));
    assert(rUpper > T(0));
    assert(extentZ.proper());

    T t = T(1) / T(nz);

    T dr = rUpper - rLower;
    T r0 = rLower;
    T r1 = math::lerp(rLower, rUpper, t);

    T dz = zUpper - zLower;
    T z0 = zLower;
    T z1 = math::lerp(zLower, zUpper, t);

    math::Interval<T> azimuth(T(1));

    normal_vector_type<M> normalProto0 {dz, T(0), -dr};
    normal_vector_type<M> normalProto1 {dz, T(0), -dr};

    T ty0 = T(1);
    T ty1 = ty0 - t;

    M m;
    id_type idSubmesh = m.create_submesh(name);

    id_type idSurface0 = nullid;
    if (bSurfacesDistinct)
    {
        idSurface0 = m.surface_count();
        for (size_t i = 0; i < 2 * nAzimuth; ++i)
        {
            m.create_surface(idSubmesh, SurfaceName::BodySection + "_" + to_string(idSurface0 + i));
        }
    }
    else
    {
        idSurface0 = m.create_surface(idSubmesh, SurfaceName::Body);
    }

    create_antiband(m, nAzimuth, r0, r1, z0, z1, normalProto0, normalProto1, ty0, ty1, idSurface0, bSurfacesDistinct);

    for (size_type i = 2; i <= nz; ++i)
    {
        t = T(i) / T(nz);
        r1 = math::lerp(rLower, rUpper, t);
        z1 = math::lerp(zLower, zUpper, t);
        ty1 = ty0 - t;

        if (bSurfacesDistinct)
        {
            idSurface0 = m.surface_count();
            for (size_t j = 0; j < 2 * nAzimuth; ++j)
            {
                m.create_surface(idSubmesh, SurfaceName::BodySection + "_" + to_string(idSurface0 + j));
            }
        }

        connect_antiband(m, nAzimuth, r1, z1, ty1, idSurface0, bSurfacesDistinct);
    }

    seal_cylinder(m, nAzimuth, nz, false, false, false, Extent<T>(), extentZ, idSubmesh);

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_sphere(M& mesh, const std::string& name, size_type nAzimuth, size_type nElevation, value_type<M> radius, const Extent<value_type<M>>& extentAzimuth, const Extent<value_type<M>>& extentElevation)
{
    using T = typename M::value_type;

    assert(nAzimuth > 2);
    assert(nElevation > 1);
    assert(radius > T(0));
    assert(extentAzimuth.proper());
    assert(extentElevation.proper());
    assert(extentElevation.interval().lower() >= T(0) && extentElevation.interval().length() <= T(1));

    bool bCuspLower = math::float_eq0(extentElevation.interval().lower());
    bool bCuspUpper = math::float_eq(extentElevation.interval().upper(), T(1));
    bool bOpenSide = !extentAzimuth.full();

    T t = T(1) / T(nElevation);
    T t0 = extentElevation.interval().lower();
    T t1 = extentElevation.interval().lerp(t);

    T theta0 = math::Pi<T> * (t0 - T(0.5));
    T theta1 = math::Pi<T> * (t1 - T(0.5));
    assert(theta0 >= -math::PiHalf<T>);

    T r0 = bCuspLower ? T(0) : radius * cos(theta0);
    T r1 = radius * cos(theta1);

    T z0 = radius * sin(theta0);
    T z1 = radius * sin(theta1);

    normal_vector_type<M> normalProto0 {r0, T(0), z0};
    normal_vector_type<M> normalProto1 {r1, T(0), z1};

    auto tsProto0 = texture_span<T>(0, nElevation, bCuspLower, bCuspUpper);
    auto tsProto1 = texture_span<T>(1, nElevation, bCuspLower, bCuspUpper);

    M m;
    id_type idSubmesh = m.create_submesh(name);
    id_type idSurface = m.create_surface(idSubmesh, SurfaceName::Body);

    if (bCuspLower)
    {
        normalProto0 = {T(0), T(0), T(-1)};
        create_base_cusp(m, nAzimuth, r1, -radius, z1, extentAzimuth.interval(), normalProto0, normalProto1, tsProto1, idSurface, false);
    }
    else
    {
        create_band(m, nAzimuth, r0, r1, z0, z1, extentAzimuth.interval(), extentAzimuth.interval(), normalProto0, normalProto1, tsProto0, tsProto1, idSurface, false);
    }

    for (size_type i = 2; i <= nElevation; ++i)
    {
        t = T(i) / T(nElevation);
        t1 = extentElevation.interval().lerp(t);
        theta1 = math::Pi<T> * (t1 - T(0.5));
        r1 = i == nElevation && bCuspUpper ? T(0) : radius * cos(theta1);
        z1 = radius * sin(theta1);
        normalProto1 = {r1, T(0), z1};
        tsProto1 = texture_span<T>(i, nElevation, bCuspLower, bCuspUpper);

        if (i == nElevation && bCuspUpper)
        {
            connect_apex_cusp(m, nAzimuth, radius, extentAzimuth.interval(), {T(0), T(0), T(1)}, idSurface);
        }
        else
        {
            connect_band(m, nAzimuth, r1, z1, extentAzimuth.interval(), normalProto1, tsProto1, idSurface, false, false);
        }
    }

    seal_sphere(m, nAzimuth, nElevation, bCuspLower, bCuspUpper, bOpenSide, extentAzimuth, extentElevation, idSubmesh);

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_sphere(M& mesh, const std::string& name, size_type nAzimuth, const geometry::Polyline<typename M::vector_reduced_traits>& contour, const Extent<value_type<M>>& extentAzimuth, const Extent<value_type<M>>& extentElevation)
{
    using T = typename M::value_type;

    assert(nAzimuth > 2);
    assert(contour.edge_count() > 0);
    assert(extentAzimuth.proper());
    assert(extentElevation.proper());
    assert(extentElevation.interval().lower() >= T(0) && extentElevation.interval().length() <= T(1));

    size_type nElevation = contour.edge_count();

    bool bCuspLower = math::float_eq0(extentElevation.interval().lower());
    bool bCuspUpper = math::float_eq(extentElevation.interval().upper(), T(1));
    bool bOpenSide = !extentAzimuth.full();

    T t0 = contour.vertex(0).x();
    T t1 = contour.vertex(1).x();
    T dt = contour.back().x() - t0;

    T radius0 = contour.vertex(0).y();
    T radius1 = contour.vertex(1).y();

    T theta0 = math::Pi<T> * (t0 - T(0.5));
    T theta1 = math::Pi<T> * (t1 - T(0.5));

    T r0 = radius0 * cos(theta0);
    T z0 = radius0 * sin(theta0);
    T r1 = radius1 * cos(theta1);
    T z1 = radius1 * sin(theta1);

    // normalProto not meaningful here ...
    normal_vector_type<M> normalProto0 {r0, T(0), z0};
    normal_vector_type<M> normalProto1 {r1, T(0), z1};

    T dd = contour.length();
    T d1 = contour.length(0, 1);

    auto tsProto0 = texture_span<T>(T(0), bCuspLower, bCuspUpper);
    auto tsProto1 = texture_span<T>(d1 / dd, bCuspLower, bCuspUpper);

    M m;
    id_type idSubmesh = m.create_submesh(name);
    id_type idSurface = m.create_surface(idSubmesh, SurfaceName::Body);

    if (bCuspLower)
    {
        create_base_cusp(m, nAzimuth, r1, z0, z1, extentAzimuth.interval(), normalProto0, normalProto1, tsProto1, idSurface, false);
    }
    else if (nElevation == 1 && bCuspUpper)
    {
        create_apex_cusp(m, nAzimuth, r0, z0, z1, extentAzimuth.interval(), normalProto0, normalProto1, idSurface, false);
    }
    else
    {
        create_band(m, nAzimuth, r0, r1, z0, z1, extentAzimuth.interval(), extentAzimuth.interval(), normalProto0, normalProto1, tsProto0, tsProto1, idSurface, false);
    }

    for (size_type i = 2; i <= nElevation; ++i)
    {
        const auto& point = contour.vertex(i);
        T t1 = point.x();
        T theta1 = math::Pi<T> * (t1 - T(0.5));
        T radius1 = point.y();
        r1 = radius1 * cos(theta1);
        z1 = radius1 * sin(theta1);
        normalProto1 = {r1, T(0), z1};
        d1 += contour.length(i - 1, i);
        if (i == nElevation)
        {
            d1 = dd;
        }
        tsProto1 = texture_span<T>(d1 / dd, bCuspLower, bCuspUpper);

        if (i == nElevation && bCuspUpper)
        {
            T radius0 = contour.vertex(i - 1).y();
assert(float_eq0(radius0)); // ...
            connect_apex_cusp(m, nAzimuth, radius0, extentAzimuth.interval(), {T(0), T(0), T(1)}, idSurface);
        }
        else
        {
            connect_band(m, nAzimuth, r1, z1, extentAzimuth.interval(), normalProto1, tsProto1, idSurface);
        }
    }

    calculate_surface_normals(m);

    seal_sphere(m, nAzimuth, nElevation, bCuspLower, bCuspUpper, bOpenSide, extentAzimuth, extentElevation, idSubmesh);

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_sphere(M& mesh, const std::string& name, size_type nAzimuth, size_type nElevation, std::function<value_type<M>(value_type<M>)> fr, const Extent<value_type<M>>& extentAzimuth, const Extent<value_type<M>>& extentElevation)
{
    using T = typename M::value_type;

    assert(nAzimuth > 2);
    assert(nElevation > 1);
    assert(extentAzimuth.proper());
    assert(extentElevation.proper());
    assert(extentElevation.interval().lower() >= T(0) && extentElevation.interval().length() <= T(1));

    T rLower = fr(T(0));
    T rUpper = fr(T(1));

    bool bCuspLower = math::float_eq0(extentElevation.interval().lower());
    bool bCuspUpper = math::float_eq(extentElevation.interval().upper(), T(1));
    bool bOpenSide = !extentAzimuth.full();

    T t = T(1) / T(nElevation);
    T t0 = extentElevation.interval().lower();
    T t1 = extentElevation.interval().lerp(t);

    T theta0 = math::Pi<T> * (t0 - T(0.5));
    T theta1 = math::Pi<T> * (t1 - T(0.5));

    T radius0 = rLower;
    T radius1 = fr(t1);

    T r0 = radius0 * cos(theta0);
    T r1 = radius1 * cos(theta1);

    T z0 = radius0 * sin(theta0);
    T z1 = radius1 * sin(theta1);

    // normalProto not meaningful here ...
    normal_vector_type<M> normalProto0 {r0, T(0), z0};
    normal_vector_type<M> normalProto1 {r1, T(0), z1};

    auto tsProto0 = texture_span<T>(0, nElevation, bCuspLower, bCuspUpper);
    auto tsProto1 = texture_span<T>(1, nElevation, bCuspLower, bCuspUpper);

    M m;
    id_type idSubmesh = m.create_submesh(name);
    id_type idSurface = m.create_surface(idSubmesh, SurfaceName::Body);

    if (bCuspLower)
    {
        create_base_cusp(m, nAzimuth, r1, z0, z1, extentAzimuth.interval(), normalProto0, normalProto1, tsProto1, idSurface, false);
    }
    else if (nElevation == 1 && bCuspUpper)
    {
        create_apex_cusp(m, nAzimuth, r0, z0, z1, extentAzimuth.interval(), normalProto0, normalProto1, idSurface, false);
    }
    else
    {
        create_band(m, nAzimuth, r0, r1, z0, z1, extentAzimuth.interval(), extentAzimuth.interval(), normalProto0, normalProto1, tsProto0, tsProto1, idSurface, false);
    }

    for (size_type i = 2; i <= nElevation; ++i)
    {
        t = T(i) / T(nElevation);
        t1 = extentElevation.interval().lerp(t);
        theta1 = math::Pi<T> * (t1 - T(0.5));
        radius1 = fr(t1);
        r1 = radius1 * cos(theta1);
        z1 = radius1 * sin(theta1);
        normalProto1 = {r1, T(0), z1}; // ...
        tsProto1 = texture_span<T>(i, nElevation, bCuspLower, bCuspUpper);

        if (i == nElevation && bCuspUpper)
        {
            connect_apex_cusp(m, nAzimuth, z1, {T(0), T(0), T(1)}, idSurface);
        }
        else
        {
            connect_band(m, nAzimuth, r1, z1, extentAzimuth.interval(), normalProto1, tsProto1, idSurface, false, false);
        }
    }

    calculate_surface_normals(m);

    seal_sphere(m, nAzimuth, nElevation, bCuspLower, bCuspUpper, bOpenSide, extentAzimuth, extentElevation, idSubmesh);

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_sphere(M& mesh, const std::string& name, size_type nAzimuth, size_type nElevation, std::function<value_type<M>(value_type<M>, value_type<M>)> fr, const Extent<value_type<M>>& extentAzimuth, const Extent<value_type<M>>& extentElevation)
{
    // create_sphere ...
    id_type idSubmesh = nullid;
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_torus(M& mesh, const std::string& name, size_type nMajor, size_type nMinor, value_type<M> rMajor, value_type<M> rMinor, const Extent<value_type<M>>& extentMajor, const Extent<value_type<M>>& extentMinor)
{
    using T = typename M::value_type;

    assert(nMajor > 2);
    assert(nMinor > 2);
    assert(rMajor > T(0));
    assert(rMinor > T(0));
    assert(extentMajor.proper());
    assert(extentMinor.proper());

    // theta/major and phi/minor follow cylinder band orientation
    // phi measured from innermost points and with rotation reversed

    bool bOpenMinor = !extentMinor.full();
    bool bOpenMajor = !extentMajor.full();
    bool bCuspLower = math::float_eq0(rMinor);
    bool bCuspUpper = math::float_eq0(rMinor);

    M m;
    id_type idSubmesh = m.create_submesh(name);
    id_type idSurface = m.create_surface(idSubmesh, SurfaceName::Body);

    std::vector<typename M::vertex_attributes_type> avs0(nMinor + 1);
    std::vector<typename M::vertex_attributes_type> avs1(nMinor + 1);

    for (size_type i = 0; i <= nMajor; ++i)
    {
        T t = T(i) / T(nMajor);
//        T ty = T(1) - t;
        T ty = T(nMajor - i) / T(nMajor);
        T theta = math::PiTwo<T> * extentMajor.interval().lerp(t);

        if (i == nMajor && !bOpenMajor)
        {
            avs1 = avs0;
            for (size_type j = 0; j <= nMinor; ++j)
            {
                avs1[j].texcoord().set_y(T(0));
            }
        }
        else
        {
            T sxMajor = cos(theta);
            T syMajor = sin(theta);

            typename M::point_type center = {rMajor * sxMajor, rMajor * syMajor, T(0)};

            for (size_type j = 0; j <= nMinor; ++j)
            {
                T s = T(j) / T(nMinor);
                T phi =  math::PiTwo<T> * (math::lerp(extentMinor.interval().upper(), extentMinor.interval().lower(), s) - T(0.5));

                if (j == nMinor && !bOpenMinor)
                {
                    avs1[j] = avs1[0];
                    avs1[j].texcoord().set_x(T(1));
                }
                else
                {
                    T sxMinor = cos(phi);
                    T syMinor = sin(phi);

                    avs1[j].set_position({(rMajor + rMinor * sxMinor) * sxMajor, (rMajor + rMinor * sxMinor) * syMajor, rMinor * syMinor});
                    avs1[j].set_normal(normalize(avs1[j].position() - center));
                    avs1[j].set_texcoord({s, ty});
                }
            }
        }

        if (i == 0)
        {
            avs0 = avs1;
        }
        else if (i == 1)
        {
            create_band(m, avs0, avs1, texture_span_top<M>, texture_span_top<M>, !bOpenMinor, idSurface); // bottom? ...
        }
        else
        {
            connect_band(m, avs1, texture_span_top<M>, !bOpenMinor, idSurface); // testure_span? ...
        }
    }

    seal_torus(m, nMinor, nMajor, rMajor, bCuspLower, bCuspUpper, bOpenMinor, bOpenMajor, extentMinor, extentMajor, idSubmesh);

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_torus(M& mesh, const std::string& name, size_type nMajor, size_type nMinor, value_type<M> rMajorLower, value_type<M> rMajorUpper, value_type<M> rMinorLower, value_type<M> rMinorUpper, value_type<M> zLower, value_type<M> zUpper, const Extent<value_type<M>>& extentMajor, const Extent<value_type<M>>& extentMinor)
{
    using T = typename M::value_type;

    assert(nMajor > 2);
    assert(nMinor > 2);
    assert(rMajorLower >= T(0));
    assert(rMajorUpper >= T(0));
    assert(rMajorLower > T(0) || rMajorUpper > T(0));
    assert(rMinorLower >= T(0));
    assert(rMinorUpper >= T(0));
    assert(rMinorLower > T(0) || rMinorUpper > T(0));
    assert(extentMajor.proper());
    assert(extentMinor.proper());

    // theta/major and phi/minor follow cylinder band orientation
    // phi measured from innermost points and with rotation reversed

    bool bOpenMinor = !extentMinor.full();
    bool bOpenMajor = !extentMajor.full() || rMajorUpper != rMajorLower || rMinorUpper != rMinorLower || zUpper != zLower;
    bool bCuspLower = math::float_eq0(rMinorLower);
    bool bCuspUpper = math::float_eq0(rMinorUpper);

    M m;
    id_type idSubmesh = m.create_submesh(name);
    id_type idSurface = m.create_surface(idSubmesh, SurfaceName::Body);

    std::vector<typename M::vertex_attributes_type> avs0(nMinor + 1);
    std::vector<typename M::vertex_attributes_type> avs1(nMinor + 1);

    for (size_type i = 0; i <= nMajor; ++i)
    {
        T t = T(i) / T(nMajor);
//        T ty = T(1) - t;
        T ty = T(nMajor - i) / T(nMajor);
        T theta = math::PiTwo<T> * extentMajor.interval().lerp(t);
        T rMajor = math::lerp(rMajorLower, rMajorUpper, t);
        T rMinor = math::lerp(rMinorLower, rMinorUpper, t);
        T zCenter = math::lerp(zLower, zUpper, t);

        if (i == nMajor && !bOpenMajor)
        {
            avs1 = avs0;
            for (size_type j = 0; j <= nMinor; ++j)
            {
                avs1[j].texcoord().set_y(T(0));
            }
        }
        else
        {
            T sxMajor = cos(theta);
            T syMajor = sin(theta);

            typename M::point_type center = {rMajor * sxMajor, rMajor * syMajor, zCenter};

            for (size_type j = 0; j <= nMinor; ++j)
            {
                T s = T(j) / T(nMinor);
                T phi =  math::PiTwo<T> * (math::lerp(extentMinor.interval().upper(), extentMinor.interval().lower(), s) - T(0.5));

                if (j == nMinor && !bOpenMinor)
                {
                    avs1[j] = avs1[0];
                    avs1[j].texcoord().set_x(T(1));
                }
                else
                {
                    T sxMinor = cos(phi);
                    T syMinor = sin(phi);

                    avs1[j].set_position({center.x() + rMinor * sxMinor * sxMajor, center.y() + rMinor * sxMinor * syMajor, center.z() + rMinor * syMinor});
                    avs1[j].set_normal(normalize(avs1[j].position() - center));
                    avs1[j].set_texcoord({s, ty});
                }
            }
        }

        if (i == 0)
        {
            avs0 = avs1;
        }
        else if (i == 1)
        {
            create_band(m, avs0, avs1, !bOpenMinor, idSurface);
        }
        else
        {
            connect_band(m, avs1, !bOpenMinor, idSurface);
        }
    }

    // close avs since we don't have enough info here to close off side center ...
    // not a big deal except for normal calculations of internal faces ...
    seal_torus(m, nMinor, nMajor, (rMajorLower + rMajorUpper) / T(2), bCuspLower, bCuspUpper, bOpenMinor, bOpenMajor, extentMinor, extentMajor, idSubmesh); // ...

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_torus(M& mesh, const std::string& name, size_type nMajor, size_type nMinor, std::function<value_type<M>(value_type<M>, value_type<M>)> frMajor, std::function<value_type<M>(value_type<M>, value_type<M>)> frMinor, std::function<value_type<M>(value_type<M>, value_type<M>)> fz, const Extent<value_type<M>>& extentMajor, const Extent<value_type<M>>& extentMinor)
{
    using T = typename M::value_type;

    assert(nMajor > 2);
    assert(nMinor > 2);
    assert(extentMajor.proper());
    assert(extentMinor.proper());

    // theta/major and phi/minor follow cylinder band orientation
    // phi measured from innermost points and with rotation reversed

    bool bOpenMinor = !extentMinor.full();
//    bool bOpenMajor = !extentMajor.full() || rMajorUpper != rMajorLower || rMinorUpper != rMinorLower || zUpper != zLower;
//    bool bCuspLower = math::float_eq0(rMinorLower);
//    bool bCuspUpper = math::float_eq0(rMinorUpper);
    bool bOpenMajor = !extentMajor.full(); // ...
    bool bCuspLower = false;  // ...
    bool bCuspUpper = false;  // ...

    M m;
    id_type idSubmesh = m.create_submesh(name);
    id_type idSurface = m.create_surface(idSubmesh, SurfaceName::Body);

    std::vector<typename M::vertex_attributes_type> avs0(nMinor + 1);
    std::vector<typename M::vertex_attributes_type> avs1(nMinor + 1);

    for (size_type i = 0; i <= nMajor; ++i)
    {
        T t = T(i) / T(nMajor);
//        T ty = T(1) - t;
        T ty = T(nMajor - i) / T(nMajor);
        T theta = math::PiTwo<T> * extentMajor.interval().lerp(t);

        if (i == nMajor && !bOpenMajor)
        {
            avs1 = avs0;
            for (size_type j = 0; j <= nMinor; ++j)
            {
                avs1[j].texcoord().set_y(T(0));
            }
        }
        else
        {
            T sxMajor = cos(theta);
            T syMajor = sin(theta);

            for (size_type j = 0; j <= nMinor; ++j)
            {
                T s = T(j) / T(nMinor);
                T phi =  math::PiTwo<T> * (math::lerp(extentMinor.interval().upper(), extentMinor.interval().lower(), s) - T(0.5));

                T rMajor = frMajor(t, s);
                T rMinor = frMinor(t, s);
                T zCenter = fz(t, s);

                if (j == nMinor && !bOpenMinor)
                {
                    avs1[j] = avs1[0];
                    avs1[j].texcoord().set_x(T(1));
                }
                else
                {
                    T sxMinor = cos(phi);
                    T syMinor = sin(phi);

                    typename M::point_type center = {rMajor * sxMajor, rMajor * syMajor, zCenter};

                    avs1[j].set_position({center.x() + rMinor * sxMinor * sxMajor, center.y() + rMinor * sxMinor * syMajor, center.z() + rMinor * syMinor});
                    avs1[j].set_normal(normalize(avs1[j].position() - center));
                    avs1[j].set_texcoord({s, ty});
                }
            }
        }

        if (i == 0)
        {
            avs0 = avs1;
        }
        else if (i == 1)
        {
            create_band(m, avs0, avs1, !bOpenMinor, idSurface);
        }
        else
        {
            connect_band(m, avs1, !bOpenMinor, idSurface);
        }
    }

    // close avs since we don't have enough info here to close off side center ...
    // not a big deal except for normal calculations of internal faces ...
    seal_torus(m, nMinor, nMajor, fr(T(0.5), T(0.5)), bCuspLower, bCuspUpper, bOpenMinor, bOpenMajor, extentMinor, extentMajor, idSubmesh); // ...

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_tetrahedron(M& mesh, const std::string& name, value_type<M> radius)
{
    using T = typename M::value_type;

    assert(radius > T(0));

    // a = 4 / sqrt(6); edge length of unit tetrahedron
    T x1 = radius * sqrt(T(2)) / T(3); // radius * a * sqrt(3) / 6;
    T y1 = radius * sqrt(T(6)) / T(3); // radius * a / 2;
    T z1 = radius / T(3);

    typename M::point_type position0 = {T(2) * x1, T(0), -z1};
    typename M::point_type position1 = {-x1, y1, -z1};
    typename M::point_type position2 = {-x1, -y1, -z1};
    typename M::point_type position3 = {T(0), T(0), radius};

    typename M::vector_type normal0 = normalize(cross(position2 - position0, position1 - position0));
    typename M::vector_type normal1 = normalize(cross(position3 - position1, position0 - position1));
    typename M::vector_type normal2 = normalize(cross(position3 - position2, position1 - position2));
    typename M::vector_type normal3 = normalize(cross(position2 - position3, position0 - position3));

    T yt = T(1) - sqrt(T(3)) / T(2);

    std::vector<typename M::vertex_attributes_type> avs =
    {
        {position2, normal0, {T(0), T(1)}},
        {position1, normal0, {T(1), T(1)}},
        {position0, normal0, {T(0.5), yt}},

        {position0, normal1, {T(0), T(1)}},
        {position1, normal1, {T(1), T(1)}},
        {position3, normal1, {T(0.5), yt}},

        {position1, normal2, {T(0), T(1)}},
        {position2, normal2, {T(1), T(1)}},
        {position3, normal2, {T(0.5), yt}},

        {position2, normal3, {T(0), T(1)}},
        {position0, normal3, {T(1), T(1)}},
        {position3, normal3, {T(0.5), yt}},
    };

    M m;
    id_type idSubmesh = m.create_submesh(name);

    m.create_halfedge(6, 1, 2, 0, 0);
    m.create_halfedge(3, 2, 0, 1, 0);
    m.create_halfedge(9, 0, 1, 2, 0);

    m.create_halfedge(1, 4, 5, 3, 1);
    m.create_halfedge(8, 5, 3, 4, 1);
    m.create_halfedge(10, 3, 4, 5, 1);

    m.create_halfedge(0, 7, 8, 6, 2);
    m.create_halfedge(11, 8, 6, 7, 2);
    m.create_halfedge(4, 6, 7, 8, 2);

    m.create_halfedge(2, 10, 11, 9, 3);
    m.create_halfedge(5, 11, 9, 10, 3);
    m.create_halfedge(7, 9, 10, 11, 3);

    for (size_t i = 0; i < m.halfedge_count(); ++i) // One vertex per halfedge
    {
        m.create_vertex(i, avs[i]);
    }

    for (size_t i = 0; i < 4; ++i)
    {
        typename M::vector_type normal = avs[3 * i].normal();
        id_type idSurface = m.create_surface(idSubmesh, SurfaceName::BodySection + "_" + to_string(i), {normal});
        m.create_face(idSurface, 3 * i, {normal});
    }

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_octahedron(M& mesh, const std::string& name, value_type<M> radius, bool bVertex)
{
    using T = typename M::value_type;

    assert(radius > T(0));

    std::vector<typename M::vertex_attributes_type> avs;

    if (bVertex)
    {
        T n0 = T(1) / sqrt(T(3));

        avs =
        {
            {{T(0), T(0), -radius}, {n0, n0, -n0}, {T(0.5), T(0)}},
            {{T(0), radius, T(0)}, {n0, n0, -n0}, {T(0), T(1)}},
            {{radius, T(0), T(0)}, {n0, n0, -n0}, {T(1), T(1)}},

            {{T(0), T(0), -radius}, {-n0, n0, -n0}, {T(0.5), T(0)}},
            {{-radius, T(0), T(0)}, {-n0, n0, -n0}, {T(0), T(1)}},
            {{T(0), radius, T(0)}, {-n0, n0, -n0}, {T(1), T(1)}},

            {{T(0), T(0), -radius}, {-n0, -n0, -n0}, {T(0.5), T(0)}},
            {{T(0), -radius, T(0)}, {-n0, -n0, -n0}, {T(0), T(1)}},
            {{-radius, T(0), T(0)}, {-n0, -n0, -n0}, {T(1), T(1)}},

            {{T(0), T(0), -radius}, {n0, -n0, -n0}, {T(0.5), T(0)}},
            {{radius, T(0), T(0)}, {n0, -n0, -n0}, {T(0), T(1)}},
            {{T(0), -radius, T(0)}, {n0, -n0, -n0}, {T(1), T(1)}},

            {{T(0), T(0), radius}, {n0, n0, n0}, {T(0.5), T(0)}},
            {{radius, T(0), T(0)}, {n0, n0, n0}, {T(0), T(1)}},
            {{T(0), radius, T(0)}, {n0, n0, n0}, {T(1), T(1)}},

            {{T(0), T(0), radius}, {-n0, n0, n0}, {T(0.5), T(0)}},
            {{T(0), radius, T(0)}, {-n0, n0, n0}, {T(0), T(1)}},
            {{-radius, T(0), T(0)}, {-n0, n0, n0}, {T(1), T(1)}},

            {{T(0), T(0), radius}, {-n0, -n0, n0}, {T(0.5), T(0)}},
            {{-radius, T(0), T(0)}, {-n0, -n0, n0}, {T(0), T(1)}},
            {{T(0), -radius, T(0)}, {-n0, -n0, n0}, {T(1), T(1)}},

            {{T(0), T(0), radius}, {n0, -n0, n0}, {T(0.5), T(0)}},
            {{T(0), -radius, T(0)}, {n0, -n0, n0}, {T(0), T(1)}},
            {{radius, T(0), T(0)}, {n0, -n0, n0}, {T(1), T(1)}},
        };
    }
    else
    {
        // a = sqrt(2); edge length of unit octahedron
        T x1 = radius / sqrt(T(6)); // radius * a * sqrt(3) / 6;
        T x2 = radius * sqrt(T(6)) / T(3); // radius * a * sqrt(3) / 3;
        T y1 = radius / sqrt(T(2)); // radius * a / 2;
        T z1 = radius / sqrt(T(3)); // radius * a / sqrt(6);

        T xn1 = sqrt(T(2)) / T(3);
        T xn2 = xn1 * T(2);
        T yn = sqrt(T(2) / T(3));
        T zn = T(1) / T(3);

        avs =
        {
            // Bottom face
            {{x2, T(0), -z1}, {T(0), T(0), T(-1)}, {T(0.5), T(0)}},
            {{-x1, -y1, -z1}, {T(0), T(0), T(-1)}, {T(0), T(1)}},
            {{-x1, y1, -z1}, {T(0), T(0), T(-1)}, {T(1), T(1)}},

            {{x2, T(0), -z1}, {xn1, -yn, -zn}, {T(1), T(1)}},
            {{x1, -y1, z1}, {xn1, -yn, -zn}, {T(0.5), T(0)}},
            {{-x1, -y1, -z1}, {xn1, -yn, -zn}, {T(0), T(1)}},

            {{x2, T(0), -z1}, {xn2, T(0), zn}, {T(0.5), T(0)}},
            {{x1, y1, z1}, {xn2, T(0), zn}, {T(0), T(1)}},
            {{x1, -y1, z1}, {xn2, T(0), zn}, {T(1), T(1)}},

            {{x2, T(0), -z1}, {xn1, yn, -zn}, {T(0), T(1)}},
            {{-x1, y1, -z1}, {xn1, yn, -zn}, {T(1), T(1)}},
            {{x1, y1, z1}, {xn1, yn, -zn}, {T(0.5), T(0)}},

            {{-x2, T(0), z1}, {-xn2, T(0), -zn}, {T(0.5), T(0)}},
            {{-x1, y1, -z1}, {-xn2, T(0), -zn}, {T(0), T(1)}},
            {{-x1, -y1, -z1}, {-xn2, T(0), -zn}, {T(1), T(1)}},

            {{-x2, T(0), z1}, {-xn1, -yn, zn}, {T(1), T(1)}},
            {{-x1, -y1, -z1}, {-xn1, -yn, zn}, {T(0.5), T(0)}},
            {{x1, -y1, z1}, {-xn1, -yn, zn}, {T(0), T(1)}},

            // Top face                            
            {{-x2, T(0), z1}, {T(0), T(0), T(1)}, {T(0.5), T(0)}}, 
            {{x1, -y1, z1}, {T(0), T(0), T(1)}, {T(0), T(1)}}, 
            {{x1, y1, z1}, {T(0), T(0), T(1)}, {T(1), T(1)}},
                                                   
            {{-x2, T(0), z1}, {-xn1, yn, zn}, {T(0), T(1)}}, 
            {{x1, y1, z1}, {-xn1, yn, zn}, {T(1), T(1)}}, 
            {{-x1, y1, -z1}, {-xn1, yn, zn}, {T(0.5), T(0)}},
        };
    }

    M m;
    id_type idSubmesh = m.create_submesh(name);

    m.create_halfedge(5, 1, 2, 0, 0);
    m.create_halfedge(13, 2, 0, 1, 0);
    m.create_halfedge(9, 0, 1, 2, 0);

    m.create_halfedge(8, 4, 5, 3, 1);
    m.create_halfedge(16, 5, 3, 4, 1);
    m.create_halfedge(0, 3, 4, 5, 1);

    m.create_halfedge(11, 7, 8, 6, 2);
    m.create_halfedge(19, 8, 6, 7, 2);
    m.create_halfedge(3, 6, 7, 8, 2);

    m.create_halfedge(2, 10, 11, 9, 3);
    m.create_halfedge(22, 11, 9, 10, 3);
    m.create_halfedge(6, 9, 10, 11, 3);

    m.create_halfedge(23, 13, 14, 12, 4);
    m.create_halfedge(1, 14, 12, 13, 4);
    m.create_halfedge(15, 12, 13, 14, 4);

    m.create_halfedge(14, 16, 17, 15, 5);
    m.create_halfedge(4, 17, 15, 16, 5);
    m.create_halfedge(18, 15, 16, 17, 5);

    m.create_halfedge(17, 19, 20, 18, 6);
    m.create_halfedge(7, 20, 18, 19, 6);
    m.create_halfedge(21, 18, 19, 20, 6);

    m.create_halfedge(20, 22, 23, 21, 7);
    m.create_halfedge(10, 23, 21, 22, 7);
    m.create_halfedge(12, 21, 22, 23, 7);

    for (size_t i = 0; i < m.halfedge_count(); ++i) // One vertex per halfedge
    {
        m.create_vertex(i, avs[i]);
    }

    for (size_t i = 0; i < 8; ++i)
    {
        typename M::vector_type normal = avs[3 * i].normal();
        id_type idSurface = m.create_surface(idSubmesh, SurfaceName::BodySection + "_" + to_string(i), {normal});
        m.create_face(idSurface, 3 * i, {normal});
    }

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_dodecahedron(M& mesh, const std::string& name, value_type<M> radius)
{
    using T = typename M::value_type;

    assert(radius > T(0));

    // Pentagonal face parameters
    T gr = math::GoldenRatio<T>;
    T a = radius * T(2) / (sqrt(T(3)) * gr);
    T d = radius * T(2) / sqrt(T(3));
    T ri = a / (T(2) * sqrt(T(5) - sqrt(T(20)))); // inscribed radius, a * .68819
    T rc = a * sqrt(T(50) + T(10) * sqrt(T(5))) / T(10); // circumscribed radius, a * .85065

    // Dodecahedron face parameters
    T x1 = radius * T(0.6070620); // rc
    T x2 = radius * T(0.1875925);
    T x3 = radius * T(0.4911235); // ri
    T x4 = radius * T(0.9822469); // 2 * ri
    T x5 = radius * T(0.3035310); // rc / 2
    T x6 = radius * T(0.7946545);
    T y1 = radius * T(0.3568221); // a / 2
    T y2 = radius * T(0.5773503); // d / 2
    T y3 = radius * T(0.9341724);
    T z1 = radius * T(0.1875925);
    T z2 = radius * T(0.7946545);

    const std::vector<typename M::point_type> positions =
    {
        {x1, T(0), -z2},
        {x2, y2, -z2},
        {-x3, y1, -z2},
        {-x3, -y1, -z2},
        {x2, -y2, -z2},
        {x4, T(0), -z1},
        {x5, y3, -z1},
        {-x6, y2, -z1},
        {-x6, -y2, -z1},
        {x5, -y3, -z1},
        {x6, y2, z1},
        {-x5, y3, z1},
        {-x4, T(0), z1},
        {-x5, -y3, z1},
        {x6, -y2, z1},
        {x3, y1, z2},
        {-x2, y2, z2},
        {-x1, T(0), z2},
        {-x2, -y2, z2},
        {x3, -y1, z2}
    };

    std::vector<id_type> vertex_position_ids =
    {
        0, 4, 3, 2, 1,
        0, 1, 6, 10, 5,
        1, 2, 7, 11, 6,
        2, 3, 8, 12, 7,
        3, 4, 9, 13, 8,
        4, 0, 5, 14, 9,
        5, 10, 15, 19, 14,
        6, 11, 16, 15, 10,
        7, 12, 17, 16, 11,
        8, 13, 18, 17, 12,
        9, 14, 19, 18, 13,
        15, 16, 17, 18, 19
    };

    T xt2 = a / (T(2) * d);
    T yt1 = T(1) - (ri + rc) / d;
    T ah = a * sqrt(T(4) - gr * gr) / (T(2) * d);
    T yt2 = yt1 + ah;

    const std::vector<typename M::texcoord_type> texcoords =
    {
        {T(0.5), yt1},
        {T(0), yt2},
        {T(0.5) - xt2, T(1)},
        {T(0.5) + xt2, T(1)},
        {T(1), yt2}
    };

    M m;
    id_type idSubmesh = m.create_submesh(name);

    m.create_halfedge(25,  1,  4,  0, 0);
    m.create_halfedge(20,  2,  0,  1, 0);
    m.create_halfedge(15,  3,  1,  2, 0);
    m.create_halfedge(10,  4,  2,  3, 0);
    m.create_halfedge( 5,  0,  3,  4, 0);

    m.create_halfedge( 4,  6,  9,  5, 1);
    m.create_halfedge(14,  7,  5,  6, 1);
    m.create_halfedge(39,  8,  6,  7, 1);
    m.create_halfedge(30,  9,  7,  8, 1);
    m.create_halfedge(26,  5,  8,  9, 1);

    m.create_halfedge( 3, 11, 14, 10, 2);
    m.create_halfedge(19, 12, 10, 11, 2);
    m.create_halfedge(44, 13, 11, 12, 2);
    m.create_halfedge(35, 14, 12, 13, 2);
    m.create_halfedge( 6, 10, 13, 14, 2);

    m.create_halfedge( 2, 16, 19, 15, 3);
    m.create_halfedge(24, 17, 15, 16, 3);
    m.create_halfedge(49, 18, 16, 17, 3);
    m.create_halfedge(40, 19, 17, 18, 3);
    m.create_halfedge(11, 15, 18, 19, 3);

    m.create_halfedge( 1, 21, 24, 20, 4);
    m.create_halfedge(29, 22, 20, 21, 4);
    m.create_halfedge(54, 23, 21, 22, 4);
    m.create_halfedge(45, 24, 22, 23, 4);
    m.create_halfedge(16, 20, 23, 24, 4);

    m.create_halfedge( 0, 26, 29, 25, 5);
    m.create_halfedge( 9, 27, 25, 26, 5);
    m.create_halfedge(34, 28, 26, 27, 5);
    m.create_halfedge(50, 29, 27, 28, 5);
    m.create_halfedge(21, 25, 28, 29, 5);

    m.create_halfedge( 8, 31, 34, 30, 6);
    m.create_halfedge(38, 32, 30, 31, 6);
    m.create_halfedge(59, 33, 31, 32, 6);
    m.create_halfedge(51, 34, 32, 33, 6);
    m.create_halfedge(27, 30, 33, 34, 6);

    m.create_halfedge(13, 36, 39, 35, 7);
    m.create_halfedge(43, 37, 35, 36, 7);
    m.create_halfedge(55, 38, 36, 37, 7);
    m.create_halfedge(31, 39, 37, 38, 7);
    m.create_halfedge( 7, 35, 38, 39, 7);

    m.create_halfedge(18, 41, 44, 40, 8);
    m.create_halfedge(48, 42, 40, 41, 8);
    m.create_halfedge(56, 43, 41, 42, 8);
    m.create_halfedge(36, 44, 42, 43, 8);
    m.create_halfedge(12, 40, 43, 44, 8);

    m.create_halfedge(23, 46, 49, 45, 9);
    m.create_halfedge(53, 47, 45, 46, 9);
    m.create_halfedge(57, 48, 46, 47, 9);
    m.create_halfedge(41, 49, 47, 48, 9);
    m.create_halfedge(17, 45, 48, 49, 9);

    m.create_halfedge(28, 51, 54, 50, 10);
    m.create_halfedge(33, 52, 50, 51, 10);
    m.create_halfedge(58, 53, 51, 52, 10);
    m.create_halfedge(46, 54, 52, 53, 10);
    m.create_halfedge(22, 50, 53, 54, 10);

    m.create_halfedge(37, 56, 59, 55, 11);
    m.create_halfedge(42, 57, 55, 56, 11);
    m.create_halfedge(47, 58, 56, 57, 11);
    m.create_halfedge(52, 59, 57, 58, 11);
    m.create_halfedge(32, 55, 58, 59, 11);

    for (size_t i = 0; i < m.halfedge_count(); ++i)
    {
        typename M::vertex_attributes_type attributes {positions[vertex_position_ids[i]], {}, texcoords[i % 5]};
        m.create_vertex(i, attributes);
    }

    for (size_t i = 0; i < 12; ++i)
    {
        id_type idSurface = m.create_surface(idSubmesh, SurfaceName::BodySection + "_" + to_string(i));
        id_type idFace = m.create_face(idSurface, 5 * i);
        auto& face = m.face(idFace);

        typename M::vector_type normal = normalize(centroid(face));
        face.attributes().set_normal(normal);
        m.surface(idSurface).attributes().set_normal(normal);
        for (auto& halfedge : face.halfedges())
        {
            halfedge.attributes().set_normal(normal);
        }
    }

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_icosahedron(M& mesh, const std::string& name, value_type<M> radius, bool bVertex, bool bSmooth)
{
    using T = typename M::value_type;

    assert(radius > T(0));

    std::vector<typename M::point_type> positions;
    std::vector<id_type> vertex_position_ids;

    M m;
    id_type idSubmesh = m.create_submesh(name);

    bool bFace = !bVertex;

    // a = 1.0514622242382672120513381696958
    // gr = 1.6180339887498948482045868343656

    if (bVertex)
    {
        // Vertices
        // a = 4 / sqrt(10 + 2 * sqrt(5)); edge length of unit icosahedron
        // gr = (1 + sqrt(5)) / 2
        T x0 = radius * T(0.89442719099991587856366946749247); // x0 + x2 = gr
        T x1 = radius * T(0.27639320225002103035908263312687); // x1 + x2 = 1
        T x2 = radius * T(0.72360679774997896964091736687313); // x1 = x0 - gr + 1
        T y1 = radius * T(0.85065080835203993218154049706299); // a * gr / 2
        T y2 = radius * T(0.5257311121191336060256690848479); // a / 2
        T z1 = radius * T(0.44721359549995793928183473374624); // x0 / 2
        T z2 = radius;

        positions =
        {
            {T(0), T(0), -z2},
            {x2, y2, -z1},
            {-x1, y1, -z1},
            {-x0, T(0), -z1},
            {-x1, -y1, -z1},
            {x2, -y2, -z1},
            {x0, T(0), z1},
            {x1, y1, z1},
            {-x2, y2, z1},
            {-x2, -y2, z1},
            {x1, -y1, z1},
            {T(0), T(0), z2},
        };
    }
    else if (bFace)
    {
        // Vertices
        // a = 4 / sqrt(10 + 2 * sqrt(5)); edge length of unit icosahedron
        T x1 = radius * T(0.30353099910334311154769579071001); // a * sqrt(3) / 6
        T x2 = radius * T(0.60706199820668622309539158142001); // a * sqrt(3) / 3
        T x3 = radius * T(0.49112347318842301140783513761761); // x1 * gr
        T x4 = radius * T(0.98224694637684602281567027523522); // x2 * gr
        T y1 = radius * T(0.85065080835203993218154049706299); // a * gr / 2
        T y2 = radius * T(0.5257311121191336060256690848479); // a / 2
        T z1 = radius * T(0.18759247408507989986013934690762); // x1 / gr
        T z2 = radius * T(0.79465447229176612295553092832763);

        positions =
        {
            {-x2, T(0), -z2},
            {x1, y2, -z2},
            {-x3, y1, -z1},
            {-x4, T(0), z1},
            {-x3, -y1, -z1},
            {x1, -y2, -z2},
            {x4, T(0), -z1},
            {x3, y1, z1},
            {-x1, y2, z2},
            {-x1, -y2, z2},
            {x3, -y1, z1},
            {x2, T(0), z2},
        };
    }
    else // bEdge
    {
/*
        const T gr = (T(1) + sqrt(T(5))) / T(2);
        const T r = sqrt((T(5) + sqrt(T(5))) / T(2));
        const T n1 = T(1) / r;
        const T ngr = gr / r;

        positions =
        {
            {{T(0), n1, ngr}, {T(0), n1, ngr}},
            {{T(0), -n1, ngr}, {T(0), -n1, ngr}},
            {{ngr, T(0), n1}, {ngr, T(0), n1}},
            {{-ngr, T(0), n1}, {-ngr, T(0), n1}},
            {{n1, ngr, T(0)}, {n1, ngr, T(0)}},
            {{-n1, ngr, T(0)}, {-n1, ngr, T(0)}},
            {{n1, -ngr, T(0)}, {n1, -ngr, T(0)}},
            {{-n1, -ngr, T(0)}, {-n1, -ngr, T(0)}},
            {{ngr, T(0), -n1}, {ngr, T(0), -n1}},
            {{-ngr, T(0), -n1}, {-ngr, T(0), -n1}},
            {{T(0), n1, -ngr}, {T(0), n1, -ngr}},
            {{T(0), -n1, -ngr}, {T(0), -n1, -ngr}},
        };

        indices =
        {
             0, 1, 2,
             0, 3, 1,
             0, 5, 3,
             0, 4, 5,
             0, 2, 4,
             2, 6, 8,
             2, 1, 6,
             1, 7, 6,
             1, 3, 7,
             3, 9, 7,
             3, 5, 9,
             5, 10, 9,
             5, 4, 10,
             4, 8, 10,
             4, 2, 8,
             8, 6, 11,
             6, 7, 11,
             7, 9, 11,
             9, 10, 11,
            10, 8, 11
        };
*/
    }

    vertex_position_ids =
    {
        0, 1, 5,
        0, 2, 1,
        0, 3, 2,
        0, 4, 3,
        0, 5, 4,
        1, 6, 5,
        2, 7, 1,
        3, 8, 2,
        4, 9, 3,
        5, 10, 4,
        6, 1, 7,
        7, 2, 8,
        8, 3, 9,
        9, 4, 10,
        10, 5, 6,
        11, 6, 7,
        11, 7, 8,
        11, 8, 9,
        11, 9, 10,
        11, 10, 6
    };

    const std::vector<typename M::texcoord_type> texcoords =
    {
        {T(0.5), T(1) - sqrt(T(3)) / T(2)},
        {T(0), T(1)},
        {T(1), T(1)}
    };

    m.create_halfedge( 5,  1,  2,  0, 0);
    m.create_halfedge(17,  2,  0,  1, 0);
    m.create_halfedge(12,  0,  1,  2, 0);

    m.create_halfedge( 8,  4,  5,  3, 1);
    m.create_halfedge(20,  5,  3,  4, 1);
    m.create_halfedge( 0,  3,  4,  5, 1);

    m.create_halfedge(11,  7,  8,  6, 2);
    m.create_halfedge(23,  8,  6,  7, 2);
    m.create_halfedge( 3,  6,  7,  8, 2);

    m.create_halfedge(14, 10, 11,  9, 3);
    m.create_halfedge(26, 11,  9, 10, 3);
    m.create_halfedge( 6,  9, 10, 11, 3);

    m.create_halfedge( 2, 13, 14, 12, 4);
    m.create_halfedge(29, 14, 12, 13, 4);
    m.create_halfedge( 9, 12, 13, 14, 4);

    m.create_halfedge(30, 16, 17, 15, 5);
    m.create_halfedge(43, 17, 15, 16, 5);
    m.create_halfedge( 1, 15, 16, 17, 5);

    m.create_halfedge(33, 19, 20, 18, 6);
    m.create_halfedge(31, 20, 18, 19, 6);
    m.create_halfedge( 4, 18, 19, 20, 6);

    m.create_halfedge(36, 22, 23, 21, 7);
    m.create_halfedge(34, 23, 21, 22, 7);
    m.create_halfedge( 7, 21, 22, 23, 7);

    m.create_halfedge(39, 25, 26, 24, 8);
    m.create_halfedge(37, 26, 24, 25, 8);
    m.create_halfedge(10, 24, 25, 26, 8);

    m.create_halfedge(42, 28, 29, 27, 9);
    m.create_halfedge(40, 29, 27, 28, 9);
    m.create_halfedge(13, 27, 28, 29, 9);

    m.create_halfedge(15, 31, 32, 30, 10);
    m.create_halfedge(19, 32, 30, 31, 10);
    m.create_halfedge(46, 30, 31, 32, 10);

    m.create_halfedge(18, 34, 35, 33, 11);
    m.create_halfedge(22, 35, 33, 34, 11);
    m.create_halfedge(49, 33, 34, 35, 11);

    m.create_halfedge(21, 37, 38, 36, 12);
    m.create_halfedge(25, 38, 36, 37, 12);
    m.create_halfedge(52, 36, 37, 38, 12);

    m.create_halfedge(24, 40, 41, 39, 13);
    m.create_halfedge(28, 41, 39, 40, 13);
    m.create_halfedge(55, 39, 40, 41, 13);

    m.create_halfedge(27, 43, 44, 42, 14);
    m.create_halfedge(16, 44, 42, 43, 14);
    m.create_halfedge(58, 42, 43, 44, 14);

    m.create_halfedge(59, 46, 47, 45, 15);
    m.create_halfedge(32, 47, 45, 46, 15);
    m.create_halfedge(48, 45, 46, 47, 15);

    m.create_halfedge(47, 49, 50, 48, 16);
    m.create_halfedge(35, 50, 48, 49, 16);
    m.create_halfedge(51, 48, 49, 50, 16);

    m.create_halfedge(50, 52, 53, 51, 17);
    m.create_halfedge(38, 53, 51, 52, 17);
    m.create_halfedge(54, 51, 52, 53, 17);

    m.create_halfedge(53, 55, 56, 54, 18);
    m.create_halfedge(41, 56, 54, 55, 18);
    m.create_halfedge(57, 54, 55, 56, 18);

    m.create_halfedge(56, 58, 59, 57, 19);
    m.create_halfedge(44, 59, 57, 58, 19);
    m.create_halfedge(45, 57, 58, 59, 19);

    for (size_t i = 0; i < m.halfedge_count(); ++i)
    {
        typename M::point_type position = positions[vertex_position_ids[i]];
        typename M::vector_type normal;
        if (bSmooth)
        {
            normal = normalize(position);
        }

        typename M::vertex_attributes_type attributes {position, normal, texcoords[i % 3]};
        m.create_vertex(i, attributes);
    }

    id_type idSurface = bSmooth ? m.create_surface(idSubmesh, SurfaceName::Body) : nullid;
    for (size_t i = 0; i < 20; ++i)
    {
        if (!bSmooth)
        {
            idSurface = m.create_surface(idSubmesh, SurfaceName::BodySection + "_" + to_string(i));
        }

        id_type idFace = m.create_face(idSurface, 3 * i);
        auto& face = m.face(idFace);

        typename M::vector_type normal = normalize(centroid(face));
        face.attributes().set_normal(normal);

        if (!bSmooth)
        {
            for (auto& halfedge : face.halfedges())
            {
                halfedge.attributes().set_normal(normal);
            }

            m.surface(idSurface).attributes().set_normal(normal);
        }
    }

    if (bSmooth)
    {
        calculate_spherical_normals(m);
        // spherical texture coordinates ...
    }

    m.check();
    assert(check_spherical(m, radius));
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_geodesic_sphere(M& mesh, const std::string& name, value_type<M> radius, size_type nSubdivisions, bool bVertex, bool bSmooth)
{
    using T = typename M::value_type;

    assert(radius > T(0));
    assert(nSubdivisions > 0);

    M m;

    // Approximate a sphere by tessellating an icosahedron

    create_icosahedron(m, name, radius, bVertex, bSmooth);

    if (nSubdivisions > 1)
    {
        auto nFacesOrig = m.faces().size();
        for (size_t i = 0; i < nFacesOrig; ++i)
        {
            // Split all unmarked edges
            id_type idHalfedge0 = m.face(i).halfedge_id();
            id_type idHalfedge = idHalfedge0;
            do
            {
                id_type idHalfedgeNext = m.halfedge(idHalfedge).next_id();

                // Split unmarked edge
                if (!m.halfedge(idHalfedge).marked())
                {
                    split_edge(m, idHalfedge, nSubdivisions);

                    // Mark partners of this edge's segments so that adjacent face edges will not be split
                    id_type idHalfedgeEdge = idHalfedge;
                    for (int j = 0; j < nSubdivisions; ++j)
                    {
                        m.halfedge(idHalfedgeEdge).partner().set_marked();
                        idHalfedgeEdge = m.halfedge(idHalfedgeEdge).next_id();
                    }
                }

                idHalfedge = idHalfedgeNext;
            } while (idHalfedge != idHalfedge0);

            // Recursively triangulate by row
            triangulate_rows(m, m.face(i).halfedge_id(), nSubdivisions);
        }
    }

    project_vertices(m.vertices().begin(), m.vertices().end(), radius);
    if (bSmooth)
    {
        calculate_spherical_normals(m);
        // spherical texture coordinates, with smooth icosahedron, splits should produce mostly correct texture coordinates ...
        m.remove_surfaces();
        for (auto& face : m.faces())
        {
            m.add_surface_face(face.submesh_id(), SurfaceName::Body, face.id());
        }
    }
    else
    {
        calculate_spherical_face_normals(m);
        apply_triangular_face_texcoords(m);
        add_face_surfaces(m, SurfaceName::BodySection + "_");

        for (auto& face : m.faces())
        {
            m.surface(face.surface_id()).attributes().set_normal(face.attributes().normal());
        }
    }

    m.check();
    assert(check_spherical(m, radius));
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_extrusion(M& mesh, const std::string& name, const geometry::Polygon<typename M::vector_traits>& polygon, const typename M::vector_type& normal, const typename M::vector_type& displacement, const Extent<value_type<M>>& extentZ)
{
    using T = typename M::value_type;

    size_type nAzimuth = polygon.edge_count();
    assert(nAzimuth > 2);

    using point_type = typename geometry::Polygon<M::vector_traits>::point_type;

    std::vector<typename M::vertex_attributes_type> avs0 = vertices_attributes<M>(polygon, T(1));

for (const auto& av : avs0)
{
const auto& texcoord = av.texcoord();
assert(texcoord.y() == T(0) || texcoord.y() == T(1));
}

    geometry::Polygon<typename M::vector_traits> polygon1;
    std::transform(polygon.vertices().begin(), polygon.vertices().end(), std::back_inserter(polygon1.vertices()), [displacement](const point_type& point) -> point_type { return point + displacement; });
    std::vector<typename M::vertex_attributes_type> avs1 = vertices_attributes<M>(polygon1, T(0));

for (const auto& av : avs1)
{
const auto& texcoord = av.texcoord();
assert(texcoord.y() == T(0) || texcoord.y() == T(1));
}

    M m;
    id_type idSubmesh = m.create_submesh(name);

    id_type idSurface0 = m.surface_store_count();
    for (size_t i = 0; i < nAzimuth; ++i)
    {
        m.create_surface(idSubmesh, SurfaceName::BodySection + "_" + to_string(i));
    }

    auto tsProto0 = texture_span<T>(0, 1, false, false);
    auto tsProto1 = texture_span<T>(1, 1, false, false);

    create_band(m, avs0, avs1, tsProto0, tsProto1, true, idSurface0, true);
for (const auto& face : m.faces())
{
for (const auto& halfedge : face.halfedges())
{
const auto& texcoord = halfedge.attributes().texcoord();
assert(texcoord.y() == T(0) || texcoord.y() == T(1));
}
}

    for (size_t i = 0; i < nAzimuth; ++i)
    {
        m.surface(i).attributes().set_normal(m.face(i).attributes().normal());
    }

    id_type idLowerFirst = 0;
    id_type idUpperFirst = m.halfedge_store_count() - 2 * nAzimuth;

    if (extentZ.lower_termination().type == Termination::Type::Flat)
    {
        id_type idSurface = m.create_surface(idSubmesh, extentZ.lower_name(), {-normal});
        create_border_face(m, idLowerFirst, -normal, idSurface);
    }
for (const auto& face : m.faces())
{
if (face.vertex_count() == 4)
{
for (const auto& halfedge : face.halfedges())
{
const auto& texcoord = halfedge.attributes().texcoord();
assert(texcoord.y() == T(0) || texcoord.y() == T(1));
}
}
}

    if (extentZ.upper_termination().type == Termination::Type::Flat)
    {
        id_type idSurface = m.create_surface(idSubmesh, extentZ.upper_name(), {normal});
        create_border_face(m, idUpperFirst, normal, idSurface);
    }

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_extrusion(M& mesh, const std::string& name, const geometry::PolygonWithHoles<typename M::vector_traits>& polygons, const typename M::vector_type& normal, const typename M::vector_type& displacement, const Extent<value_type<M>>& extentZ)
{
    using T = typename M::value_type;

    assert(extentZ.proper());

    using point_type = typename geometry::Polygon<typename M::vector_traits>::point_type;

    std::vector<typename M::vertex_attributes_type> avs0 = vertices_attributes<M>(polygons.polygon(), T(1));

    geometry::Polygon<typename M::vector_traits> polygon1;
    std::transform(polygons.polygon().vertices().begin(), polygons.polygon().vertices().end(), std::back_inserter(polygon1.vertices()), [displacement](const point_type& point) -> point_type { return point + displacement; });
    std::vector<typename M::vertex_attributes_type> avs1 = vertices_attributes<M>(polygon1, T(0));

    M m;
    id_type idSubmesh = m.create_submesh(name);
    id_type idSurface0 = mesh.surface_count();

    auto tsProto0 = texture_span<T>(0, 1, false, false);
    auto tsProto1 = texture_span<T>(1, 1, false, false);

    id_type idSurface = m.create_surface(idSubmesh, SurfaceName::BodySection + "_" + to_string(idSurface0));
    create_band(m, avs0, avs1, tsProto0, tsProto1, true, idSurface, false);

    id_type idHalfedgeLower = 0;
    assert(m.halfedge(idHalfedgeLower).border());
    id_type idHalfedgeUpper = m.halfedge_store_count() - 2 * polygons.polygon().edge_count();
    assert(m.halfedge(idHalfedgeUpper).border());

    std::vector<id_type> idHalfedgeHolesLower(polygons.hole_count());
    std::vector<id_type> idHalfedgeHolesUpper(polygons.hole_count());
    for (size_t i = 0; i < polygons.hole_count(); ++i)
    {
        const auto& polygon = polygons.hole(i);

        avs0 = vertices_attributes<M>(polygon, T(1));

        polygon1.clear();
        std::transform(polygon.vertices().begin(), polygon.vertices().end(), std::back_inserter(polygon1.vertices()), [displacement](const point_type& point) -> point_type { return point + displacement; });
        avs1 = vertices_attributes<M>(polygon1, T(0));

        idHalfedgeHolesLower[i] = m.halfedge_store_count();

        M mm;
        idSurface = mm.create_surface(idSubmesh, SurfaceName::BodySection + "_" + to_string(idSurface0 + 1 + i));
        create_band(mm, avs0, avs1, tsProto0, tsProto1, true, idSurface, false);
        brep::invert(mm);
        m.append(mm);

        assert(m.halfedge(idHalfedgeHolesLower[i]).border());
        idHalfedgeHolesUpper[i] = m.halfedge_store_count() - 2 * polygon.edge_count();
        assert(m.halfedge(idHalfedgeHolesUpper[i]).border());
    }

    // make sure that the orientation of holes is correct due to the reversed winding order ...

    if (extentZ.lower_termination().type == Termination::Type::Flat)
    {
        id_type idSurfaceLower = m.create_surface(idSubmesh, extentZ.lower_name(), {-normal});

        create_border_with_holes_face(m, idHalfedgeLower, idHalfedgeHolesLower, -normal, idSurfaceLower);
        calculate_surface_texcoords(m, idSurfaceLower);
    }

    if (extentZ.upper_termination().type == Termination::Type::Flat)
    {
        id_type idSurfaceUpper = m.create_surface(idSubmesh, extentZ.upper_name(), {normal});

        create_border_with_holes_face(m, idHalfedgeUpper, idHalfedgeHolesUpper, normal, idSurfaceUpper);
        calculate_surface_texcoords(m, idSurfaceUpper);
    }

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_swept_volume(M& mesh, const std::string& name, const geometry::OrientedPolygon<typename M::vector_traits>& polygon, const geometry::Path<typename M::vector_traits>& path, const Extent<typename M::value_type>& extentZ, bool bSurfacesDistinct)
{
    using T = typename M::value_type;

    size_type nAzimuth = polygon.edge_count();
    size_type nz = path.size() - 1;
    assert(nAzimuth > 2);
    assert(nz > 0);

    M m;
    id_type idSubmesh = m.create_submesh(name);
    id_type idSurface0 = nullid;
    if (bSurfacesDistinct)
    {
        idSurface0 = m.surface_store_count();
        for (size_t i = 0; i < nAzimuth; ++i)
        {
            m.create_surface(idSubmesh, SurfaceName::BodySection + "_" + to_string(i));
        }
    }
    else
    {
        idSurface0 = m.create_surface(idSubmesh, SurfaceName::Body);
    }

    auto tsProto0 = texture_span<T>(0, nz, false, false);
    auto tsProto1 = texture_span<T>(1, nz, false, false);

    vertices_attributes_type<M> avs0 = vertices_attributes<M>(polygon, path[0], tsProto0[2]);
    vertices_attributes_type<M> avs1 = vertices_attributes<M>(polygon, path[1], tsProto1[2]);

    create_band(m, avs0, avs1, tsProto0, tsProto1, true, idSurface0, bSurfacesDistinct);

    for (size_type i = 2; i <= nz; ++i)
    {
        tsProto1 = texture_span<T>(i, nz, false, false);
        avs1 = vertices_attributes<M>(polygon, path[i], tsProto1[2]);
        connect_band(m, avs1, tsProto1, true, idSurface0, bSurfacesDistinct, false);
    }

    size_type nh = m.halfedge_store_count();

    if (extentZ.lower_termination().type == Termination::Type::Flat)
    {
        typename M::vector_type normal = math::normalize(-path.front().orientation().up());
        id_type idSurfaceLower = m.create_surface(idSubmesh, extentZ.lower_name(), {normal});
        create_border_face(m, 0, normal, idSurfaceLower);
    }

    if (extentZ.upper_termination().type == Termination::Type::Flat)
    {
        typename M::vector_type normal = math::normalize(path.back().orientation().up());
        id_type idSurfaceUpper = m.create_surface(idSubmesh, extentZ.upper_name(), {normal});
        create_border_face(m, nh - 2 * nAzimuth, normal, idSurfaceUpper);
    }

    m.check();
    mesh.append(m);
    return;
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::model::create_swept_volume(M& mesh, const std::string& name, const geometry::OrientedPolygon<typename M::vector_traits>& polygon, const geometry::Path<typename M::vector_traits>& path, std::function<geometry::OrientedPolygon<typename M::vector_traits>(const geometry::OrientedPolygon<typename M::vector_traits>& polygon, value_type<M>)> f, const Extent<value_type<M>>& extentZ, bool bSurfacesDistinct)
{
    using T = typename M::value_type;

    size_type nAzimuth = polygon.edge_count();
    size_type nz = path.size() - 1;
    assert(nAzimuth > 2);
    assert(nz > 0);

    M m;
    id_type idSubmesh = m.create_submesh(name);
    id_type idSurface0 = nullid;
    if (bSurfacesDistinct)
    {
        idSurface0 = m.surface_store_count();
        for (size_t i = 0; i < nAzimuth; ++i)
        {
            m.create_surface(idSubmesh, SurfaceName::BodySection + "_" + to_string(i));
        }
    }
    else
    {
        idSurface0 = m.create_surface(idSubmesh, SurfaceName::Body);
    }

    T t0 = T(0);
    T t1 = T(1) / T(nz);

    auto tsProto0 = texture_span<T>(0, nz, false, false);
    auto tsProto1 = texture_span<T>(1, nz, false, false);

    vertices_attributes_type<M> avs0 = vertices_attributes<M>(f(polygon, t0), path[0], tsProto0[2]);
    vertices_attributes_type<M> avs1 = vertices_attributes<M>(f(polygon, t1), path[1], tsProto1[2]);

    create_band(m, avs0, avs1, tsProto0, tsProto1, true, idSurface0, bSurfacesDistinct);

    for (size_type i = 2; i <= nz; ++i)
    {
        t1 = T(i) / T(nz);
        tsProto1 = texture_span<T>(i, nz, false, false);
        avs1 = vertices_attributes<M>(f(polygon, t1), path[i], tsProto1[2]);
        connect_band(m, avs1, tsProto1, true, idSurface0, bSurfacesDistinct);
    }

    size_type nh = m.halfedge_store_count();

    if (extentZ.lower_termination().type == Termination::Type::Flat)
    {
        typename M::vector_type normal = normalize(-path.front().orientation().up());
        id_type idSurfaceLower = m.create_surface(idSubmesh, extentZ.lower_name(), {normal});
        create_border_face(m, 0, normal, idSurfaceLower);
    }

    if (extentZ.upper_termination().type == Termination::Type::Flat)
    {
        typename M::vector_type normal = normalize(path.back().orientation().up());
        id_type idSurfaceUpper = m.create_surface(idSubmesh, extentZ.upper_name(), {normal});
        create_border_face(m, nh - 2 * nAzimuth, normal, idSurfaceUpper);
    }

    m.check();
    mesh.append(m);
    return;
}

#endif // QUETZAL_MODEL_PRIMITIVES_HPP
