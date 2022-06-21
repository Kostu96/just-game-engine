/*
 * Copyright (C) 2020-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "core/engine.hpp"

#include "core/base_internal.hpp"
#include "core/event.hpp"
#include "core/layer.hpp"
#include "core/window_events.hpp"
#include "platform/graphics_context.hpp"
#include "platform/window.hpp"
#include "renderer/renderer2d.hpp"
#include "renderer/renderer_api.hpp"

namespace jng {

    Engine::Engine(const char* title, unsigned int width, unsigned int height)
    {
        JNG_PROFILE_FUNCTION();

        JNG_CORE_ASSERT(!s_instance, "Application already exists!");
        s_instance = this;

        // TODO: do proper render API choosing
        RendererBackend backend = RendererBackend::OpenGL;

        m_window = Window::create(title, width, height, backend);
        m_window->setEventCallback(JNG_BIND_EVENT_FUNC(Engine::onEvent));
        m_window->setVSync(true);

        // TODO: choose 3D/2D, 2D or NO_RENDERER mode
        RendererAPI::init(backend);
        Renderer2D::init(backend);
    }

    Engine::~Engine()
    {
        JNG_PROFILE_FUNCTION();

        Renderer2D::shutdown();
    }

    void Engine::run()
    {
        JNG_PROFILE_FUNCTION();

        while (m_isRunning)
        {
            JNG_PROFILE_SCOPE("Main loop frame");

            auto time = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::nano> dt = time - m_lastFrameTime;
            m_lastFrameTime = time;

            // TODO: let pause on minimize be controlled by client
            if (!m_window->isMinimized())
                for (auto layer : m_layerStack)
                    layer->onUpdate(static_cast<float>(dt.count() * 0.000000001));

            m_window->onUpdate();
        }
    }

    void Engine::onEvent(Event& event)
    {
        JNG_PROFILE_FUNCTION();

        EventDispatcher dispatcher(event);
        dispatcher.dispatch<WindowCloseEvent>(JNG_BIND_EVENT_FUNC(Engine::onWindowClose));
        dispatcher.dispatch<WindowResizeEvent>(JNG_BIND_EVENT_FUNC(Engine::onWindowResize));

        for (auto it = m_layerStack.end(); it != m_layerStack.begin(); ) {
            (*--it)->onEvent(event);
            if (event.isHandled())
                break;
        }
    }

    bool Engine::onWindowClose(WindowCloseEvent& /*event*/)
    {
        m_isRunning = false;
        return true;
    }

    bool Engine::onWindowResize(WindowResizeEvent& event)
    {
        if (!m_window->isMinimized())
            Renderer2D::setViewport(0, 0, event.getWidth(), event.getHeight());

        return false;
    }

    Engine* Engine::s_instance = nullptr;

} // namespace jng
