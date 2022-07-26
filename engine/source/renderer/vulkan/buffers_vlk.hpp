/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/buffers.hpp"

namespace jng {

    class VulkanVertexBuffer
    {
    public:
        VulkanVertexBuffer(const void* vertices, size_t size);
        VulkanVertexBuffer(size_t size);
        ~VulkanVertexBuffer();

        void bind() const;
        void unbind() const;
        void setData(const void* data, size_t size) const;
    private:
        uint32 m_id;
    };

    class VulkanIndexBuffer
    {
    public:
        VulkanIndexBuffer(uint32* indices, uint32 count);
        ~VulkanIndexBuffer();

        void bind() const;
        void unbind() const;
        uint32 getCount() const { return m_count; }
    private:
        uint32 m_id;
        uint32 m_count;
    };

} // namespace jng
