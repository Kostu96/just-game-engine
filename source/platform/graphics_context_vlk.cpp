/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "platform/graphics_context_vlk.hpp"

#include "core/base_internal.hpp"
#include "platform/window.hpp"

#include <GLFW/glfw3.h>

namespace jng {

    VulkanGraphicsContext::VulkanGraphicsContext(Window& window) :
        m_window{ window },
        m_windowHandle{ window.getNativeWindowHandle() }
    {
        
    }

    VulkanGraphicsContext::~VulkanGraphicsContext()
    {
        
    }

    void VulkanGraphicsContext::makeCurrent() const
    {
        
    }

    void VulkanGraphicsContext::swapBuffers() const
    {
        
    }

} // namespace jng
