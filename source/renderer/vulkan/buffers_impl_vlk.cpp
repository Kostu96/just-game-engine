/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/vulkan/buffers_impl_vlk.hpp"

#include <glad/glad.h>

namespace k2d {

    VertexBufferImpl::VertexBufferImpl(const void* vertices, size_t size)
    {
        glCreateBuffers(1, &m_id);
        bind();
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), vertices, GL_STATIC_DRAW);
    }

    VertexBufferImpl::VertexBufferImpl(size_t size)
    {
        glCreateBuffers(1, &m_id);
        bind();
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), nullptr, GL_DYNAMIC_DRAW);
    }

    VertexBufferImpl::~VertexBufferImpl()
    {
        glDeleteBuffers(1, &m_id);
    }

    void VertexBufferImpl::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void VertexBufferImpl::unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBufferImpl::setData(const void* data, size_t size) const
    {
        bind();
        glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(size), data);
    }

    IndexBufferImpl::IndexBufferImpl(uint32* indices, uint32 count) :
        m_count(count)
    {
        glCreateBuffers(1, &m_id);
        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(count * sizeof(uint32)), indices, GL_STATIC_DRAW);
    }

    IndexBufferImpl::~IndexBufferImpl()
    {
        glDeleteBuffers(1, &m_id);
    }

    void IndexBufferImpl::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void IndexBufferImpl::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

} // namespace k2d
