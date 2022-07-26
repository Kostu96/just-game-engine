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

namespace jng {

    class Window;

    class VulkanGraphicsContext :
        public GraphicsContext
    {
    public:
        VulkanGraphicsContext(Window& window);
        ~VulkanGraphicsContext();

        void makeCurrent() const;
        void swapBuffers() const;
    private:
        Window& m_window;
        GLFWwindow* m_windowHandle;
        VkInstance m_vkInstance = nullptr;
    };

} // namespace jng
