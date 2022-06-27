/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/direct3d/buffers_d3d.hpp"

#include "core/engine.hpp"
#include "platform/window.hpp"
#include "platform/windows/error_checks_macros_win.hpp"

#include <d3d11.h>

namespace jng {

    Direct3DVertexBuffer::Direct3DVertexBuffer(const void* vertices, size_t size) :
        m_graphicsContext{ reinterpret_cast<const Direct3DGraphicsContext*>(Engine::get().getWindow().getGraphicsContext()) }
    {
        HRESULT hr;
        const auto& device = m_graphicsContext->getNativeDevice();

        D3D11_BUFFER_DESC bd{};
        bd.ByteWidth = static_cast<UINT>(size);
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        // NOTE: stride left 0

        D3D11_SUBRESOURCE_DATA data{};
        data.pSysMem = vertices;

        hr = device->CreateBuffer(&bd, &data, &m_buffer);
        JNG_D3D_CHECK_HR(hr);
    }

    Direct3DVertexBuffer::Direct3DVertexBuffer(size_t size) :
        m_graphicsContext{ reinterpret_cast<const Direct3DGraphicsContext*>(Engine::get().getWindow().getGraphicsContext()) }
    {
        HRESULT hr;
        const auto& device = m_graphicsContext->getNativeDevice();

        D3D11_BUFFER_DESC bd{};
        bd.ByteWidth = static_cast<UINT>(size);
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        // NOTE: stride left 0

        hr = device->CreateBuffer(&bd, nullptr, &m_buffer);
        JNG_D3D_CHECK_HR(hr);
    }

    // NOTE: this needs to be here for com::wrl to work
    Direct3DVertexBuffer::~Direct3DVertexBuffer() = default;

    void Direct3DVertexBuffer::bind() const
    {
        const auto& deviceContext = m_graphicsContext->getNativeDeviceContext();
        
        UINT strides[1] = { /*m_layout.getStride()*/ };
        UINT offsets[] = { 0 };
        deviceContext->IASetVertexBuffers(0, 1, m_buffer.GetAddressOf(), strides, offsets);
    }

    void Direct3DVertexBuffer::unbind() const
    {
        // TODO: implement
    }

    void Direct3DVertexBuffer::setData(const void* data, size_t size) const
    {
        HRESULT hr;
        const auto& deviceContext = m_graphicsContext->getNativeDeviceContext();

        D3D11_MAPPED_SUBRESOURCE mappedResource{};
        hr = deviceContext->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        JNG_D3D_CHECK_HR(hr);
        memcpy(mappedResource.pData, data, size);
        deviceContext->Unmap(m_buffer.Get(), 0);
    }

    Direct3DIndexBuffer::Direct3DIndexBuffer(const uint32* indices, uint32 count) :
        m_graphicsContext{ reinterpret_cast<const Direct3DGraphicsContext*>(Engine::get().getWindow().getGraphicsContext()) },
        m_count(count)
    {
        HRESULT hr;
        const auto& device = m_graphicsContext->getNativeDevice();

        D3D11_BUFFER_DESC bd{};
        bd.ByteWidth = static_cast<UINT>(count * sizeof(uint32));
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        // NOTE: stride left 0

        D3D11_SUBRESOURCE_DATA data{};
        data.pSysMem = indices;

        hr = device->CreateBuffer(&bd, &data, &m_buffer);
        JNG_D3D_CHECK_HR(hr);
    }

    // NOTE: this needs to be here for com::wrl to work
    Direct3DIndexBuffer::~Direct3DIndexBuffer() = default;

    void Direct3DIndexBuffer::bind() const
    {
        const auto& deviceContext = m_graphicsContext->getNativeDeviceContext();

        deviceContext->IASetIndexBuffer(m_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    }

    void Direct3DIndexBuffer::unbind() const
    {
        // TODO: implement
    }

} // namespace jng
