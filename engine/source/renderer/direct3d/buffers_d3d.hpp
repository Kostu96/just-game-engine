/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/buffers.hpp"
#include "platform/windows/windows_base.hpp"
#include "platform/windows/graphics_context_d3d.hpp"

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
        virtual ~Direct3DVertexBuffer();

        void bind() const override;
        void unbind() const override {}
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
        virtual ~Direct3DIndexBuffer();

        void bind() const override;
        void unbind() const override {}
        uint32 getCount() const override { return m_count; }
    private:
        const Direct3DGraphicsContext* m_graphicsContext;
        wrl::ComPtr<ID3D11Buffer> m_buffer;
        uint32 m_count;
    };

    class Direct3DUniformBuffer :
        public UniformBuffer
    {
    public:
        explicit Direct3DUniformBuffer(size_t size);
        virtual ~Direct3DUniformBuffer();

        void bind(uint32 slot) const override;
        void unbind(uint32 /*slot*/) const override {}
        void setData(const void* data, size_t size, size_t offset = 0) const override;
    private:
        const Direct3DGraphicsContext* m_graphicsContext;
        wrl::ComPtr<ID3D11Buffer> m_buffer;
    };

} // namespace jng
