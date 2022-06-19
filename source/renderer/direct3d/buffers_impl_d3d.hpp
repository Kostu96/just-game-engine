/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/buffers.hpp"
#include "platform/windows/windows_base.hpp"
#include "platform/graphics_context_d3d.hpp"

#pragma warning(disable:4265)
#include <wrl.h>
#pragma warning(default:4265)

struct ID3D11Buffer;

namespace k2d {

    namespace wrl = Microsoft::WRL;

    class VertexBufferImpl
    {
    public:
        VertexBufferImpl(const void* vertices, size_t size);
        VertexBufferImpl(size_t size);
        ~VertexBufferImpl();

        void bind() const;
        void unbind() const;
        void setData(const void* data, size_t size) const;
    private:
        const Direct3DGraphicsContext& m_graphicsContext;
        wrl::ComPtr<ID3D11Buffer> m_buffer;
    };

    class IndexBufferImpl
    {
    public:
        IndexBufferImpl(uint32* indices, uint32 count);
        ~IndexBufferImpl();

        void bind() const;
        void unbind() const;
        uint32 getCount() const { return m_count; }
    private:
        const Direct3DGraphicsContext& m_graphicsContext;
        wrl::ComPtr<ID3D11Buffer> m_buffer;
        uint32 m_count;
    };

} // namespace k2d
