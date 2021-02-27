#if !defined(QUETZAL_WAVEFRONT_OBJ_READER_HPP)
#define QUETZAL_WAVEFRONT_OBJ_READER_HPP
//------------------------------------------------------------------------------
// Wavefront obj file i/o
// Reader.hpp
//------------------------------------------------------------------------------

#include "quetzal/common/Exception.hpp"
#include "quetzal/math/Vector.hpp"
#include "quetzal/math/VectorTraits.hpp"
#include "reader_util.hpp"
#include "symbols.hpp"
#include <array>
#include <filesystem>
#include <functional>
#include <fstream>
#include <iostream> // warning messages, change to log ...
#include <sstream>
#include <string>
#include <vector>
#include <cassert>

namespace quetzal::wavefront_obj
{

    //--------------------------------------------------------------------------
    template<typename M>
    class Reader
    {
    public:

        using value_type = typename M::value_type;
        using point_type = typename M::point_type;
        using vector_type = typename M::vector_type;
        using texcoord_type = typename M::texcoord_type;

        using open_function_type = std::function<void(M&, const std::string&)>;
        using object_function_type = std::function<void(M&, const std::string&)>;
        using group_function_type = std::function<void(M&, const std::string&)>;
        using face_open_function_type = std::function<void(M&)>;
        using face_vertex_function_type = std::function<void(M&, const point_type&, const vector_type&, const texcoord_type&)>;
        using face_close_function_type = std::function<void(M&)>;
        using materials_function_type = std::function<void(M&, const std::string&)>;
        using material_function_type = std::function<void(M&, const std::string&)>;

        Reader(open_function_type on_open, object_function_type on_object, group_function_type on_group, face_open_function_type on_face_open, face_vertex_function_type on_face_vertex, face_close_function_type on_face_close, materials_function_type on_materials, material_function_type on_material);
        Reader(const Reader&) = delete;
        ~Reader() = default;

        Reader& operator=(const Reader&) = delete;

        void read(const std::filesystem::path& pathname, M& m) const;

    private:

        open_function_type m_on_open;
        object_function_type m_on_object;
        group_function_type m_on_group;
        face_open_function_type m_on_face_open;
        face_vertex_function_type m_on_face_vertex;
        face_close_function_type m_on_face_close;
        materials_function_type m_on_materials;
        material_function_type m_on_material;
    };

} // namespace quetzal::wavefront_obj

//------------------------------------------------------------------------------
template<typename M>
quetzal::wavefront_obj::Reader<M>::Reader(open_function_type on_open, object_function_type on_object, group_function_type on_group, face_open_function_type on_face_open, face_vertex_function_type on_face_vertex, face_close_function_type on_face_close, materials_function_type on_materials, material_function_type on_material) :
    m_on_open(on_open),
    m_on_object(on_object),
    m_on_group(on_group),
    m_on_face_open(on_face_open),
    m_on_face_vertex(on_face_vertex),
    m_on_face_close(on_face_close),
    m_on_materials(on_materials),
    m_on_material(on_material)
{
}

