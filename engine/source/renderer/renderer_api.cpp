/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/renderer_api.hpp"

#include "core/base_internal.hpp"
#include "renderer/opengl/renderer_api_ogl.hpp"

#if defined(JNG_WINDOWS)
#include "renderer/direct3d/renderer_api_d3d.hpp"
#endif

namespace jng {

    void RendererAPI::init(RendererBackend backend)
    {
        s_backend = backend;

        switch (backend)
        {
#if defined(JNG_WINDOWS)
        case RendererBackend::Direct3D:
            s_implementation = makeScope<Direct3DRendererAPI>();
            break;
#endif
        case RendererBackend::OpenGL:
            s_implementation = makeScope<OpenGLRendererAPI>();
            break;
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
        }
    }

    RendererBackend RendererAPI::s_backend;
    Scope<RendererAPI::RendererAPIImpl> RendererAPI::s_implementation;

} // namespace jng
