//------------------------------------------------------------------------------
// wavefront_obj
// MaterialLibrary.hpp
//------------------------------------------------------------------------------

#include <iostream>
#include "MaterialLibrary.hpp"
#include "reader_util.hpp"
#include "symbols.hpp"

using namespace std;

//------------------------------------------------------------------------------
quetzal::wavefront_obj::MaterialLibrary::MaterialLibrary(const filesystem::path& pathname) :
    m_ifs(pathname),
    m_tokensPrev(),
    m_pathname(pathname),
    m_line(1)
{
}

//------------------------------------------------------------------------------
bool quetzal::wavefront_obj::MaterialLibrary::eof() const
{
    return m_ifs.eof();
}

//------------------------------------------------------------------------------
quetzal::wavefront_obj::Material quetzal::wavefront_obj::MaterialLibrary::read_material()
{
    Material material;
    bool bMaterial = false;

    if (!m_tokensPrev.empty())
    {
        assert(m_tokensPrev[0] == Keyword::Newmtl && m_tokensPrev.size() >= 2);
        material.m_name = m_tokensPrev[1];
        bMaterial = true;
        m_tokensPrev.clear();
    }

    while (!m_ifs.eof())
    {
        std::string line;
        getline(m_ifs, line);
        ++m_line;

        vector<string> tokens = tokenize(line);
        if (tokens.empty() || string(1, tokens[0][0]) == Keyword::Comment)
        {
            continue;
        }

        if (tokens[0] == Keyword::Newmtl)
        {
            // validate ...
            if (bMaterial)
            {
                m_tokensPrev = tokens;
                break;
            }
            else
            {
                material.m_name = tokens[1];
                bMaterial = true;
                continue;
            }
        }

        assert(bMaterial);
        material.set(tokens, m_pathname.string(), m_line);
    }

    return material;
}
