//------------------------------------------------------------------------------
// direct3d11
// ShaderBytecode.cpp
//------------------------------------------------------------------------------

#include "ShaderBytecode.hpp"

//------------------------------------------------------------------------------
quetzal::direct3d11::ShaderBytecode::ShaderBytecode(const void* data, size_t size) :
    m_data(data),
    m_size(size)
{
}

//------------------------------------------------------------------------------
const void* quetzal::direct3d11::ShaderBytecode::data() const
{
    return m_data;
}

//------------------------------------------------------------------------------
size_t quetzal::direct3d11::ShaderBytecode::size() const
{
    return m_size;
}
