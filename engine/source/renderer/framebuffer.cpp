/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/framebuffer.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer_api.hpp"
#include "renderer/opengl/framebuffer_ogl.hpp"

namespace jng {

Ref<Framebuffer> Framebuffer::create(const Properties& properties)
    {
        JNG_PROFILE_FUNCTION();

        return makeRef<OpenGLFramebuffer>(properties);
    }

} // namespace jng
