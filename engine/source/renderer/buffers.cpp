/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/buffers.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer_api.hpp"
#include "renderer/opengl/buffers_ogl.hpp"

namespace jng {

    Ref<VertexBuffer> VertexBuffer::create(const void* vertices, size_t size)
    {
        JNG_PROFILE_FUNCTION();

        switch (RendererAPI::getRendererBackend())
        {
        case RendererBackend::OpenGL: return makeRef<OpenGLVertexBuffer>(vertices, size);
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
            return nullptr;
        }
    }

    Ref<VertexBuffer> VertexBuffer::create(size_t size)
    {
        JNG_PROFILE_FUNCTION();

        switch (RendererAPI::getRendererBackend())
        {
        case RendererBackend::OpenGL: return makeRef<OpenGLVertexBuffer>(size);
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
            return nullptr;
        }
    }

    Ref<IndexBuffer> IndexBuffer::create(const uint32* indices, uint32 count)
    {
        JNG_PROFILE_FUNCTION();

        switch (RendererAPI::getRendererBackend())
        {
        case RendererBackend::OpenGL: return makeRef<OpenGLIndexBuffer>(indices, count);
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
            return nullptr;
        }
    }

    Ref<UniformBuffer> UniformBuffer::create(size_t size)
    {
        JNG_PROFILE_FUNCTION();

        switch (RendererAPI::getRendererBackend())
        {
        case RendererBackend::OpenGL: return makeRef<OpenGLUniformBuffer>(size);
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
            return nullptr;
        }
    }

} // namespace jng
