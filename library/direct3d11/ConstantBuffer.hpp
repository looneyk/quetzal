#if !defined(QUETZAL_DIRECT3D11_CONSTANTBUFFER_HPP)
#define QUETZAL_DIRECT3D11_CONSTANTBUFFER_HPP
//------------------------------------------------------------------------------
// direct3d11
// ConstantBuffer.hpp
//------------------------------------------------------------------------------

#include "Renderer.hpp"
#include "common/ComException.hpp"
#include "common/com_ptr.hpp"
#include "DirectXHelpers.h"
#include <d3d11_1.h>

namespace quetzal::direct3d11
{

    //--------------------------------------------------------------------------
    // Strongly typed wrapper around a D3D constant buffer.
    template<typename T>
    class ConstantBuffer
    {
    public:

        ConstantBuffer() = default;
        explicit ConstantBuffer(Renderer& renderer);
        ConstantBuffer(const ConstantBuffer&) = delete;
        ConstantBuffer(ConstantBuffer&&) = delete;
        ~ConstantBuffer() = default;

        ConstantBuffer& operator=(const ConstantBuffer&) = delete;
        ConstantBuffer& operator=(ConstantBuffer&&) = delete;

        void create(Renderer& renderer);

        // Writes new data into the constant buffer.
        void set(ID3D11DeviceContext* pDeviceContext, T const& value);

        ID3D11Buffer* buffer();

    private:

        com_ptr<ID3D11Buffer> m_pBuffer;
    };

} // namespace quetzal::direct3d11

//----------------------------------------------------------------------
template<typename T>
quetzal::direct3d11::ConstantBuffer<T>::ConstantBuffer(Renderer& renderer)
{
    create(renderer);
}

//----------------------------------------------------------------------
template<typename T>
void quetzal::direct3d11::ConstantBuffer<T>::create(Renderer& renderer)
{
    HRESULT hr;

    D3D11_BUFFER_DESC desc = {};

    desc.ByteWidth = sizeof(T);
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HR_CHECK(renderer.device()->CreateBuffer(&desc, nullptr, &m_pBuffer.p()));
    DirectX::SetDebugObjectName(m_pBuffer.get(), "Buffer");
    return;
}

//----------------------------------------------------------------------
template<typename T>
void quetzal::direct3d11::ConstantBuffer<T>::set(ID3D11DeviceContext* pDeviceContext, T const& value)
{
    assert(pDeviceContext);
    assert(m_pBuffer);

    HRESULT hr;

    D3D11_MAPPED_SUBRESOURCE mappedResource;

    HR_CHECK(pDeviceContext->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

    *(T*)mappedResource.pData = value;

    pDeviceContext->Unmap(m_pBuffer, 0);
    return;
}

//----------------------------------------------------------------------
template<typename T>
ID3D11Buffer* quetzal::direct3d11::ConstantBuffer<T>::buffer()
{
    return m_pBuffer;
}

#endif // QUETZAL_DIRECT3D11_CONSTANTBUFFER_HPP
