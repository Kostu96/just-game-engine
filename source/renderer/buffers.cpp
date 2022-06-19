/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/buffers.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer_api.hpp"

#include "renderer/opengl/buffers_impl_ogl.hpp"
//#include "renderer/direct3d/buffers_impl_d3d.hpp"
// #include "renderer/vulkan/buffers_impl_vlk.hpp"

namespace jng {

    void VertexBuffer::bind() const
    {
        m_implementation->bind();
    }

    void VertexBuffer::unbind() const
    {
        m_implementation->unbind();
    }

    void VertexBuffer::setData(const void* data, size_t size)
    {
        m_implementation->setData(data, size);
    }

    Ref<VertexBuffer> VertexBuffer::create(const void* vertices, size_t size)
    {
        K2D_PROFILE_FUNCTION();

        return makeRef<VertexBuffer>(makeScope<VertexBufferImpl>(vertices, size));
    }

    Ref<VertexBuffer> VertexBuffer::create(size_t size)
    {
        K2D_PROFILE_FUNCTION();

        return makeRef<VertexBuffer>(makeScope<VertexBufferImpl>(size));
    }

    VertexBuffer::VertexBuffer(Scope<VertexBufferImpl>&& implementation) :
        m_implementation{ std::move(implementation) } {}

    VertexBuffer::~VertexBuffer() = default;

    void IndexBuffer::bind() const
    {
        m_implementation->bind();
    }

    void IndexBuffer::unbind() const
    {
        m_implementation->unbind();
    }

    uint32 IndexBuffer::getCount() const
    {
        return m_implementation->getCount();
    }

    Ref<IndexBuffer> IndexBuffer::create(uint32* indices, uint32 count)
    {
        K2D_PROFILE_FUNCTION();

        return makeRef<IndexBuffer>(makeScope<IndexBufferImpl>(indices, count));
    }

    IndexBuffer::IndexBuffer(Scope<IndexBufferImpl>&& implementation) :
        m_implementation{ std::move(implementation)} {}

    IndexBuffer::~IndexBuffer() = default;

} // namespace jng
