/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include <jng/core/engine.hpp>
#include <jng/core/entry_point.hpp>
#include <jng/core/layer.hpp>
#include <jng/renderer/renderer_api.hpp>

#include <imgui.h>

class SampleLayer :
    public jng::Layer
{
public:
    SampleLayer() {}

    void onUpdate(float /*dt*/) override
    {
        jng::RendererAPI::clear({ 0.f, 0.f, 0.f });
    }

    void onImGuiUpdate() override
    {
        static bool open = true;
        ImGui::ShowDemoWindow(&open);
    }
private:
};

class App :
    public jng::Engine
{
public:
    static constexpr unsigned int WindowWidth = 1600;
    static constexpr unsigned int WindowHeight = 900;

    App() :
        Engine({
            "Hello ImGui!",
            WindowWidth,
            WindowHeight,
            RendererType::None
        })
    {
        getLayerStack().pushLayer(new SampleLayer{});
    }
};

jng::Engine* createApp() {
    return new App{};
}
