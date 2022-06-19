/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/buffers.hpp"

namespace k2d {

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
        uint32 m_id;
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
        uint32 m_id;
        uint32 m_count;
    };

} // namespace k2d
