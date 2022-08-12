/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/buffers.hpp"

#include <glad/gl.h>

namespace jng {

#pragma region VertexBuffer
    VertexBuffer::VertexBuffer(const void* vertices, size_t size)
    {
        glCreateBuffers(1, &m_id);
        glNamedBufferData(m_id, static_cast<GLsizeiptr>(size), vertices, GL_STATIC_DRAW);
    }

    VertexBuffer::VertexBuffer(size_t size)
    {
        glCreateBuffers(1, &m_id);
        glNamedBufferData(m_id, static_cast<GLsizeiptr>(size), nullptr, GL_DYNAMIC_DRAW);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    void VertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void VertexBuffer::unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::setData(const void* data, size_t size) const
    {
        glNamedBufferSubData(m_id, 0, static_cast<GLsizeiptr>(size), data);
    }
#pragma endregion

#pragma region IndexBuffer
    static size_t indexTypeToSize(RendererAPI::IndexType type)
    {
        switch (type)
        {
        case RendererAPI::IndexType::U8:  return sizeof(u8);
        case RendererAPI::IndexType::U16: return sizeof(u16);
        case RendererAPI::IndexType::U32: return sizeof(u32);
        }

        JNG_CORE_ASSERT(false, "This should never be triggered!");
        return 0;
    }

    IndexBuffer::IndexBuffer(const void* indices, u32 count, RendererAPI::IndexType type) :
        m_count{ count },
        m_indexType{ type }
    {
        glCreateBuffers(1, &m_id);
        glNamedBufferData(m_id, static_cast<GLsizeiptr>(count * indexTypeToSize(type)), indices, GL_STATIC_DRAW);
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    void IndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void IndexBuffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
#pragma endregion

#pragma region UniformBuffer
    UniformBuffer::UniformBuffer(size_t size)
    {
        glCreateBuffers(1, &m_id);
        glNamedBufferData(m_id, static_cast<GLsizeiptr>(size), nullptr, GL_DYNAMIC_DRAW);
    }

    UniformBuffer::~UniformBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    void UniformBuffer::bind(u32 slot) const
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, slot, m_id);
    }

    void UniformBuffer::unbind(u32 slot) const
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, slot, 0);
    }

    void UniformBuffer::setData(const void* data, size_t size, size_t offset) const
    {
        glNamedBufferSubData(m_id, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
    }
#pragma endregion

} // namespace jng
