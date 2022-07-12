/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "game_data.hpp"
#include "subtexture.hpp"

#include <jng/core/layer.hpp>
#include <jng/scene/scene.hpp>

class GameLayer :
    public jng::Layer
{
public:
    GameLayer(const GameData& gameData);

    void onEvent(jng::Event& event);
    void onUpdate(float dt) override;
private:
    const GameData& m_gameData;
    jng::Scene m_activeScene;
};
