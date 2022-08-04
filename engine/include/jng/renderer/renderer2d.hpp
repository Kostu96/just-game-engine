/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

#include <glm/glm.hpp>

namespace jng {

    class Texture;
    struct CircleRendererComponent;
    struct SpriteRendererComponent;

    class Renderer2D :
        public NonCopyable
    {
    public:
        struct DrawQuadProperties
        {
            const glm::vec3* vertexPositions;
            const glm::vec2* textureCoords;
            const Ref<Texture>& texture;
            uint32 color;
            int32 entityID = -1;
        };

        struct DrawCircleProperties
        {
            const glm::vec3* vertexPositions;
            const glm::vec2* localVertexPositions;
            float thickness;
            float fade;
            uint32 color;
            int32 entityID = -1;
        };

        static void init();
        static void shutdown();

        static void beginScene(const glm::mat4& viewProjection);
        static void endScene();

        static void drawSprite(const glm::mat4& transform, const SpriteRendererComponent& src, int32 entityID);
        static void drawCircle(const glm::mat4& transform, const CircleRendererComponent& crc, int32 entityID);
        
        static void drawQuad(const DrawQuadProperties& properties);
        static void drawCircle(const DrawCircleProperties& properties);

        static void drawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color);
        static void drawRect(const glm::mat4& transform, const glm::vec4& color);

        static void fillQuad(glm::vec3 position, glm::vec2 size, const glm::vec4& color);
        static void fillQuad(glm::vec3 position, glm::vec2 size, const Ref<Texture>& texture, const glm::vec4& color = { 1.f, 1.f, 1.f, 1.f });
        static void fillQuad(const glm::mat4& transform, const glm::vec4& color);
        static void fillQuad(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& color = { 1.f, 1.f, 1.f, 1.f });

        struct Statistics
        {
            uint32 drawCalls = 0;
            uint32 quadCount = 0;
            uint32 circleCount = 0;
            uint32 lineCount = 0;
        };

        static const Statistics& getStatistics();
    private:
        static void beginQuadBatch();
        static void endQuadBatch();
        static void beginCircleBatch();
        static void endCircleBatch();
        static void beginLineBatch();
        static void endLineBatch();
    };

} // namespace jng
