//------------------------------------------------------------------------------
// spikosahedron
// main.cpp
//------------------------------------------------------------------------------

#include "quetzal/brep/Mesh.hpp"
#include "quetzal/brep/MeshTraits.hpp"
#include "quetzal/brep/mesh_util.hpp"
#include "quetzal/brep/validation.hpp"
#include "quetzal/common/FileValidator.hpp"
#include "quetzal/math/Matrix.hpp"
#include "quetzal/math/transformation_matrix.hpp"
#include "quetzal/model/helix_cone.hpp"
#include "quetzal/model/obj_io.hpp"
#include "quetzal/model/primitives.hpp"
#include "quetzal/model/transforms.hpp"
#include <array>
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
    using matrix_type = math::Matrix<value_type>;

    const size_t s_nLevels = 6;

    static const string dirOut = "out/";
    static const string dirReference = "reference/";

    //--------------------------------------------------------------------------
    string to_filename(const string& name)
    {
        return dirOut + name + ".obj";
    }

    //--------------------------------------------------------------------------
    void create_spikosahedron(mesh_type& mesh, const string& name, value_type r0, value_type r1, value_type z1, size_t nzSpikeMax, value_type revsSpikeMax, value_type tuskification = 0.0f, value_type undulation = 0.0f)
    {
        assert(nzSpikeMax > 0);

        mesh_type m;
        model::create_icosahedron(m, name, r0, false);

        value_type zBase = 0;

        size_t nFaces = m.faces().size();
        for (size_t n = 0; n < nFaces; ++n)
        {
            mesh_type::vector_type upSpike = {0.0f, 0.0f, 1.0f};
            mesh_type::vector_type upFace = m.face(n).attributes().normal();

            // Angle to rotate face so that it is perpendicular to z-axis
            value_type theta = math::angle_unit(upSpike, upFace);

            size_t nLevel = 0; // Controls detail and rotation
            value_type r = 0; // Length of spike

            if (math::float_eq(theta, math::Pi<value_type>)) // Bottom face
            {
                nLevel = 0;
                r = r0;
            }
            else if (math::float_ne(theta, value_type(0)))
            {
                nLevel = static_cast<size_t>((math::Pi<value_type> - theta) / math::Pi<value_type> * 4 + 0.05f) + 1;

                value_type theta0 = math::PiHalf<value_type> - asin(upFace.z());
                value_type theta2 = asin(z1 * sin(theta0) / r1);
                value_type theta1 = math::Pi<value_type> - (theta0 + theta2);
                r = r1 * sin(theta1) / sin(theta0);
            }
            else // Top face
            {
                nLevel = s_nLevels - 1;
                r = r1 + z1;
            }

            if (nLevel == 0) // Bottom face, just record base z coordinate
            {
                zBase = m.face(n).halfedge().attributes().position().z();
                continue;
            }

            if (nLevel == 1 || nzSpikeMax == 1) // Replace face with pyramidal projection
            {
                triangulate_face_central_vertex(m, n, upFace * r, true);
                continue;
            }

            size_t nAzimuth = m.face(n).halfedges().size();
            size_t nz = nzSpikeMax * (nLevel - 1) / (s_nLevels - 2);
            value_type dzSpike = r - r0;
            value_type revs = revsSpikeMax * (nLevel - 1) / (s_nLevels - 2);

            string nameSpike = "spike_" + to_string(n);
            string nameSpikeBase = nameSpike + "/end0";

            // Body face position and orientation
            mesh_type::point_type position = centroid(m.face(n));
            mesh_type::vector_type vFace = m.face(n).halfedge().attributes().position() - position;
            mesh_type::vector_type rightFace = normalize(vFace);
            value_type rBase = vFace.norm();

            // Spike and orientation
            mesh_type mSpike;
            model::create_helix_cone(mSpike, nameSpike, nAzimuth, nz, rBase, dzSpike, revs);
            mesh_type::vector_type rightSpike = normalize(mSpike.surface(nameSpikeBase).faces().front().halfedge().attributes().position());
            assert(math::float_eq0(rightSpike.z()));

            // Apply spike transformations

            if (undulation != 0)
            {
                model::undulate(mSpike, undulation, 3.0f, 0.0f, dzSpike);
            }

            if (tuskification != 0 && theta != 0)
            {
                mesh_type::vector_type axisRotationBase;
                auto rightFaceOriented = rightFace;

                axisRotationBase = normalize(cross(upSpike, upFace)); // axis to rotate face, perpendicular to z-axis
                rightFaceOriented *= math::rotation_axis_unit(axisRotationBase, -theta); // Rotate face parallel to xy-plane
                value_type phi = atan2(rightFaceOriented.y(), rightFaceOriented.x()) - atan2(rightSpike.y(), rightSpike.x());

                model::tuskify(mSpike, axisRotationBase * math::rotation_z(-phi), tuskification, 0.0f, dzSpike);
            }

            // Rotate spike to align with face normal and translate spike to face position
//            matrix_type matrix = alignment2(upSpike, rightSpike, upFace, rightFace) * translation(position);
            matrix_type matrix = alignment(upSpike, rightSpike, upFace, rightFace) * translation(position);
            model::transform(mSpike, matrix);

            // Connect spike to body
            m.append(mSpike);

            id_type idHalfedgeA = m.face(n).halfedge_id();
            id_type idHalfedgeB = m.surface(nameSpikeBase).faces().front().halfedge().prev_id();

            model::unify_vertex_pairs(m, idHalfedgeA, idHalfedgeB);
            brep::weld(m, idHalfedgeA, idHalfedgeB);
        }

        m.pack();

        // Ensure that nothing projects below the bottom face
        for (auto& vertex : m.vertices())
        {
            if (vertex.attributes().position().z() < zBase)
            {
                vertex.attributes().position().set_z(zBase);
            }
        }

        mesh.append(m);
        return;
    }

} // namespace

