//------------------------------------------------------------------------------
// primitives_test
// main.cpp
//------------------------------------------------------------------------------

#include "quetzal/brep/Mesh.hpp"
#include "quetzal/brep/MeshTraits.hpp"
#include "quetzal/brep/mesh_texcoord.hpp"
#include "quetzal/brep/mesh_util.hpp"
#include "quetzal/brep/triangulation.hpp"
#include "quetzal/common/FileValidator.hpp"
#include "quetzal/math/Interval.hpp"
#include "quetzal/math/VectorTraits.hpp"
#include "quetzal/model/obj_io.hpp"
#include "quetzal/model/primitives.hpp"
#include <iostream>
#include <string>
#include <cassert>

using namespace std;
using namespace quetzal;

namespace
{

    using value_type = float;
//    using value_type = double;
    using vector_traits = math::VectorTraits<value_type, 3>;
    using mesh_type = brep::Mesh<brep::MeshTraits<vector_traits>>;

    static const string dirOut = "out/";
    static const string dirReference = "reference/";

    //--------------------------------------------------------------------------
    string to_filename(const string& name)
    {
        return dirOut + name + ".obj";
    }

    //--------------------------------------------------------------------------
    // Disk morphed into cone
    template<typename M>
    void create_disk_test(M& mesh, const string& name, size_t nAzimuth, size_t nRadial, value_type radius, const math::Interval<value_type>& intervalAzimuth = math::Interval(value_type(0), value_type(1)))
    {
        M m;
        model::create_disk(m, name, nAzimuth, nRadial, radius, intervalAzimuth);

        for (auto& vertex : m.vertices())
        {
            auto& position = vertex.attributes().position();
            position.set_z(radius - position.norm());
            // normal here, or calculate_surface_normals below ...
        }

        // Bottom
        typename M::vector_type normal = {0.0f, 0.0f, -1.0f};
        id_type idSurface = m.create_surface(m.submesh_id(name), "bottom", {normal});
        create_border_face(m, 0, normal, idSurface);
        calculate_surface_texcoords(m, idSurface);

        mesh.append(m);
        mesh.check();
        return;
    }

} // namespace

