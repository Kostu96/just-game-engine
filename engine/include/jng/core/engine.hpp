/*
 * Copyright (C) 2020-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"
#include "jng/core/layer_stack.hpp"

#include <chrono>

namespace jng {

    class Event;
    class WindowCloseEvent;
    class WindowResizeEvent;
    class Window;

    /** Main engine class from which all client applications derive
    *
    */
    class Engine
    {
    public:
        enum class RendererType { None, Renderer2D, Renderer3D };

        struct Properties
        {
            const char* windowTitle;
            unsigned int windowWidth;
            unsigned int windowHeight;
            RendererType rendererType;
            const char* assetsDirectory = "assets";
        };

        explicit Engine(const Properties& properties);
        virtual ~Engine();

        void run();
        void close() { m_isRunning = false; }

        static Engine& get() { return *s_instance; }
        const Properties& getProperties() const { return m_properties; }
        Window& getWindow() { return *m_window; }
    protected:
        LayerStack& getLayerStack() { return m_layerStack; }
    private:
        void onEvent(Event& event);
        bool onWindowClose(WindowCloseEvent& event);
        bool onWindowResize(WindowResizeEvent& event);

        static Engine* s_instance;
        Properties m_properties;
        Scope<Window> m_window;
        RendererType m_rendererType;
        LayerStack m_layerStack;
        bool m_isRunning = true;
        std::chrono::time_point<std::chrono::steady_clock> m_lastFrameTime;
    };

} // namespace jng
