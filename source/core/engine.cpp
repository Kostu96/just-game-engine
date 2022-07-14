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
#include "imgui/imgui_layer.hpp"
#include "platform/graphics_context.hpp"
#include "platform/window.hpp"
#include "renderer/renderer.hpp"
#include "renderer/renderer2d.hpp"
#include "renderer/renderer_api.hpp"

namespace jng {

    constexpr double NANO_TO_BASE_MULTIPLIER = 0.000000001;

    Engine::Engine(const Properties& properties) :
        m_properties{ properties },
        m_rendererType{ properties.rendererType }
    {
        JNG_PROFILE_FUNCTION();

        JNG_CORE_ASSERT(!s_instance, "Application already exists!");
        s_instance = this;

        m_window = Window::create(properties.windowTitle, properties.windowWidth, properties.windowHeight);
        m_window->setEventCallback(JNG_BIND_EVENT_FUNC(Engine::onEvent));
        m_window->setVSync(true);

        ImGuiLayer::init(m_window.get());

        switch (properties.rendererType)
        {
        case RendererType::None: break;
        case RendererType::Renderer2D:
            Renderer2D::init();
            break;
        case RendererType::Renderer3D:
            Renderer::init();
            break;
        }
    }

    Engine::~Engine()
    {
        JNG_PROFILE_FUNCTION();

        switch (m_rendererType)
        {
        case RendererType::None: break;
        case RendererType::Renderer2D:
            Renderer2D::shutdown();
            break;
        case RendererType::Renderer3D:
            Renderer::shutdown();
            break;
        }
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

            if (!m_window->isMinimized())
                for (auto layer : m_layerStack)
                    layer->onUpdate(static_cast<float>(dt.count() * NANO_TO_BASE_MULTIPLIER));

            ImGuiLayer::newFrame();
            if (!m_window->isMinimized())
                for (auto layer : m_layerStack)
                    layer->onImGuiUpdate();
            ImGuiLayer::render();
            
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
            RendererAPI::setViewport(0, 0, event.getWidth(), event.getHeight());

        return false;
    }

    Engine* Engine::s_instance = nullptr;

} // namespace jng
