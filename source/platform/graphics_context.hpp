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

        virtual void swapBuffers() const = 0;

        static Scope<GraphicsContext> create(Window& window);
    };

} // namespace jng
