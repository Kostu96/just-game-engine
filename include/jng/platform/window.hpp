/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

#include <functional>

struct GLFWwindow;

namespace jng {

    class Event;
    class GraphicsContext;

    class Window
    {
    public:
        using EventCallbackFunc = std::function<void(Event&)>;

        struct WindowData {
            std::string title;
            uint32 width{}, height{};
            bool isMinimized = false;
            EventCallbackFunc eventCallback;
        };

        void onUpdate();
        void setVSync(bool enabled);
        GLFWwindow* getNativeWindowHandle() const { return m_windowHandle; }
        void setTitle(const char* title);
        uint32 getWidth() const { return m_windowData.width; }
        uint32 getHeight() const { return m_windowData.height; }
        bool isMinimized() const { return m_windowData.isMinimized; }
        void setEventCallback(const EventCallbackFunc& callback) { m_windowData.eventCallback = callback; }
        const EventCallbackFunc& getEventCallback() const { return m_windowData.eventCallback; }
        const GraphicsContext* getGraphicsContext() const { return m_graphicsContext.get(); }

        static Scope<Window> create(const char* title, uint32 width, uint32 height, RendererBackend backend);
        Window(const char* title, uint32 width, uint32 height, RendererBackend backend);
        ~Window();
    private:
        GLFWwindow* m_windowHandle;
        bool m_isVSyncEnabled;
        Window::WindowData m_windowData;
        Scope<GraphicsContext> m_graphicsContext;
    };

} // namespace jng
