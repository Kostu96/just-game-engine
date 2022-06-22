/*
 * Copyright (C) 2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#define JNG_DECLARE_MAIN
#include <jng/jng.hpp>

class EditorLayer :
    public jng::Layer
{
public:
    EditorLayer() = default;

    void onUpdate(float /*dt*/) override
    {
        static bool show = true;
        ImGui::ShowDemoWindow(&show);
    }
private:
};

class EditorApp :
    public jng::Engine
{
public:
    static constexpr unsigned int WindowWidth = 1600;
    static constexpr unsigned int WindowHeight = 900;

    EditorApp() :
        Engine("JNG Editor!", WindowWidth, WindowHeight)
    {
        getLayerStack().pushLayer(new EditorLayer{});
    }
};

jng::Engine* createApp() {
    return new EditorApp{};
}
