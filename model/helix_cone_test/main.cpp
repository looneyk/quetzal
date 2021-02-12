//------------------------------------------------------------------------------
// helix_cone_test
// main.cpp
//------------------------------------------------------------------------------

#include "quetzal/brep/Mesh.hpp"
#include "quetzal/brep/MeshTraits.hpp"
#include "quetzal/brep/mesh_util.hpp"
#include "quetzal/common/FileValidator.hpp"
#include "quetzal/model/helix_cone.hpp"
#include "quetzal/model/obj_io.hpp"
#include "quetzal/model/primitives.hpp"
#include <string>
#include <iostream>
#include <cassert>

using namespace std;
using namespace quetzal;

namespace
{

    using value_type = float;
    using vector_traits = math::VectorTraits<value_type, 3>;
    using mesh_type = brep::Mesh<brep::MeshTraits<vector_traits>>;

    static const std::string dirOut = "out/";
    static const string dirReference = "reference/";

    //--------------------------------------------------------------------------
    std::string to_filename(const std::string& name)
    {
        return dirOut + name + ".obj";
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

    name = "helix_cone_3_1";
    cout << name << endl;
    mesh.clear();
    model::create_helix_cone(mesh, name, 3, 1, 20.0f, 60.0f, 0.25f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "helix_cone_3_2";
    cout << name << endl;
    mesh.clear();
    model::create_helix_cone(mesh, name, 3, 2, 20.0f, 60.0f, 0.25f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "helix_cone_3_3";
    cout << name << endl;
    mesh.clear();
    model::create_helix_cone(mesh, name, 3, 3, 20.0f, 60.0f, 0.25f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "helix_cone_3_4_convex";
    cout << name << endl;
    mesh.clear();
    model::create_helix_cone(mesh, name, 3, 4, 15.0f, 60.0f, 0.25f);
    triangulate(mesh); // convex ...
    model::write_obj(mesh, to_filename(name));

    name = "helix_cone_3_4_concave";
    cout << name << endl;
    mesh.clear();
    model::create_helix_cone(mesh, name, 3, 4, 15.0f, 60.0f, 0.25f);
    triangulate(mesh); // concave ...
    model::write_obj(mesh, to_filename(name));

    name = "helix_cone_3_100";
    cout << name << endl;
    mesh.clear();
    model::create_helix_cone(mesh, name, 3, 100, 20.0f, 60.0f, 0.25f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "helix_cone_5_1";
    cout << name << endl;
    mesh.clear();
    model::create_helix_cone(mesh, name, 5, 1, 20.0f, 60.0f, 0.25f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "helix_cone_5_2";
    cout << name << endl;
    mesh.clear();
    model::create_helix_cone(mesh, name, 5, 2, 20.0f, 60.0f, 0.25f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "helix_cone_5_3";
    cout << name << endl;
    mesh.clear();
    model::create_helix_cone(mesh, name, 5, 3, 20.0f, 60.0f, 0.25f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "helix_cone_5_100";
    cout << name << endl;
    mesh.clear();
    model::create_helix_cone(mesh, name, 5, 100, 20.0f, 60.0f, 0.25f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "helix_cone_6_4";
    cout << name << endl;
    mesh.clear();
    model::create_helix_cone(mesh, name, 6, 4, 20.0f, 60.0f, 0.25f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "helix_cone_3_200_3";
    cout << name << endl;
    mesh.clear();
    model::create_helix_cone(mesh, name, 3, 200, 20.0f, 60.0f, 3.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cylinder_3_1";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 3, 1, 20.0f, 0.0f, 0.0f, 60.0f, 0.0f, 0.25f, model::ExtentSideFlat<value_type>(), model::ExtentEndsFlat<value_type>(), true);
    triangulate(mesh);
    model::calculate_surface_normals(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cylinder_3_2";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 3, 2, 20.0f, 0.0f, 0.0f, 60.0f, 0.0f, 0.25f, model::ExtentSideFlat<value_type>(), model::ExtentEndsFlat<value_type>(), true);
    triangulate(mesh);
    model::calculate_surface_normals(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cylinder_3_3";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 3, 3, 20.0f, 0.0f, 0.0f, 60.0f, 0.0f, 0.25f, model::ExtentSideFlat<value_type>(), model::ExtentEndsFlat<value_type>(), true);
    triangulate(mesh);
    model::calculate_surface_normals(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cylinder_3_4_convex";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 3, 4, 15.0f, 0.0f, 0.0f, 60.0f, 0.0f, 0.25f, model::ExtentSideFlat<value_type>(), model::ExtentEndsFlat<value_type>(), true);
    triangulate(mesh); // convex ...
    model::calculate_surface_normals(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cylinder_3_4_concave";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 3, 4, 15.0f, 0.0f, 0.0f, 60.0f, 0.0f, 0.25f, model::ExtentSideFlat<value_type>(), model::ExtentEndsFlat<value_type>(), true);
    triangulate(mesh); // concave ...
    model::calculate_surface_normals(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cylinder_3_100";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 3, 100, 20.0f, 0.0f, 0.0f, 60.0f, 0.0f, 0.25f, model::ExtentSideFlat<value_type>(), model::ExtentEndsFlat<value_type>(), true);
    triangulate(mesh);
    model::calculate_surface_normals(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cylinder_5_1";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 5, 1, 20.0f, 0.0f, 0.0f, 60.0f, 0.0f, 0.25f, model::ExtentSideFlat<value_type>(), model::ExtentEndsFlat<value_type>(), true);
    triangulate(mesh);
    model::calculate_surface_normals(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cylinder_5_2";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 5, 2, 20.0f, 0.0f, 0.0f, 60.0f, 0.0f, 0.25f, model::ExtentSideFlat<value_type>(), model::ExtentEndsFlat<value_type>(), true);
    triangulate(mesh);
    model::calculate_surface_normals(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cylinder_5_3";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 5, 3, 20.0f, 0.0f, 0.0f, 60.0f, 0.0f, 0.25f, model::ExtentSideFlat<value_type>(), model::ExtentEndsFlat<value_type>(), true);
    triangulate(mesh);
    model::calculate_surface_normals(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cylinder_5_100";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 5, 100, 20.0f, 0.0f, 0.0f, 60.0f, 0.0f, 0.25f, model::ExtentSideFlat<value_type>(), model::ExtentEndsFlat<value_type>(), true);
    triangulate(mesh);
    model::calculate_surface_normals(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cylinder_6_4";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 6, 4, 20.0f, 0.0f, 0.0f, 60.0f, 0.0f, 0.25f, model::ExtentSideFlat<value_type>(), model::ExtentEndsFlat<value_type>(), true);
    triangulate(mesh);
    model::calculate_surface_normals(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "cylinder_3_200_3";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 3, 200, 20.0f, 0.0f, 0.0f, 60.0f, 0.0f, 3.0f, model::ExtentSideFlat<value_type>(), model::ExtentEndsFlat<value_type>(), true);
    triangulate(mesh);
    model::calculate_surface_normals(mesh);
    model::write_obj(mesh, to_filename(name));

    validator.report_results();
    return 0;
}
