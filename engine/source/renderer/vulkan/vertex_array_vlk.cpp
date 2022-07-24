/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/vulkan/vertex_array_vlk.hpp"

#include "core/base_internal.hpp"
#include "renderer/buffers.hpp"

namespace jng {

    VulkanVertexArray::VulkanVertexArray(const Ref<VertexBuffer>& vbo, const VertexLayout& /*layout*/, const Ref<Shader>& /*shader*/)
    {
        m_VBO = vbo;
    }

    VulkanVertexArray::~VulkanVertexArray()
    {

    }

    void VulkanVertexArray::bind() const
    {
        
    }

    void VulkanVertexArray::unbind() const
    {
        
    }

    void VulkanVertexArray::setIndexBuffer(const Ref<IndexBuffer>& ibo)
    {
        m_IBO = ibo;
        bind();
        ibo->bind();
    }

} // namespace jng
