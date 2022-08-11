/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "core/base.hpp"

#include <array>

namespace jng {

    class Window;

    class GraphicsContext
    {
    public:
        explicit GraphicsContext(Window& window);
        ~GraphicsContext() = default;

        void setVSync(bool enabled);
        bool isVSync() const { return m_isVSync; }
        void swapBuffers() const;
    private:
        struct Hint { int id; int value; };
        static std::array<Hint, 4> getContextCreationHints();

        Window& m_window;
        bool m_isVSync;

        friend class Window;
    };

} // namespace jng
