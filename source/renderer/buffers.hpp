/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "core/base.hpp"

namespace jng {

    class VertexBufferImpl;
    class IndexBufferImpl;

    class VertexBuffer
    {
    public:
        void bind() const;
        void unbind() const;
        void setData(const void* data, size_t size);

        static Ref<VertexBuffer> create(const void* vertices, size_t size);
        static Ref<VertexBuffer> create(size_t size);
        explicit VertexBuffer(Scope<VertexBufferImpl>&& implementation);
        ~VertexBuffer();
    private:
        Scope<VertexBufferImpl> m_implementation;
    };

    // TODO: support for less than 32bit ibos
    class IndexBuffer
    {
    public:
        void bind() const;
        void unbind() const;
        uint32 getCount() const;

        static Ref<IndexBuffer> create(uint32* indices, uint32 count);
        explicit IndexBuffer(Scope<IndexBufferImpl>&& implementation);
        ~IndexBuffer();
    private:
        Scope<IndexBufferImpl> m_implementation;
    };

} // namespace jng
