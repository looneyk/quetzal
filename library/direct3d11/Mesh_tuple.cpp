//------------------------------------------------------------------------------
// direct3d11
// Mesh.cpp
//------------------------------------------------------------------------------

#include "Mesh.hpp"
#include <map>
#include <fstream>

#include <tuple>

using namespace std;
using namespace DirectX;

namespace
{

    //------------------------------------------------------------------------------
    struct F3Less
    {
        bool operator()(const XMFLOAT3& lhs, const XMFLOAT3& rhs) const
        {
            if (lhs.z < rhs.z) return true;
            if (lhs.z > rhs.z) return false;
            if (lhs.y < rhs.y) return true;
            if (lhs.y > rhs.y) return false;
            return lhs.x < rhs.x;
        }
    };

    //------------------------------------------------------------------------------
    struct F2Less
    {
        bool operator()(const XMFLOAT2& lhs, const XMFLOAT2& rhs) const
        {
            if (lhs.y < rhs.y) return true;
            if (lhs.y > rhs.y) return false;
            return lhs.x < rhs.x;
        }
    };

//    typedef map<XMFLOAT3, unsigned int, F3Less> M3;
//    typedef map<XMFLOAT2, unsigned int, F2Less> M2;

typedef map<tuple<float, float, float>, unsigned int> M3;
typedef map<tuple<float, float>, unsigned int> M2;

} // namespace

//------------------------------------------------------------------------------
void quetzal::direct3d11::Mesh::clear()
{
    m_vertices.clear();
    m_indices.clear();
    return;
}

//------------------------------------------------------------------------------
bool quetzal::direct3d11::Mesh::write_obj(const string& filename) const
{
    ofstream ofs(filename);
    if (!ofs)
    {
        return false;
    }

    vector<XMFLOAT3> positions;
    vector<XMFLOAT3> normals;
    vector<XMFLOAT2> textures;

    M3 m3Positions;
    M3 m3Normals;
    M2 m2Textures;

    for (const auto& v : m_vertices)
    {
        const auto& ip = m3Positions.find(make_tuple(v.m_position.x, v.m_position.y, v.m_position.z));
        if (ip == m3Positions.end())
        {
            m3Positions.insert(make_pair(make_tuple(v.m_position.x, v.m_position.y, v.m_position.z), positions.size()));
            positions.push_back(v.m_position);
        }

        const auto& in = m3Normals.find(make_tuple(v.m_normal.x, v.m_normal.y, v.m_normal.z));
        if (in == m3Normals.end())
        {
            m3Normals.insert(make_pair(make_tuple(v.m_normal.x, v.m_normal.y, v.m_normal.z), normals.size()));
            normals.push_back(v.m_normal);
        }

        const auto& it = m2Textures.find(make_tuple(v.m_texture.x, v.m_texture.y));
        if (it == m2Textures.end())
        {
            m2Textures.insert(make_pair(make_tuple(v.m_texture.x, v.m_texture.y), textures.size()));
            textures.push_back(v.m_texture);
        }
    }

    ofs << "# Vertices" << endl;
    for (const auto& v : positions)
    {
        ofs << "v " << v.x << " " << v.y << " " << v.z << endl;
    }

    ofs << endl;
    ofs << "# Texture Coordinates" << endl;
    for (const auto& v : textures)
    {
        ofs << "vt " << v.x << " " << v.y << endl;
    }

    ofs << endl;
    ofs << "# Normals" << endl;
    for (const auto& v : normals)
    {
        ofs << "vn " << v.x << " " << v.y << " " << v.z << endl;
    }

    assert(m_indices.size() %3 == 0);

    ofs << endl;
    ofs << "# Faces" << endl;
    for (auto i = m_indices.begin(); i != m_indices.end(); )
    {
        ofs << "f";

        for (unsigned int j = 0; j < 3; ++j)
        {
            const auto& v = m_vertices[m_indices[*i]];

            const auto& ip = m3Positions.find(make_tuple(v.m_position.x, v.m_position.y, v.m_position.z));
            assert(ip != m3Positions.end());

            const auto& in = m3Normals.find(make_tuple(v.m_normal.x, v.m_normal.y, v.m_normal.z));
            assert(in != m3Normals.end());

            const auto& it = m2Textures.find(make_tuple(v.m_texture.x, v.m_texture.y));
            assert(it != m2Textures.end());

            ofs << " " << (ip->second + 1) << "/" << (it->second + 1) << "/" <<(in->second + 1);
//ofs << " " << (*i + 1) << "/" << (*i + 1) << "/" << (*i + 1);

            ++i;
        }

        ofs << endl;
    }

    return true;
}
