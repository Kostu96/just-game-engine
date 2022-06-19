/*
 * Copyright (C) 2020-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#define K2D_DECLARE_MAIN

#include "game_data.hpp"
#include "game_layer.hpp"

#include <k2d/k2d.hpp>

class App :
    public k2d::Engine
{
public:
    static constexpr unsigned int WindowWidth = 1600;
    static constexpr unsigned int WindowHeight = 900;

    App() :
        Engine("Sandbox Application", WindowWidth, WindowHeight)
    {
        m_gameData.WindowWidth = WindowWidth;
        m_gameData.WindowHeight = WindowHeight;

        getLayerStack().pushLayer(new GameLayer{ m_gameData });
    }
private:
    GameData m_gameData;
};

k2d::Engine* createApp() {
    return new App{};
}
