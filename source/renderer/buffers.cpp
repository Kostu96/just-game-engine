/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/buffers.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer_api.hpp"
#include "renderer/opengl/buffers_ogl.hpp"
//#include "renderer/vulkan/buffers_vlk.hpp"

#if defined(JNG_WINDOWS)
#include "renderer/direct3d/buffers_d3d.hpp"
#endif

namespace jng {

    Ref<VertexBuffer> VertexBuffer::create(const void* vertices, size_t size)
    {
        JNG_PROFILE_FUNCTION();

        switch (RendererAPI::getRendererBackend())
        {
#if defined(JNG_WINDOWS)
        case RendererBackend::Direct3D: return makeRef<Direct3DVertexBuffer>(vertices, size);
#endif
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
#if defined(JNG_WINDOWS)
        case RendererBackend::Direct3D: return makeRef<Direct3DVertexBuffer>(size);
#endif
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
#if defined(JNG_WINDOWS)
        case RendererBackend::Direct3D: return makeRef<Direct3DIndexBuffer>(indices, count);
#endif
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
#if defined(JNG_WINDOWS)
        case RendererBackend::Direct3D: return makeRef<Direct3DUniformBuffer>(size);
#endif
        case RendererBackend::OpenGL: return makeRef<OpenGLUniformBuffer>(size);
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
            return nullptr;
        }
    }

} // namespace jng
