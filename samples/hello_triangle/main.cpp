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
    SampleLayer() = default;
private:

};

class App :
    public jng::Engine
{
public:
    static constexpr unsigned int WindowWidth = 800;
    static constexpr unsigned int WindowHeight = 600;

    App() :
        Engine("Hello Triangle!", WindowWidth, WindowHeight)
    {
        getLayerStack().pushLayer(new SampleLayer{});
    }
};

jng::Engine* createApp() {
    return new App{};
}
