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

        return makeRef<OpenGLTexture>(path);
    }

    Ref<Texture> Texture::create(const Properties& properties)
    {
        JNG_PROFILE_FUNCTION();

        return makeRef<OpenGLTexture>(properties);
    }

} // namespace jng
