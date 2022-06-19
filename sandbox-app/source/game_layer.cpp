/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#include "game_layer.hpp"

#include <sstream>

GameLayer::GameLayer(const GameData& gameData) :
    m_gameData(gameData),
    m_cameraController(static_cast<float>(gameData.WindowWidth) / static_cast<float>(gameData.WindowHeight))
{
    m_texture = k2d::Texture::create("assets/textures/RPGpack_sheet_2X.png");

    constexpr float x = 7.f, y = 6.f;
    constexpr float spriteW = 128.f, spriteH = 128.f;
    m_stairTexture = k2d::makeRef<SubTexture>(m_texture, glm::vec2{ x * spriteW, y * spriteH }, glm::vec2{ (x + 1) * spriteW, (y + 1) * spriteH });
}

void GameLayer::onEvent(k2d::Event& event)
{
    m_cameraController.onEvent(event);
}

void GameLayer::onUpdate(float dt)
{
    m_cameraController.onUpdate(dt);

    k2d::Renderer::clear({ 0.1f, 0.07f, 0.07f });
    k2d::Renderer::beginScene(m_cameraController.getCamera());

    // test texture atlas
    {
        const glm::vec2 verts[4] = { { 0.f, 0.f }, { 1.f, 0.f }, { 1.f, 1.f, }, { 0.f, 1.f } };
        const k2d::Renderer::Properties props{
            verts,
            m_stairTexture->getTextureCoords(),
            m_stairTexture->getTexture(),
            { 1.f, 1.f, 1.f, 1.f }
        };
        k2d::Renderer::fillQuad(props);
    }
    // test rotation
    {
        static float rotation = 0.f;
        rotation += 60.f * dt;
        if (rotation > 360.f) rotation -= 360.f;
        k2d::Renderer::fillQuad({ -0.9f, -0.5f }, { 0.4f, 0.4f }, rotation, { 0.3f, 0.8f, 0.9f, 0.7f });
    }
    // test lots of quads
    {
        for (float y = -0.9f; y < 0.9f; y += 0.05f)
            for (float x = -1.6f; x < 1.6f; x += 0.05f)
            {
                glm::vec4 color{ (y + 0.9f) / 1.8f, (x + 1.6f) / 3.2f, 0.f, 0.3f };
                k2d::Renderer::fillQuad({ x, y }, { 0.05f, 0.05f }, color);
            }
    }
    k2d::Renderer::endScene();

    static float timer = 0.f;
    static uint32_t frames = 0;
    ++frames;
    timer += dt;
    if (timer >= 1.f)
    {
        std::stringstream title;
        auto& stats = k2d::Renderer::getStatistics();
        title << "FPS: " << frames << " Quads: " << stats.quadCount << " DrawCalls: " << stats.drawCalls;
        k2d::Engine::get().getWindow().setTitle(title.str().c_str());

        timer = 0.f;
        frames = 0;
    }
}

void GameLayer::onImGuiRender()
{
    // TODO: disabled until platform code gets more features 
    //ImGuiIO& io = ImGui::GetIO();
    //ImGui::Begin("Debug");
    //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    //ImGui::End();
    //ImGui::ShowDemoWindow();
}