//------------------------------------------------------------------------------
int main(int argc, char* argv)
{
    argc;
    argv;

    FileValidator validator(dirOut, dirReference);

    string name;
    mesh_type mesh;
    id_type idSurface = nullid;

    name = "disk_cone";
    cout << name << endl;
    mesh.clear();
    create_disk_test(mesh, name, 12, 4, 10.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "box";
    cout << name << endl;
    mesh.clear();
    model::create_box(mesh, name, 5.0f, 10.0f, 20.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cylinder";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 12, 10, 5.0f, 5.0f, -10.0f, 10.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cylinder_1";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 12, 1, 5.0f, 5.0f, -10.0f, 10.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cylinder_left_half";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 6, 10, 5.0f, 5.0f, -10.0f, 10.0f, model::ExtentSideFlat<value_type>(0.25f, 0.75f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cylinder_right_half";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 6, 10, 5.0f, 5.0f, -10.0f, 10.0f, model::ExtentSideFlat<value_type>(-0.25f, 0.25f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cylinder_quarter";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 3, 10, 5.0f, 5.0f, -10.0f, 10.0f, model::ExtentSideFlat<value_type>(-0.125f, 0.125f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cylinder_three_quarter";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 9, 10, 5.0f, 5.0f, -10.0f, 10.0f, model::ExtentSideFlat<value_type>(0.125f, 0.875f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cylinder_half_center";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 6, 10, 5.0f, 5.0f, -10.0f, 10.0f, model::ExtentSideCenter<value_type>(0.25f, 0.75f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cylinder_quarter_center";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 3, 10, 5.0f, 5.0f, -10.0f, 10.0f, model::ExtentSideCenter<value_type>(-0.125f, 0.125f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cylinder_three_quarter_center";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 9, 10, 5.0f, 5.0f, -10.0f, 10.0f, model::ExtentSideCenter<value_type>(0.125f, 0.875f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cylinder_three_quarter_none";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 9, 10, 5.0f, 5.0f, -10.0f, 10.0f, model::Extent<value_type>(0.125f, 0.875f));
    idSurface = mesh.create_surface(mesh.submesh_id(name), "side", {{1.0f, 0.0f, 0.0f}});
    create_border_face(mesh, 3 * 9, {1.0f, 0.0f, 0.0f}, idSurface);
    calculate_surface_texcoords(mesh, idSurface);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "prism";
    cout << name << endl;
    mesh.clear();
    model::create_prism(mesh, name, 3, 6, 5.0f, 5.0f, -10.0f, 10.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "antiprism_3";
    cout << name << endl;
    mesh.clear();
    model::create_antiprism(mesh, name, 3, 7.5f, 7.5f, -10.0f, 10.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "antiprism_6";
    cout << name << endl;
    mesh.clear();
    model::create_antiprism(mesh, name, 6, 7.5f, 7.5f, -10.0f, 10.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "anticylinder_3_f";
    cout << name << endl;
    mesh.clear();
    model::create_anticylinder(mesh, name, 3, 1, 7.5f, 7.5f, -10.0f, 10.0f, model::ExtentEndsFlat<value_type>(0.0f, 1.0f), true);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "anticylinder_3";
    cout << name << endl;
    mesh.clear();
    model::create_anticylinder(mesh, name, 3, 1, 7.5f, 7.5f, -10.0f, 10.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "anticylinder_6_f";
    cout << name << endl;
    mesh.clear();
    model::create_anticylinder(mesh, name, 6, 1, 7.5f, 7.5f, -10.0f, 10.0f, model::ExtentEndsFlat<value_type>(0.0f, 1.0f), true);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "anticylinder_6";
    cout << name << endl;
    mesh.clear();
    model::create_anticylinder(mesh, name, 6, 1, 7.5f, 7.5f, -10.0f, 10.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "anticylinder_6_8_f";
    cout << name << endl;
    mesh.clear();
    model::create_anticylinder(mesh, name, 6, 8, 7.5f, 7.5f, -10.0f, 10.0f, model::ExtentEndsFlat<value_type>(0.0f, 1.0f), true);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "anticylinder_6_8";
    cout << name << endl;
    mesh.clear();
    model::create_anticylinder(mesh, name, 6, 8, 7.5f, 7.5f, -10.0f, 10.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cone_up";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 12, 6, 7.5f, 0.0f, -10.0f, 10.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cone_up_left_half";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 6, 6, 7.5f, 0.0f, -10.0f, 10.0f, model::ExtentSideFlat<value_type>(0.25f, 0.75f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cone_up_right_half";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 6, 6, 7.5f, 0.0f, -10.0f, 10.0f, model::ExtentSideFlat<value_type>(-0.25f, 0.25f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cone_up_quarter";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 3, 6, 7.5f, 0.0f, -10.0f, 10.0f, model::ExtentSideFlat<value_type>(-0.125f, 0.125f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cone_up_three_quarter";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 9, 6, 7.5f, 0.0f, -10.0f, 10.0f, model::ExtentSideFlat<value_type>(0.125f, 0.875f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cone_up_quarter_center";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 3, 6, 7.5f, 0.0f, -10.0f, 10.0f, model::ExtentSideCenter<value_type>(-0.125f, 0.125f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cone_up_three_quarter_center";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 9, 6, 7.5f, 0.0f, -10.0f, 10.0f, model::ExtentSideCenter<value_type>(0.125f, 0.875f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cone_down";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 12, 6, 0.0f, 7.5f, -10.0f, 10.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cone_down_left_half";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 6, 6, 0.0f, 7.5f, -10.0f, 10.0f, model::ExtentSideFlat<value_type>(0.25f, 0.75f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cone_down_right_half";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 6, 6, 0.0f, 7.5f, -10.0f, 10.0f, model::ExtentSideFlat<value_type>(-0.25f, 0.25f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cone_down_quarter";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 3, 6, 0.0f, 7.5f, -10.0f, 10.0f, model::ExtentSideFlat<value_type>(-0.125f, 0.125f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cone_down_three_quarter";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 9, 6, 0.0f, 7.5f, -10.0f, 10.0f, model::ExtentSideFlat<value_type>(0.125f, 0.875f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cone_down_quarter_center";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 3, 6, 0.0f, 7.5f, -10.0f, 10.0f, model::ExtentSideCenter<value_type>(-0.125f, 0.125f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cone_down_three_quarter_center";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 9, 6, 0.0f, 7.5f, -10.0f, 10.0f, model::ExtentSideCenter<value_type>(0.125f, 0.875f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cone_up_1";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 6, 1, 7.5f, 0.0f, -10.0f, 10.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cone_down_1";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 6, 1, 0.0f, 7.5f, -10.0f, 10.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "pyramid_up";
    cout << name << endl;
    mesh.clear();
    model::create_prism(mesh, name, 12, 6, 7.5f, 0.0f, -10.0f, 10.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "pyramid_up_left_half";
    cout << name << endl;
    mesh.clear();
    model::create_prism(mesh, name, 6, 6, 7.5f, 0.0f, -10.0f, 10.0f, model::ExtentSideFlat<value_type>(0.25f, 0.75f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "pyramid_up_right_half";
    cout << name << endl;
    mesh.clear();
    model::create_prism(mesh, name, 6, 6, 7.5f, 0.0f, -10.0f, 10.0f, model::ExtentSideFlat<value_type>(-0.25f, 0.25f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "pyramid_up_quarter";
    cout << name << endl;
    mesh.clear();
    model::create_prism(mesh, name, 3, 6, 7.5f, 0.0f, -10.0f, 10.0f, model::ExtentSideFlat<value_type>(-0.125f, 0.125f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "pyramid_up_three_quarter";
    cout << name << endl;
    mesh.clear();
    model::create_prism(mesh, name, 9, 6, 7.5f, 0.0f, -10.0f, 10.0f, model::ExtentSideFlat<value_type>(0.125f, 0.875f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "pyramid_up_quarter_center";
    cout << name << endl;
    mesh.clear();
    model::create_prism(mesh, name, 3, 6, 7.5f, 0.0f, -10.0f, 10.0f, model::ExtentSideCenter<value_type>(-0.125f, 0.125f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "pyramid_up_three_quarter_center";
    cout << name << endl;
    mesh.clear();
    model::create_prism(mesh, name, 9, 6, 7.5f, 0.0f, -10.0f, 10.0f, model::ExtentSideCenter<value_type>(0.125f, 0.875f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "pyramid_down";
    cout << name << endl;
    mesh.clear();
    model::create_prism(mesh, name, 12, 6, 0.0f, 7.5f, -10.0f, 10.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "pyramid_down_left_half";
    cout << name << endl;
    mesh.clear();
    model::create_prism(mesh, name, 6, 6, 0.0f, 7.5f, -10.0f, 10.0f, model::ExtentSideFlat<value_type>(0.25f, 0.75f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "pyramid_down_right_half";
    cout << name << endl;
    mesh.clear();
    model::create_prism(mesh, name, 6, 6, 0.0f, 7.5f, -10.0f, 10.0f, model::ExtentSideFlat<value_type>(-0.25f, 0.25f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "pyramid_down_quarter";
    cout << name << endl;
    mesh.clear();
    model::create_prism(mesh, name, 3, 6, 0.0f, 7.5f, -10.0f, 10.0f, model::ExtentSideFlat<value_type>(-0.125f, 0.125f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "pyramid_down_three_quarter";
    cout << name << endl;
    mesh.clear();
    model::create_prism(mesh, name, 9, 6, 0.0f, 7.5f, -10.0f, 10.0f, model::ExtentSideFlat<value_type>(0.125f, 0.875f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "pyramid_down_quarter_center";
    cout << name << endl;
    mesh.clear();
    model::create_prism(mesh, name, 3, 6, 0.0f, 7.5f, -10.0f, 10.0f, model::ExtentSideCenter<value_type>(-0.125f, 0.125f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "pyramid_down_three_quarter_center";
    cout << name << endl;
    mesh.clear();
    model::create_prism(mesh, name, 9, 6, 0.0f, 7.5f, -10.0f, 10.0f, model::ExtentSideCenter<value_type>(0.125f, 0.875f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "pyramid_up_1";
    cout << name << endl;
    mesh.clear();
    model::create_prism(mesh, name, 6, 1, 7.5f, 0.0f, -10.0f, 10.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "pyramid_down_1";
    cout << name << endl;
    mesh.clear();
    model::create_prism(mesh, name, 6, 1, 0.0f, 7.5f, -10.0f, 10.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "sphere";
    cout << name << endl;
    mesh.clear();
    model::create_sphere(mesh, name, 24, 12, 10.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "sphere_rough";
    cout << name << endl;
    mesh.clear();
    model::create_sphere(mesh, name, 12, 6, 10.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "sphere_fine";
    cout << name << endl;
    mesh.clear();
    model::create_sphere(mesh, name, 36, 18, 10.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "sphere_left_half";
    cout << name << endl;
    mesh.clear();
    model::create_sphere(mesh, name, 12, 12, 10.0f, model::ExtentSideFlat<value_type>(0.25f, 0.75f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "sphere_right_half";
    cout << name << endl;
    mesh.clear();
    model::create_sphere(mesh, name, 12, 12, 10.0f, model::ExtentSideFlat<value_type>(-0.25f, 0.25f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "sphere_quarter";
    cout << name << endl;
    mesh.clear();
    model::create_sphere(mesh, name, 6, 12, 10.0f, model::ExtentSideFlat<value_type>(-0.125f, 0.125f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "sphere_three_quarter";
    cout << name << endl;
    mesh.clear();
    model::create_sphere(mesh, name, 18, 12, 10.0f, model::ExtentSideFlat<value_type>(0.125f, 0.875f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "sphere_quarter_center";
    cout << name << endl;
    mesh.clear();
    model::create_sphere(mesh, name, 6, 12, 10.0f, model::ExtentSideCenter<value_type>(-0.125f, 0.125f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "sphere_three_quarter_center";
    cout << name << endl;
    mesh.clear();
    model::create_sphere(mesh, name, 18, 12, 10.0f, model::ExtentSideCenter<value_type>(0.125f, 0.875f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "sphere_top";
    cout << name << endl;
    mesh.clear();
    model::create_sphere(mesh, name, 24, 6, 10.0f, model::Extent<value_type>(), model::ExtentEndsFlat<value_type>(0.5f, 1.0f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "sphere_bottom";
    cout << name << endl;
    mesh.clear();
    model::create_sphere(mesh, name, 24, 6, 10.0f, model::Extent<value_type>(), model::ExtentEndsFlat<value_type>(0.0f, 0.5f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "sphere_middle";
    cout << name << endl;
    mesh.clear();
    model::create_sphere(mesh, name, 24, 6, 10.0f, model::Extent<value_type>(), model::ExtentEndsFlat<value_type>(0.25f, 0.75f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "sphere_top_three_quarter_flat";
    cout << name << endl;
    mesh.clear();
    model::create_sphere(mesh, name, 18, 6, 10.0f, model::ExtentSideFlat<value_type>(0.125f, 0.875f), model::ExtentEndsFlat<value_type>(0.5f, 1.0f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "sphere_top_three_quarter_center";
    cout << name << endl;
    mesh.clear();
    model::create_sphere(mesh, name, 18, 6, 10.0f, model::ExtentSideCenter<value_type>(0.125f, 0.875f), model::ExtentEndsFlat<value_type>(0.5f, 1.0f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "sphere_bottom_three_quarter_flat";
    cout << name << endl;
    mesh.clear();
    model::create_sphere(mesh, name, 18, 6, 10.0f, model::ExtentSideFlat<value_type>(0.125f, 0.875f), model::ExtentEndsFlat<value_type>(0.0f, 0.5f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "sphere_bottom_three_quarter_center";
    cout << name << endl;
    mesh.clear();
    model::create_sphere(mesh, name, 18, 6, 10.0f, model::ExtentSideCenter<value_type>(0.125f, 0.875f), model::ExtentEndsFlat<value_type>(0.0f, 0.5f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "sphere_middle_three_quarter_flat";
    cout << name << endl;
    mesh.clear();
    model::create_sphere(mesh, name, 18, 6, 10.0f, model::ExtentSideFlat<value_type>(0.125f, 0.875f), model::ExtentEndsFlat<value_type>(0.25f, 0.75f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "sphere_middle_three_quarter_center";
    cout << name << endl;
    mesh.clear();
    model::create_sphere(mesh, name, 18, 6, 10.0f, model::ExtentSideCenter<value_type>(0.125f, 0.875f), model::ExtentEndsFlat<value_type>(0.25f, 0.75f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "sphere_middle_three_quarter_none";
    cout << name << endl;
    mesh.clear();
    model::create_sphere(mesh, name, 18, 6, 10.0f, model::Extent<value_type>(0.125f, 0.875f), model::ExtentEndsFlat<value_type>(0.25f, 0.75f));
    idSurface = mesh.create_surface(mesh.submesh_id(name), "side", {{1.0f, 0.0f, 0.0f}});
    create_border_face(mesh, 3 * 18, {1.0f, 0.0f, 0.0f}, idSurface);
    calculate_surface_texcoords(mesh, idSurface);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "sphere_octahedron";
    cout << name << endl;
    mesh.clear();
    model::create_sphere(mesh, name, 4, 2, 10.0f);
    model::write_obj(mesh, to_filename(name));

    name = "torus";
    cout << name << endl;
    mesh.clear();
    model::create_torus(mesh, name, 24, 12, 15.0f, 7.5f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "torus_right_half";
    cout << name << endl;
    mesh.clear();
    model::create_torus(mesh, name, 12, 12, 15.0f, 7.5f, model::ExtentEndsFlat<value_type>(-0.25f, 0.25f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "torus_top_half";
    cout << name << endl;
    mesh.clear();
    model::create_torus(mesh, name, 24, 6, 15.0f, 7.5f, model::Extent<value_type>(), model::ExtentSideFlat<value_type>(0.5f, 1.0f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "torus_bottom_half";
    cout << name << endl;
    mesh.clear();
    model::create_torus(mesh, name, 24, 6, 15.0f, 7.5f, model::Extent<value_type>(), model::ExtentSideFlat<value_type>(0.0f, 0.5f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "torus_bottom_quarter";
    cout << name << endl;
    mesh.clear();
    model::create_torus(mesh, name, 24, 3, 15.0f, 7.5f, model::Extent<value_type>(), model::ExtentSideFlat<value_type>(0.125f, 0.375f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "torus_bottom_three_quarter";
    cout << name << endl;
    mesh.clear();
    model::create_torus(mesh, name, 24, 9, 15.0f, 7.5f, model::Extent<value_type>(), model::ExtentSideFlat<value_type>(-0.125f, 0.625f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "torus_inward_slant";
    cout << name << endl;
    mesh.clear();
    model::create_torus(mesh, name, 24, 6, 15.0f, 7.5f, model::Extent<value_type>(), model::ExtentSideFlat<value_type>(0.125f, 0.625f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "torus_outward_slant";
    cout << name << endl;
    mesh.clear();
    model::create_torus(mesh, name, 24, 6, 15.0f, 7.5f, model::Extent<value_type>(), model::ExtentSideFlat<value_type>(-.125f, 0.375f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "torus_inside_half";
    cout << name << endl;
    mesh.clear();
    model::create_torus(mesh, name, 24, 6, 15.0f, 7.5f, model::Extent<value_type>(), model::ExtentSideFlat<value_type>(-0.25f, 0.25f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "torus_outside_half";
    cout << name << endl;
    mesh.clear();
    model::create_torus(mesh, name, 24, 6, 15.0f, 7.5f, model::Extent<value_type>(), model::ExtentSideFlat<value_type>(0.25f, 0.75f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "torus_inside_three_quarter_center";
    cout << name << endl;
    mesh.clear();
    model::create_torus(mesh, name, 24, 9, 15.0f, 7.5f, model::Extent<value_type>(), model::ExtentSideCenter<value_type>(-0.375f, 0.375f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "torus_outside_quarter_center";
    cout << name << endl;
    mesh.clear();
    model::create_torus(mesh, name, 24, 3, 15.0f, 7.5f, model::Extent<value_type>(), model::ExtentSideCenter<value_type>(0.375f, 0.625f));
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "tetrahedron";
    cout << name << endl;
    mesh.clear();
    model::create_tetrahedron(mesh, name, 10.0f);
    model::write_obj(mesh, to_filename(name));

    name = "octahedron_vertex";
    cout << name << endl;
    mesh.clear();
    model::create_octahedron(mesh, name, 10.0f, true);
    model::write_obj(mesh, to_filename(name));

    name = "octahedron_face";
    cout << name << endl;
    mesh.clear();
    model::create_octahedron(mesh, name, 10.0f, false);
    model::write_obj(mesh, to_filename(name));

    name = "dodecahedron";
    cout << name << endl;
    mesh.clear();
    model::create_dodecahedron(mesh, name, 10.0f);
    triangulate_central_vertex(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "icosahedron_vertex";
    cout << name << endl;
    mesh.clear();
    model::create_icosahedron(mesh, name, 10.0f);
    model::write_obj(mesh, to_filename(name));

    name = "icosahedron_face";
    cout << name << endl;
    mesh.clear();
    model::create_icosahedron(mesh, name, 10.0f, false, false);
    model::write_obj(mesh, to_filename(name));

    name = "icosahedron_spherical";
    cout << name << endl;
    mesh.clear();
    model::create_icosahedron(mesh, name, 10.0f, false, true);
    model::write_obj(mesh, to_filename(name));

    name = "d20";
    cout << name << endl;
    mesh.clear();
    model::create_icosahedron(mesh, name, 12.0f, false, false);
    model::write_obj(mesh, to_filename(name));

    name = "geodesic_sphere_1";
    cout << name << endl;
    mesh.clear();
    model::create_geodesic_sphere(mesh, name, 10.0f, 1, false, false);
    model::write_obj(mesh, to_filename(name));

    name = "geodesic_sphere_2";
    cout << name << endl;
    mesh.clear();
    model::create_geodesic_sphere(mesh, name, 10.0f, 2, false, false);
    model::write_obj(mesh, to_filename(name));

    name = "geodesic_sphere_3";
    cout << name << endl;
    mesh.clear();
    model::create_geodesic_sphere(mesh, name, 10.0f, 3, false, false);
    model::write_obj(mesh, to_filename(name));

    name = "geodesic_sphere_4";
    cout << name << endl;
    mesh.clear();
    model::create_geodesic_sphere(mesh, name, 10.0f, 4, false, false);
    model::write_obj(mesh, to_filename(name));

    name = "geodesic_sphere_5";
    cout << name << endl;
    mesh.clear();
    model::create_geodesic_sphere(mesh, name, 10.0f, 5, false, false);
    model::write_obj(mesh, to_filename(name));

    name = "geodesic_sphere_6";
    cout << name << endl;
    mesh.clear();
    model::create_geodesic_sphere(mesh, name, 10.0f, 6, false, false);
    model::write_obj(mesh, to_filename(name));

    name = "geodesic_sphere_6_spherical";
    cout << name << endl;
    mesh.clear();
    model::create_geodesic_sphere(mesh, name, 10.0f, 6, true, true);
    model::write_obj(mesh, to_filename(name));

    validator.report_results();
    return 0;
}
