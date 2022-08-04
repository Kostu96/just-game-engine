/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/framebuffer.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer_api.hpp"
#include "renderer/opengl/framebuffer_ogl.hpp"

#if 0
#if defined(JNG_WINDOWS)
#include "renderer/direct3d/framebuffer_d3d.hpp"
#endif
#endif

namespace jng {

Ref<Framebuffer> Framebuffer::create(const Properties& properties)
    {
        JNG_PROFILE_FUNCTION();

        switch (RendererAPI::getRendererBackend())
        {
#if 0
#if defined(JNG_WINDOWS)
        case RendererBackend::Direct3D: return makeRef<Direct3DFramebuffer>(properties);
#endif
#endif
        case RendererBackend::OpenGL: return makeRef<OpenGLFramebuffer>(properties);
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
            return nullptr;
        }
    }

} // namespace jng
