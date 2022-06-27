/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/buffers.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer_api.hpp"

#include "renderer/opengl/buffers_ogl.hpp"
#include "renderer/direct3d/buffers_d3d.hpp"
//#include "renderer/vulkan/buffers_impl_vlk.hpp"

namespace jng {

    Ref<VertexBuffer> VertexBuffer::create(const void* vertices, size_t size)
    {
        JNG_PROFILE_FUNCTION();

        switch (RendererAPI::getRendererBackend())
        {
        case RendererBackend::Direct3D: return makeRef<Direct3DVertexBuffer>(vertices, size);
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
        case RendererBackend::Direct3D: return makeRef<Direct3DVertexBuffer>(size);
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
        case RendererBackend::Direct3D: return makeRef<Direct3DIndexBuffer>(indices, count);
        case RendererBackend::OpenGL: return makeRef<OpenGLIndexBuffer>(indices, count);
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
            return nullptr;
        }
    }

} // namespace jng
