/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/texture.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer_api.hpp"

#include "renderer/opengl/texture_ogl.hpp"

namespace jng {

    Ref<Texture> Texture::create(const char* path)
    {
        JNG_PROFILE_FUNCTION();

        switch (RendererAPI::getRendererBackend())
        {
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
        case RendererBackend::OpenGL: return makeRef<OpenGLTexture>(properties);
        default:
            JNG_CORE_ASSERT(false, "API unsupported!");
            return nullptr;
        }
    }

} // namespace jng
