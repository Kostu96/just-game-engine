/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/vulkan/vertex_array_impl_vlk.hpp"

#include "core/base_internal.hpp"
#include "renderer/buffers.hpp"

#include <glad/glad.h>

namespace k2d {

    static uint32 dataTypeToGLEnum(LayoutElement::DataType type)
    {
        switch (type)
        {
        case LayoutElement::DataType::Float:  return GL_FLOAT;
        case LayoutElement::DataType::Float2: return GL_FLOAT;
        case LayoutElement::DataType::Float3: return GL_FLOAT;
        case LayoutElement::DataType::Float4: return GL_FLOAT;
        case LayoutElement::DataType::Mat3:   return GL_FLOAT;
        case LayoutElement::DataType::Mat4:   return GL_FLOAT;
        case LayoutElement::DataType::Int:    return GL_INT;
        case LayoutElement::DataType::Int2:   return GL_INT;
        case LayoutElement::DataType::Int3:   return GL_INT;
        case LayoutElement::DataType::Int4:   return GL_INT;
        case LayoutElement::DataType::Bool:   return GL_BOOL;
        }

        K2D_CORE_ASSERT(false, "This should never be triggered!")
            return 0;
    }

    static int dataTypeToCount(LayoutElement::DataType type)
    {
        switch (type)
        {
        case LayoutElement::DataType::Float:   return 1;
        case LayoutElement::DataType::Float2:  return 2;
        case LayoutElement::DataType::Float3:  return 3;
        case LayoutElement::DataType::Float4:  return 4;
        case LayoutElement::DataType::Mat3:    return 3 * 3;
        case LayoutElement::DataType::Mat4:    return 4 * 4;
        case LayoutElement::DataType::Int:     return 1;
        case LayoutElement::DataType::Int2:    return 2;
        case LayoutElement::DataType::Int3:    return 3;
        case LayoutElement::DataType::Int4:    return 4;
        case LayoutElement::DataType::Bool:    return 1;
        }

        K2D_CORE_ASSERT(false, "This should never be triggered!")
            return 0;
    }

    VertexArrayImpl::VertexArrayImpl(const Ref<VertexBuffer>& vbo, const VertexLayout& layout, const Ref<Shader>& /*shader*/)
    {
        m_VBO = vbo;
        K2D_CORE_ASSERT(!layout.getElements().empty(), "Vertex buffer layout is empty!");

        glCreateVertexArrays(1, &m_id);
        VertexArrayImpl::bind();
        vbo->bind();
        uint32 index = 0;
        for (const auto& element : layout) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index,
                dataTypeToCount(element.Type),
                dataTypeToGLEnum(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                static_cast<int>(layout.getStride()),
                reinterpret_cast<const void*>(element.Offset)
            );
            ++index;
        }
    }

    VertexArrayImpl::~VertexArrayImpl()
    {
        glDeleteVertexArrays(1, &m_id);
    }

    void VertexArrayImpl::bind() const
    {
        glBindVertexArray(m_id);
    }

    void VertexArrayImpl::unbind() const
    {
        glBindVertexArray(0);
    }

    void VertexArrayImpl::setIndexBuffer(const Ref<IndexBuffer>& ibo)
    {
        m_IBO = ibo;
        bind();
        ibo->bind();
    }

} // namespace k2d
