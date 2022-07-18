/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/opengl/vertex_array_ogl.hpp"

#include "core/base_internal.hpp"
#include "renderer/buffers.hpp"

#include <glad/gl.h>

namespace jng {

    static uint32 dataTypeToGLEnum(LayoutElement::DataType type)
    {
        switch (type)
        {
        case LayoutElement::DataType::Float:
        case LayoutElement::DataType::Float2:
        case LayoutElement::DataType::Float3:
        case LayoutElement::DataType::Float4:
            return GL_FLOAT;
        case LayoutElement::DataType::UInt:
        case LayoutElement::DataType::UInt2:
        case LayoutElement::DataType::UInt3:
        case LayoutElement::DataType::UInt4:
            return GL_UNSIGNED_INT;
        case LayoutElement::DataType::UInt4x8:
            return GL_UNSIGNED_BYTE;
        }

        JNG_CORE_ASSERT(false, "This should never be triggered!")
            return 0;
    }

    static int dataTypeToCount(LayoutElement::DataType type)
    {
        switch (type)
        {
        case LayoutElement::DataType::Float:    return 1;
        case LayoutElement::DataType::Float2:   return 2;
        case LayoutElement::DataType::Float3:   return 3;
        case LayoutElement::DataType::Float4:   return 4;
        case LayoutElement::DataType::UInt:     return 1;
        case LayoutElement::DataType::UInt2:    return 2;
        case LayoutElement::DataType::UInt3:    return 3;
        case LayoutElement::DataType::UInt4:    return 4;
        case LayoutElement::DataType::UInt4x8:  return 4;
        }

        JNG_CORE_ASSERT(false, "This should never be triggered!")
            return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray(const Ref<VertexBuffer>& vbo, const VertexLayout& layout, const Ref<Shader>& /*shader*/)
    {
        m_VBO = vbo;
        JNG_CORE_ASSERT(!layout.getElements().empty(), "Vertex buffer layout is empty!");

        glCreateVertexArrays(1, &m_id);
        OpenGLVertexArray::bind();
        vbo->bind();
        uint32 index = 0;
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            
            if (dataTypeToGLEnum(element.Type) == GL_FLOAT || element.PassAsFloat)
                glVertexAttribPointer(index,
                    dataTypeToCount(element.Type),
                    dataTypeToGLEnum(element.Type),
                    element.Normalized ? GL_TRUE : GL_FALSE,
                    static_cast<int>(layout.getStride()),
                    reinterpret_cast<const void*>(element.Offset)
                );
            else    
                glVertexAttribIPointer(index,
                    dataTypeToCount(element.Type),
                    dataTypeToGLEnum(element.Type),
                    static_cast<int>(layout.getStride()),
                    reinterpret_cast<const void*>(element.Offset)
                );

            ++index;
        }
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_id);
    }

    void OpenGLVertexArray::bind() const
    {
        glBindVertexArray(m_id);
    }

    void OpenGLVertexArray::unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer>& ibo)
    {
        m_IBO = ibo;
        bind();
        ibo->bind();
    }

} // namespace jng
