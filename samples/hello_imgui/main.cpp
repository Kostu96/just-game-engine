/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#define JNG_DECLARE_MAIN
#include <jng/jng.hpp>

class SampleLayer :
    public jng::Layer
{
public:
    SampleLayer()
    {
        
    }

    void onUpdate(float /*dt*/) override
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
            RendererType::None // TODO: temp?
        })
    {
        getLayerStack().pushLayer(new SampleLayer{});
    }
};

jng::Engine* createApp() {
    return new App{};
}
