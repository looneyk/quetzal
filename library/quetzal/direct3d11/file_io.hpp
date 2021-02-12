#if !defined(QUETZAL_DIRECT3D11_FILE_IO_HPP)
#define QUETZAL_DIRECT3D11_FILE_IO_HPP
//------------------------------------------------------------------------------
// file_io.hpp
//------------------------------------------------------------------------------

#include <filesystem>
#include <vector>

namespace quetzal::direct3d11
{

    std::vector<char> read(const std::filesystem::path& pathname);

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_FILE_IO_HPP
