/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/vertex_array.hpp"

#include "core/base_internal.hpp"
#include "renderer/buffers.hpp"

#include <glad/gl.h>

namespace jng {

    static u32 dataTypeToGLEnum(LayoutElement::DataType type)
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
        case LayoutElement::DataType::Int:
        case LayoutElement::DataType::Int2:
        case LayoutElement::DataType::Int3:
        case LayoutElement::DataType::Int4:
            return GL_INT;
        }

        JNG_CORE_ASSERT(false, "This should never be triggered!");
        return 0;
    }

    static s32 dataTypeToCount(LayoutElement::DataType type)
    {
        switch (type)
        {
        case LayoutElement::DataType::Float:   return 1;
        case LayoutElement::DataType::Float2:  return 2;
        case LayoutElement::DataType::Float3:  return 3;
        case LayoutElement::DataType::Float4:  return 4;
        case LayoutElement::DataType::UInt:    return 1;
        case LayoutElement::DataType::UInt2:   return 2;
        case LayoutElement::DataType::UInt3:   return 3;
        case LayoutElement::DataType::UInt4:   return 4;
        case LayoutElement::DataType::UInt4x8: return 4;
        case LayoutElement::DataType::Int:     return 1;
        case LayoutElement::DataType::Int2:    return 2;
        case LayoutElement::DataType::Int3:    return 3;
        case LayoutElement::DataType::Int4:    return 4;
        }

        JNG_CORE_ASSERT(false, "This should never be triggered!");
        return 0;
    }

    VertexArray::VertexArray(const Ref<VertexBuffer>& vbo, const VertexLayout& layout)
    {
        m_VBO = vbo;
        JNG_CORE_ASSERT(!layout.getElements().empty(), "Vertex buffer layout is empty!");

        glCreateVertexArrays(1, &m_id);
        glBindVertexArray(m_id);
        vbo->bind();
        u32 index = 0;
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            
            if (dataTypeToGLEnum(element.type) == GL_FLOAT || element.passAsFloat)
                glVertexAttribPointer(index,
                    dataTypeToCount(element.type),
                    dataTypeToGLEnum(element.type),
                    element.normalized ? GL_TRUE : GL_FALSE,
                    static_cast<int>(layout.getStride()),
                    reinterpret_cast<const void*>(element.offset)
                );
            else    
                glVertexAttribIPointer(index,
                    dataTypeToCount(element.type),
                    dataTypeToGLEnum(element.type),
                    static_cast<int>(layout.getStride()),
                    reinterpret_cast<const void*>(element.offset)
                );

            ++index;
        }
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_id);
    }

    void VertexArray::bind() const
    {
        glBindVertexArray(m_id);
    }

    void VertexArray::unbind() const
    {
        glBindVertexArray(0);
    }

    void VertexArray::setIndexBuffer(const Ref<IndexBuffer>& ibo)
    {
        m_IBO = ibo;
        glBindVertexArray(m_id);
        ibo->bind();
    }

} // namespace jng
