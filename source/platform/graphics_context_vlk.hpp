/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include  "platform/graphics_context.hpp"

struct GLFWwindow;
struct VkInstance_T;
typedef VkInstance_T* VkInstance;

namespace k2d {

    class WindowImpl;

    class GraphicsContextImpl
    {
    public:
        GraphicsContextImpl(WindowImpl& window);
        ~GraphicsContextImpl();

        void makeCurrent() const;
        void swapBuffers() const;
    private:
        WindowImpl& m_window;
        GLFWwindow* m_windowHandle;
        VkInstance m_vkInstance;
    };

} // namespace k2d
