/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "camera_controller.hpp"
#include "game_data.hpp"
#include "subtexture.hpp"

#include <k2d/k2d.hpp>

class GameLayer :
    public k2d::Layer
{
public:
    GameLayer(const GameData& gameData);

    void onEvent(k2d::Event& event);
    void onUpdate(float dt) override;
    void onImGuiRender() override;
private:
    const GameData& m_gameData;
    CameraController m_cameraController;
    k2d::Ref<k2d::Texture> m_texture;
    k2d::Ref<SubTexture> m_stairTexture;
};
