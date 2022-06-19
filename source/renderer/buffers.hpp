/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "core/base.hpp"

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

    // TODO: support for less than 32bit ibos
    class IndexBuffer
    {
    public:
        virtual void bind() const = 0;
        virtual void unbind() const = 0;
        virtual uint32 getCount() const = 0;

        static Ref<IndexBuffer> create(uint32* indices, uint32 count);
        virtual ~IndexBuffer() = default;
    };

} // namespace jng
