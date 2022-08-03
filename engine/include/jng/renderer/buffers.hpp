/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

namespace jng {

    class VertexBuffer
    {
    public:
        virtual void bind() const = 0;
        virtual void unbind() const = 0;
        virtual void setData(const void* data, size_t size) const = 0;

        static Ref<VertexBuffer> create(const void* vertices, size_t size);
        static Ref<VertexBuffer> create(size_t size);
        virtual ~VertexBuffer() = default;
    };

    class IndexBuffer
    {
    public:
        virtual void bind() const = 0;
        virtual void unbind() const = 0;
        virtual uint32 getCount() const = 0;

        static Ref<IndexBuffer> create(const uint32* indices, uint32 count);
        virtual ~IndexBuffer() = default;
    };

    class UniformBuffer
    {
    public:
        virtual void bind(uint32 slot) const = 0;
        virtual void unbind(uint32 slot) const = 0;
        virtual void setData(const void* data, size_t size, size_t offset = 0) const = 0;

        static Ref<UniformBuffer> create(size_t size);
        virtual ~UniformBuffer() = default;
    };

} // namespace jng
