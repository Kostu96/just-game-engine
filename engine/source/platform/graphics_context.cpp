/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "platform/graphics_context.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer_api.hpp"
#include "platform/graphics_context_ogl.hpp"

#if 0
#if defined(JNG_WINDOWS)
#include "platform/windows/graphics_context_d3d.hpp"
#endif
#endif

namespace jng {

    Scope<GraphicsContext> GraphicsContext::create(Window& window)
    {
        JNG_PROFILE_FUNCTION();

        switch (RendererAPI::getRendererBackend())
        {
#if 0
#if defined(JNG_WINDOWS)
        case RendererBackend::Direct3D: return makeScope<Direct3DGraphicsContext>(window);
#endif
#endif
        case RendererBackend::OpenGL: return makeScope<OpenGLGraphicsContext>(window);
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
            return nullptr;
        }
    }

} // namespace jng
