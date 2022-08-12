/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"
#include "jng/renderer/renderer_api.hpp"

namespace jng {

    class VertexBuffer final
    {
    public:
        VertexBuffer(const void* vertices, size_t size);
        explicit VertexBuffer(size_t size);
        ~VertexBuffer();

        void bind() const;
        void unbind() const;
        void setData(const void* data, size_t size) const;
    private:
        u32 m_id;
    };

    class IndexBuffer final
    {
    public:
        IndexBuffer(const void* indices, u32 count, RendererAPI::IndexType type);
        ~IndexBuffer();

        void bind() const;
        void unbind() const;
        u32 getCount() const { return m_count; }
        RendererAPI::IndexType getIndexType() const { return m_indexType; }
    private:
        u32 m_id;
        u32 m_count;
        RendererAPI::IndexType m_indexType;
    };

    class UniformBuffer final
    {
    public:
        explicit UniformBuffer(size_t size);
        ~UniformBuffer();

        void bind(u32 slot) const;
        void unbind(u32 slot) const;
        void setData(const void* data, size_t size, size_t offset = 0) const;
    private:
        u32 m_id;
    };

} // namespace jng
