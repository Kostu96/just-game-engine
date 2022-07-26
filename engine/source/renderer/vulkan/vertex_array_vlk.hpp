/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/vertex_array.hpp"

namespace jng {

    class VulkanVertexArray
    {
    public:
        VulkanVertexArray(const Ref<VertexBuffer>& vbo, const VertexLayout& layout, const Ref<Shader>& shader);
        ~VulkanVertexArray();

        void bind() const;
        void unbind() const;

        const Ref<VertexBuffer>& getVertexBuffer() const { return m_VBO; }
        void setIndexBuffer(const Ref<IndexBuffer>& ibo);
        const Ref<IndexBuffer>& getIndexBuffer() const { return m_IBO; }
    private:
        uint32 m_id;
        Ref<VertexBuffer> m_VBO;
        Ref<IndexBuffer> m_IBO;
    };

} // namespace jng
