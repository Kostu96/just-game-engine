/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

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
        uint32 m_id;
    };

    class IndexBuffer final
    {
    public:
        IndexBuffer(const uint32* indices, uint32 count);
        ~IndexBuffer();

        void bind() const;
        void unbind() const;
        uint32 getCount() const { return m_count; }
    private:
        uint32 m_id;
        uint32 m_count;
    };

    class UniformBuffer final
    {
    public:
        explicit UniformBuffer(size_t size);
        ~UniformBuffer();

        void bind(uint32 slot) const;
        void unbind(uint32 slot) const;
        void setData(const void* data, size_t size, size_t offset = 0) const;
    private:
        uint32 m_id;
    };

} // namespace jng
