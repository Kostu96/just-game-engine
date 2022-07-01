/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/buffers.hpp"

namespace jng {

    class OpenGLVertexBuffer :
        public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(const void* vertices, size_t size);
        explicit OpenGLVertexBuffer(size_t size);
        virtual ~OpenGLVertexBuffer();

        void bind() const override;
        void unbind() const override;
        void setData(const void* data, size_t size) const override;
    private:
        uint32 m_id;
    };

    class OpenGLIndexBuffer :
        public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(const uint32* indices, uint32 count);
        virtual ~OpenGLIndexBuffer();

        void bind() const override;
        void unbind() const override;
        uint32 getCount() const override { return m_count; }
    private:
        uint32 m_id;
        uint32 m_count;
    };

    class OpenGLUniformBuffer :
        public UniformBuffer
    {
    public:
        OpenGLUniformBuffer(size_t size, uint32 binding);
        virtual ~OpenGLUniformBuffer();

        void setData(const void* data, size_t size, size_t offset) const override;
    private:
        uint32 m_id;
    };

} // namespace jng
