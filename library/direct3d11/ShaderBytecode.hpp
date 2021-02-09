#if !defined(QUETZAL_DIRECT3D11_SHADERBYTECODE_HPP)
#define QUETZAL_DIRECT3D11_SHADERBYTECODE_HPP
//------------------------------------------------------------------------------
// direct3d11
// ShaderBytecode.hpp
//------------------------------------------------------------------------------

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    // Points to a precompiled vertex or pixel shader program
    class ShaderBytecode
    {
    public:

        ShaderBytecode(const void* data, size_t size);

        const void* data() const;
        size_t size() const;

    private:

        const void* m_data;
        size_t m_size;
    };

} // namespace quetzal::direct3d11

#endif // QUETZAL_DIRECT3D11_SHADERBYTECODE_HPP
