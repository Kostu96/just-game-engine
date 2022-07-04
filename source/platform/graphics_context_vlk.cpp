/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "platform/glfw/graphics_context_impl_glfw_vlk.hpp"

#include "core/base_internal.hpp"
#include "platform/glfw/window_impl_glfw.hpp"

#include <GLFW/glfw3.h>

namespace jng {

    GraphicsContextImpl::GraphicsContextImpl(WindowImpl& window) :
        m_window{ window },
        m_windowHandle{ reinterpret_cast<GLFWwindow*>(window.getNativeWindowHandle()) }
    {
        //makeCurrent(); ?

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "TODO: set this properly";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "K2DEngine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo instanceCreateInfo{};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &appInfo;
        [[maybe_unused]] VkResult result =
            vkCreateInstance(&instanceCreateInfo, nullptr, &m_vkInstance);

        uint32_t deviceCount = 0;
        result = vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, nullptr);
        JNG_CORE_TRACE("Vulkan physical devices count = {0}", deviceCount);

        JNG_CORE_ASSERT(deviceCount, "No Vulkan physical devices found!");

        std::vector<VkPhysicalDevice> devices(deviceCount);
        result = vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, devices.data());
    }

    GraphicsContextImpl::~GraphicsContextImpl()
    {
        vkDestroyInstance(m_vkInstance, nullptr);
    }

    void GraphicsContextImpl::makeCurrent() const
    {
        // ?
    }

    void GraphicsContextImpl::swapBuffers() const
    {
        //glfwSwapBuffers(m_windowHandle); ?
    }

} // namespace jng
