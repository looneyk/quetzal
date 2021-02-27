//------------------------------------------------------------------------------
// primitives_test
// main.cpp
//------------------------------------------------------------------------------

#include "quetzal/brep/Mesh.hpp"
#include "quetzal/brep/MeshTraits.hpp"
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
    using vector_traits = math::VectorTraits<value_type, 3>;
    using mesh_type = brep::Mesh<brep::MeshTraits<vector_traits>>;

    static const string dirOut = "out/";
    static const string dirReference = "reference/";

    //--------------------------------------------------------------------------
    string to_filename(const string& name)
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

    name = "first_layer_block";
    cout << name << endl;
    mesh.clear();
    model::create_box(mesh, name, 125.0f, 25.0f, 5.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "axis_shaft";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 12, 1, 0.02f, 0.02f, 0.0f, 1.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "axis_head";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 12, 1, 0.05f, 0.0f, 0.0f, 0.2f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "axis_origin";
    cout << name << endl;
    mesh.clear();
    model::create_icosahedron(mesh, name, 0.05f, true, true);
    model::write_obj(mesh, to_filename(name));

    name = "unit_cylinder";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 24, 1, 1.0f, 1.0f, 0.0f, 1.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "unit_cone";
    cout << name << endl;
    mesh.clear();
    model::create_cylinder(mesh, name, 24, 1, 1.0f, 0.0f, 0.0f, 1.0f);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "unit_icosahedron";
    cout << name << endl;
    mesh.clear();
    model::create_icosahedron(mesh, name, 1.0f, true, true);
    model::write_obj(mesh, to_filename(name));

    name = "unit_geosphere2";
    cout << name << endl;
    mesh.clear();
    model::create_geodesic_sphere(mesh, name, 1.0f, 2, true, true);
    model::write_obj(mesh, to_filename(name));

    validator.report_results();
    return 0;
}
