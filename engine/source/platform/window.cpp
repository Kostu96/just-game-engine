/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "platform/window.hpp"

#include "core/base_internal.hpp"
#include "core/key_events.hpp"
#include "core/mouse_events.hpp"
#include "core/window_events.hpp"
#include "platform/graphics_context.hpp"

#include <GLFW/glfw3.h>

namespace jng {
    
#ifdef JNG_DEBUG
    static void glfwErrorCallback(int error, const char* description)
    {
        JNG_CORE_ERROR("GLFW error {0}: {1}", error, description);
    }
#endif

    static void glfwWindowCloseCallback(GLFWwindow* window)
    {
        WindowCloseEvent event;
        auto data = reinterpret_cast<Window::WindowData*>(glfwGetWindowUserPointer(window));
        data->eventCallback(event);
    }

    static void glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
    {
        auto data = reinterpret_cast<Window::WindowData*>(glfwGetWindowUserPointer(window));
        data->width = static_cast<uint32>(width);
        data->height = static_cast<uint32>(height);
        data->isMinimized = (data->width == 0 || data->height == 0);
        WindowResizeEvent event(data->width, data->height);
        data->eventCallback(event);
    }

    static void glfwKeyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
    {
        auto data = reinterpret_cast<Window::WindowData*>(glfwGetWindowUserPointer(window));
        switch (action) {
        case GLFW_RELEASE:
        {
            KeyReleaseEvent event{ static_cast<Key::Code>(key) };
            data->eventCallback(event);
        } break;
        case GLFW_PRESS:
        {
            KeyPressEvent event{ static_cast<Key::Code>(key), false };
            data->eventCallback(event);
        } break;
        case GLFW_REPEAT:
        {
            KeyPressEvent event{ static_cast<Key::Code>(key), true };
            data->eventCallback(event);
        } break;
        }
    }

    static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int /*mods*/)
    {
        auto data = reinterpret_cast<Window::WindowData*>(glfwGetWindowUserPointer(window));
        switch (action) {
        case GLFW_RELEASE:
        {
            MouseButtonReleaseEvent event{ static_cast<Mouse::Code>(button) };
            data->eventCallback(event);
        } break;
        case GLFW_PRESS:
        {
            MouseButtonPressEvent event{ static_cast<Mouse::Code>(button) };
            data->eventCallback(event);
        } break;
        }
    }

    static void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        MouseScrollEvent event(static_cast<float>(xoffset), static_cast<float>(yoffset));
        auto data = reinterpret_cast<Window::WindowData*>(glfwGetWindowUserPointer(window));
        data->eventCallback(event);
    }

    static void glfwCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        MouseMoveEvent event(static_cast<float>(xpos), static_cast<float>(ypos));
        auto data = reinterpret_cast<Window::WindowData*>(glfwGetWindowUserPointer(window));
        data->eventCallback(event);
    }

    void Window::onUpdate()
    {
        m_graphicsContext->swapBuffers();
        
        glfwPollEvents();
    }

    void Window::setTitle(const char* title)
    {
        m_windowData.title = title;
        glfwSetWindowTitle(m_windowHandle, title);
    }

    Scope<Window> Window::create(const char* title, uint32 width, uint32 height)
    {
        JNG_PROFILE_FUNCTION();

        return makeScope<Window>(title, width, height);
    }

    Window::Window(const char* title, uint32 width, uint32 height)
    {
        m_windowData.title = title;
        m_windowData.width = width;
        m_windowData.height = height;

        auto hints = GraphicsContext::getContextCreationHints();
        for (auto& hint : hints)
            glfwWindowHint(hint.id, hint.value);

        m_windowHandle = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title, nullptr, nullptr);
        
        m_graphicsContext = makeScope<GraphicsContext>(*this);

        glfwSetWindowUserPointer(m_windowHandle, &m_windowData);

        glfwSetWindowCloseCallback(m_windowHandle, glfwWindowCloseCallback);
        glfwSetWindowSizeCallback(m_windowHandle, glfwWindowSizeCallback);
        glfwSetKeyCallback(m_windowHandle, glfwKeyCallback);
        glfwSetMouseButtonCallback(m_windowHandle, glfwMouseButtonCallback);
        glfwSetScrollCallback(m_windowHandle, glfwScrollCallback);
        glfwSetCursorPosCallback(m_windowHandle, glfwCursorPosCallback);

        JNG_CORE_TRACE("Window created: {0}x{1}", width, height);
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_windowHandle);
    }

    struct GLFWInitializer
    {
        GLFWInitializer()
        {
#ifdef JNG_DEBUG
            glfwSetErrorCallback(glfwErrorCallback);
#endif
            [[maybe_unused]] int success = glfwInit();
            JNG_CORE_ASSERT(success, "Could not initialize GLFW!");
        }

        ~GLFWInitializer()
        {
            glfwTerminate();
        }
    }
    globalGLFWInitializer;

} // namespace jng
