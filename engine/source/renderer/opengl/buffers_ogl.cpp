/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/buffers.hpp"

#include <glad/gl.h>

namespace jng {

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

    IndexBuffer::IndexBuffer(const uint32* indices, uint32 count) :
        m_count(count)
    {
        glCreateBuffers(1, &m_id);
        glNamedBufferData(m_id, static_cast<GLsizeiptr>(count * sizeof(uint32)), indices, GL_STATIC_DRAW);
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

    UniformBuffer::UniformBuffer(size_t size)
    {
        glCreateBuffers(1, &m_id);
        glNamedBufferData(m_id, static_cast<GLsizeiptr>(size), nullptr, GL_DYNAMIC_DRAW);
    }

    UniformBuffer::~UniformBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    void UniformBuffer::bind(uint32 slot) const
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, slot, m_id);
    }

    void UniformBuffer::unbind(uint32 slot) const
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, slot, 0);
    }

    void UniformBuffer::setData(const void* data, size_t size, size_t offset) const
    {
        glNamedBufferSubData(m_id, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
    }

} // namespace jng
