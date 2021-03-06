/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/direct3d/buffers_d3d.hpp"

#include "core/engine.hpp"
#include "platform/window.hpp"
#include "platform/windows/error_checks_macros_win.hpp"
#include "renderer/vertex_array.hpp"

#include <d3d11.h>

namespace jng {

    Direct3DVertexBuffer::Direct3DVertexBuffer(const void* vertices, size_t size) :
        m_graphicsContext{ reinterpret_cast<const Direct3DGraphicsContext*>(Engine::get().getWindow().getGraphicsContext()) }
    {
        const auto& device = m_graphicsContext->getDevice();

        D3D11_BUFFER_DESC bd{};
        bd.ByteWidth = static_cast<UINT>(size);
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA data{};
        data.pSysMem = vertices;

        [[ maybe_unused ]] HRESULT hr = device->CreateBuffer(&bd, &data, &m_buffer);
        JNG_D3D_CHECK_HR(hr);
    }

    Direct3DVertexBuffer::Direct3DVertexBuffer(size_t size) :
        m_graphicsContext{ reinterpret_cast<const Direct3DGraphicsContext*>(Engine::get().getWindow().getGraphicsContext()) }
    {
        const auto& device = m_graphicsContext->getDevice();

        D3D11_BUFFER_DESC bd{};
        bd.ByteWidth = static_cast<UINT>(size);
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        [[ maybe_unused ]] HRESULT hr = device->CreateBuffer(&bd, nullptr, &m_buffer);
        JNG_D3D_CHECK_HR(hr);
    }

    // NOTE: this needs to be here for com::wrl to work
    Direct3DVertexBuffer::~Direct3DVertexBuffer() = default;

    void Direct3DVertexBuffer::bind() const
    {
        const auto& deviceContext = m_graphicsContext->getDeviceContext();
        
        UINT strides[1] = { m_layout->getStride() };
        UINT offsets[1] = { 0 };
        deviceContext->IASetVertexBuffers(0, 1, m_buffer.GetAddressOf(), strides, offsets);
    }

    void Direct3DVertexBuffer::setData(const void* data, size_t size) const
    {
        const auto& deviceContext = m_graphicsContext->getDeviceContext();

        D3D11_MAPPED_SUBRESOURCE mappedResource{};
        [[ maybe_unused ]] HRESULT hr = deviceContext->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        JNG_D3D_CHECK_HR(hr);
        memcpy(mappedResource.pData, data, size);
        deviceContext->Unmap(m_buffer.Get(), 0);
    }

    Direct3DIndexBuffer::Direct3DIndexBuffer(const uint32* indices, uint32 count) :
        m_graphicsContext{ reinterpret_cast<const Direct3DGraphicsContext*>(Engine::get().getWindow().getGraphicsContext()) },
        m_count(count)
    {
        const auto& device = m_graphicsContext->getDevice();

        D3D11_BUFFER_DESC bd{};
        bd.ByteWidth = static_cast<UINT>(count * sizeof(uint32));
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA data{};
        data.pSysMem = indices;

        [[ maybe_unused ]] HRESULT hr = device->CreateBuffer(&bd, &data, &m_buffer);
        JNG_D3D_CHECK_HR(hr);
    }

    // NOTE: this needs to be here for com::wrl to work
    Direct3DIndexBuffer::~Direct3DIndexBuffer() = default;

    void Direct3DIndexBuffer::bind() const
    {
        const auto& deviceContext = m_graphicsContext->getDeviceContext();

        deviceContext->IASetIndexBuffer(m_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    }

    Direct3DUniformBuffer::Direct3DUniformBuffer(size_t size) :
        m_graphicsContext{ reinterpret_cast<const Direct3DGraphicsContext*>(Engine::get().getWindow().getGraphicsContext()) }
    {
        D3D11_BUFFER_DESC cbd;
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.Usage = D3D11_USAGE_DYNAMIC;
        cbd.CPUAccessFlags =  D3D11_CPU_ACCESS_WRITE;
        cbd.MiscFlags = 0u;
        cbd.ByteWidth = static_cast<UINT>(size);
        cbd.StructureByteStride = 0u;

        const auto& device = m_graphicsContext->getDevice();
        [[ maybe_unused ]] HRESULT hr = device->CreateBuffer(&cbd, nullptr, &m_buffer);
        JNG_D3D_CHECK_HR(hr);
    }

    // NOTE: this needs to be here for com::wrl to work
    Direct3DUniformBuffer::~Direct3DUniformBuffer() = default;

    void Direct3DUniformBuffer::bind(uint32 slot) const
    {
        const auto& deviceContext = m_graphicsContext->getDeviceContext();
        deviceContext->VSSetConstantBuffers(slot, 1u, m_buffer.GetAddressOf());
    }

    void Direct3DUniformBuffer::setData(const void* data, size_t size, size_t offset) const
    {
        const auto& deviceContext = m_graphicsContext->getDeviceContext();

        D3D11_MAPPED_SUBRESOURCE mappedResource{};
        [[ maybe_unused ]] HRESULT hr = deviceContext->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        JNG_D3D_CHECK_HR(hr);
        memcpy(reinterpret_cast<void*>((uintptr_t)mappedResource.pData + (ptrdiff_t)offset), data, size);
        deviceContext->Unmap(m_buffer.Get(), 0);
    }

} // namespace jng
