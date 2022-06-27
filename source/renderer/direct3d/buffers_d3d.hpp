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

namespace jng {

    class VertexLayout;

    namespace wrl = Microsoft::WRL;

    class Direct3DVertexBuffer :
        public VertexBuffer
    {
    public:
        Direct3DVertexBuffer(const void* vertices, size_t size);
        Direct3DVertexBuffer(size_t size);
        ~Direct3DVertexBuffer();

        void bind() const override;
        void unbind() const override;
        void setData(const void* data, size_t size) const override;

        void setVertexLayout(const VertexLayout& layout) { m_layout = &layout; }
    private:
        const Direct3DGraphicsContext* m_graphicsContext;
        wrl::ComPtr<ID3D11Buffer> m_buffer;
        const VertexLayout* m_layout = nullptr;
    };

    class Direct3DIndexBuffer :
        public IndexBuffer
    {
    public:
        Direct3DIndexBuffer(const uint32* indices, uint32 count);
        ~Direct3DIndexBuffer();

        void bind() const override;
        void unbind() const override;
        uint32 getCount() const override { return m_count; }
    private:
        const Direct3DGraphicsContext* m_graphicsContext;
        wrl::ComPtr<ID3D11Buffer> m_buffer;
        uint32 m_count;
    };

} // namespace jng
