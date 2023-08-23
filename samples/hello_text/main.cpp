/*
 * Copyright (C) 2023 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include <jng/core/engine.hpp>
#include <jng/core/entry_point.hpp>
#include <jng/core/event.hpp>
#include <jng/core/window_events.hpp>
#include <jng/core/layer.hpp>
#include <jng/scene/camera.hpp>
#include <jng/renderer/renderer2d.hpp>
#include <jng/renderer/renderer_api.hpp>
#include <jng/renderer/font.hpp>
#include <jng/renderer/texture.hpp>
#include <imgui.h>

static constexpr unsigned int WindowWidth = 800;
static constexpr unsigned int WindowHeight = 600;

class SampleLayer :
    public jng::Layer
{
public:
    SampleLayer()
    {
        m_camera.setViewportSize(WindowWidth, WindowHeight);
        m_camera.setOrthographicSize(1.f);
    }

    void onUpdate(float /*dt*/) override
    {
        jng::RendererAPI::clear({ 0.1f, 0.1f, 0.2f });
        jng::Renderer2D::beginScene(m_camera.getVP(glm::mat4{ 1.f }));
        jng::Renderer2D::drawText(glm::mat4{ 1.f }, "Kostu96", jng::Font::getDefaultFont());
        jng::Renderer2D::endScene();
    }

    void onImGuiUpdate() override
    {
        ImGui::Begin("Framebuffer");
        ImGui::Image(jng::Font::getDefaultFont()->getAtlasTexture()->getRendererID(), { 512, 512 });
        ImGui::End();
    }

    void onEvent(jng::Event& event) override
    {
        jng::EventDispatcher dispatcher(event);
        dispatcher.dispatch<jng::WindowResizeEvent>(JNG_BIND_EVENT_FUNC(SampleLayer::onWindowResize));
    }
private:
    bool onWindowResize(jng::WindowResizeEvent& event)
    {
        m_camera.setViewportSize(static_cast<float>(event.getWidth()), static_cast<float>(event.getHeight()));
        return false;
    }

    jng::Camera m_camera;
};

class App :
    public jng::Engine
{
public:
    App() :
        Engine({
            "Hello Text!",
            WindowWidth,
            WindowHeight,
            RendererType::Renderer2D,
            "assets/hello_text"
        })
    {
        getLayerStack().pushLayer(new SampleLayer{});
    }
};

jng::Engine* createApp() {
    return new App{};
}
