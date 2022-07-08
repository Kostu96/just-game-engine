/*
 * Copyright (C) 2020-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#define JNG_DECLARE_MAIN

#include "game_data.hpp"
#include "game_layer.hpp"

#include <jng/jng.hpp>

class App :
    public jng::Engine
{
public:
    static constexpr unsigned int WindowWidth = 1600;
    static constexpr unsigned int WindowHeight = 900;

    App() :
        Engine({
            "Sandbox Application",
            WindowWidth,
            WindowHeight,
            RendererType::Renderer2D
        })
    {
        m_gameData.WindowWidth = WindowWidth;
        m_gameData.WindowHeight = WindowHeight;

        getLayerStack().pushLayer(new GameLayer{ m_gameData });
    }
private:
    GameData m_gameData;
};

jng::Engine* createApp() {
    return new App{};
}
