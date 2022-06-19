/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "platform/input.hpp"

#include "core/engine.hpp"
#include "platform/window.hpp"

#include <GLFW/glfw3.h>

namespace jng {

    bool Input::isKeyPressed(Key::KeyCode key)
    {
        GLFWwindow* window = reinterpret_cast<GLFWwindow*>(Engine::get().getWindow().getNativeWindowHandle());
        int state = glfwGetKey(window, static_cast<int>(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::isMouseButtonPressed(Mouse::MouseCode button)
    {
        GLFWwindow* window = reinterpret_cast<GLFWwindow*>(Engine::get().getWindow().getNativeWindowHandle());
        int state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    glm::vec2 Input::getMousePosition()
    {
        GLFWwindow* window = reinterpret_cast<GLFWwindow*>(Engine::get().getWindow().getNativeWindowHandle());
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return { static_cast<float>(x), static_cast<float>(y) };
    }

} // namespace jng
