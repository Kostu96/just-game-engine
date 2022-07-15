/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/opengl/buffers_ogl.hpp"

#include <glad/gl.h>

namespace jng {

    OpenGLVertexBuffer::OpenGLVertexBuffer(const void* vertices, size_t size)
    {
        glCreateBuffers(1, &m_id);
        glNamedBufferData(m_id, static_cast<GLsizeiptr>(size), vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size)
    {
        glCreateBuffers(1, &m_id);
        glNamedBufferData(m_id, static_cast<GLsizeiptr>(size), nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    void OpenGLVertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void OpenGLVertexBuffer::unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer::setData(const void* data, size_t size) const
    {
        glNamedBufferSubData(m_id, 0, static_cast<GLsizeiptr>(size), data);
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32* indices, uint32 count) :
        m_count(count)
    {
        glCreateBuffers(1, &m_id);
        glNamedBufferData(m_id, static_cast<GLsizeiptr>(count * sizeof(uint32)), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    void OpenGLIndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void OpenGLIndexBuffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    OpenGLUniformBuffer::OpenGLUniformBuffer(size_t size)
    {
        glCreateBuffers(1, &m_id);
        glNamedBufferData(m_id, static_cast<GLsizeiptr>(size), nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLUniformBuffer::~OpenGLUniformBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    void OpenGLUniformBuffer::bind(uint32 slot) const
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, slot, m_id);
    }

    void OpenGLUniformBuffer::unbind(uint32 slot) const
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, slot, 0);
    }

    void OpenGLUniformBuffer::setData(const void* data, size_t size, size_t offset) const
    {
        glNamedBufferSubData(m_id, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(size), data);
    }

} // namespace jng
