/*
 * Copyright (C) 2021-2023 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

namespace jng {

    class Font;
    class Texture;
    struct CircleRendererComponent;
    struct SpriteRendererComponent;

    namespace Renderer2D {

        struct DrawQuadProperties
        {
            const glm::vec3* vertexPositions;
            const glm::vec2* textureCoords;
            const Ref<Texture>& texture;
            u32 color;
            s32 entityID = -1;
        };

        struct DrawCircleProperties
        {
            const glm::vec3* vertexPositions;
            const glm::vec2* localVertexPositions;
            f32 thickness;
            f32 fade;
            u32 color;
            s32 entityID = -1;
        };

        void init();
        void shutdown();

        void beginScene(const glm::mat4& viewProjection);
        void endScene();

        void drawText(const glm::mat4& transform, const std::string& text, Ref<Font> font, const glm::vec4& color = { 1.f, 1.f, 1.f, 1.f });

        // Quads
        void drawSprite(const glm::mat4& transform, const SpriteRendererComponent& src, s32 entityID);
        void drawQuad(const DrawQuadProperties& properties);
        
        // Circles
        void drawCircle(const glm::mat4& transform, const CircleRendererComponent& crc, s32 entityID);
        void drawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness = 1.f, float fade = 0.005f);
        void drawCircle(const DrawCircleProperties& properties);

        // Lines
        void drawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color);
        void drawRect(const glm::mat4& transform, const glm::vec4& color);

        void fillQuad(glm::vec3 position, glm::vec2 size, const glm::vec4& color);
        void fillQuad(glm::vec3 position, glm::vec2 size, const Ref<Texture>& texture, const glm::vec4& color = { 1.f, 1.f, 1.f, 1.f });
        void fillQuad(const glm::mat4& transform, const glm::vec4& color);
        void fillQuad(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& color = { 1.f, 1.f, 1.f, 1.f });

        struct Statistics
        {
            u32 drawCalls = 0;
            u32 quadCount = 0;
            u32 circleCount = 0;
            u32 lineCount = 0;
        };

        const Statistics& getStatistics();

    } // namespace Renderer2D

} // namespace jng
