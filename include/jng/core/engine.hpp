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
        };

        explicit Engine(const Properties& properties);
        virtual ~Engine();

        void run();

        static Engine& get() { return *s_instance; }
        Window& getWindow() { return *m_window; }
    protected:
        LayerStack& getLayerStack() { return m_layerStack; }
    private:
        void onEvent(Event& event);
        bool onWindowClose(WindowCloseEvent& event);
        bool onWindowResize(WindowResizeEvent& event);

        static Engine* s_instance;
        Scope<Window> m_window;
        RendererType m_rendererType;
        LayerStack m_layerStack;
        bool m_isRunning = true;
        std::chrono::time_point<std::chrono::steady_clock> m_lastFrameTime;
    };

} // namespace jng
