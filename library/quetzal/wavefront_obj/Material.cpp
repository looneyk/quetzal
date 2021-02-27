//------------------------------------------------------------------------------
// wavefront_obj
// Material.cpp
//------------------------------------------------------------------------------

#include "Material.hpp"
#include "symbols.hpp"
#include <functional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include <cassert>

using namespace std;
using namespace quetzal;
using namespace quetzal::wavefront_obj;

namespace
{

    class Statement
    {
    public:

        function<void(const vector<string>&)> validate;
        function<void(const vector<string>&, Material&)> apply;
    };

    unordered_map<string, Statement> statements =
    {
        {
            Keyword::Newmtl,
            {
                [](const vector<string>& tokens) -> void
                {
                    assert(tokens[0] == Keyword::Newmtl);
                    size_t nArgsMin = 2;
                    if (tokens.size() < nArgsMin)
                    {
                        ostringstream oss;
                        oss << "Too few arguments for " << Keyword::Newmtl << " statement: " << (tokens.size() - 1) << " / " << (nArgsMin - 1);
                        throw runtime_error(oss.str());
                    }
                },
                [](const vector<string>& tokens, Material& material) -> void
                {
                    material.m_name = tokens.back();
                }
            }
        },
        {
            Keyword::AmbientColor,
            {
                [](const vector<string>& tokens) -> void
                {
                    assert(tokens[0] == Keyword::AmbientColor);
                    size_t nArgsMin = 4;
                    if (tokens.size() < nArgsMin)
                    {
                        ostringstream oss;
                        oss << "Too few arguments for " << Keyword::AmbientColor << " statement: " << (tokens.size() - 1) << " / " << (nArgsMin - 1);
                        throw runtime_error(oss.str());
                    }
                },
                [](const vector<string>& tokens, Material& material) -> void
                {
                    if (tokens.size() == 4)
                    {
                        material.m_ambientColor = {stof(tokens[1]), stof(tokens[2]), stof(tokens[3])};
                    }
                },
            }
        },
        {
            Keyword::DiffuseColor,
            {
                [](const vector<string>& tokens) -> void
                {
                    assert(tokens[0] == Keyword::DiffuseColor);
                    size_t nArgsMin = 4;
                    if (tokens.size() < nArgsMin)
                    {
                        ostringstream oss;
                        oss << "Too few arguments for " << Keyword::DiffuseColor << " statement: " << (tokens.size() - 1) << " / " << (nArgsMin - 1);
                        throw runtime_error(oss.str());
                    }
                },
                [](const vector<string>& tokens, Material& material) -> void
                {
                    if (tokens.size() == 4)
                    {
                        material.m_diffuseColor = {stof(tokens[1]), stof(tokens[2]), stof(tokens[3])};
                    }
                },
            }
        },
        {
            Keyword::SpecularColor,
            {
                [](const vector<string>& tokens) -> void
                {
                    assert(tokens[0] == Keyword::SpecularColor);
                    size_t nArgsMin = 4;
                    if (tokens.size() < nArgsMin)
                    {
                        ostringstream oss;
                        oss << "Too few arguments for " << Keyword::SpecularColor << " statement: " << (tokens.size() - 1) << " / " << (nArgsMin - 1);
                        throw runtime_error(oss.str());
                    }
                },
                [](const vector<string>& tokens, Material& material) -> void
                {
                    if (tokens.size() == 4)
                    {
                        material.m_specularColor = {stof(tokens[1]), stof(tokens[2]), stof(tokens[3])};
                    }
                },
            }
        },
        {
            Keyword::EmissiveColor,
            {
                [](const vector<string>& tokens) -> void
                {
                    assert(tokens[0] == Keyword::EmissiveColor);
                    size_t nArgsMin = 4;
                    if (tokens.size() < nArgsMin)
                    {
                        ostringstream oss;
                        oss << "Too few arguments for " << Keyword::EmissiveColor << " statement: " << (tokens.size() - 1) << " / " << (nArgsMin - 1);
                        throw runtime_error(oss.str());
                    }
                },
                [](const vector<string>& tokens, Material& material) -> void
                {
                    if (tokens.size() == 4)
                    {
                        material.m_emissiveColor = {stof(tokens[1]), stof(tokens[2]), stof(tokens[3])};
                    }
                },
            }
        },
        {
            Keyword::TransmissionFilter,
            {
                [](const vector<string>& tokens) -> void
                {
                    assert(tokens[0] == Keyword::TransmissionFilter);
                    size_t nArgsMin = 4;
                    if (tokens.size() < nArgsMin)
                    {
                        ostringstream oss;
                        oss << "Too few arguments for " << Keyword::TransmissionFilter << " statement: " << (tokens.size() - 1) << " / " << (nArgsMin - 1);
                        throw runtime_error(oss.str());
                    }
                },
                [](const vector<string>& tokens, Material& material) -> void
                {
                    tokens;
                    material;
                },
            }
        },
        {
            Keyword::IlluminationModel,
            {
                [](const vector<string>& tokens) -> void
                {
                    assert(tokens[0] == Keyword::IlluminationModel);
                    size_t nArgsMin = 2;
                    if (tokens.size() < nArgsMin)
                    {
                        ostringstream oss;
                        oss << "Too few arguments for " << Keyword::IlluminationModel << " statement: " << (tokens.size() - 1) << " / " << (nArgsMin - 1);
                        throw runtime_error(oss.str());
                    }
                },
                [](const vector<string>& tokens, Material& material) -> void
                {
                    tokens;
                    material;
                },
            }
        },
        {
            Keyword::Dissolve,
            {
                [](const vector<string>& tokens) -> void
                {
                    assert(tokens[0] == Keyword::Dissolve);
                    size_t nArgsMin = 2;
                    if (tokens.size() < nArgsMin)
                    {
                        ostringstream oss;
                        oss << "Too few arguments for " << Keyword::Dissolve << " statement: " << (tokens.size() - 1) << " / " << (nArgsMin - 1);
                        throw runtime_error(oss.str());
                    }
                },
                [](const vector<string>& tokens, Material& material) -> void
                {
                    material.m_alpha = stof(tokens.back());
                },
            }
        },
        {
            Keyword::SpecularExponent,
            {
                [](const vector<string>& tokens) -> void
                {
                    assert(tokens[0] == Keyword::SpecularExponent);
                    size_t nArgsMin = 2;
                    if (tokens.size() < nArgsMin)
                    {
                        ostringstream oss;
                        oss << "Too few arguments for " << Keyword::SpecularExponent << " statement: " << (tokens.size() - 1) << " / " << (nArgsMin - 1);
                        throw runtime_error(oss.str());
                    }
                },
                [](const vector<string>& tokens, Material& material) -> void
                {
                    material.m_specularExponent = stof(tokens.back());
                },
            }
        },
        {
            Keyword::Sharpness,
            {
                [](const vector<string>& tokens) -> void
                {
                    assert(tokens[0] == Keyword::Sharpness);
                    size_t nArgsMin = 2;
                    if (tokens.size() < nArgsMin)
                    {
                        ostringstream oss;
                        oss << "Too few arguments for " << Keyword::Sharpness << " statement: " << (tokens.size() - 1) << " / " << (nArgsMin - 1);
                        throw runtime_error(oss.str());
                    }
                },
                [](const vector<string>& tokens, Material& material) -> void
                {
                    tokens;
                    material;
                },
            }
        },
        {
            Keyword::OpticalDensity,
            {
                [](const vector<string>& tokens) -> void
                {
                    assert(tokens[0] == Keyword::OpticalDensity);
                    size_t nArgsMin = 2;
                    if (tokens.size() < nArgsMin)
                    {
                        ostringstream oss;
                        oss << "Too few arguments for " << Keyword::OpticalDensity << " statement: " << (tokens.size() - 1) << " / " << (nArgsMin - 1);
                        throw runtime_error(oss.str());
                    }
                },
                [](const vector<string>& tokens, Material& material) -> void
                {
                    tokens;
                    material;
                },
            }
        },
        {
            Keyword::AmbientTexture,
            {
                [](const vector<string>& tokens) -> void
                {
                    assert(tokens[0] == Keyword::AmbientTexture);
                    size_t nArgsMin = 2;
                    if (tokens.size() < nArgsMin)
                    {
                        ostringstream oss;
                        oss << "Too few arguments for " << Keyword::AmbientTexture << " statement: " << (tokens.size() - 1) << " / " << (nArgsMin - 1);
                        throw runtime_error(oss.str());
                    }
                },
                [](const vector<string>& tokens, Material& material) -> void
                {
                    material.m_ambientTextureName = tokens.back();
                },
            }
        },
        {
            Keyword::DiffuseTexture,
            {
                [](const vector<string>& tokens) -> void
                {
                    assert(tokens[0] == Keyword::DiffuseTexture);
                    size_t nArgsMin = 2;
                    if (tokens.size() < nArgsMin)
                    {
                        ostringstream oss;
                        oss << "Too few arguments for " << Keyword::DiffuseTexture << " statement: " << (tokens.size() - 1) << " / " << (nArgsMin - 1);
                        throw runtime_error(oss.str());
                    }
                },
                [](const vector<string>& tokens, Material& material) -> void
                {
                    material.m_diffuseTextureName = tokens.back();
                },
            }
        },
        {
            Keyword::SpecularTexture,
            {
                [](const vector<string>& tokens) -> void
                {
                    assert(tokens[0] == Keyword::SpecularTexture);
                    size_t nArgsMin = 2;
                    if (tokens.size() < nArgsMin)
                    {
                        ostringstream oss;
                        oss << "Too few arguments for " << Keyword::SpecularTexture << " statement: " << (tokens.size() - 1) << " / " << (nArgsMin - 1);
                        throw runtime_error(oss.str());
                    }
                },
                [](const vector<string>& tokens, Material& material) -> void
                {
                    material.m_specularTextureName = tokens.back();
                },
            }
        },
        {
            Keyword::EmissiveTexture,
            {
                [](const vector<string>& tokens) -> void
                {
                    assert(tokens[0] == Keyword::EmissiveTexture);
                    size_t nArgsMin = 2;
                    if (tokens.size() < nArgsMin)
                    {
                        ostringstream oss;
                        oss << "Too few arguments for " << Keyword::EmissiveTexture << " statement: " << (tokens.size() - 1) << " / " << (nArgsMin - 1);
                        throw runtime_error(oss.str());
                    }
                },
                [](const vector<string>& tokens, Material& material) -> void
                {
                    material.m_emissiveTextureName = tokens.back();
                },
            }
        },
        {
            Keyword::SpecularExponentMap,
            {
                [](const vector<string>& tokens) -> void
                {
                    assert(tokens[0] == Keyword::SpecularExponentMap);
                    size_t nArgsMin = 2;
                    if (tokens.size() < nArgsMin)
                    {
                        ostringstream oss;
                        oss << "Too few arguments for " << Keyword::SpecularExponentMap << " statement: " << (tokens.size() - 1) << " / " << (nArgsMin - 1);
                        throw runtime_error(oss.str());
                    }
                },
                [](const vector<string>& tokens, Material& material) -> void
                {
                    tokens;
                    material;
                },
            }
        },
        {
            Keyword::DissolveMap,
            {
                [](const vector<string>& tokens) -> void
                {
                    assert(tokens[0] == Keyword::DissolveMap);
                    size_t nArgsMin = 2;
                    if (tokens.size() < nArgsMin)
                    {
                        ostringstream oss;
                        oss << "Too few arguments for " << Keyword::DissolveMap << " statement: " << (tokens.size() - 1) << " / " << (nArgsMin - 1);
                        throw runtime_error(oss.str());
                    }
                },
                [](const vector<string>& tokens, Material& material) -> void
                {
                    tokens;
                    material;
                },
            }
        },
        {
            Keyword::DisplacementMap,
            {
                [](const vector<string>& tokens) -> void
                {
                    assert(tokens[0] == Keyword::DisplacementMap);
                    size_t nArgsMin = 2;
                    if (tokens.size() < nArgsMin)
                    {
                        ostringstream oss;
                        oss << "Too few arguments for " << Keyword::DisplacementMap << " statement: " << (tokens.size() - 1) << " / " << (nArgsMin - 1);
                        throw runtime_error(oss.str());
                    }
                },
                [](const vector<string>& tokens, Material& material) -> void
                {
                    tokens;
                    material;
                },
            }
        },
        {
            Keyword::Decal,
            {
                [](const vector<string>& tokens) -> void
                {
                    assert(tokens[0] == Keyword::Decal);
                    size_t nArgsMin = 2;
                    if (tokens.size() < nArgsMin)
                    {
                        ostringstream oss;
                        oss << "Too few arguments for " << Keyword::Decal << " statement: " << (tokens.size() - 1) << " / " << (nArgsMin - 1);
                        throw runtime_error(oss.str());
                    }
                },
                [](const vector<string>& tokens, Material& material) -> void
                {
                    tokens;
                    material;
                },
            }
        },
        {
            Keyword::BumpMap,
            {
                [](const vector<string>& tokens) -> void
                {
                    assert(tokens[0] == Keyword::BumpMap);
                    size_t nArgsMin = 2;
                    if (tokens.size() < nArgsMin)
                    {
                        ostringstream oss;
                        oss << "Too few arguments for " << Keyword::BumpMap << " statement: " << (tokens.size() - 1) << " / " << (nArgsMin - 1);
                        throw runtime_error(oss.str());
                    }
                },
                [](const vector<string>& tokens, Material& material) -> void
                {
                    tokens;
                    material;
                },
            }
        },
        {
            Keyword::NormalMap,
            {
                [](const vector<string>& tokens) -> void
                {
                    assert(tokens[0] == Keyword::NormalMap);
                    size_t nArgsMin = 2;
                    if (tokens.size() < nArgsMin)
                    {
                        ostringstream oss;
                        oss << "Too few arguments for " << Keyword::NormalMap << " statement: " << (tokens.size() - 1) << " / " << (nArgsMin - 1);
                        throw runtime_error(oss.str());
                    }
                },
                [](const vector<string>& tokens, Material& material) -> void
                {
                    material.m_normalMapName = tokens.back();
                },
            }
        },
        {
            Keyword::ReflectionMap,
            {
                [](const vector<string>& tokens) -> void
                {
                    assert(tokens[0] == Keyword::ReflectionMap);
                    size_t nArgsMin = 2;
                    if (tokens.size() < nArgsMin)
                    {
                        ostringstream oss;
                        oss << "Too few arguments for " << Keyword::ReflectionMap << " statement: " << (tokens.size() - 1) << " / " << (nArgsMin - 1);
                        throw runtime_error(oss.str());
                    }
                },
                [](const vector<string>& tokens, Material& material) -> void
                {
                    tokens;
                    material;
                },
            }
        },
    };

} // namespace

