/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "core/base.hpp"

namespace jng {

    class Window;

    class GraphicsContext
    {
    public:
        virtual ~GraphicsContext() = default;

        virtual void makeCurrent() const = 0;
        virtual void swapBuffers() const = 0;

        static Scope<GraphicsContext> create(Window& window, RendererBackend api);
    };

} // namespace jng
