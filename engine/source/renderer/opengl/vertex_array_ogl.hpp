/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "renderer/vertex_array.hpp"

namespace jng {

    class OpenGLVertexArray :
        public VertexArray
    {
    public:
        OpenGLVertexArray(const Ref<VertexBuffer>& vbo, const VertexLayout& layout, const Ref<Shader>& shader);
        virtual ~OpenGLVertexArray();

        void bind() const override;
        void unbind() const override;

        const Ref<VertexBuffer>& getVertexBuffer() const override { return m_VBO; }
        void setIndexBuffer(const Ref<IndexBuffer>& ibo) override;
        const Ref<IndexBuffer>& getIndexBuffer() const override { return m_IBO; }
    private:
        uint32 m_id;
        Ref<VertexBuffer> m_VBO;
        Ref<IndexBuffer> m_IBO;
    };

} // namespace jng
