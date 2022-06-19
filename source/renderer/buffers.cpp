/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/buffers.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer_api.hpp"

#include "renderer/opengl/buffers_ogl.hpp"
//#include "renderer/direct3d/buffers_impl_d3d.hpp"
//#include "renderer/vulkan/buffers_impl_vlk.hpp"

namespace jng {

    Ref<VertexBuffer> VertexBuffer::create(const void* vertices, size_t size)
    {
        JNG_PROFILE_FUNCTION();

        return makeRef<OpenGLVertexBuffer>(vertices, size);
    }

    Ref<VertexBuffer> VertexBuffer::create(size_t size)
    {
        JNG_PROFILE_FUNCTION();

        return makeRef<OpenGLVertexBuffer>(size);
    }

    Ref<IndexBuffer> IndexBuffer::create(uint32* indices, uint32 count)
    {
        JNG_PROFILE_FUNCTION();

        return makeRef<OpenGLIndexBuffer>(indices, count);
    }

} // namespace jng
