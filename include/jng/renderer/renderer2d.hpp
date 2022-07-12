/*
 * Copyright (C) 2021-2022 Konstanty Misiak
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once
#include "jng/core/base.hpp"

#include <glm/glm.hpp>

namespace jng {

    class VertexArray;
    class RendererAPI;
    class Texture;

    class Renderer2D :
        public ccl::NonCopyable
    {
    public:
        struct Properties
        {
            const glm::vec2* quadVertexPositions;
            const glm::vec2* textureCoords;
            const Ref<Texture>& texture;
            const glm::vec4& color;
        };

        static void init();
        static void shutdown();

        static void beginScene(const glm::mat4& viewProjection);
        static void endScene();

        static void fillQuad(const Properties& properties);
        static void fillQuad(glm::vec2 position, glm::vec2 size, const glm::vec4& color);
        static void fillQuad(glm::vec2 position, glm::vec2 size, const Ref<Texture>& texture, const glm::vec4& color = { 1.f, 1.f, 1.f, 1.f });
        static void fillQuad(glm::mat4 transform, const glm::vec4& color);
        static void fillQuad(glm::mat4 transform, const Ref<Texture>& texture, const glm::vec4& color = { 1.f, 1.f, 1.f, 1.f });

        struct Statistics
        {
            uint32 drawCalls = 0;
            uint32 quadCount = 0;
        };

        static const Statistics& getStatistics();
    private:
        static void beginBatch();
        static void endBatch();
    };

} // namespace jng
