/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "game_layer.hpp"

#include <jng/scene/components.hpp>
#include<jng/scripting/native_script.hpp>

#include <sstream>

class CameraController :
    public jng::NativeScript
{
public:
    void onCreate() override
    {
        //auto& cameraComponent = getComponent<jng::CameraComponent>();
    }

    void onUpdate(float /*dt*/) override
    {

    }
private:

};

GameLayer::GameLayer(const GameData& gameData) :
    m_gameData{ gameData }
{
    auto camera = m_activeScene.createEntity("Main Camera");
    camera.addComponent<jng::CameraComponent>();
    camera.addComponent<jng::NativeScriptComponent>().bind<CameraController>();

    auto square = m_activeScene.createEntity("Square");
    square.addComponent<jng::SpriteComponent>().Color = {0.f, 1.f, 0.f, 1.f };
}

void GameLayer::onEvent(jng::Event& /*event*/)
{
    
}

void GameLayer::onUpdate(float dt)
{
    jng::RendererAPI::clear({ 0.1f, 0.07f, 0.07f });

    m_activeScene.onUpdate(dt);

    static float timer = 0.f;
    static uint32_t frames = 0;
    ++frames;
    timer += dt;
    if (timer >= 1.f)
    {
        std::stringstream title;
        auto& stats = jng::Renderer2D::getStatistics();
        title << "FPS: " << frames << " Quads: " << stats.quadCount << " DrawCalls: " << stats.drawCalls;
        jng::Engine::get().getWindow().setTitle(title.str().c_str());

        timer = 0.f;
        frames = 0;
    }
}
