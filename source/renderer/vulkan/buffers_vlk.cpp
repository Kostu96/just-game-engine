/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/vulkan/buffers_vlk.hpp"

namespace jng {

    VulkanVertexBuffer::VulkanVertexBuffer(const void* /*vertices*/, size_t /*size*/)
    {
        m_id = 0;
    }

    VulkanVertexBuffer::VulkanVertexBuffer(size_t /*size*/)
    {
        m_id = 0;
    }

    VulkanVertexBuffer::~VulkanVertexBuffer()
    {
        
    }

    void VulkanVertexBuffer::bind() const
    {
        
    }

    void VulkanVertexBuffer::unbind() const
    {
        
    }

    void VulkanVertexBuffer::setData(const void* /*data*/, size_t /*size*/) const
    {
        
    }

    VulkanIndexBuffer::VulkanIndexBuffer(uint32* /*indices*/, uint32 count) :
        m_count{ count }
    {
        m_id = 0;
    }

    VulkanIndexBuffer::~VulkanIndexBuffer()
    {
        
    }

    void VulkanIndexBuffer::bind() const
    {
        
    }

    void VulkanIndexBuffer::unbind() const
    {
        
    }

} // namespace jng