//------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    argc;
    argv;

    FileValidator validator(dirOut, dirReference);

    string name;
    mesh_type mesh;

    // Make sure that z1 is large enough that lower spikes don't extend below base

    name = "spikosahedron";
    cout << name << endl;
    create_spikosahedron(mesh, "spikosahedron", 29.0f, 58.0f, 31.1688f, 1, 0.0f); // Not swirled, not twisted, single section spike
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    // For glass, may be better off not twisted, but still with divisions ...

    name = "spikosahedron_glass";
    cout << name << endl;
    mesh.clear();
    create_spikosahedron(mesh, "spikosahedron", 29.0f, 58.0f, 31.1688f, 4, 0.0f); // Chunky spike
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "twisted_spikosahedron_glass";
    cout << name << endl;
    mesh.clear();
    create_spikosahedron(mesh, "spikosahedron", 29.0f, 58.0f, 31.1688f, 4, 0.25f); // Twisted, chunky spike
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "tuskified_spikosahedron_glass";
    cout << name << endl;
    mesh.clear();
    create_spikosahedron(mesh, "spikosahedron", 29.0f, 58.0f, 31.1688f, 4, 0.0f, math::PiHalf<value_type>); // Tuskified, chunky spike
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "tuskified_twisted_spikosahedron_glass";
    cout << name << endl;
    mesh.clear();
    create_spikosahedron(mesh, "spikosahedron", 29.0f, 58.0f, 31.1688f, 4, 0.25f, math::PiHalf<value_type>); //  Tuskified, twisted, chunky spike
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    // Smooth combinations

    name = "swirled_spikosahedron";
    cout << name << endl;
    mesh.clear();
    create_spikosahedron(mesh, "spikosahedron", 29.0f, 58.0f, 31.1688f, 100, 0.0f); // Swirled, smooth spike
    model::swirl(mesh, math::Pi<value_type>);
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "twisted_spikosahedron";
    cout << name << endl;
    mesh.clear();
    create_spikosahedron(mesh, "spikosahedron", 29.0f, 58.0f, 31.1688f, 100, 0.5f); // Twisted, smooth spike
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    // Use previous mesh
    name = "swirled_twisted_spikosahedron";
    cout << name << endl;
	model::swirl(mesh, math::Pi<value_type>);
    model::write_obj(mesh, to_filename(name));

    name = "screwed_spikosahedron";
    cout << name << endl;
    mesh.clear();
    create_spikosahedron(mesh, "spikosahedron", 29.0f, 58.0f, 31.1688f, 100, 2.5f); // Twisted, smooth spike
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    // Tuskification

    name = "tuskified_spikosahedron";
    cout << name << endl;
    mesh.clear();
    create_spikosahedron(mesh, "spikosahedron", 29.0f, 58.0f, 31.1688f, 200, 0.0f, math::PiHalf<value_type>); // Tuskified, smooth spike
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    // Use previous mesh
    name = "tuskified_swirled_spikosahedron";
    cout << name << endl;
	model::swirl(mesh, math::Pi<value_type>);
    model::write_obj(mesh, to_filename(name));

    name = "tuskified_twisted_spikosahedron";
    cout << name << endl;
    mesh.clear();
    create_spikosahedron(mesh, "spikosahedron", 29.0f, 58.0f, 31.1688f, 200, 0.5f, math::PiHalf<value_type>); // Tuskified, twisted, smooth spike
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    name = "tuskified_twisted_spikosahedron_model";
    cout << name << endl;
    mesh.clear();
    create_spikosahedron(mesh, "spikosahedron", 29.0f, 58.0f, 31.1688f, 20, 0.5f, math::PiHalf<value_type>); // Tuskified, twisted, smooth spike
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    // Use previous mesh
    name = "tuskified_swirled_twisted_spikosahedron";
    cout << name << endl;
	model::swirl(mesh, math::Pi<value_type>);
    model::write_obj(mesh, to_filename(name));
/*
    // Undulation

    name = "undulating_spikosahedron";
    cout << name << endl;
    mesh.clear();
    create_spikosahedron(mesh, "spikosahedron", 29.0f, 58.0f, 31.1688f, 200, 0.0f, 0.0f, 3.0f * math::PiTwo<value_type>); // Undulating, smooth spike
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    // Use previous mesh
    name = "undulating_swirled_spikosahedron";
    cout << name << endl;
	model::swirl(mesh, math::Pi<value_type>);
    model::write_obj(mesh, to_filename(name));

    name = "undulating_twisted_spikosahedron";
    cout << name << endl;
    mesh.clear();
    create_spikosahedron(mesh, "spikosahedron", 29.0f, 58.0f, 31.1688f, 200, 0.5f, 0.0f, 3.0f * math::PiTwo<value_type>); // Undulating, twisted, smooth spike
    triangulate(mesh);
    model::write_obj(mesh, to_filename(name));

    // Use previous mesh
    name = "undulating_swirled_twisted_spikosahedron";
    cout << name << endl;
	model::swirl(mesh, math::Pi<value_type>);
    model::write_obj(mesh, to_filename(name));
*/
    validator.report_results();
    return 0;
}
