/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "core/base.hpp"

namespace jng {

    class OpenGLVertexBuffer
    {
    public:
        OpenGLVertexBuffer(const void* vertices, size_t size);
        OpenGLVertexBuffer(size_t size);
        ~OpenGLVertexBuffer();

        void bind() const;
        void unbind() const;
        void setData(const void* data, size_t size) const;
    private:
        uint32 m_id;
    };

    class OpenGLIndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32* indices, uint32 count);
        ~OpenGLIndexBuffer();

        void bind() const;
        void unbind() const;
        uint32 getCount() const { return m_count; }
    private:
        uint32 m_id;
        uint32 m_count;
    };

} // namespace jng
