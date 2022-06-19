/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/renderer_api.hpp"

#include "core/base_internal.hpp"


//#include "renderer/direct3d/renderer_api_impl_d3d.hpp"
#include "renderer/opengl/renderer_api_ogl.hpp"
//#include "renderer/vulkan/renderer_api_impl_vlk.hpp"

namespace jng {

    Scope<RendererAPI> RendererAPI::create()
    {
        return makeScope<OpenGLRendererAPI>();
    }

} // namespace jng
