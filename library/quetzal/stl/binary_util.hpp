#if !defined(QUETZAL_STL_BINARY_UTIL_HPP)
#define QUETZAL_STL_BINARY_UTIL_HPP
//------------------------------------------------------------------------------
// stl
// binary_util.hpp
//------------------------------------------------------------------------------

#include "quetzal/math/Vector.hpp"
#include <array>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace quetzal::stl
{

    //--------------------------------------------------------------------------
    float read_float(std::istream& is)
    {
        char c[4];
        is.read(c, 4);
        float f;
        reinterpret_cast<char*>(&f)[0] = c[0];
        reinterpret_cast<char*>(&f)[1] = c[1];
        reinterpret_cast<char*>(&f)[2] = c[2];
        reinterpret_cast<char*>(&f)[3] = c[3];
        return f;
    }

    //--------------------------------------------------------------------------
    uint32_t read_uint32(std::istream& is)
    {
        char c[4];
        is.read(c, 4);
        uint32_t i;
        reinterpret_cast<char*>(&i)[0] = c[0];
        reinterpret_cast<char*>(&i)[1] = c[1];
        reinterpret_cast<char*>(&i)[2] = c[2];
        reinterpret_cast<char*>(&i)[3] = c[3];
        return i;
    }

    //--------------------------------------------------------------------------
    uint16_t read_uint16(std::istream& is)
    {
        char c[2];
        is.read(c, 2);
        uint16_t i;
        reinterpret_cast<char*>(&i)[0] = c[0];
        reinterpret_cast<char*>(&i)[1] = c[1];
        return i;
    }

    //--------------------------------------------------------------------------
    template<typename V>
    V read_vector(std::istream& is)
    {
        assert(V::dimension == 3);
        typename V::value_type x = read_float(is);
        typename V::value_type y = read_float(is);
        typename V::value_type z = read_float(is);
        return {x, y, z};
    }

    //--------------------------------------------------------------------------
    void write_float(std::ostream& os, float f)
    {
        char c[4];
        c[0] = reinterpret_cast<char*>(&f)[0];
        c[1] = reinterpret_cast<char*>(&f)[1];
        c[2] = reinterpret_cast<char*>(&f)[2];
        c[3] = reinterpret_cast<char*>(&f)[3];
        os.write(c, 4);
        return;
    }

    //--------------------------------------------------------------------------
    void write_uint32(std::ostream& os, size_t i)
    {
        char c[4];
        c[0] = reinterpret_cast<char*>(&i)[0];
        c[1] = reinterpret_cast<char*>(&i)[1];
        c[2] = reinterpret_cast<char*>(&i)[2];
        c[3] = reinterpret_cast<char*>(&i)[3];
        os.write(c, 4);
        return;
    }

    //--------------------------------------------------------------------------
    void write_uint16(std::ostream& os, unsigned int i)
    {
        uint16_t ui16 = static_cast<uint16_t>(i);
        char c[2];
        c[0] = reinterpret_cast<char*>(&ui16)[0];
        c[1] = reinterpret_cast<char*>(&ui16)[1];
        os.write(c, 2);
        return;
    }

    //--------------------------------------------------------------------------
    template<typename V>
    void write_vector(std::ostream& os, const V& v)
    {
        assert(V::dimension == 3);
        write_float(os, v.x());
        write_float(os, v.y());
        write_float(os, v.z());
        return;
    }

} // namespace quetzal::stl

#endif // QUETZAL_STL_BINARY_UTIL_HPP
