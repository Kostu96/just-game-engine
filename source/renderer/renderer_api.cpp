/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/renderer_api.hpp"

#include "core/base_internal.hpp"
#include "renderer/opengl/renderer_api_ogl.hpp"

namespace jng {

    void RendererAPI::init(RendererBackend backend)
    {
        switch (backend)
        {
        case RendererBackend::OpenGL:
            s_implementation = makeScope<OpenGLRendererAPI>();
            break;
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
        }
    }

    Scope<RendererAPI> RendererAPI::s_implementation;

} // namespace jng
