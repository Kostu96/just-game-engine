/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "platform/graphics_context.hpp"

#include "core/base_internal.hpp"
#include "renderer/renderer_api.hpp"
#include "platform/graphics_context_ogl.hpp"

namespace jng {

    Scope<GraphicsContext> GraphicsContext::create(Window& window)
    {
        JNG_PROFILE_FUNCTION();

        return makeScope<OpenGLGraphicsContext>(window);
    }

} // namespace jng
