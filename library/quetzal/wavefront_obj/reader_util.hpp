#if !defined(QUETZAL_WAVEFRONT_OBJ_READER_UTIL_HPP)
#define QUETZAL_WAVEFRONT_OBJ_READER_UTIL_HPP
//------------------------------------------------------------------------------
// wavefront_obj
// reader_util.hpp
//------------------------------------------------------------------------------

#include "quetzal/common/string_util.hpp"
#include <array>
#include <string>
#include <vector>
#include <cassert>

namespace quetzal::wavefront_obj
{

    using vertex_reference_type = std::array<long, 3>;

    //--------------------------------------------------------------------------
    enum class empties
    {
        allowed,
        rejected
    };

    std::vector<std::string> tokenize(const std::string& s, std::string delimiters = " ", empties e = empties::rejected);

    bool parse_vertex_reference(const std::string& s, vertex_reference_type& vr);

    template<typename T, size_t N>
    bool parse_components(const std::vector<std::string>& tokens, std::array<T, N>& components, size_t nMin, T w = T(0));

} // namespace quetzal::wavefront_obj

//--------------------------------------------------------------------------
template<typename T, size_t N>
bool quetzal::wavefront_obj::parse_components(const std::vector<std::string>& tokens, std::array<T, N>& components, size_t nMin, T w)
{
    assert(nMin >= 2);

    // First token is the keyword
    if (tokens.size() < (nMin + 1) || tokens.size() > (N + 1))
    {
        return false;
    }

    components[0] = quetzal::to_type<T>(tokens[1]);
    components[1] = quetzal::to_type<T>(tokens[2]);

    if constexpr (N > 2)
    {
        components[2] = tokens.size() > 3 ? quetzal::to_type<T>(tokens[3]) : T(0);
    }

    if constexpr (N > 3)
    {
        components[3] = tokens.size() > 4 ? quetzal::to_type<T>(tokens[4]) : w;
    }

    return true;
}

#endif // QUETZAL_WAVEFRONT_OBJ_READER_UTIL_HPP
