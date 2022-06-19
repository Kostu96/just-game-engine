/*
 * Copyright (C) 2020-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "core/base.hpp"
#include "core/layer_stack.hpp"

#include <chrono>

namespace jng {

    class Event;
    class WindowCloseEvent;
    class WindowResizeEvent;
    class Window;

    class Engine
    {
    public:
        Engine(const char* title, unsigned int width, unsigned int height);
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
        LayerStack m_layerStack;
        bool m_isRunning = true;
        std::chrono::time_point<std::chrono::steady_clock> m_lastFrameTime;
    };

} // namespace jng