//------------------------------------------------------------------------------
template<typename M>
void quetzal::wavefront_obj::Reader<M>::read(const std::filesystem::path& pathname, M& m) const
{
    std::ifstream is(pathname, std::ios_base::binary);
    if (!is)
    {
        std::ostringstream oss;
        oss << "Error opening input file " << pathname;
        throw Exception(oss.str(), __FILE__, __LINE__);
    }

    m_on_open(m, pathname.filename().string());

    std::vector<point_type> positions;
    std::vector<vector_type> normals;
    std::vector<texcoord_type> texcoords;
    size_t n = 0;

    while (!is.eof())
    {
        std::string line;
        getline(is, line);
        ++n;

        auto tokens = tokenize(line);
        if (tokens.empty())
        {
            continue;
        }

        std::string keyword = tokens[0];

        if (keyword == wavefront_obj::Keyword::Position)
        {
            std::array<value_type, 4> v;
            bool b = parse_components(tokens, v, 3, value_type(1)); // w = 1 for homogeneous coordinates
            if (!b)
            {
                std::ostringstream oss;
                oss << "Error reading vertex position in file " << pathname << " line " << n;
                throw Exception(oss.str(), __FILE__, __LINE__);
            }

            if constexpr (point_type::dimension == 3)
            {
                positions.emplace_back(v[0], v[1], v[2]);
            }
            else if constexpr (point_type::dimension == 4)
            {
                positions.emplace_back(v[0], v[1], v[2], v[3]);
            }
        }
        else if (keyword == wavefront_obj::Keyword::Normal)
        {
            std::array<value_type, 3> v;
            bool b = parse_components(tokens, v, 3);
            if (!b)
            {
                std::ostringstream oss;
                oss << "Error reading vertex normal in file " << pathname << " line " << n;
                throw Exception(oss.str(), __FILE__, __LINE__);
            }

            normals.emplace_back(v[0], v[1], v[2]);
        }
        else if (keyword == wavefront_obj::Keyword::Texcoord)
        {
            std::array<value_type, 3> v;
            bool b = parse_components(tokens, v, 2);
            if (!b)
            {
                std::ostringstream oss;
                oss << "Error reading texture coordinates in file " << pathname << " line " << n;
                throw Exception(oss.str(), __FILE__, __LINE__);
            }

            if constexpr (texcoord_type::dimension == 2)
            {
                texcoords.emplace_back(v[0], v[1]);
            }
            else if constexpr (texcoord_type::dimension == 3)
            {
                texcoords.emplace_back(v[0], v[1], v[2]);
            }
        }
        else if (keyword == wavefront_obj::Keyword::Face)
        {
            if (tokens.size() < 4)
            {
                std::ostringstream oss;
                oss << "Error reading face in file " << pathname << " line " << n;
                throw Exception(oss.str(), __FILE__, __LINE__);
            }

            m_on_face_open(m);

            for (size_t i = 1; i < tokens.size(); ++i)
            {
                const std::string& s = tokens[i];

                vertex_reference_type vr;
                bool b = parse_vertex_reference(s, vr);
                if (!b)
                {
                    std::ostringstream oss;
                    oss << "Error reading vertex reference in file " << pathname << " line " << n;
                    throw Exception(oss.str(), __FILE__, __LINE__);
                }

                point_type position = positions[vr[0] - 1];

                vector_type normal;
                if (vr[2] > 0)
                {
                    normal = normals[vr[2] - 1];
                }

                texcoord_type texcoord;
                if (vr[1] > 0)
                {
                    texcoord = texcoords[vr[1] - 1];
                }

                m_on_face_vertex(m, position, normal, texcoord);
            }

            m_on_face_close(m);
        }
        else if (keyword == wavefront_obj::Keyword::Group)
        {
            if (tokens.size() < 2)
            {
                std::cout << "Warning: No parameter for " << wavefront_obj::Keyword::Group << " keyword: " << (tokens.size() - 1) << " found, at least 1 expected." << std::endl;
            }

            m_on_group(m, tokens.size() > 1 && !tokens[1].empty() ? tokens[1] : GroupNameDefault);
        }
        else if (keyword == wavefront_obj::Keyword::Object)
        {
            if (tokens.size() != 2)
            {
                std::cout << "Warning: Wrong number of parameters for " << wavefront_obj::Keyword::Object << " keyword: " << (tokens.size() - 1) << " found, 1 expected." << std::endl;
            }

            m_on_object(m, tokens[1]);
        }
        else if (keyword == wavefront_obj::Keyword::Materials)
        {
            if (tokens.size() != 2)
            {
                std::cout << "Warning: Wrong number of parameters for " << wavefront_obj::Keyword::Materials << " keyword: " << (tokens.size() - 1) << " found, 1 expected." << std::endl;
            }

            m_on_materials(m, tokens[1]);
        }
        else if (keyword == wavefront_obj::Keyword::Material)
        {
            if (tokens.size() != 2)
            {
                std::cout << "Warning: Wrong number of parameters for " << wavefront_obj::Keyword::Material << " keyword: " << (tokens.size() - 1) << " found, 1 expected." << std::endl;
            }

            m_on_material(m, tokens[1]);
        }

        // Comments, missing, unrecognized, and all other keywords ignored ...
    }
}

#endif // QUETZAL_WAVEFRONT_OBJ_READER_HPP