//------------------------------------------------------------------------------
void quetzal::wavefront_obj::Material::set(const vector<string>& tokens, const std::string& filename, size_t line)
{
    assert(tokens.size() >= 1);

    if (!statements.contains(tokens[0]))
    {
        ostringstream oss;
        oss << "Invalid material statement: " << tokens[0] << " in file " << filename << " line " << line;
        throw(runtime_error(oss.str()));
    }

    auto statement = statements[tokens[0]];
    statement.validate(tokens);
    statement.apply(tokens, *this);
    return;
}

/*
Material color and illumination statements:
	Ka 0.0435 0.0435 0.0435
	Kd 0.1086 0.1086 0.1086
	Ks 0.0000 0.0000 0.0000
	Tf 0.9885 0.9885 0.9885
	illum 6
	d -halo 0.6600
	Ns 10.0000
	sharpness 60
	Ni 1.19713

ambient reflectivity
diffuse reflectivity
specular reflectivity
transmission filter

r g b
spectral factor
x y z

illumination model

dissolve - halo factor

specular exponent

sharpness 1..1000/60

optical density/index of refraction


Texture map statements:
	map_Ka -s 1 1 1 -o 0 0 0 -mm 0 1 chrome.mpc
	map_Kd -s 1 1 1 -o 0 0 0 -mm 0 1 chrome.mpc
	map_Ks -s 1 1 1 -o 0 0 0 -mm 0 1 chrome.mpc
	map_Ns -s 1 1 1 -o 0 0 0 -mm 0 1 wisp.mps
	map_d -s 1 1 1 -o 0 0 0 -mm 0 1 wisp.mps
	disp -s 1 1 .5 wisp.mps
	decal -s 1 1 1 -o 0 0 0 -mm 0 1 sand.mps
	bump -s 1 1 1 -o 0 0 0 -bm 1 sand.mpb

-blendu on | off
-blendv on | off
-bm mult
-boost value
-cc on | off
-clamp on | off
-imfchan r | g | b | m | l | z
-mm base gain
-o u v w
-s u v w
-t u v w
-texres value


Reflection map statement:
	refl -type sphere -mm 0 1 clouds.mpc


struct EffectInfo
{
    EffectInfo();

    std::string m_name;
    bool m_bVertexColor;
    bool m_bSkinning;
    bool m_bDualTexture;
    bool m_bNormalMap;
    bool m_bBiasedVertexNormals;
    float m_specularPower;
    float m_alpha;
    DirectX::XMFLOAT3 m_colorAmbient;
    DirectX::XMFLOAT3 m_colorDiffuse;
    DirectX::XMFLOAT3 m_colorSpecular;
    DirectX::XMFLOAT3 m_colorEmissive;
    std::string m_diffuseTextureName;
    std::string m_specularTextureName;
    std::string m_normalTextureName;
};
*/
