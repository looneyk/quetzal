#if !defined(QUETZAL_WAVEFRONT_OBJ_MATERIALLIBRARY_HPP)
#define QUETZAL_WAVEFRONT_OBJ_MATERIALLIBRARY_HPP
//------------------------------------------------------------------------------
// wavefront_obj
// MaterialLibrary.hpp
//------------------------------------------------------------------------------

#include <filesystem>
#include <fstream>
#include <string>
#include "Material.hpp"

// template class, value_type?

namespace quetzal::wavefront_obj
{

    class Tokens; // ...

    //--------------------------------------------------------------------------
    class MaterialLibrary
    {
    public:

        MaterialLibrary(const std::filesystem::path& pathname);
        MaterialLibrary(const MaterialLibrary&) = default;
        MaterialLibrary(MaterialLibrary&&) = default;
        ~MaterialLibrary() = default;

        MaterialLibrary& operator=(const MaterialLibrary&) = default;
        MaterialLibrary& operator=(MaterialLibrary&&) = default;

        bool eof() const;
        Material read_material();

    private:

        std::ifstream m_ifs;
        std::vector<std::string> m_tokensPrev;
        std::filesystem::path m_pathname;
        size_t m_line;
    };

} // namespace quetzal::wavefront_obj

#endif // QUETZAL_WAVEFRONT_OBJ_MATERIALLIBRARY_HPP
