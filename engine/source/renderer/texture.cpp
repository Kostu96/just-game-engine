/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/texture.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer_api.hpp"

#include "renderer/opengl/texture_ogl.hpp"

#if 0
#if defined(JNG_WINDOWS)
#include "renderer/direct3d/texture_d3d.hpp"
#endif
#endif

namespace jng {

    Ref<Texture> Texture::create(const char* path)
    {
        JNG_PROFILE_FUNCTION();

        switch (RendererAPI::getRendererBackend())
        {
#if 0
#if defined(JNG_WINDOWS)
        case RendererBackend::Direct3D: return makeRef<Direct3DTexture>(path);
#endif
#endif
        case RendererBackend::OpenGL: return makeRef<OpenGLTexture>(path);
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
            return nullptr;
        }
    }

    Ref<Texture> Texture::create(const Properties& properties)
    {
        JNG_PROFILE_FUNCTION();

        switch (RendererAPI::getRendererBackend())
        {
#if 0
#if defined(JNG_WINDOWS)
        case RendererBackend::Direct3D: return makeRef<Direct3DTexture>(properties);
#endif
#endif
        case RendererBackend::OpenGL: return makeRef<OpenGLTexture>(properties);
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
            return nullptr;
        }
    }

} // namespace jng
