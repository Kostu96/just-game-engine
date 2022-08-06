/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "renderer/renderer.hpp"

#include "renderer/renderer2d.hpp"

namespace jng::Renderer {

    void init()
    {
        Renderer2D::init();
    }

    void shutdown()
    {
        Renderer2D::shutdown();
    }

} // namespace jng::Renderer
